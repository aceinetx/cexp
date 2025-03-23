cmake_minimum_required(VERSION 3.20)
project(cexp)

set(CMAKE_C_STANDARD 90)
set(CMAKE_C_STANDARD_REQUIRED YES)

if(LINUX)
	add_compile_options(-Wall -Wextra -Wpedantic -Wno-type-limits -Wno-unused-parameter)
endif()

include_directories("include")

add_library("cexp" "src/cexp.c")
target_link_libraries("cexp" "m")

add_executable("test" "src/test.c")
target_link_libraries("test" cexp)
