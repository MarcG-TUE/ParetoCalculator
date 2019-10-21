cmake_minimum_required(VERSION 3.1)

enable_testing()

add_subdirectory(ParetoCalculatorLibrary/src)
add_subdirectory(ParetoCalculatorConsole)
add_subdirectory(doc)

# does the console application run
add_test(RunParetoCalculatorConsoleBetsy ParetoCalculatorConsole/ParetoCalculatorConsole test/betsy.xml)

