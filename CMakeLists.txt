
cmake_minimum_required(VERSION 3.10)
project(chip8_emulator)

# Find SDL3
find_package(SDL3 REQUIRED)

# Add executable
add_executable(chip8_emulator src/main.cpp src/Chip8.cpp)

# Add include directory
target_include_directories(chip8_emulator PUBLIC include)

# Link SDL3
target_link_libraries(chip8_emulator PRIVATE SDL3::SDL3)
