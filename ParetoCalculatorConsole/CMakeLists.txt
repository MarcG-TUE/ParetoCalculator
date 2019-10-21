cmake_minimum_required(VERSION 2.8)

set(CMAKE_C_STANDARD 11)

include_directories(../ParetoCalculatorLibrary/src)

find_package(LibXml2 REQUIRED)
message(STATUS LIBXML2_FOUND=${LIBXML2_FOUND})


add_executable(ParetoCalculatorConsole
	paretocalccons.cpp
	pcconsole.cpp
)

target_link_libraries(ParetoCalculatorConsole
	ParetoCalculator
	${LIBXML2_LIBRARIES}
)
