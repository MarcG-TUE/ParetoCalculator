# some argument checking:

# test_cmd is the command to run with all its arguments
if( NOT test_cmd )
    message( FATAL_ERROR "Variable test_cmd not defined" )
endif()

# output_blessed contains the name of the "blessed" output file
if( NOT output_blessed )
    message( FATAL_ERROR "Variable output_blessed not defined" )
endif()

# output_test contains the name of the output file the test_cmd will produce
if( NOT output_test )
    message( FATAL_ERROR "Variable output_test not defined" )
endif()

execute_process(
    COMMAND ${test_cmd} ${test_cmd_arg1} ${test_cmd_arg2}
    OUTPUT_FILE ${output_test}
    message(STATUS "produced output" )
)

execute_process(
    COMMAND ${CMAKE_SOURCE_DIR}/scripts/compareoutput ${output_blessed} ${output_test}
    RESULT_VARIABLE test_not_successful
    OUTPUT_VARIABLE OUTVAR
)

if( test_not_successful )
    message( SEND_ERROR "${output_test} does not match ${output_blessed}!\n${OUTVAR}" )
endif( test_not_successful )
