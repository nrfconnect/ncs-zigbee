"""
Copyright (c) 2026 Nordic Semiconductor ASA

SPDX-License-Identifier: LicenseRef-Nordic-5-Clause

Sphinx extension for stack usage bar charts in Zigbee documentation.
"""

from __future__ import annotations

import html
from typing import Any

from docutils import nodes
from docutils.parsers.rst import directives
from docutils.statemachine import StringList
from memory_data import stack_samples, stack_thread_sizes, load_board_data
from sphinx.application import Sphinx
from sphinx.util.docutils import SphinxDirective

__version__ = "0.1.0"

RESOURCES_DIR = __import__("pathlib").Path(__file__).parent / "static"

THREAD_COLORS = {
    "main": ("#2563eb", "#93c5fd"),
    "zboss": ("#0891b2", "#a5f3fc"),
}


def _b_label(value_b: int) -> str:
    return f"{value_b} B"


def _render_thread_bar(thread_id: str, label: str, used_b: int, size_b: int) -> str:
    used_color, free_color = THREAD_COLORS[thread_id]
    used_pct = min(100.0, 100.0 * used_b / size_b) if size_b else 0
    free_pct = 100.0 - used_pct
    free_b = max(size_b - used_b, 0)
    title = (
        f"{label}: {_b_label(size_b)} "
        f"({_b_label(used_b)} used, {_b_label(free_b)} free)"
    )
    inner = (
        f'<span class="memory-viz-fill memory-viz-fill-used" '
        f'style="width:{used_pct:.2f}%;background:{used_color}"></span>'
        f'<span class="memory-viz-fill memory-viz-fill-free" '
        f'style="width:{free_pct:.2f}%;background:{free_color}"></span>'
    )
    return (
        f'<div class="memory-viz-bar" role="img" aria-label="{html.escape(label, quote=True)}">'
        f'<div class="memory-viz-segment memory-viz-segment-{thread_id}" '
        f'style="flex:1 1 100%" '
        f'data-tooltip="{html.escape(title, quote=True)}">'
        f"{inner}</div></div>"
    )


def _parse_rst_label(directive: SphinxDirective, text: str) -> nodes.Element:
    label = nodes.paragraph()
    label["classes"] = ["memory-viz-sample-label"]
    content = StringList([text], "<stack_viz>")
    directive.state.nested_parse(content, 0, label)
    if len(label) == 1 and isinstance(label[0], nodes.paragraph):
        inner = label[0]
        label.remove(inner)
        label.extend(inner.children)
    return label


def _legend_swatch(color: str) -> str:
    return f'<span class="memory-viz-swatch" style="background:{color}"></span>'


def _legend_items_used_free(thread_id: str, label_base: str) -> str:
    used_color, free_color = THREAD_COLORS[thread_id]
    items: list[str] = []
    for suffix, color in (("used", used_color), ("free", free_color)):
        items.append(
            f'<span class="memory-viz-legend-item">'
            f"{_legend_swatch(color)}"
            f"{label_base} ({suffix})"
            f"</span>"
        )
    return "".join(items)


def _render_board_header(main_size_b: int, zboss_size_b: int) -> str:
    return (
        f'<div class="stack-viz-chart-header">'
        f'<div class="memory-viz-panel-title memory-viz-panel-title-sample">Sample</div>'
        f'<div class="stack-viz-header-thread">'
        f'<div class="memory-viz-panel-title">'
        f'main thread ({_b_label(main_size_b)})</div>'
        f'<div class="memory-viz-legend">{_legend_items_used_free("main", "main")}</div>'
        f"</div>"
        f'<div class="stack-viz-header-thread">'
        f'<div class="memory-viz-panel-title">'
        f'zboss thread ({_b_label(zboss_size_b)})</div>'
        f'<div class="memory-viz-legend">{_legend_items_used_free("zboss", "zboss")}</div>'
        f"</div>"
        f"</div>"
    )


def _grid_style(main_size_b: int, zboss_size_b: int) -> str:
    return (
        f"--stack-viz-main-fr:{main_size_b}fr;"
        f"--stack-viz-zboss-fr:{zboss_size_b}fr"
    )


def _build_board_nodes(directive: SphinxDirective, board: str) -> nodes.Element:
    data = load_board_data(board)
    samples = stack_samples(data)
    if not samples:
        paragraph = nodes.paragraph()
        paragraph += nodes.Text(f"No stack measurements for board {board}.")
        return paragraph

    main_size_b, zboss_size_b = stack_thread_sizes(samples)
    style = _grid_style(main_size_b, zboss_size_b)

    wrapper = nodes.container()
    wrapper["classes"] = ["stack-viz-board-outer"]
    wrapper += nodes.raw(
        "",
        f'<div class="stack-viz-board" data-board="{board}" style="{style}">',
        format="html",
    )

    board_node = nodes.container()
    board_node["classes"] = ["stack-viz-board-inner"]
    board_node += nodes.raw(
        "",
        _render_board_header(main_size_b, zboss_size_b),
        format="html",
    )

    samples_node = nodes.container()
    samples_node["classes"] = ["stack-viz-samples"]

    for sample in samples:
        stack = sample["stack"]
        row = nodes.container()
        row["classes"] = ["stack-viz-sample-row"]
        row += _parse_rst_label(directive, sample["label"])
        row += nodes.raw(
            "",
            '<div class="memory-viz-bar-wrap">'
            + _render_thread_bar(
                "main",
                "main thread",
                int(stack["main"]["used_b"]),
                int(stack["main"]["size_b"]),
            )
            + "</div>",
            format="html",
        )
        row += nodes.raw(
            "",
            '<div class="memory-viz-bar-wrap">'
            + _render_thread_bar(
                "zboss",
                "zboss thread",
                int(stack["zboss"]["used_b"]),
                int(stack["zboss"]["size_b"]),
            )
            + "</div>",
            format="html",
        )
        samples_node += row

    board_node += samples_node
    wrapper += board_node
    wrapper += nodes.raw("", "</div>", format="html")
    return wrapper


class StackBoard(SphinxDirective):
    """Render stack usage charts for a board defined in docs/data/memory/."""

    required_arguments = 0
    optional_arguments = 0
    final_argument_whitespace = True
    has_content = False
    option_spec = {
        "board": directives.unchanged_required,
    }

    def run(self) -> list[nodes.Node]:
        return [_build_board_nodes(self, self.options["board"])]


def add_stack_viz_resources(app: Sphinx) -> None:
    static_path = RESOURCES_DIR.as_posix()
    if static_path not in app.config.html_static_path:
        app.config.html_static_path.append(static_path)
    app.add_css_file("stack_viz.css")


def setup(app: Sphinx) -> dict[str, Any]:
    app.add_directive("stack-board", StackBoard)
    app.connect("builder-inited", add_stack_viz_resources)
    return {
        "version": __version__,
        "parallel_read_safe": True,
        "parallel_write_safe": True,
    }
