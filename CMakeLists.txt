cmake_minimum_required(VERSION 3.0.0)
project(Assembler VERSION 0.1.0)

find_package(scn CONFIG REQUIRED)

include(CTest)
enable_testing()

add_executable(Assembler main.cpp)
target_link_libraries(Assembler scn::scn)
target_include_directories(Assembler 
        PRIVATE header/ )

# CTEST
add_test(NAME AssemblyTest1 
        COMMAND Assembler /home/akaifox/_workspace/CMake_Assembler/test/assembly.txt /home/akaifox/_workspace/CMake_Assembler/test/machine.txt)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
set(CMAKE_CXX_STANDARD 20)
include(CPack)
