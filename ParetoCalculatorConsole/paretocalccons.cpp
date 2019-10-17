
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
// Main entry point to the console application
//

#include "pcconsole.h"
#include "paretocalccons.h"

#include <codecvt>

using namespace Pareto;

int main(int argc, char* argv[])
{

	if (argc != 2) {
		std::cout << "Please provide an xml file to process." << std::endl;
		std::cout << "Usage: ParetoCalculator <input_file>" << std::endl;
		return -1;
	}

	// convert filename to wstring
	typedef std::codecvt_utf8<wchar_t> convert_type;
	std::wstring_convert<convert_type, wchar_t> converter;
	std::wstring xmlfile = converter.from_bytes(argv[1]);
	
	// create Pareto Calculator
	PCConsole PCC(std::cout);

	// Read XML file
	PCC.LoadFile(xmlfile);

	// Process XML file
	PCC.LoadQuantityTypes();
	PCC.LoadConfigurationSpaces();
	PCC.LoadConfigurationSets();

	// Execute operations
	PCC.LoadOperations();

	return 0;
}