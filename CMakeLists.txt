cmake_minimum_required(VERSION 3.10)
project(chip8_emulator)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find SDL3
find_package(SDL3 REQUIRED)

add_library(chip8_core src/Chip8.cpp)
target_include_directories(chip8_core PUBLIC include)

add_executable(chip8_emulator src/main.cpp)
target_link_libraries(chip8_emulator PRIVATE chip8_core SDL3::SDL3)

add_executable(chip8_tests tests/Chip8Tests.cpp)
target_link_libraries(chip8_tests PRIVATE chip8_core)

include(CTest)
if(BUILD_TESTING)
    add_test(NAME chip8_core_tests COMMAND chip8_tests)
endif()
