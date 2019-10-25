
//
// The MIT License
//
// Copyright (c) 2008-2019 Eindhoven University of Technology
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.
//

//
// Author: Marc Geilen, e-mail: m.c.w.geilen@tue.nl
// Electronic Systems Group (ES), Department of Electrical Engineering,
// Eindhoven University of Technology
//
// Description:
// UnitTester class
//


#include "unittester.h"

#include <stdexcept>


#define ASSERT_THROW( condition, msg )                              \
{                                                                   \
  if( !( condition ) )                                              \
  {                                                                 \
    throw std::runtime_error(   std::string( msg )                  \
                              + std::string( "\nIn:" )              \
                              + std::string( __FILE__ )             \
                              + std::string( ":" )                  \
                              + std::to_string( __LINE__ )          \
                              + std::string( " in " )               \
                              + std::string( __FUNCTION__ )         \
    );                                                              \
  }                                                                 \
}

#define ASSERT_EQUAL( x, y, msg )                                        \
{                                                                   \
  if( ( x ) != ( y ) )                                              \
  {                                                                 \
    throw std::runtime_error(   std::string( msg )             \
                              + std::string( "\nIn:" )              \
                              + std::string( __FILE__ )              \
                              + std::string( ":" )                  \
                              + std::to_string( __LINE__ )          \
                              + std::string( " in " )               \
                              + std::string( __FUNCTION__ )  \
                              + std::string( ": " )                 \
                              + std::to_string( ( x ) )             \
                              + std::string( " != " )               \
                              + std::to_string( ( y ) )             \
    );                                                              \
  }                                                                 \
}



using namespace Pareto;

UnitTesterException::UnitTesterException(const std::string s) {
	errorMsg = s;
}

UnitTester::UnitTester(){
	this->generator.seed(1234);
}


bool UnitTester::test_all(void) {

	// checking if the asserts work
	ASSERT_THROW(true, "ASSERT failed!");
	ASSERT_EQUAL(0, 0, "Zero is not zero!");

	ASSERT_THROW(this->test_calculator(), "Calculator tests failed.");
	return true;
}

bool UnitTester::test_calculator(void) {
	ASSERT_THROW(this->test_DCMinimization(), "Divide and Conquer minimization failed.");
	return true;
}

bool UnitTester::test_DCMinimization(void) {


	return true;
}