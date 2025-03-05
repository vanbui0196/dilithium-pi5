cmake_minimum_required(VERSION 3.25)

project(pqstudies
        VERSION 1.0.0
        LANGUAGES C CXX)

# Set the version to C++20
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Enable testing
enable_testing()

# Include directories
include_directories(${PROJECT_SOURCE_DIR}/include
                    ${PROJECT_SOURCE_DIR}/ref/include)

# Set the source file
set(SOURCES
    source/mldsa.cpp
    source/poly_algo.cpp
    source/utils.cpp
    ref/source/stream.cpp
    ref/source/fips202.c)

# Create the libraries for different modes
add_library(dilithium_mode2 ${SOURCES})
target_compile_definitions(dilithium_mode2 PRIVATE DILITHIUM_MODE=2)

add_library(dilithium_mode3 ${SOURCES})
target_compile_definitions(dilithium_mode3 PRIVATE DILITHIUM_MODE=3)

add_library(dilithium_mode5 ${SOURCES})
target_compile_definitions(dilithium_mode5 PRIVATE DILITHIUM_MODE=5)

# Create the executables for different modes
add_executable(mldsa_mode2 test/main.cpp)
target_link_libraries(mldsa_mode2 PRIVATE dilithium_mode2)
target_compile_definitions(mldsa_mode2 PRIVATE DILITHIUM_MODE=2)

add_executable(mldsa_mode3 test/main.cpp)
target_link_libraries(mldsa_mode3 PRIVATE dilithium_mode3)
target_compile_definitions(mldsa_mode3 PRIVATE DILITHIUM_MODE=3)

add_executable(mldsa_mode5 test/main.cpp)
target_link_libraries(mldsa_mode5 PRIVATE dilithium_mode5)
target_compile_definitions(mldsa_mode5 PRIVATE DILITHIUM_MODE=5)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)