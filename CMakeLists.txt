cmake_minimum_required(VERSION 3.25)

project(pqstudies
        VERSION 1.0.0
        LANGUAGES C CXX)

# Set the version to C++20
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


# Include directories
include_directories(${PROJECT_SOURCE_DIR}/include
                    ${PROJECT_SOURCE_DIR}/ref/include)

#set the source file
set(SOURCES
    source/ntt.cpp
    source/poly_algo.cpp
    source/utils.cpp
    ref/source/stream.cpp
    ref/source/fips202.c)

# create the library dilithium
add_library(dilithium ${SOURCES})

add_executable(main_test
    test/main.cpp
)

# Link the test executable with the library
target_link_libraries(main_test PRIVATE dilithium)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)