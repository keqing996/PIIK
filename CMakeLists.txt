cmake_minimum_required(VERSION 3.22)

project (CppPlatformIndependentInfrastructureKit)
set (CMAKE_CXX_STANDARD 20)

# Set output path
set (LIBRARY_OUTPUT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/artifacts/${CMAKE_CXX_COMPILER_ID}/${CMAKE_BUILD_TYPE}/)

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
    add_executable          (test_socket ./test/TestSocket.cpp)
    target_link_libraries   (test_socket piik)

    add_executable          (test_string ./test/TestString.cpp)
    target_link_libraries   (test_string piik)
    add_test                (NAME test_string COMMAND test_string)

    add_executable          (test_console ./test/TestConsole.cpp)
    target_link_libraries   (test_console piik)

    add_executable          (test_process ./test/TestProcess.cpp)
    target_link_libraries   (test_process piik)

    add_executable          (test_system ./test/TestSystem.cpp)
    target_link_libraries   (test_system piik)

    add_executable          (test_command_line ./test/TestCommandLine.cpp)
    target_link_libraries   (test_command_line piik)
endif ()