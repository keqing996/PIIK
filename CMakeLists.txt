cmake_minimum_required(VERSION 3.22)

project (CppPlatformIndependentInfrastructureKit)
set (CMAKE_CXX_STANDARD 20)

# Source files
file (GLOB_RECURSE PIIK_SRC ./src/*.cpp)

# Native win app
add_library                 (piik STATIC ${PIIK_SRC})
target_include_directories  (piik PUBLIC ./include/)

if (WIN32)
    target_link_libraries   (piik wsock32 ws2_32)
endif ()

# Test code
option (ENABLE_PIIK_TEST OFF)

if (ENABLE_PIIK_TEST)
    add_subdirectory(test)
endif ()