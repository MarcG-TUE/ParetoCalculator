# build the unit test executable

cmake_policy (VERSION 3.1)

set(CMAKE_C_STANDARD 17)

include_directories(../ParetoCalculatorLibrary/src)

find_package(LibXml2 REQUIRED)

add_executable(ParetoCalculatorTest
	unittest/src/unittest.cpp
	unittest/src/unittester.cpp
)

target_link_libraries(ParetoCalculatorTest
	ParetoCalculator
	${LIBXML2_LIBRARIES}
)

# does the console application run
add_test(
        NAME RunParetoCalculatorConsoleBetsy 
        COMMAND ../ParetoCalculatorConsole/ParetoCalculatorConsole ../examples/betsy.xml)

# run the unit test application
add_test(
        NAME PCUnitTest
        COMMAND ./ParetoCalculatorTest
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
