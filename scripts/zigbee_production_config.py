#!/usr/bin/env python3
#
# Copyright (c) 2026 Nordic Semiconductor ASA
#
# SPDX-License-Identifier: LicenseRef-Nordic-5-Clause

"""Generate a ZBOSS production configuration for the zboss_product_config partition.

The production configuration is read by the ZBOSS stack at startup. It can provide
the install code, passcode and IEEE (EUI64) address of a device, the 2.4 GHz channel
mask and TX power, and application-specific data.
Generate a separate production configuration for each device.
"""

import argparse
import pickle
import re
import struct
import sys
import zlib
from pathlib import Path

import yaml
from intelhex import IntelHex

PARTITION_LABEL = "zboss_product_config"

# Magic expected by the OSIF layer at the beginning of the partition.
OSIF_MAGIC = bytes([0xE7, 0x37, 0xDD, 0xF6])

PROD_CFG_VERSION_4 = 4
CHANNEL_PAGES = 10
TX_POWER_CHANNELS = 27
# Makes the MAC layer use its default TX power for the channel.
TX_POWER_INVALID_DBM = 127
# The TX power table of page 0 is indexed by the channel number.
CHANNELS_2_4_GHZ = range(11, 27)

ALL_2_4_GHZ_CHANNELS_MASK = 0x07FFF800

IC_SIZE = 16
IC_CRC_SIZE = 2
IC_FIELD_SIZE = IC_SIZE + IC_CRC_SIZE
IC_TYPE_128 = 3
PASSCODE_SIZE = 4
# Setup code alphabet of BDB 3.1, section 6.12.1, mapping each character to 5 bits.
PASSCODE_BASE32_ALPHABET = "ABCDEFGHJKLMNPQRSTUVWXYZ23456789"
# 20 to 30 bits recommended by BDB 3.1, section 6.12, and at most 32 bits stored by ZBOSS.
PASSCODE_BASE32_LENGTHS = range(4, 7)
OPTIONS_PASSCODE_PRESENT = 0x40
APP_DATA_MAX_SIZE = 128

# Header (crc, len, version) followed by the version 4 body.
HDR_FORMAT = "<IHH"
BODY_FORMAT = (
    f"<{CHANNEL_PAGES}I8s{CHANNEL_PAGES * TX_POWER_CHANNELS}bB{IC_FIELD_SIZE}s{PASSCODE_SIZE}s"
)


def install_code_crc(data: bytes) -> int:
    """CRC-16/X-25 of the install code, as defined by the Zigbee specification."""
    crc = 0xFFFF
    for byte in data:
        crc ^= byte
        for _ in range(8):
            crc = (crc >> 1) ^ 0x8408 if crc & 1 else crc >> 1
    return crc ^ 0xFFFF


def parse_hex(value: str) -> bytes:
    try:
        return bytes.fromhex(value)
    except ValueError as e:
        raise argparse.ArgumentTypeError(f"not a hexadecimal string: {value}") from e


def parse_install_code(value: str) -> bytes:
    ic = parse_hex(value)
    expected_crc = install_code_crc(ic[:IC_SIZE]).to_bytes(IC_CRC_SIZE, "little")

    if len(ic) == IC_SIZE:
        return ic + expected_crc
    if len(ic) != IC_FIELD_SIZE:
        raise argparse.ArgumentTypeError(
            f"the install code must be {IC_SIZE} bytes, optionally followed by its "
            f"{IC_CRC_SIZE}-byte CRC"
        )
    if ic[IC_SIZE:] != expected_crc:
        raise argparse.ArgumentTypeError(
            f"invalid install code CRC, expected {expected_crc.hex().upper()}"
        )
    return ic


def parse_passcode(value: str) -> bytes:
    if len(value) == 2 * PASSCODE_SIZE:
        passcode = parse_hex(value)
        if len(passcode) != PASSCODE_SIZE:
            raise argparse.ArgumentTypeError(
                f"the hexadecimal passcode must be {PASSCODE_SIZE} bytes long"
            )
        return passcode

    code = value.upper()
    if len(code) not in PASSCODE_BASE32_LENGTHS or any(
        c not in PASSCODE_BASE32_ALPHABET for c in code
    ):
        raise argparse.ArgumentTypeError(
            f"the passcode must be a setup code of {PASSCODE_BASE32_LENGTHS.start} to "
            f"{PASSCODE_BASE32_LENGTHS.stop - 1} characters from {PASSCODE_BASE32_ALPHABET}, "
            f"or {PASSCODE_SIZE} bytes as a hexadecimal string"
        )
    # The first character holds the least significant bits.
    passcode = sum(PASSCODE_BASE32_ALPHABET.index(c) << (5 * i) for i, c in enumerate(code))
    return passcode.to_bytes(PASSCODE_SIZE, "little")


def parse_eui64(value: str) -> bytes:
    eui64 = parse_hex(value)
    if len(eui64) != 8:
        raise argparse.ArgumentTypeError("the EUI64 must be 8 bytes long")
    if not any(eui64):
        raise argparse.ArgumentTypeError("the EUI64 must not be all zeros")
    return eui64


def split_qr_code(value: str, separator: str) -> list[str]:
    """Split a QR code string, skipping separators escaped with '*'."""
    parts = [""]
    chars = iter(value)
    for c in chars:
        if c == "*":
            parts[-1] += c + next(chars, "")
        elif c == separator:
            parts.append("")
        else:
            parts[-1] += c
    return parts


def parse_qr_code(value: str) -> tuple[bytes, bytes]:
    """Read the install code and EUI64 from a QR code string, as defined in CSA 18-01000."""
    global_params: dict[str, str] = {}
    zigbee_params: dict[str, str] | None = None

    for parameter_list in split_qr_code(value.replace(" ", ""), "%"):
        interface, *parameters = split_qr_code(parameter_list, "$")
        key, _, shortcut = interface.partition(":")
        params = {
            k: re.sub(r"\*(.)", r"\1", v) for k, _, v in (p.partition(":") for p in parameters)
        }
        if key == "G":
            global_params.update(params)
        elif key == "Z":
            if zigbee_params is not None:
                raise argparse.ArgumentTypeError("the QR code has more than one Zigbee interface")
            zigbee_params = params | ({"A": shortcut} if shortcut else {})

    if zigbee_params is None:
        raise argparse.ArgumentTypeError("the QR code has no Zigbee interface (Z)")

    # Zigbee interface parameters override the global ones.
    params = global_params | zigbee_params
    if "A" not in params or "I" not in params:
        raise argparse.ArgumentTypeError(
            "the QR code must contain the EUI64 (A) and install code (I)"
        )
    if len(params["I"]) != 2 * IC_FIELD_SIZE:
        raise argparse.ArgumentTypeError(
            f"the install code in the QR code must be {2 * IC_FIELD_SIZE} hexadecimal characters"
        )
    return parse_install_code(params["I"]), parse_eui64(params["A"])


def parse_channel_mask(value: str) -> int:
    mask = int(value, 0)
    if mask == 0 or mask & ~ALL_2_4_GHZ_CHANNELS_MASK:
        raise argparse.ArgumentTypeError(
            f"the channel mask must be a non-zero subset of {ALL_2_4_GHZ_CHANNELS_MASK:#010x}"
        )
    return mask


def parse_tx_power(value: str) -> int:
    tx_power = int(value, 0)
    if not -128 <= tx_power < TX_POWER_INVALID_DBM:
        raise argparse.ArgumentTypeError(
            f"the TX power must be between -128 and {TX_POWER_INVALID_DBM - 1} dBm"
        )
    return tx_power


def parse_app_data(value: str) -> bytes:
    app_data = parse_hex(value)
    if not 0 < len(app_data) <= APP_DATA_MAX_SIZE:
        raise argparse.ArgumentTypeError(
            f"the application data must be 1 to {APP_DATA_MAX_SIZE} bytes long"
        )
    return app_data


def build_production_config(args: argparse.Namespace) -> bytes:
    channel_masks = [0] * CHANNEL_PAGES
    if args.channel_mask is not None:
        channel_masks[0] = args.channel_mask

    tx_power = [TX_POWER_INVALID_DBM] * (CHANNEL_PAGES * TX_POWER_CHANNELS)
    if args.tx_power is not None:
        for channel in CHANNELS_2_4_GHZ:
            tx_power[channel] = args.tx_power

    options = 0
    ic_field = bytes(IC_FIELD_SIZE)
    if args.install_code is not None:
        options |= IC_TYPE_128
        ic_field = args.install_code

    passcode = bytes(PASSCODE_SIZE)
    if args.passcode is not None:
        options |= OPTIONS_PASSCODE_PRESENT
        passcode = args.passcode

    body = struct.pack(
        BODY_FORMAT,
        *channel_masks,
        # ZBOSS stores IEEE addresses in little-endian byte order.
        args.eui64[::-1] if args.eui64 is not None else bytes(8),
        *tx_power,
        options,
        ic_field,
        passcode,
    )
    # The stack passes the data that follows the stack section to the application.
    body += args.app_data or b""

    length = struct.calcsize(HDR_FORMAT) + len(body)
    len_and_version = struct.pack("<HH", length, PROD_CFG_VERSION_4)
    crc = zlib.crc32(len_and_version + body)

    return OSIF_MAGIC + struct.pack("<I", crc) + len_and_version + body


def read_cmake_cache(build_dir: Path, name: str) -> str | None:
    cache = build_dir / "CMakeCache.txt"
    if not cache.is_file():
        return None
    for line in cache.read_text().splitlines():
        if line.startswith(f"{name}:"):
            return line.split("=", 1)[1]
    return None


def partition_address_from_pm(build_dir: Path) -> int | None:
    partitions = build_dir / "partitions.yml"
    if not partitions.is_file():
        return None

    pm = yaml.safe_load(partitions.read_text())
    if PARTITION_LABEL not in pm:
        sys.exit(f"error: no {PARTITION_LABEL} partition in {partitions}")
    return pm[PARTITION_LABEL]["address"]


def application_build_dir(build_dir: Path) -> Path:
    domains = build_dir / "domains.yaml"
    if not domains.is_file():
        return build_dir

    content = yaml.safe_load(domains.read_text())
    for domain in content["domains"]:
        if domain["name"] == content["default"]:
            return Path(domain["build_dir"])
    sys.exit(f"error: default domain not found in {domains}")


def partition_address_from_dts(build_dir: Path) -> int:
    app_dir = application_build_dir(build_dir)
    edt_pickle = app_dir / "zephyr" / "edt.pickle"
    zephyr_base = read_cmake_cache(app_dir, "ZEPHYR_BASE")
    if not edt_pickle.is_file() or zephyr_base is None:
        sys.exit(f"error: {app_dir} is not a complete Zephyr build directory")

    # edtlib is needed to unpickle the devicetree.
    sys.path.insert(0, str(Path(zephyr_base) / "scripts" / "dts" / "python-devicetree" / "src"))
    with edt_pickle.open("rb") as f:
        edt = pickle.load(f)

    node = edt.label2node.get(PARTITION_LABEL)
    if node is None:
        sys.exit(f"error: no {PARTITION_LABEL} node label in the devicetree of {app_dir}")

    flash = node.parent.parent
    if "soc-nv-flash" not in flash.compats:
        sys.exit(f"error: {PARTITION_LABEL} is not located in the internal non-volatile memory")
    return flash.regs[0].addr + node.regs[0].addr


def partition_address(build_dir: Path) -> int:
    address = partition_address_from_pm(build_dir)
    return address if address is not None else partition_address_from_dts(build_dir)


def parse_args() -> argparse.Namespace:
    parser = argparse.ArgumentParser(
        description=__doc__,
        formatter_class=argparse.RawDescriptionHelpFormatter,
        allow_abbrev=False,
    )
    location = parser.add_mutually_exclusive_group(required=True)
    location.add_argument(
        "-d",
        "--build-dir",
        type=Path,
        help="build directory of the application, used to find the partition address",
    )
    location.add_argument(
        "--address",
        type=lambda v: int(v, 0),
        help="address of the zboss_product_config partition",
    )
    parser.add_argument(
        "--install-code",
        type=parse_install_code,
        help="128-bit install code as a hexadecimal string, with or without its CRC",
    )
    parser.add_argument(
        "--eui64",
        type=parse_eui64,
        help="IEEE (EUI64) address of the device, most significant byte first",
    )
    parser.add_argument(
        "--qr-code",
        type=parse_qr_code,
        help="QR code string of the device, as defined in CSA document 18-01000, "
        "instead of --install-code and --eui64",
    )
    parser.add_argument(
        "--channel-mask",
        type=parse_channel_mask,
        help="2.4 GHz channel mask that overrides the one set by the application",
    )
    parser.add_argument(
        "--passcode",
        type=parse_passcode,
        help="Zigbee R23 PAKE passcode, as a setup code of 4 to 6 characters (BDB 3.1, "
        "section 6.12.1) or as a 4-byte hexadecimal string",
    )
    parser.add_argument(
        "--tx-power",
        type=parse_tx_power,
        help="TX power in dBm for all 2.4 GHz channels, instead of the stack default",
    )
    parser.add_argument(
        "--app-data",
        type=parse_app_data,
        help="application-specific data passed with the ZB_ZDO_SIGNAL_PRODUCTION_CONFIG_READY "
        f"signal, as a hexadecimal string of up to {APP_DATA_MAX_SIZE} bytes",
    )
    parser.add_argument(
        "-o",
        "--output",
        type=Path,
        default=Path("zboss_production_config.hex"),
        help="output file, in Intel HEX format (default: %(default)s)",
    )

    args = parser.parse_args()
    if args.qr_code is not None:
        if args.install_code is not None or args.eui64 is not None:
            parser.error("--qr-code cannot be used with --install-code or --eui64")
        args.install_code, args.eui64 = args.qr_code

    content = ("install_code", "eui64", "channel_mask", "passcode", "tx_power", "app_data")
    if all(getattr(args, name) is None for name in content):
        parser.error(
            "provide at least one of " + ", ".join(f"--{n.replace('_', '-')}" for n in content)
        )
    return args


def main() -> None:
    args = parse_args()
    address = args.address if args.address is not None else partition_address(args.build_dir)

    hex_file = IntelHex()
    hex_file.frombytes(build_production_config(args), offset=address)
    hex_file.write_hex_file(str(args.output))

    print(f"Production configuration written to {args.output} at address {address:#x}")
    if args.install_code is not None:
        print(f"Install code with CRC: {args.install_code.hex().upper()}")
    if args.passcode is not None:
        print(f"Passcode: {args.passcode.hex().upper()}")
    if args.eui64 is not None:
        print(f"EUI64: {args.eui64.hex().upper()}")


if __name__ == "__main__":
    main()
