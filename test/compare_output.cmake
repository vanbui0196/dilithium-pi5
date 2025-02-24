# Run the test program and capture its output
execute_process(
    COMMAND ${TEST_PROG}
    OUTPUT_FILE ${TEST_OUTPUT}
    RESULT_VARIABLE TEST_RESULT
)

# Check if the program ran successfully
if(NOT TEST_RESULT EQUAL 0)
    message(FATAL_ERROR "Test program failed to run")
endif()

# Read both files
file(READ ${EXPECTED_OUTPUT} EXPECTED_CONTENT)
file(READ ${TEST_OUTPUT} ACTUAL_CONTENT)

# Strip whitespace and normalize line endings
string(REPLACE "\r\n" "\n" EXPECTED_CONTENT "${EXPECTED_CONTENT}")
string(REPLACE "\r\n" "\n" ACTUAL_CONTENT "${ACTUAL_CONTENT}")
string(STRIP "${EXPECTED_CONTENT}" EXPECTED_CONTENT)
string(STRIP "${ACTUAL_CONTENT}" ACTUAL_CONTENT)

# Compare contents
if(NOT EXPECTED_CONTENT STREQUAL ACTUAL_CONTENT)
    message(FATAL_ERROR "Output does not match expected content.\n"
        "Expected:\n${EXPECTED_CONTENT}\n"
        "Got:\n${ACTUAL_CONTENT}")
endif()