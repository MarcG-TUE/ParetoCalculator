
# does the console application run
add_test(
        NAME RunParetoCalculatorConsoleBetsy 
        COMMAND ../ParetoCalculatorConsole/ParetoCalculatorConsole ../examples/betsy.xml)

# run the unit test application
add_test(
        NAME PCUnitTest
        COMMAND ../ParetoCalculatorTest/ParetoCalculatorTest
)

# does the console application give the expected output for the betsy example
add_test(NAME TestBetsyOutput
	COMMAND ${CMAKE_COMMAND}
        -Dtest_cmd=${CMAKE_SOURCE_DIR}/ParetoCalculatorConsole/ParetoCalculatorConsole
	-Dtest_cmd_arg1=${CMAKE_SOURCE_DIR}/examples/betsy.xml
        -Doutput_blessed=${CMAKE_SOURCE_DIR}/tests/blessed_output/TestBetsyoutput.output
        -Doutput_test=${CMAKE_SOURCE_DIR}/tests/output/TestBetsyoutput.output
        -P ${CMAKE_SOURCE_DIR}/tests/run_test.cmake
)
