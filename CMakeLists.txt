project(ParetoCalculator)

cmake_minimum_required(VERSION 3.1)

enable_testing()

add_subdirectory(ParetoCalculatorLibrary/src)
add_subdirectory(ParetoCalculatorConsole)
add_subdirectory(doc)
add_subdirectory(tests)


