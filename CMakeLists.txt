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

add_executable(main_test
    test/main.cpp
)

add_executable(poly_algo_test
    test/test_poly_algo.cpp
)

add_executable(poly_vector_test
    test/test_poly_vector.cpp
)

add_executable(sig_test
    test/test_signature.cpp
)

# Link the test executable with the library
target_link_libraries(main_test PRIVATE dilithium)
target_link_libraries(poly_algo_test PRIVATE dilithium)
target_link_libraries(poly_vector_test PRIVATE dilithium)
target_link_libraries(sig_test PRIVATE dilithium)

# Add a custom command to run the test and capture output
add_custom_command(
    TARGET poly_vector_test
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E echo "Running poly_vector_test and comparing output..."
    COMMAND ${CMAKE_COMMAND} -E copy_if_different 
            ${CMAKE_SOURCE_DIR}/test/test_poly_vector_expected_output.txt
            ${CMAKE_BINARY_DIR}/expected_output.txt
    COMMAND ${CMAKE_COMMAND} -E touch ${CMAKE_BINARY_DIR}/test_output.txt
)

# Add the test with output comparison
add_test(
    NAME poly_vector_test_output
    COMMAND ${CMAKE_COMMAND}
        -DTEST_PROG=$<TARGET_FILE:poly_vector_test>
        -DEXPECTED_OUTPUT=${CMAKE_BINARY_DIR}/expected_output.txt
        -DTEST_OUTPUT=${CMAKE_BINARY_DIR}/test_output.txt
        -P ${CMAKE_SOURCE_DIR}/test/compare_output.cmake
)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)