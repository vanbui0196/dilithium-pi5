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

# Create the library dilithium
add_library(dilithium ${SOURCES})

add_executable(mldsa
    test/main.cpp
)

# Link the test executable with the library
target_link_libraries(mldsa PRIVATE dilithium)


set(CMAKE_EXPORT_COMPILE_COMMANDS ON)