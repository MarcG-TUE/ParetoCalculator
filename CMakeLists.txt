cmake_minimum_required(VERSION 2.8)

macro(use_cxx14)
  if (CMAKE_VERSION VERSION_LESS "3.1")
    if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
      set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++14")
    endif ()
  else ()
    set (CMAKE_CXX_STANDARD 14)
  endif ()
endmacro(use_cxx14)


enable_testing()

add_subdirectory(ParetoCalculatorLibrary/src)
add_subdirectory(ParetoCalculatorConsole)
add_subdirectory(doc)

# does the console application run
add_test(RunParetoCalculatorConsoleBetsy ParetoCalculatorConsole/ParetoCalculatorConsole test/betsy.xml)

