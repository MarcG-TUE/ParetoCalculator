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
// Implementation of the PCConsole class, implementing a simple console interface to the calculator
//

#pragma once

#include <iostream>
#include <string>

#include <calculator.h>

namespace Pareto {


	/// Provides interface to the Pareto Calculator for all functions used in the console app
	/// Inherits from StatusCallback to deal with the information returned by the calculator
	class PCConsole: StatusCallback
	{
	public:
		// constructor, takes a stream to direct output to
		PCConsole(std::ostream& outstr);

		// Interface methods used by the application for processing xml input
		void LoadFile(std::string filename);
		void LoadQuantityTypes();
		void LoadConfigurationSpaces();
		void LoadConfigurationSets();
		void LoadOperations();

	private:

		// stream to direct calculator output to
		std::ostream& _outstr;

		// the calculator that will do the work
		ParetoCalculator _pc;

	private:

		/// called by the calculator to indicate the status of its activities
		virtual void setStatus(const std::string& s);

		/// called by the calculator to generate verbose stream description of what it is doing.
		virtual void verbose(const std::string& s);

	};

}