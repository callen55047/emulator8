# Chip-8 Emulator

A minimal Chip-8 interpreter written in modern C++17 with an SDL3 + OpenGL renderer. The project is structured around a reusable `chip8_core` library, a desktop frontend, and a lightweight regression test harness.

## Prerequisites (macOS)
- macOS 14 (tested) with Xcode Command Line Tools
- Homebrew packages: `brew install cmake sdl3`
- OpenGL-capable GPU/driver (SDL is configured to request the `opengl` renderer)

If SDL3 is installed somewhere other than `/opt/homebrew`, note the prefix so CMake can locate the headers and libraries.

## Configure, Build, and Run
```bash
cmake -S . -B build [-DSDL3_DIR=/opt/homebrew/opt/sdl3]
cmake --build build
./build/chip8_emulator roms/PONG.ch8
```
The executable expects a `.ch8` ROM path. Sample programs live in `roms/`; all files have already been renamed with the correct extension. The default window is a 64×32 surface scaled 10× for clarity.

## Testing
Core instruction behavior is covered by a small CTest suite:
```bash
cmake --build build --target chip8_tests
ctest --test-dir build
```
Add new test cases under `tests/` and they will be picked up automatically by CTest.

## Controls
The hexadecimal keypad is mapped to SDL scancodes commonly found on QWERTY layouts:
```
1 2 3 4      →   1 2 3 4
Q W E R      →   4 5 6 D
A S D F      →   7 8 9 E
Z X C V      →   A 0 B F
```
Press `Esc` or close the window to exit.

## Project Layout
- `include/`, `src/` – Chip-8 core implementation
- `tests/` – Instruction-level regression tests
- `roms/` – Sample Chip-8 programs (`*.ch8`)
- `build/` – CMake build artifacts (not tracked)

Happy hacking!
