cmake_minimum_required(VERSION 3.20)
project(chip8 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)  # нужен редактору для подсказок

find_package(SDL3 REQUIRED CONFIG)

add_executable(chip8 src/main.cpp)
target_link_libraries(chip8 PRIVATE SDL3::SDL3)

# Строгие предупреждения всегда, а в Debug ещё и санитайзеры:
# они ловят выход за границы массива и неопределённое поведение прямо во время работы
target_compile_options(chip8 PRIVATE -Wall -Wextra -Wpedantic
    $<$<CONFIG:Debug>:-fsanitize=address,undefined -fno-omit-frame-pointer>)
target_link_options(chip8 PRIVATE
    $<$<CONFIG:Debug>:-fsanitize=address,undefined>)
