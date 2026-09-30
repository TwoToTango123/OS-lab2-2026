cmake_minimum_required(VERSION 3.16)
project(os_lab2 LANGUAGES C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

# compile_commands.json для редактора (clangd / VS Code), чтобы он видел include/
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif()

add_compile_options(-Wall -Wextra -Wpedantic)
# Открывает POSIX-функции (getline и др.) при строгом -std=c11
add_compile_definitions(_POSIX_C_SOURCE=200809L)

# Общий код для обеих программ
add_library(utils STATIC src/utils.c)
target_include_directories(utils PUBLIC include)

add_executable(child src/child.c)
target_link_libraries(child PRIVATE utils)

add_executable(parent src/parent.c)
target_link_libraries(parent PRIVATE utils)
# Абсолютный путь к собранному child, чтобы parent находил его из любой папки
target_compile_definitions(parent PRIVATE CHILD_PATH="$<TARGET_FILE:child>")
add_dependencies(parent child)
