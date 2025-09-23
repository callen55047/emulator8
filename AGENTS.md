# Repository Guidelines

## Project Structure & Module Organization
Runtime code lives in `src/`, with the main loop in `src/main.cpp` and the Chip-8 core in `src/Chip8.cpp`. Public headers stay in `include/`, test sources sit in `tests/`, and reference ROMs belong in `roms/`. CMake places generated files under `build/`; keep that tree out of version control and create throwaway subfolders there for experiments.

## Build, Run, and Development Commands
Commands verified on macOS 14 (Apple Clang + Homebrew SDL3) and Ubuntu 22.04 (GCC + `libsdl3-dev`):
```bash
cmake -S . -B build [-DSDL3_DIR=/opt/homebrew/opt/sdl3]
cmake --build build
./build/chip8_emulator roms/PONG.ch8
```
On Windows 11, MSYS2 with `pacman -S mingw-w64-ucrt-x86_64-sdl3` or vcpkg (`vcpkg install sdl3`) works; point CMake at `%VCPKG_ROOT%/installed/x64-windows`. Use `cmake --build build --target clean` before regenerating toolchains, and pass `--parallel` during tight edit loops. When SDL3 lives elsewhere, export `CMAKE_PREFIX_PATH` or adjust `SDL3_DIR` to point at the install root.

## Coding Style & Naming Conventions
Match the existing C++ style: 4-space indentation, braces on the same line, and `std::array`/`std::vector` for containers. Classes stay in PascalCase (`Chip8`), member functions in camelCase (`emulateCycle`), and constants in SCREAMING_SNAKE_CASE (`VIDEO_HEIGHT`). Keep interfaces in headers and implementation in `src/`, prefer standard-library facilities over raw pointers, and add brief `//` comments only when intent is non-obvious.

## Testing Guidelines
Core regression coverage runs through CTest:
```bash
cmake --build build --target chip8_tests
ctest --test-dir build
```
The suite exercises instruction decoding (LD, ADD, CALL/RET, CLS) and is safe for CI. Continue manual smoke checks by loading representative ROMs (e.g., `roms/PONG.ch8`) and watching console diagnostics like `debugMemory()`. Name future test files after the behavior under test (e.g., `chip8_timer_tests.cpp`) so they auto-align with the current pattern.

## Commit & Pull Request Guidelines
Follow the existing Git history: keep commit subjects short and imperative (`fix timer decrement`), bundle related changes, and avoid formatting-only churn. Pull requests should summarise functional impact, list ROMs used for manual verification, and mention any SDL3 or toolchain prerequisites. Attach screenshots or short clips when the change affects rendering, and link issues or discussions for added context.

## SDL3 Setup Notes
Install SDL3 through your package manager (Homebrew `brew install sdl3`) or from source, then ensure CMake can resolve it via `CMAKE_PREFIX_PATH` or `SDL3_DIR`. Re-run `cmake -S . -B build` after environment changes, and confirm the toolchain with `cmake --build build && ctest --test-dir build` before large refactors.
