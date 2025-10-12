cmake_minimum_required(VERSION 3.10)
project(chip8_emulator)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Optional: show more helpful compiler warnings
if (CMAKE_CXX_COMPILER_ID MATCHES "Clang|GNU")
    add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# Point CMake to your include/ directory
include_directories(include)

# --- Find SDL3 and SDL3_ttf ---
# These should work automatically with Homebrew-installed versions
find_package(SDL3 REQUIRED)
find_package(SDL3_ttf REQUIRED)

# --- chip8_core static library ---
add_library(chip8_core
        src/App.cpp
        src/app/emulator/Chip8.cpp
        src/app/Window.cpp
        src/app/Emulator.cpp
        src/app/Menu.cpp
)
target_include_directories(chip8_core PUBLIC include)
target_link_libraries(chip8_core
        PUBLIC
        SDL3::SDL3
        SDL3_ttf::SDL3_ttf
)

# --- Main executable ---
add_executable(chip8_emulator src/main.cpp)
target_link_libraries(chip8_emulator PRIVATE chip8_core)

# --- Optional: unit tests ---
add_executable(chip8_tests tests/Chip8Tests.cpp)
target_link_libraries(chip8_tests PRIVATE chip8_core)

include(CTest)
if(BUILD_TESTING)
    add_test(NAME chip8_core_tests COMMAND chip8_tests)
endif()

# --- Helpful runtime message ---
message(STATUS "SDL3 include dir: ${SDL3_INCLUDE_DIRS}")
message(STATUS "SDL3_ttf include dir: ${SDL3_ttf_INCLUDE_DIRS}")