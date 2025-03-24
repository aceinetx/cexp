cmake_minimum_required(VERSION 3.20)
project(cexp)

set(CMAKE_C_STANDARD 90)
set(CMAKE_C_STANDARD_REQUIRED YES)

if(LINUX)
	add_compile_options(-Wall -Wextra -Wpedantic -Wno-type-limits -Wno-unused-parameter)
endif()

include_directories("include")
set(CEXP_SOURCES "src/cexp.c" "src/cexp_pow.c")

if(LINUX)
	add_library(cexp_static ${CEXP_SOURCES})
	set_target_properties(cexp_static PROPERTIES OUTPUT_NAME "cexp")

	add_library(cexp SHARED ${CEXP_SOURCES})
	set_target_properties(cexp PROPERTIES OUTPUT_NAME "cexp")
else()
	add_library(cexp_static ${CEXP_SOURCES})
	set_target_properties(cexp_static PROPERTIES OUTPUT_NAME "cexp")
endif()

add_executable(test "src/test.c")
target_link_libraries(test cexp_static)
