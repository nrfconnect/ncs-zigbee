"""
Copyright (c) 2026 Nordic Semiconductor ASA

SPDX-License-Identifier: LicenseRef-Nordic-5-Clause

Shared memory requirement data for Zigbee documentation tables and charts.

Board data lives in docs/data/memory/<board>.yaml.
"""

from __future__ import annotations

from pathlib import Path
from typing import Any

import yaml
from docutils import nodes
from docutils.parsers.rst import directives
from docutils.statemachine import StringList
from sphinx.application import Sphinx
from sphinx.util.docutils import SphinxDirective

__version__ = "0.1.0"

RESOURCES_DIR = Path(__file__).parent / "static"
DATA_DIR = Path(__file__).parents[1] / "data" / "memory"

INTERNAL_NVM_COLUMNS = (
    ("boot", "MCUboot (used / free)"),
    ("slot0", "Application (used / free)"),
    ("slot1", "Upgrade slot (used / free)"),
    ("factory_data", "Factory data"),
    ("storage", "Storage"),
    ("zboss_nvram", "ZBOSS NVRAM"),
    ("zboss_product_config", "ZBOSS product config"),
)

EXTERNAL_NVM_COLUMN_ORDER = ("slot1_ext", "slot3_ext")

RAM_SUBHEAD = "used / free"
EMPTY_CELL = "--"

STACK_MAIN_SUBHEADS = ("stack usage", "stack size")
STACK_ZBOSS_SUBHEADS = ("stack usage", "stack size")

PADDING_LABEL = "Unused"


def applicable_samples(data: dict[str, Any]) -> list[dict[str, Any]]:
    return [sample for sample in data["samples"] if not sample.get("not_applicable")]


def stack_samples(data: dict[str, Any]) -> list[dict[str, Any]]:
    return [sample for sample in data.get("samples", []) if sample.get("stack")]


def stack_thread_sizes(samples: list[dict[str, Any]]) -> tuple[int, int]:
    main_sizes = [int(sample["stack"]["main"]["size_b"]) for sample in samples]
    zboss_sizes = [int(sample["stack"]["zboss"]["size_b"]) for sample in samples]
    return max(main_sizes), max(zboss_sizes)


def sample_stack_table_cells(sample: dict[str, Any]) -> list[str]:
    stack = sample["stack"]
    return [
        str(int(stack["main"]["used_b"])),
        str(int(stack["main"]["size_b"])),
        str(int(stack["zboss"]["used_b"])),
        str(int(stack["zboss"]["size_b"])),
    ]


def load_board_data(board: str) -> dict[str, Any]:
    path = DATA_DIR / f"{board}.yaml"
    if not path.is_file():
        raise FileNotFoundError(f"Memory data file not found: {path}")
    with path.open(encoding="utf-8") as handle:
        return yaml.safe_load(handle)


def _part_size_bytes(part: dict[str, Any]) -> int:
    size_kb = float(part["size_kb"])
    return round(size_kb * 1024)


def _padding_part(offset: int, size_bytes: int) -> dict[str, Any]:
    return {
        "id": "padding",
        "label": PADDING_LABEL,
        "size_kb": round(size_bytes / 1024),
        "offset": offset,
    }


def _sort_partitions(partitions: list[dict[str, Any]]) -> list[dict[str, Any]]:
    real = [part for part in partitions if part.get("id") != "padding"]
    if real and all("offset" in part for part in real):
        return sorted(partitions, key=lambda part: (part.get("offset", 0), part["order"]))
    return sorted(partitions, key=lambda part: part["order"])


def expand_partition_gaps(
    partitions: list[dict[str, Any]],
    flash_total_kb: float | None,
) -> list[dict[str, Any]]:
    real = [
        part
        for part in partitions
        if part.get("id") != "padding" and part.get("memory") != "ram"
    ]
    if not real:
        return []

    if all("offset" in part for part in real):
        ordered = sorted(real, key=lambda part: part["offset"])
        expanded: list[dict[str, Any]] = []
        cursor = 0
        flash_end = round(float(flash_total_kb) * 1024) if flash_total_kb is not None else None

        for part in ordered:
            offset = int(part["offset"])
            if offset > cursor:
                gap_bytes = offset - cursor
                if gap_bytes > 0:
                    expanded.append(_padding_part(cursor, gap_bytes))
            expanded.append(dict(part))
            cursor = max(cursor, offset + _part_size_bytes(part))

        if flash_end is not None and cursor < flash_end:
            gap_bytes = flash_end - cursor
            if gap_bytes > 0:
                expanded.append(_padding_part(cursor, gap_bytes))

        for order, part in enumerate(expanded):
            part["order"] = order
        return expanded

    ordered = sorted(real, key=lambda part: part["order"])
    if flash_total_kb is None:
        return ordered

    used_kb = sum(float(part["size_kb"]) for part in ordered)
    if flash_total_kb <= used_kb:
        return ordered

    padding_kb = flash_total_kb - used_kb
    expanded = list(ordered)
    expanded.append(
        {
            "id": "padding",
            "label": PADDING_LABEL,
            "size_kb": padding_kb,
            "order": len(expanded),
        }
    )
    return expanded


def resolve_layout_raw(data: dict[str, Any], layout_name: str) -> dict[str, Any]:
    layouts = data["layouts"]
    layout = layouts[layout_name]
    if "extends" in layout:
        base = resolve_layout_raw(data, layout["extends"])
        own = layout.get("partitions", [])
        own_ids = {part["id"] for part in own}

        merged: dict[str, dict[str, Any]] = {
            part["id"]: dict(part) for part in own
        }
        for part in base["partitions"]:
            if part["id"] not in own_ids and part["id"] != "padding":
                merged[part["id"]] = dict(part)

        partitions = _sort_partitions(list(merged.values()))
        external = layout.get("external", base.get("external"))
        return {"partitions": partitions, "external": external}
    return {
        "partitions": _sort_partitions(list(layout["partitions"])),
        "external": layout.get("external"),
    }


def _apply_layout_gaps(data: dict[str, Any], layout: dict[str, Any]) -> dict[str, Any]:
    partitions = expand_partition_gaps(
        layout["partitions"],
        board_nvm_total_kb(data),
    )
    external = layout.get("external")
    if external is not None:
        external = {
            **external,
            "partitions": expand_partition_gaps(
                external["partitions"],
                float(external["nvm_total_kb"]),
            ),
        }
    return {"partitions": partitions, "external": external}


def resolve_layout(data: dict[str, Any], layout_name: str) -> dict[str, Any]:
    return _apply_layout_gaps(data, resolve_layout_raw(data, layout_name))


def layout_nvm_total_kb(layout: dict[str, Any]) -> float:
    real = [
        part
        for part in layout["partitions"]
        if part.get("memory") != "ram" and part.get("id") != "padding"
    ]
    if real and all("offset" in part for part in real):
        end = max(int(part["offset"]) + _part_size_bytes(part) for part in real)
        return end / 1024
    return float(sum(part["size_kb"] for part in real))


def board_nvm_total_kb(data: dict[str, Any]) -> float:
    board_total = data.get("board", {}).get("nvm_total_kb")
    if board_total is not None:
        return float(board_total)
    return layout_nvm_total_kb(resolve_layout_raw(data, "base"))


def sample_layout(data: dict[str, Any], sample: dict[str, Any]) -> dict[str, Any] | None:
    layout_name = sample.get("layout")
    if layout_name is None:
        return None
    return resolve_layout(data, layout_name)


def table_constants(data: dict[str, Any]) -> dict[str, int]:
    board_table = data.get("board", {}).get("table")
    if board_table:
        constants = {
            "zboss_nvram_kb": int(board_table["zboss_nvram_kb"]),
            "zboss_product_config_kb": int(board_table["zboss_product_config_kb"]),
        }
        if "storage_kb" in board_table:
            constants["storage_kb"] = int(board_table["storage_kb"])
        return constants

    if "layouts" not in data:
        raise KeyError(
            f"Board data must define board.table or layouts for table generation: {data.get('board')}"
        )

    layout = resolve_layout(data, "base")
    by_id = {part["id"]: part for part in layout["partitions"]}
    return {
        "zboss_nvram_kb": int(by_id["zboss_nvram"]["size_kb"]),
        "zboss_product_config_kb": int(by_id["zboss_product_config"]["size_kb"]),
        "storage_kb": int(by_id["storage"]["size_kb"]),
    }


def board_external_nvm_columns(data: dict[str, Any]) -> list[tuple[str, str]]:
    """External NVM table columns when any layout defines external upgrade slots."""
    labels: dict[str, str] = {}
    order: dict[str, int] = {}
    for layout in data.get("layouts", {}).values():
        external = layout.get("external")
        if not external:
            continue
        for part in external["partitions"]:
            if part["id"] == "padding":
                continue
            labels.setdefault(part["id"], part["label"])
            order.setdefault(part["id"], int(part["order"]))

    if not labels:
        return []

    column_order = {
        part_id: index for index, part_id in enumerate(EXTERNAL_NVM_COLUMN_ORDER)
    }
    return sorted(
        labels.items(),
        key=lambda item: (column_order.get(item[0], len(EXTERNAL_NVM_COLUMN_ORDER)), order[item[0]]),
    )


def board_has_external_nvm(data: dict[str, Any]) -> bool:
    return bool(board_external_nvm_columns(data))


def board_external_total_kb(data: dict[str, Any]) -> float | None:
    """Return external NVM capacity when any layout defines external flash."""
    for layout in data.get("layouts", {}).values():
        external = layout.get("external")
        if external:
            return float(external["nvm_total_kb"])
    return None


def _partition_map(layout: dict[str, Any] | None) -> dict[str, dict[str, Any]]:
    if layout is None:
        return {}
    parts = {
        part["id"]: part
        for part in layout["partitions"]
        if part.get("id") != "padding"
    }
    external = layout.get("external")
    if external:
        for part in external["partitions"]:
            if part.get("id") != "padding":
                parts[part["id"]] = part
    return parts


def _fmt_kb(value: int | float) -> str:
    if value == int(value):
        return str(int(value))
    return f"{value:.1f}"


def _used_free(used: int, total: int | None) -> str:
    if total is None:
        return _fmt_kb(used)
    free = max(int(total) - used, 0)
    return f"{_fmt_kb(used)} / {_fmt_kb(free)}"


def _partition_size(
    parts: dict[str, dict[str, Any]],
    part_id: str,
    constants: dict[str, int],
) -> int | None:
    if part_id in parts:
        return int(parts[part_id]["size_kb"])
    if part_id == "storage":
        return constants.get("storage_kb")
    if part_id == "zboss_nvram":
        return constants["zboss_nvram_kb"]
    if part_id == "zboss_product_config":
        return constants["zboss_product_config_kb"]
    return None


def sample_table_cells(
    data: dict[str, Any],
    sample: dict[str, Any],
    *,
    include_external: bool,
) -> list[str]:
    constants = table_constants(data)
    usage = sample.get("usage", {})
    nvm = usage.get("nvm", {})
    layout = sample_layout(data, sample)
    parts = _partition_map(layout)
    ram_total = data.get("board", {}).get("ram_total_kb")

    cells: list[str] = []

    # MCUboot — used / free within boot partition when present.
    if "boot" in parts or nvm.get("boot"):
        boot_used = int(nvm.get("boot", 0))
        boot_total = _partition_size(parts, "boot", constants)
        cells.append(_used_free(boot_used, boot_total))
    else:
        cells.append(EMPTY_CELL)

    # Application — used / free within slot0.
    app_used = int(nvm.get("slot0", 0))
    app_total = _partition_size(parts, "slot0", constants)
    cells.append(_used_free(app_used, app_total))

    external_usage = usage.get("external", {})

    # Reserved internal NVM partitions — used/free when measured, else size or --.
    for part_id, _ in INTERNAL_NVM_COLUMNS[2:]:
        size = _partition_size(parts, part_id, constants)
        used = nvm.get(part_id)
        if used is not None and size is not None:
            cells.append(_used_free(int(used), size))
        elif size is not None:
            cells.append(_fmt_kb(size))
        else:
            cells.append(EMPTY_CELL)

    if include_external:
        for part_id, _ in board_external_nvm_columns(data):
            ext_size = _partition_size(parts, part_id, constants)
            ext_used = external_usage.get(part_id)
            if ext_used is not None and ext_size is not None:
                cells.append(_used_free(int(ext_used), ext_size))
            elif ext_size is not None:
                cells.append(_fmt_kb(ext_size))
            else:
                cells.append(EMPTY_CELL)

    ram_used = int(usage.get("ram_used_kb", 0))
    cells.append(_used_free(ram_used, int(ram_total) if ram_total is not None else None))

    return cells


def _header_label(data: dict[str, Any], kind: str) -> str:
    board = data.get("board", {})
    if kind == "nvm":
        total = board_nvm_total_kb(data)
        return f"Internal NVM ({_fmt_kb(total)} kB)"
    if kind == "external":
        for sample in applicable_samples(data):
            if "layout" not in sample:
                continue
            external = resolve_layout(data, sample["layout"]).get("external")
            if external:
                total = external["nvm_total_kb"]
                return f"External NVM ({_fmt_kb(total)} kB)"
        return "External NVM"
    total = board.get("ram_total_kb")
    return f"RAM ({_fmt_kb(total)} kB)" if total is not None else "RAM"


def _append_rst_cell(state, entry: nodes.entry, rst_text: str) -> None:
    paragraph = nodes.paragraph()
    content = StringList([rst_text], "<memory_table>")
    state.nested_parse(content, 0, paragraph)
    if len(paragraph) == 1 and isinstance(paragraph[0], nodes.paragraph):
        inner = paragraph[0]
        paragraph.remove(inner)
        paragraph.extend(inner.children)
    entry += paragraph


def _append_text_cell(row: nodes.row, text: str, *, css_class: str = "memory-req-value") -> None:
    entry = nodes.entry()
    entry["classes"] = [css_class]
    entry += nodes.Text(text)
    row += entry


def build_memory_table_nodes(directive: SphinxDirective, data: dict[str, Any]) -> nodes.table:
    external_columns = board_external_nvm_columns(data)
    include_external = bool(external_columns)
    internal_count = len(INTERNAL_NVM_COLUMNS)
    external_count = len(external_columns)
    total_cols = 1 + internal_count + external_count + 1

    table = nodes.table()
    table["classes"] = ["memory-req-table"]

    tgroup = nodes.tgroup(cols=total_cols)
    table += tgroup

    colspecs = [nodes.colspec(colwidth=28)] + [nodes.colspec(colwidth=10)] * (total_cols - 1)
    tgroup.extend(colspecs)

    thead = nodes.thead()
    tgroup += thead

    group_row = nodes.row()
    thead += group_row

    sample_group = nodes.entry()
    sample_group["morerows"] = 1
    sample_group += nodes.Text("Sample")
    group_row += sample_group

    nvm_group = nodes.entry()
    nvm_group["morecols"] = internal_count - 1
    nvm_group["classes"] = ["memory-req-group-nvm"]
    nvm_group += nodes.Text(_header_label(data, "nvm"))
    group_row += nvm_group

    if include_external:
        external_group = nodes.entry()
        if external_count > 1:
            external_group["morecols"] = external_count - 1
        external_group["classes"] = ["memory-req-group-external"]
        external_group += nodes.Text(_header_label(data, "external"))
        group_row += external_group

    ram_group = nodes.entry()
    ram_group["classes"] = ["memory-req-group-ram"]
    ram_group += nodes.Text(_header_label(data, "ram"))
    group_row += ram_group

    sub_row = nodes.row()
    thead += sub_row

    for _, label in INTERNAL_NVM_COLUMNS:
        entry = nodes.entry()
        entry["classes"] = ["memory-req-subhead"]
        entry += nodes.Text(label)
        sub_row += entry

    for part_id, label in external_columns:
        entry = nodes.entry()
        entry["classes"] = ["memory-req-subhead"]
        if part_id in ("slot1_ext", "slot3_ext"):
            label = f"{label} (used / free)"
        entry += nodes.Text(label)
        sub_row += entry

    ram_sub = nodes.entry()
    ram_sub["classes"] = ["memory-req-subhead"]
    ram_sub += nodes.Text(RAM_SUBHEAD)
    sub_row += ram_sub

    tbody = nodes.tbody()
    tgroup += tbody

    for sample in applicable_samples(data):
        row = nodes.row()
        tbody += row

        sample_entry = nodes.entry()
        sample_entry["classes"] = ["memory-req-sample"]
        _append_rst_cell(directive.state, sample_entry, sample["label"])
        row += sample_entry

        cells = sample_table_cells(data, sample, include_external=include_external)
        for value in cells:
            css = "memory-req-empty" if value == EMPTY_CELL else "memory-req-value"
            _append_text_cell(row, value, css_class=css)

    return table


def build_stack_table_nodes(directive: SphinxDirective, data: dict[str, Any]) -> nodes.table:
    samples = stack_samples(data)
    if not samples:
        raise ValueError(
            f"No stack measurements in docs/data/memory for board {data.get('board', {}).get('name')}"
        )

    table = nodes.table()
    table["classes"] = ["memory-req-table", "stack-req-table"]

    tgroup = nodes.tgroup(cols=5)
    table += tgroup
    tgroup.extend([nodes.colspec(colwidth=28)] + [nodes.colspec(colwidth=10)] * 4)

    thead = nodes.thead()
    tgroup += thead

    group_row = nodes.row()
    thead += group_row

    sample_group = nodes.entry()
    sample_group["morerows"] = 1
    sample_group += nodes.Text("Sample")
    group_row += sample_group

    main_group = nodes.entry()
    main_group["morecols"] = 1
    main_group["classes"] = ["memory-req-group-main"]
    main_group += nodes.Text("main thread")
    group_row += main_group

    zboss_group = nodes.entry()
    zboss_group["morecols"] = 1
    zboss_group["classes"] = ["memory-req-group-zboss"]
    zboss_group += nodes.Text("zboss thread")
    group_row += zboss_group

    sub_row = nodes.row()
    thead += sub_row

    for label in STACK_MAIN_SUBHEADS + STACK_ZBOSS_SUBHEADS:
        entry = nodes.entry()
        entry["classes"] = ["memory-req-subhead"]
        entry += nodes.Text(label)
        sub_row += entry

    tbody = nodes.tbody()
    tgroup += tbody

    for sample in samples:
        row = nodes.row()
        tbody += row

        sample_entry = nodes.entry()
        sample_entry["classes"] = ["memory-req-sample"]
        _append_rst_cell(directive.state, sample_entry, sample["label"])
        row += sample_entry

        for value in sample_stack_table_cells(sample):
            _append_text_cell(row, value)

    return table


class StackTable(SphinxDirective):
    """Render stack usage table from docs/data/memory/."""

    required_arguments = 0
    optional_arguments = 0
    final_argument_whitespace = True
    has_content = False
    option_spec = {
        "board": directives.unchanged_required,
    }

    def run(self) -> list[nodes.Node]:
        data = load_board_data(self.options["board"])
        return [build_stack_table_nodes(self, data)]


class MemoryTable(SphinxDirective):
    """Render memory requirements table from docs/data/memory/."""

    required_arguments = 0
    optional_arguments = 0
    final_argument_whitespace = True
    has_content = False
    option_spec = {
        "board": directives.unchanged_required,
    }

    def run(self) -> list[nodes.Node]:
        data = load_board_data(self.options["board"])
        return [build_memory_table_nodes(self, data)]


def add_memory_table_resources(app: Sphinx) -> None:
    static_path = RESOURCES_DIR.as_posix()
    if static_path not in app.config.html_static_path:
        app.config.html_static_path.append(static_path)
    app.add_css_file("memory_table.css")


def setup(app: Sphinx) -> dict[str, Any]:
    app.add_directive("memory-table", MemoryTable)
    app.add_directive("stack-table", StackTable)
    app.connect("builder-inited", add_memory_table_resources)
    return {
        "version": __version__,
        "parallel_read_safe": True,
        "parallel_write_safe": True,
    }
