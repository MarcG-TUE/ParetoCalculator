
# does the console application run
add_test(RunParetoCalculatorConsoleBetsy ../ParetoCalculatorConsole/ParetoCalculatorConsole examples/betsy.xml)



add_test( "TestBetsyOutput"
    ${CMAKE_COMMAND}
        -Dtest_cmd="${CMAKE_SOURCE_DIR}/ParetoCalculatorConsole/ParetoCalculatorConsole ${CMAKE_SOURCE_DIR}/examples/betsy.xml > ${CMAKE_SOURCE_DIR}/tests/output/TestBetsyOutput.output"
        -Doutput_blessed="${CMAKE_SOURCE_DIR}/tests/blessed_output/TestBetsyOutput.output"
        -Doutput_test="${CMAKE_SOURCE_DIR}/tests/output/TestBetsyOutput.output"
        -P ${CMAKE_SOURCE_DIR}/CMake/run_test.cmake
)
