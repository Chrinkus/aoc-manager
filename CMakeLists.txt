cmake_minimum_required(VERSION 3.18)

project(aoc-manager VERSION 0.1 LANGUAGES C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED YES)

add_compile_options(-Wall -Wextra -Werror)

include(GNUInstallDirs)

add_subdirectory(src)

