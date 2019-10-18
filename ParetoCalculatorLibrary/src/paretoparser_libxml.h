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
// Encapsulate the libXML specific XML parsing in this component
// This file is specific to libXML
//

#ifndef PARETO_PARSER_H
#define PARETO_PARSER_H

#include <string>
#include <vector>
#include <memory>
#include "quantity.h"
#include "operations.h"

#include <libxml/parser.h>
#include <libxml/xpath.h>
#include <libxml/xpathInternals.h>

namespace Pareto {

	class ParetoCalculator;

	//Encapsulates the libXML specific XML parser classes
	class ParetoParser {
	public:
		ParetoParser(ParetoCalculator& forPC);

		void LoadFile(std::string f);
		void SaveAll(std::string f);						// forget about saving for now
		void Save(std::string itemToSave, std::string f);
		void LoadQuantityTypes();
		//void SaveQuantityTypes();
		void LoadConfigurationSpaces();
		//void SaveConfigurationSpaces();
		void LoadConfigurationSets();
		//void SaveConfigurationSets();
		void LoadOperations();
		std::vector<std::shared_ptr<QuantityType>>& getQuantityTypes();
		std::vector<std::shared_ptr<ConfigurationSpace>>& getConfigurationSpaces();
		std::vector<std::shared_ptr<ConfigurationSet>>& getConfigurationSets();
		ParetoCalculatorOperation& getNextOperation();

	private:

		ParetoCalculator& pc;

		xmlXPathContextPtr xpathCtx = nullptr;

		ListOfQuantityNames& getListOfQuantityNames(xmlNodePtr pOperationNode);
		JoinMap& getJoinMap(xmlNodePtr pOperationNode);
	};

}

#endif
