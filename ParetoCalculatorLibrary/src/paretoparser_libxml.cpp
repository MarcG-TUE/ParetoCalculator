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
// Encapsulate the libXML specific XML parsing in this component
// This file is specific to libXML
//


//
// To do:
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


void ParetoParser::LoadFile(std::wstring f) {
	pXMLDoc = xmlParseFile(wstring_to_string(f).c_str());
	if (pXMLDoc == NULL) {
		pc->verbose("Document not parsed successfully. \n");
	}
	xpathCtx = xmlXPathNewContext(pXMLDoc);
	xmlXPathRegisterNs(xpathCtx, (xmlChar*)"pa", (xmlChar*)"uri:pareto");
}





void ParetoParser::LoadQuantityTypes() {
	// Load Quantity types from XML document

	// Select the quantity definition nodes
	xmlChar* xpath = (xmlChar*)"//pa:pareto_specification/pa:quantity_definitions/pa:quantity_definition";
	xmlXPathObjectPtr result = getNodeSetXPath(pXMLDoc, xpath, xpathCtx);
	if (result) {

		xmlNodeSetPtr nodeset = result->nodesetval;

		for (int i = 0; i < nodeset->nodeNr; i++) {

			QuantityType* qn = NULL;
			std::string pTypeStr;
			bool typeDetermined = false;

			// for each quantity_definition node pQuantNode ...
			xmlNodePtr pQuantNode = nodeset->nodeTab[i];

			// get the type 
			pTypeStr = getNodeAttribute(pQuantNode, (xmlChar*)"type");

			// real quantities
			if (pTypeStr == "real") {
				typeDetermined = true;
				qn = new QuantityType_Real(getNodeAttribute(pQuantNode, (xmlChar*)"name"));
			}

			//integer quantities
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
				xmlChar* xpath_values = (xmlChar*)"pa:values/pa:value";
				xmlXPathObjectPtr result_values = getNodeSetXPathNode(pXMLDoc, pQuantNode, xpath_values, xpathCtx);


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
				throw* new EParetoCalculatorError(std::string("Quantity of unknown type " + pTypeStr));
			}
			pc->store(*qn);

		}
		xmlXPathFreeObject(result);
	}

}






void ParetoParser::LoadConfigurationSpaces() {
	// Load Configuration Spaces from XML document

	// Select the configuration space nodes
	xmlChar* xpath = (xmlChar*)"//pa:pareto_specification/pa:configuration_spaces/pa:space";
	xmlXPathObjectPtr result = getNodeSetXPath(pXMLDoc, xpath, xpathCtx);
	if (result) {
		xmlNodeSetPtr nodeset = result->nodesetval;
		for (int i = 0; i < nodeset->nodeNr; i++) {

			// for each configuration space node pSpaceNode ...
			xmlNodePtr pSpaceNode = nodeset->nodeTab[i];

			ConfigurationSpace* cs = new ConfigurationSpace(getNodeAttribute(pSpaceNode, (xmlChar*)"name"));

			xmlChar* xpath_quantities = (xmlChar*)"pa:quantity";
			xmlXPathObjectPtr result_quantities = getNodeSetXPathNode(pXMLDoc, pSpaceNode, xpath_quantities, xpathCtx);
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




void ParetoParser::LoadConfigurationSets() {
	// Configuration Sets from XML document

	// Select the configuration set nodes
	xmlChar* xpath = (xmlChar*)"//pa:pareto_specification/pa:configuration_sets/pa:configuration_set";
	xmlXPathObjectPtr result = getNodeSetXPath(pXMLDoc, xpath, xpathCtx);
	if (result) {
		xmlNodeSetPtr nodeset = result->nodesetval;
		for (int i = 0; i < nodeset->nodeNr; i++) {

			// for each configuration space node pSpaceNode ...
			xmlNodePtr pConfSetNode = nodeset->nodeTab[i];

			const ConfigurationSpace* sp = dynamic_cast<const ConfigurationSpace*>(pc->retrieve(getNodeAttribute(pConfSetNode, (xmlChar*)"space_id")));

			ConfigurationSet* cs = new ConfigurationSet(sp, getNodeAttribute(pConfSetNode, (xmlChar*)"name"));
			const ListOfQuantityTypes* qts = &sp->quantities;

			// get all the configurations and add them to the set
			xmlChar* xpath_confs = (xmlChar*)"pa:configurations/pa:configuration";
			xmlXPathObjectPtr result_confs = getNodeSetXPathNode(pXMLDoc, pConfSetNode, xpath_confs, xpathCtx);
			if (result_confs) {
				xmlNodeSetPtr nodeset_confs = result_confs->nodesetval;
				for (int j = 0; j < nodeset_confs->nodeNr; j++) {

					// for each configuration node pConfNode ...
					xmlNodePtr pConfNode = nodeset_confs->nodeTab[j];
					Configuration* cf = new Configuration(sp);

					// get all the values and add them to the configuration
					xmlChar* xpath_value = (xmlChar*)"pa:value";
					xmlXPathObjectPtr result_value = getNodeSetXPathNode(pXMLDoc, pConfNode, xpath_value, xpathCtx);
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




// TODO: there is a lot of duplicate code in the function, reduce
void ParetoParser::LoadOperations()
{

	// Load the operations from XML document

	// check if a document is loaded
	if (pXMLDoc==NULL) return;

	// Select the configuration set nodes
	xmlChar* xpath = (xmlChar*)"//pa:pareto_specification/pa:calculation/pa:*";
	xmlXPathObjectPtr result = getNodeSetXPath(pXMLDoc, xpath, xpathCtx);
	if (result) {
		xmlNodeSetPtr nodeset = result->nodesetval;
		for (int i = 0; i < nodeset->nodeNr; i++) {
			// for each operation pOperationNode ...
			xmlNodePtr pOperationNode = nodeset->nodeTab[i];

			std::string nodeName = getNodeAttribute(pOperationNode, (xmlChar*)"name");

			if (nodeName == "join") {
				pc->verbose("join on multiple attributes\n");

				JoinMap qm;

				// Select the configuration set nodes
				xmlChar* xpath_between = (xmlChar*)"pa:between";
				xmlXPathObjectPtr result_pairs = getNodeSetXPathNode(pXMLDoc, pOperationNode, xpath_between, xpathCtx);
				if (result_pairs) {
					xmlNodeSetPtr joinset = result_pairs->nodesetval;
					for (int j = 0; j < joinset->nodeNr; j++) {
						// for each join node ...
						xmlNodePtr pJoinNode = joinset->nodeTab[j];
						// for each quantity pair ...
						std::string qa = getNodeAttribute(pJoinNode, (xmlChar*)"quanta");
						std::string qb = getNodeAttribute(pJoinNode, (xmlChar*)"quantb");
						qm[qa] = qb;
					}
					xmlXPathFreeObject(result_pairs);
				}
				POperation_Join* op = new POperation_Join(&qm);
				op->executeOn(*pc);
				delete op;
			}
			if (nodeName == "join_eff") {
				pc->verbose("efficient join on multiple attributes\n");

				JoinMap qm;

				// Select the configuration set nodes
				xmlChar* xpath_between = (xmlChar*)"pa:between";
				xmlXPathObjectPtr result_pairs = getNodeSetXPathNode(pXMLDoc, pOperationNode, xpath_between, xpathCtx);
				if (result_pairs) {
					xmlNodeSetPtr joinset = result_pairs->nodesetval;
					for (int j = 0; j < joinset->nodeNr; j++) {
						// for each join node ...
						xmlNodePtr pJoinNode = joinset->nodeTab[j];
						// for each quantity pair ...
						std::string qa = getNodeAttribute(pJoinNode, (xmlChar*)"quanta");
						std::string qb = getNodeAttribute(pJoinNode, (xmlChar*)"quantb");
						qm[qa] = qb;
					}
					xmlXPathFreeObject(result_pairs);
				}
				POperation_EfficientJoin* op = new POperation_EfficientJoin(&qm);
				op->executeOn(*pc);
				delete op;
			}
			else if (nodeName == "abstract") {
				pc->verbose("abstraction of quantities\n");

				ListOfQuantityNames lqn;

				// Select the quantity nodes
				xmlChar* xpath_quant = (xmlChar*)"pa:quant";
				xmlXPathObjectPtr result_quants = getNodeSetXPathNode(pXMLDoc, pOperationNode, xpath_quant, xpathCtx);
				if (result_quants) {
					xmlNodeSetPtr quantset = result_quants->nodesetval;
					for (int j = 0; j < quantset->nodeNr; j++) {
						// for each quantity ...
						xmlNodePtr pQuantNode = quantset->nodeTab[j];
						std::string q = getNodeText(pXMLDoc, pQuantNode);
						lqn.push_back(q);
					}
					xmlXPathFreeObject(result_quants);
				}
				POperation_Abstract* op = new POperation_Abstract(&lqn);
				op->executeOn(*pc);
				delete op;
			}
			else if (nodeName == "hide") {
				pc->verbose("hiding quantities\n");

				ListOfQuantityNames lqn;

				// Select the quantity nodes
				xmlChar* xpath_quant = (xmlChar*)"pa:quant";
				xmlXPathObjectPtr result_quants = getNodeSetXPathNode(pXMLDoc, pOperationNode, xpath_quant, xpathCtx);
				if (result_quants) {
					xmlNodeSetPtr quantset = result_quants->nodesetval;
					for (int j = 0; j < quantset->nodeNr; j++) {
						// for each quantity ...
						xmlNodePtr pQuantNode = quantset->nodeTab[j];
						std::string q = getNodeText(pXMLDoc, pQuantNode);
						lqn.push_back(q);
					}
					xmlXPathFreeObject(result_quants);
				}
				POperation_Hide* op = new POperation_Hide(&lqn);
				op->executeOn(*pc);
				delete op;
			}
			else if (nodeName == "aggregate") {
				pc->verbose("aggregating attributes\n");

				ListOfQuantityNames lqn;

				// Select the quantity nodes
				xmlChar* xpath_quant = (xmlChar*)"pa:quant";
				xmlXPathObjectPtr result_quants = getNodeSetXPathNode(pXMLDoc, pOperationNode, xpath_quant, xpathCtx);
				if (result_quants) {
					xmlNodeSetPtr quantset = result_quants->nodesetval;
					for (int j = 0; j < quantset->nodeNr; j++) {
						// for each quantity ...
						xmlNodePtr pQuantNode = quantset->nodeTab[j];
						std::string q = getNodeText(pXMLDoc, pQuantNode);
						lqn.push_back(q);
					}
					xmlXPathFreeObject(result_quants);
				}

				// Select the name node
				xmlChar* xpath_name = (xmlChar*)"pa:name";
				xmlNodePtr pNameNode = getNodeXPathNode(pXMLDoc, pOperationNode, xpath_name, xpathCtx);
				if (pNameNode) {
					std::string newname = getNodeText(pXMLDoc, pNameNode);
					POperation_Aggregate* op = new POperation_Aggregate(&lqn, newname);
					op->executeOn(*pc);
					delete op;
				}
			}
			else if (nodeName == "prodcons") {
				pc->verbose("applying producer consumer constraint\n");

				// Select the producer and consumer quantity nodes
				xmlChar* xpath_prod = (xmlChar*)"pa:producer_quant";
				xmlChar* xpath_cons = (xmlChar*)"pa:consumer_quant";
				xmlNodePtr pProdNode = getNodeXPathNode(pXMLDoc, pOperationNode, xpath_prod, xpathCtx);
				xmlNodePtr pConsNode = getNodeXPathNode(pXMLDoc, pOperationNode, xpath_cons, xpathCtx);

				std::string pq = getNodeText(pXMLDoc, pProdNode);
				std::string cq = getNodeText(pXMLDoc, pConsNode);

				ParetoCalculatorOperation* op = new POperation_ProdCons(pq, cq);
				op->executeOn(*pc);
				delete op;
			}
			else if (nodeName == "prodcons_eff") {
				pc->verbose("applying efficient producer consumer constraint\n");

				// Select the producer and consumer quantity nodes
				xmlChar* xpath_prod = (xmlChar*)"pa:producer_quant";
				xmlChar* xpath_cons = (xmlChar*)"pa:consumer_quant";
				xmlNodePtr pProdNode = getNodeXPathNode(pXMLDoc, pOperationNode, xpath_prod, xpathCtx);
				xmlNodePtr pConsNode = getNodeXPathNode(pXMLDoc, pOperationNode, xpath_cons, xpathCtx);

				std::string pq = getNodeText(pXMLDoc, pProdNode);
				std::string cq = getNodeText(pXMLDoc, pConsNode);

				ParetoCalculatorOperation* op = new POperation_EfficientProdCons(pq, cq);
				op->executeOn(*pc);
				delete op;
			}
			else if (nodeName == "push") {
				pc->verbose("pushing object onto the stack\n");
				
				ParetoCalculatorOperation* op = new POperation_Push(getNodeAttribute(pOperationNode, (xmlChar*)"name"));
				op->executeOn(*pc);
				delete op;
			}
			else if (nodeName == "store") {
				pc->verbose("storing object from the stack\n");
				ParetoCalculatorOperation* op = new POperation_Store(getNodeAttribute(pOperationNode, (xmlChar*)"name"));
				op->executeOn(*pc);
				delete op;
			}
			else if (nodeName == "minimize") {
				pc->verbose("minimizing set of configurations\n");
				ParetoCalculatorOperation* op = new POperation_Minimize();
				op->executeOn(*pc);
				delete op;
			}
			else if (nodeName == "minimize_eff") {
				pc->verbose("minimizing set of configurations\n");
				ParetoCalculatorOperation* op = new POperation_EfficientMinimize();
				op->executeOn(*pc);
				delete op;
			}
			else if (nodeName == "pop") {
				pc->verbose("popping element from the stack\n");
				ParetoCalculatorOperation* op = new POperation_Pop();
				op->executeOn(*pc);
				delete op;
			}
			else if (nodeName == "duplicate") {
				pc->verbose("duplicating element on top of the stack\n");
				ParetoCalculatorOperation* op = new POperation_Duplicate();
				op->executeOn(*pc);
				delete op;
			}
			else if (nodeName == "print") {
				pc->verbose("printing element from the stack:\n");
				ParetoCalculatorOperation* op = new POperation_Print();
				op->executeOn(*pc);
				delete op;
			}
			else if (nodeName == "product") {
				pc->verbose("computing product of sets of configurations\n");
				ParetoCalculatorOperation* op = new POperation_Product();
				op->executeOn(*pc);
				delete op;
			}

		}
		xmlXPathFreeObject(result);
	}
//	XmlNodeList^ pXMLDomNodeList = pXMLDoc->DocumentElement->SelectNodes("//pa:pareto_specification/pa:calculation/pa:*", nsmgr);

//	IEnumerator^ myEnum = pXMLDomNodeList->GetEnumerator();
//	while (myEnum->MoveNext()) {

		// for each operation pOperationNode ...
//		XmlNode^ pOperationNode = safe_cast<XmlNode^>(myEnum->Current);

//	}

}




std::vector<QuantityType*>& ParetoParser::getQuantityTypes() {
	std::vector<QuantityType*>* v = new(std::vector<QuantityType*>);
	return *v;
}


void ParetoParser::SaveAll(std::wstring f)
{
	throw EParetoCalculatorError("ParetoParser::SaveAll is not implemented!");
}


void ParetoParser::Save(std::string itemToSave, std::wstring f)
{
	throw EParetoCalculatorError("ParetoParser::Save is not implemented!");
}

