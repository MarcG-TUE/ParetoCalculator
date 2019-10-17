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
// Electronic Systems Group (ES), Faculty of Electrical Engineering,
// Eindhoven University of Technology
//
// Description:
// Implementation of the PCConsole class, implementing a simple console interface to the calculator
//


#include "pcconsole.h"

namespace Pareto {

	PCConsole::PCConsole(std::ostream& outstr): _outstr(outstr)
	{
		this->_pc.setStatusCallbackObject(this);
	}

	void PCConsole::LoadFile(std::wstring filename)
	{
		this->_pc.LoadFile(filename);
	}

	void PCConsole::LoadQuantityTypes()
	{
		this->_pc.LoadQuantityTypes();
	}

	void PCConsole::LoadConfigurationSpaces()
	{
		this->_pc.LoadConfigurationSpaces();
	}

	void PCConsole::LoadConfigurationSets()
	{
		this->_pc.LoadConfigurationSets();
	}

	void PCConsole::LoadOperations()
	{
		this->_pc.LoadOperations();
	}

	// called by the calculator to indicate the status of its activities
	void PCConsole::setStatus(const std::string& s)
	{
		std::cout << "[Status] " << s << std::endl;
	}
	// called by the calculator to generate verbose stream description of what it is doing.
	void PCConsole::verbose(const std::string& s)
	{
		std::cout << "[Verb] " << s << std::endl;
	}
}
