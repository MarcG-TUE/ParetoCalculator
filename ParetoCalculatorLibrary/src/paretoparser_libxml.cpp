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


//
// TODO:
// - Check all the string types / conversions / casts
// - Add exception and error handling

#include "paretoparser_libxml.h"
#include "operations.h"
#include "calculator.h"
#include "utils_libxml.h"


using namespace Pareto;

static xmlDocPtr pXMLDoc;


ParetoParser::ParetoParser(Pareto::ParetoCalculator* forPC)
{
	pc = forPC;
}


// load an xml file
void ParetoParser::LoadFile(std::string f) {
	pXMLDoc = xmlParseFile(f.c_str());
	if (pXMLDoc == nullptr) {
		pc->verbose("Document not parsed successfully. \n");
	}
	xpathCtx = xmlXPathNewContext(pXMLDoc);
	xmlXPathRegisterNs(xpathCtx, (xmlChar*)"pa", (xmlChar*)"uri:pareto");
}





// Load Quantity types from XML document
void ParetoParser::LoadQuantityTypes() {

	// Select the quantity definition nodes
	xmlXPathObjectPtr result = getNodeSetXPath(pXMLDoc, (xmlChar*)"//pa:pareto_specification/pa:quantity_definitions/pa:quantity_definition", xpathCtx);
	if (result) {

		xmlNodeSetPtr nodeset = result->nodesetval;
		for (int i = 0; i < nodeset->nodeNr; i++) {

			// for each quantity_definition node pQuantNode ...
			xmlNodePtr pQuantNode = nodeset->nodeTab[i];

			QuantityType* qn = nullptr;
			std::string pTypeStr;
			bool typeDetermined = false;

			// get the type 
			pTypeStr = getNodeAttribute(pQuantNode, (xmlChar*)"type");

			// for real valued quantities
			if (pTypeStr == "real") {
				typeDetermined = true;
				qn = new QuantityType_Real(getNodeAttribute(pQuantNode, (xmlChar*)"name"));
			}

			// for integer valued quantities
			if (pTypeStr == "integer") {
				typeDetermined = true;
				qn = new QuantityType_Integer(getNodeAttribute(pQuantNode, (xmlChar*)"name"));
			}

			// totally ordered and unordered enumeration type
			bool ordered;

			if ((ordered = (pTypeStr == "enumeration")) || (pTypeStr == "unordered")) {
				typeDetermined = true;
				QuantityType_Enum* qne;

				// make a QuantityType of the appropriate kind
				if (ordered)
					qne = new QuantityType_Enum(getNodeAttribute(pQuantNode, (xmlChar*)"name"));
				else
					qne = new QuantityType_Enum_Unordered(getNodeAttribute(pQuantNode, (xmlChar*)"name"));

				// get all the values of the enumeration type and add them
				xmlXPathObjectPtr result_values = getNodeSetXPathNode(pXMLDoc, pQuantNode, (xmlChar*)"pa:values/pa:value", xpathCtx);
				if (result_values) {
					xmlNodeSetPtr nodeset_values = result_values->nodesetval;
					for (int j = 0; j < nodeset_values->nodeNr; j++) {
						xmlNodePtr pValueNode = nodeset_values->nodeTab[j];
						qne->addQuantity(getNodeText(pXMLDoc, pValueNode));
					}
					xmlXPathFreeObject(result_values);
				}
				qn = qne; // temp qne is to avoid type casting
			}

			if (!typeDetermined) {
				throw EParetoCalculatorError(std::string("Quantity of unknown type " + pTypeStr));
			}
			pc->store(*qn);
		}
		xmlXPathFreeObject(result);
	}
}




/// Load Configuration Spaces from XML document
void ParetoParser::LoadConfigurationSpaces() {

	// Select the configuration space nodes
	xmlXPathObjectPtr result = getNodeSetXPath(pXMLDoc, (xmlChar*)"//pa:pareto_specification/pa:configuration_spaces/pa:space", xpathCtx);
	if (result) {
		xmlNodeSetPtr nodeset = result->nodesetval;
		for (int i = 0; i < nodeset->nodeNr; i++) {

			// for each configuration space node pSpaceNode ...
			xmlNodePtr pSpaceNode = nodeset->nodeTab[i];

			ConfigurationSpace* cs = new ConfigurationSpace(getNodeAttribute(pSpaceNode, (xmlChar*)"name"));

			xmlXPathObjectPtr result_quantities = getNodeSetXPathNode(pXMLDoc, pSpaceNode, (xmlChar*)"pa:quantity", xpathCtx);
			if (result_quantities) {
				xmlNodeSetPtr nodeset_quantities = result_quantities->nodesetval;
				for (int j = 0; j < nodeset_quantities->nodeNr; j++) {
					xmlNodePtr pQuantityNode = nodeset_quantities->nodeTab[j];
					const QuantityType& qu = dynamic_cast<const QuantityType&>(*pc->retrieve(getNodeAttribute(pQuantityNode, (xmlChar*)"name")));
					QuantityName qn;
					if (hasNodeAttribute(pQuantityNode, (xmlChar*)"referBy")) {
						qn = getNodeAttribute(pQuantityNode, (xmlChar*)"referBy");
					}
					else {
						qn = qu.name;
					}
					cs->addQuantityAs(qu, qn);
				}
			}
			pc->store(*cs);
		}
		xmlXPathFreeObject(result);
	}
}



// Load Configuration Sets from XML document
void ParetoParser::LoadConfigurationSets() {

	// Select the configuration set nodes
	xmlXPathObjectPtr result = getNodeSetXPath(pXMLDoc, (xmlChar*)"//pa:pareto_specification/pa:configuration_sets/pa:configuration_set", xpathCtx);
	if (result) {
		xmlNodeSetPtr nodeset = result->nodesetval;
		for (int i = 0; i < nodeset->nodeNr; i++) {
			// for each configuration space node pSpaceNode ...
			xmlNodePtr pConfSetNode = nodeset->nodeTab[i];

			const ConfigurationSpace* sp = dynamic_cast<const ConfigurationSpace*>(pc->retrieve(getNodeAttribute(pConfSetNode, (xmlChar*)"space_id")));

			ConfigurationSet* cs = new ConfigurationSet(sp, getNodeAttribute(pConfSetNode, (xmlChar*)"name"));
			const ListOfQuantityTypes* qts = &sp->quantities;

			// get all the configurations and add them to the set
			xmlXPathObjectPtr result_confs = getNodeSetXPathNode(pXMLDoc, pConfSetNode, (xmlChar*)"pa:configurations/pa:configuration", xpathCtx);
			if (result_confs) {
				xmlNodeSetPtr nodeset_confs = result_confs->nodesetval;
				for (int j = 0; j < nodeset_confs->nodeNr; j++) {
					// for each configuration node pConfNode ...
					xmlNodePtr pConfNode = nodeset_confs->nodeTab[j];
					Configuration* cf = new Configuration(sp);

					// get all the values and add them to the configuration
					xmlXPathObjectPtr result_value = getNodeSetXPathNode(pXMLDoc, pConfNode, (xmlChar*)"pa:value", xpathCtx);
					if (result_value) {
						xmlNodeSetPtr nodeset_value = result_value->nodesetval;
						for (int k = 0; k < nodeset_value->nodeNr; k++) {
							// for each value node pValueNode ...
							xmlNodePtr pValueNode = nodeset_value->nodeTab[k];
							const QuantityValue& qv = (*qts)[k]->valueFromString(getNodeText(pXMLDoc, pValueNode));
							cf->addQuantity(qv);
						}
					}
					cs->addConfiguration(*cf);
				}
				xmlXPathFreeObject(result_confs);
			}
			pc->store(*cs);
		}
		xmlXPathFreeObject(result);
	}
}


ListOfQuantityNames* ParetoParser::getListOfQuantityNames(xmlNodePtr pOperationNode)
{
	ListOfQuantityNames* lqn = new ListOfQuantityNames();

	// Select the quantity nodes
	xmlXPathObjectPtr result_quants = getNodeSetXPathNode(pXMLDoc, pOperationNode, (xmlChar*)"pa:quant", xpathCtx);
	if (result_quants) {
		xmlNodeSetPtr quantset = result_quants->nodesetval;
		for (int j = 0; j < quantset->nodeNr; j++) {
			// for each quantity ...
			xmlNodePtr pQuantNode = quantset->nodeTab[j];
			std::string q = getNodeText(pXMLDoc, pQuantNode);
			lqn->push_back(q);
		}
		xmlXPathFreeObject(result_quants);
	}
	return lqn;
}

JoinMap* ParetoParser::getJoinMap(xmlNodePtr pOperationNode)
{
	JoinMap* jm = new JoinMap();
	// Select the configuration set nodes
	xmlXPathObjectPtr result_pairs = getNodeSetXPathNode(pXMLDoc, pOperationNode, (xmlChar*)"pa:between", xpathCtx);
	if (result_pairs) {
		xmlNodeSetPtr joinset = result_pairs->nodesetval;
		for (int j = 0; j < joinset->nodeNr; j++) {
			// for each join node ...
			xmlNodePtr pJoinNode = joinset->nodeTab[j];
			// for each quantity pair ...
			std::string qa = getNodeAttribute(pJoinNode, (xmlChar*)"quanta");
			std::string qb = getNodeAttribute(pJoinNode, (xmlChar*)"quantb");
			// add the join to the map
			(*jm)[qa] = qb;
		}
		xmlXPathFreeObject(result_pairs);
	}
	return jm;
}

void ParetoParser::LoadOperations()
{
	// Load the operations from XML document

	// check if a document is loaded
	if (pXMLDoc == nullptr) {
		throw EParetoCalculatorError("No XML document loaded.");
		return;
	}

	// Select the configuration set nodes
	xmlXPathObjectPtr result = getNodeSetXPath(pXMLDoc, (xmlChar*)"//pa:pareto_specification/pa:calculation/pa:*", xpathCtx);
	if (!result) {
		throw EParetoCalculatorError("Could not find a calculation node in the XML document.");
		return;
	}

	xmlNodeSetPtr nodeset = result->nodesetval;
	for (int i = 0; i < nodeset->nodeNr; i++) {
		// for each operation pOperationNode ...
		xmlNodePtr pOperationNode = nodeset->nodeTab[i];
		std::string nodeName = getNodeType(pOperationNode);

		if (nodeName == "join") {
			pc->verbose("join on multiple attributes\n");

			JoinMap* qm = this->getJoinMap(pOperationNode);

			// create an operation and execute it
			POperation_Join op(qm);
			op.executeOn(*pc);
		}

		if (nodeName == "join_eff") {
			pc->verbose("efficient join on multiple attributes\n");

			JoinMap* qm = this->getJoinMap(pOperationNode);

			// create an operation and execute it
			POperation_EfficientJoin op(qm);
			op.executeOn(*pc);
		}

		else if (nodeName == "abstract") {
			pc->verbose("abstraction of quantities\n");

			// get the quantities
			ListOfQuantityNames* lqn = getListOfQuantityNames(pOperationNode);

			// create an operation and execute it
			POperation_Abstract op(lqn);
			op.executeOn(*pc);
			
			// cleanup
			delete lqn;
		}

		else if (nodeName == "hide") {
			pc->verbose("hiding quantities\n");

			// get the quantities
			ListOfQuantityNames* lqn = getListOfQuantityNames(pOperationNode);

			// create an operation and execute it
			POperation_Hide op(lqn);
			op.executeOn(*pc);

			// cleanup
			delete lqn;
		}

		else if (nodeName == "aggregate") {
			pc->verbose("aggregating attributes\n");

			// get the quantities
			ListOfQuantityNames* lqn = getListOfQuantityNames(pOperationNode);

			// Select the name node
			xmlNodePtr pNameNode = getNodeXPathNode(pXMLDoc, pOperationNode, (xmlChar*)"pa:name", xpathCtx);
			if (!pNameNode) {
				delete lqn;
				throw EParetoCalculatorError("Name node missing in aggregate operation");
			}
			std::string newname = getNodeText(pXMLDoc, pNameNode);

			// create an operation and execute it
			POperation_Aggregate op(lqn, newname);
			op.executeOn(*pc);

			// cleanup
			delete lqn;
		}

		else if (nodeName == "prodcons") {
			pc->verbose("applying producer consumer constraint\n");

			// Select the producer and consumer quantity nodes
			xmlNodePtr pProdNode = getNodeXPathNode(pXMLDoc, pOperationNode, (xmlChar*)"pa:producer_quant", xpathCtx);
			xmlNodePtr pConsNode = getNodeXPathNode(pXMLDoc, pOperationNode, (xmlChar*)"pa:consumer_quant", xpathCtx);

			std::string pq = getNodeText(pXMLDoc, pProdNode);
			std::string cq = getNodeText(pXMLDoc, pConsNode);

			// create an operation and execute it
			POperation_ProdCons op(pq, cq);
			op.executeOn(*pc);
		}

		else if (nodeName == "prodcons_eff") {
			pc->verbose("applying efficient producer consumer constraint\n");

			// Select the producer and consumer quantity nodes
			xmlNodePtr pProdNode = getNodeXPathNode(pXMLDoc, pOperationNode, (xmlChar*)"pa:producer_quant", xpathCtx);
			xmlNodePtr pConsNode = getNodeXPathNode(pXMLDoc, pOperationNode, (xmlChar*)"pa:consumer_quant", xpathCtx);
			std::string pq = getNodeText(pXMLDoc, pProdNode);
			std::string cq = getNodeText(pXMLDoc, pConsNode);

			// create an operation and execute it
			POperation_EfficientProdCons op(pq, cq);
			op.executeOn(*pc);
		}

		else if (nodeName == "push") {
			pc->verbose("pushing object onto the stack\n");
				
			// create an operation and execute it
			POperation_Push op(getNodeAttribute(pOperationNode, (xmlChar*)"name"));
			op.executeOn(*pc);
		}

		else if (nodeName == "store") {
			pc->verbose("storing object from the stack\n");
			
			// create an operation and execute it
			POperation_Store op(getNodeAttribute(pOperationNode, (xmlChar*)"name"));
			op.executeOn(*pc);
		}

		else if (nodeName == "minimize") {
			pc->verbose("minimizing set of configurations\n");

			// create an operation and execute it
			POperation_Minimize op;
			op.executeOn(*pc);
		}
		
		else if (nodeName == "minimize_eff") {
			pc->verbose("minimizing set of configurations\n");

			// create an operation and execute it
			POperation_EfficientMinimize op;
			op.executeOn(*pc);
		}

		else if (nodeName == "pop") {
			pc->verbose("popping element from the stack\n");
			POperation_Pop op;
			op.executeOn(*pc);
		}

		else if (nodeName == "duplicate") {
			pc->verbose("duplicating element on top of the stack\n");
			POperation_Duplicate op;
			op.executeOn(*pc);
		}

		else if (nodeName == "print") {
			pc->verbose("printing element from the stack:\n");
			POperation_Print op;
			op.executeOn(*pc);
		}

		else if (nodeName == "product") {
			pc->verbose("computing product of sets of configurations\n");
			POperation_Product op;
			op.executeOn(*pc);
		}

	}
	xmlXPathFreeObject(result);
}




std::vector<QuantityType*>& ParetoParser::getQuantityTypes() {
	std::vector<QuantityType*>* v = new(std::vector<QuantityType*>);
	return *v;
}


void ParetoParser::SaveAll(std::string f)
{
	throw EParetoCalculatorError("ParetoParser::SaveAll is not implemented yet!");
}


void ParetoParser::Save(std::string itemToSave, std::string f)
{
	throw EParetoCalculatorError("ParetoParser::Save is not implemented yet!");
}

