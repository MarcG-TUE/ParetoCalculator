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


ParetoParser::ParetoParser(Pareto::ParetoCalculator *forPC){
    //   NameTable^ nt = gcnew NameTable();
    //   nsmgr = gcnew XmlNamespaceManager(nt);
    //   nsmgr->AddNamespace("pa", "uri:pareto"); //default namespace

    pc = forPC;
}


void ParetoParser::LoadFile(std::wstring f){
	pXMLDoc = xmlParseFile(wstring_to_string(f).c_str());
    if (pXMLDoc == NULL ) {
        pc->verbose("Document not parsed successfully. \n");
    }
	xpathCtx = xmlXPathNewContext(pXMLDoc);
	xmlXPathRegisterNs(xpathCtx, (xmlChar*)"pa", (xmlChar*)"uri:pareto");
}





void ParetoParser::LoadQuantityTypes(){
    // Load Quantity types from XML document

    // Select the quantity definition nodes
    xmlChar *xpath = (xmlChar*) "//pa:pareto_specification/pa:quantity_definitions/pa:quantity_definition";
    xmlXPathObjectPtr result = getNodeSetXPath (pXMLDoc, xpath, xpathCtx);
    if (result) {

		xmlNodeSetPtr nodeset = result->nodesetval;

		for (int i=0; i < nodeset->nodeNr; i++) {

            QuantityType* qn = NULL;
            std::string pTypeStr;
            bool typeDetermined = false;

            // for each quantity_definition node pQuantNode ...
            xmlNodePtr pQuantNode = nodeset->nodeTab[i];

            // get the type 
            pTypeStr = getNodeAttribute(pQuantNode, (xmlChar*)"type");  

            // real quantities
            if(pTypeStr=="real"){
                typeDetermined = true;
                qn = new QuantityType_Real(getNodeAttribute(pQuantNode, (xmlChar*)"name"));
			}

            //integer quantities
            if(pTypeStr == "integer"){
                typeDetermined = true;
                qn = new QuantityType_Integer(getNodeAttribute(pQuantNode, (xmlChar*)"name"));
            }


            // totally ordered and unordered enumeration type
            bool ordered;

            if((ordered = (pTypeStr == "enumeration")) || (pTypeStr == "unordered")){
                typeDetermined = true;
                QuantityType_Enum *qne;

				// make a QuantityType of the appropriate kind
                if(ordered) 
                    qne = new QuantityType_Enum(getNodeAttribute(pQuantNode, (xmlChar*)"name"));
                else   
                    qne = new QuantityType_Enum_Unordered(getNodeAttribute(pQuantNode, (xmlChar*)"name"));


				// get all the values of the enumeration type and add them
                xmlChar *xpath_values = (xmlChar*) "pa:values/pa:value";
                xmlXPathObjectPtr result_values = getNodeSetXPathNode (pXMLDoc, pQuantNode, xpath_values, xpathCtx);

				
				if (result_values) {
                    xmlNodeSetPtr nodeset_values = result_values->nodesetval;
                    for (int j=0; j < nodeset_values->nodeNr; j++) {
                        xmlNodePtr pValueNode = nodeset_values->nodeTab[j];

						qne->addQuantity(getNodeText(pXMLDoc, pValueNode));

					
					}
                    xmlXPathFreeObject (result_values);
                    }
                qn = qne; // temp qne is to avoid type casting

            }
        
            if(!typeDetermined){
                throw *new EParetoCalculatorError(std::string("Quantity of unknown type "+pTypeStr));
            }
            pc->store(*qn);

        }
        xmlXPathFreeObject (result);
    }

}






void ParetoParser::LoadConfigurationSpaces(){
    // Load Configuration Spaces from XML document

    // Select the configuration space nodes
    xmlChar *xpath = (xmlChar*) "//pa:pareto_specification/pa:configuration_spaces/pa:space";
    xmlXPathObjectPtr result = getNodeSetXPath (pXMLDoc, xpath, xpathCtx);
    if (result) {
        xmlNodeSetPtr nodeset = result->nodesetval;
        for (int i=0; i < nodeset->nodeNr; i++) {

            // for each configuration space node pSpaceNode ...
            xmlNodePtr pSpaceNode = nodeset->nodeTab[i];

            ConfigurationSpace* cs = new ConfigurationSpace(getNodeAttribute(pSpaceNode, (xmlChar*)"name"));

            xmlChar *xpath_quantities = (xmlChar*) "pa:quantity";
            xmlXPathObjectPtr result_quantities = getNodeSetXPathNode (pXMLDoc, pSpaceNode, xpath_quantities, xpathCtx);
            if (result_quantities) {
                xmlNodeSetPtr nodeset_quantities = result_quantities->nodesetval;
                for (int j=0; j < nodeset_quantities->nodeNr; j++) {
                    xmlNodePtr pQuantityNode = nodeset_quantities->nodeTab[j];
                    QuantityType& qu = dynamic_cast<QuantityType&>(*pc->retrieve(getNodeAttribute(pQuantityNode, (xmlChar*)"name")));
                    QuantityName qn;
                    if(hasNodeAttribute(pQuantityNode, (xmlChar*)"referBy")){
                        qn = getNodeAttribute(pQuantityNode, (xmlChar*)"referBy");
                    }
                    else {
                        qn = qu.name;                        
                    }
                    cs->addQuantityAs(qu,qn);
                }
            }
            pc->store(*cs);
        }
        xmlXPathFreeObject (result);
    }
}




void ParetoParser::LoadConfigurationSets(){
    // Configuration Sets from XML document

    // Select the configuration set nodes
    xmlChar *xpath = (xmlChar*) "//pa:pareto_specification/pa:configuration_sets/pa:configuration_set";
    xmlXPathObjectPtr result = getNodeSetXPath (pXMLDoc, xpath, xpathCtx);
    if (result) {
        xmlNodeSetPtr nodeset = result->nodesetval;
        for (int i=0; i < nodeset->nodeNr; i++) {

            // for each configuration space node pSpaceNode ...
            xmlNodePtr pConfSetNode = nodeset->nodeTab[i];

            ConfigurationSpace* sp = dynamic_cast<ConfigurationSpace*>(pc->retrieve(getNodeAttribute(pConfSetNode, (xmlChar*)"space_id")));
			
			ConfigurationSet* cs = new ConfigurationSet(sp, getNodeAttribute(pConfSetNode, (xmlChar*)"name"));
			std::vector<QuantityType*> *qts = &sp->quantities;

            // get all the configurations and add them to the set
            xmlChar *xpath_confs = (xmlChar*) "pa:configurations/pa:configuration";
            xmlXPathObjectPtr result_confs = getNodeSetXPathNode (pXMLDoc, pConfSetNode, xpath_confs, xpathCtx);
            if (result_confs) {
                xmlNodeSetPtr nodeset_confs = result_confs->nodesetval;			
				for (int j=0; j < nodeset_confs->nodeNr; j++) {

					// for each configuration node pConfNode ...
		            xmlNodePtr pConfNode = nodeset_confs->nodeTab[j];
					Configuration* cf = new Configuration(sp);

					// get all the values and add them to the configuration
					xmlChar *xpath_value = (xmlChar*) "pa:value";
			        xmlXPathObjectPtr result_value = getNodeSetXPathNode (pXMLDoc, pConfNode, xpath_value, xpathCtx);
			        if (result_value) {
						xmlNodeSetPtr nodeset_value = result_value->nodesetval;
				        for (int k=0; k < nodeset_value->nodeNr; k++) {
							// for each value node pValueNode ...
				            xmlNodePtr pValueNode = nodeset_value->nodeTab[k];
							QuantityValue& qv = (*qts)[k]->valueFromString(getNodeText(pXMLDoc, pValueNode));
							cf->addQuantity(qv);
						}
					}
					cs->addConfiguration(*cf);

				}
				xmlXPathFreeObject (result_confs);
            } 
            pc->store(*cs);
        }
    xmlXPathFreeObject (result);
    }
}





void ParetoParser::LoadOperations() // throw(EParetoCalculatorError)
{
    // Load the operations from XML document
	// To be ported later if necessary...
}
    



std::vector<QuantityType*>& ParetoParser::getQuantityTypes(){
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

