#!/usr/bin/env bash
set -euo pipefail

DEFAULT_ROM="roms/PONG.ch8"
BUILD_DIR="build"
PROJECT_BIN="${BUILD_DIR}/chip8_emulator"

usage() {
    cat <<USAGE
Usage: $0 [rom_path]

Reconfigures (if needed), rebuilds, and runs the Chip-8 emulator.
If no ROM is provided, defaults to ${DEFAULT_ROM}.

Environment variables:
  CMAKE_ARGS   Additional arguments forwarded to the cmake configure step.
USAGE
}

if [[ "${1-}" == "-h" || "${1-}" == "--help" ]]; then
    usage
    exit 0
fi

ROM_PATH=${1:-${DEFAULT_ROM}}

if [[ ! -f "${ROM_PATH}" ]]; then
    echo "Error: ROM not found at '${ROM_PATH}'." >&2
    exit 1
fi

cmake -S . -B "${BUILD_DIR}" ${CMAKE_ARGS:-}
cmake --build "${BUILD_DIR}"

"${PROJECT_BIN}" "${ROM_PATH}"
