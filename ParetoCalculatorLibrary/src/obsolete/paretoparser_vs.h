//
// The MIT License
//
// Copyright (c) 2008 Eindhoven University of Technology
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
// Encapsulate the Microsoft specific XML parsing in this component
// This file is specific to Microsoft .NET / Microsoft Visual Studio
//

#ifndef PARETO_PARSER_H
#define PARETO_PARSER_H

#include <string.h>
#include <vector>
#include "quantity.h"
#include "operations.h"

#include <vcclr.h>
#using <mscorlib.dll>
#using <System.Xml.dll>

using namespace System;
using namespace System::Xml;
using namespace System::Collections;

namespace Pareto {

class ParetoCalculator;

public ref class ValidationEventHandler {
public:
	static void eventHandler (System::Object^ /*sender*/, System::Xml::Schema::ValidationEventArgs^ args);
	static ParetoCalculator* forCalculator=0L;
};

//Encapsulates the Microsoft .Net specific XML parser classes
class ParetoParser {
public:
		ParetoParser(ParetoCalculator* forPC);

        void LoadFile(std::wstring f);
        void SaveAll(std::wstring f);
        void Save(std::string itemToSave, std::wstring f);
        void LoadQuantityTypes();
        void SaveQuantityTypes();
        void LoadConfigurationSpaces();
        void SaveConfigurationSpaces();
        void LoadConfigurationSets();
        void SaveConfigurationSets();
        void LoadOperations() throw(...);
        std::vector<QuantityType*>& getQuantityTypes();
        std::vector<QuantityType*>& getConfigurationSpaces();
        std::vector<QuantityType*>& getConfigurationSets();
        ParetoCalculatorOperation& getNextOperation();

private:

	ParetoCalculator* pc;

	// Save/Load support
	void Save_PrepareXMLDoc(void);
	void Save_OutputFile(std::wstring f);
	XmlElement^ Save_XMLBuildConfigurationSetNode(ConfigurationSet* cs);
	XmlElement^ Save_XMLBuildConfigurationSpaceNode(ConfigurationSpace* cs);
	XmlElement^ ParetoParser::Save_XMLBuildQuantityTypeNode(QuantityType* qt);
};

//// Wanted to make the following a member of the ParetoParser class. 
//// This leads to an exception, but I do not know why...
//static gcroot<System::Xml::XmlDocument^> pXMLDoc;
//
//static gcroot<System::Xml::XmlNamespaceManager^> nsmgr;
//    


}

#endif