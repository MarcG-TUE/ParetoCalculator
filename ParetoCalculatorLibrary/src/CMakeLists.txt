cmake_minimum_required(VERSION 2.8)

use_cxx14()

set(CMAKE_C_STANDARD 14)

find_package(LibXml2 REQUIRED)

message(STATUS LIBXML2_FOUND = ${LIBXML2_FOUND})

include_directories(${LIBXML2_INCLUDE_DIR})

ADD_LIBRARY(ParetoCalculator STATIC
	calculator.cpp
	configuration.cpp
	exceptions.cpp
	operations.cpp
	paretoparser_libxml.cpp
	quantity.cpp
	storage.cpp
	utils_libxml.cpp
)

