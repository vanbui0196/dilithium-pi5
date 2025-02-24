
execute_process(
    COMMAND $<TARGET_FILE:poly_vector_test>
    OUTPUT_VARIABLE TEST_OUTPUT
    OUTPUT_STRIP_TRAILING_WHITESPACE
)

# Read the expected output file from test directory
file(READ /home/vanbu/Desktop/dilithium-pi5/test/test_poly_vector_expected_output.txt EXPECTED_OUTPUT)

# Compare output (strip trailing whitespace to handle different line endings)
string(STRIP "${TEST_OUTPUT}" STRIPPED_TEST_OUTPUT)
string(STRIP "${EXPECTED_OUTPUT}" STRIPPED_EXPECTED_OUTPUT)

if(NOT STRIPPED_TEST_OUTPUT STREQUAL STRIPPED_EXPECTED_OUTPUT)
    message(FATAL_ERROR 
        "Vector output test failed!\n"
        "Expected output from: test_poly_vector_expected_output.txt\n"
        "Got different output\n"
        "Expected:\n${STRIPPED_EXPECTED_OUTPUT}\n"
        "Got:\n${STRIPPED_TEST_OUTPUT}"
    )
endif()
