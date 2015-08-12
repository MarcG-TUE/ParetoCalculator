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


// Marc Geilen, 21-3-2006
//
// To do:
// - Check all the string types / conversions / casts
// - Add exception and error handling

#include "paretoparser_vs.h"
#include "operations.h"
#include "calculator.h"
#include "exceptions.h"
#include "utils_vs.h"

#pragma hdrstop

using namespace System::Xml;
using namespace System::Net;
using namespace Pareto;

// Wanted to make the following a member of the ParetoParser class. 
// This leads to an exception, but I do not know why...
static gcroot<System::Xml::XmlDocument^> pXMLDoc;
static gcroot<System::Xml::XmlNamespaceManager^> nsmgr;


ParetoParser::ParetoParser(Pareto::ParetoCalculator *forPC){
    NameTable^ nt = gcnew NameTable();
    nsmgr = gcnew XmlNamespaceManager(nt);
	nsmgr->AddNamespace("pa", "uri:pareto"); //default namespace
	pc = forPC;
}


void ParetoParser::LoadFile(wstring f){
//	original, non validating version:
//	pXMLDoc = gcnew System::Xml::XmlDocument();
//	XmlTextReader^ myXmlTextReader = gcnew XmlTextReader(gcnew System::String(f.c_str()));
//	pXMLDoc->Load(myXmlTextReader);


	pXMLDoc = gcnew System::Xml::XmlDocument();

	ValidationEventHandler::forCalculator  = pc;

	Xml::XmlReaderSettings^ settings = gcnew XmlReaderSettings();
	try{
		settings->Schemas->Add("uri:pareto", "http://www.es.ele.tue.nl/~mgeilen/paretoalgebra/xml/paretospec.xsd");
		settings->ValidationEventHandler += gcnew Xml::Schema::ValidationEventHandler(&ValidationEventHandler::eventHandler);
		settings->ValidationType = ValidationType::Schema;
		settings->ValidationFlags = settings->ValidationFlags | Xml::Schema::XmlSchemaValidationFlags::ReportValidationWarnings;
	}
	catch (System::Net::WebException^ ) { 
		pc->verbose("\nWarning: cannot access Xml schema definition. Parsing without validation.\n");
	}
	XmlReader^ reader = XmlReader::Create(gcnew System::String(f.c_str()), settings);
	pXMLDoc->Load(reader);
	ValidationEventHandler::forCalculator  = 0L;
}





void ValidationEventHandler::eventHandler (Object^ /*sender*/, Schema::ValidationEventArgs^ args){
	if(forCalculator){
		forCalculator->verbose("XML Validation error: ");
		forCalculator->verbose(system_to_std_string(args->Message));
	}
}



void ParetoParser::SaveAll(wstring f){
	this->Save_PrepareXMLDoc();
	this->SaveQuantityTypes();
	this->SaveConfigurationSpaces();
	this->SaveConfigurationSets();
	this->Save_OutputFile(f);
}

void ParetoParser::Save(string itemToSave, wstring f){
	StorableObject* o = pc->retrieve(itemToSave);
	ConfigurationSpace* csp;

	this->Save_PrepareXMLDoc();

	if(o->isConfigurationSpace()){
		csp = dynamic_cast<ConfigurationSpace*>(o);
	} else {
		csp = dynamic_cast<ConfigurationSet*>(o)->confspace;
	}

	// add quantity types
	XmlElement^ psElem = safe_cast<XmlElement^>(pXMLDoc->DocumentElement->SelectSingleNode("//pareto_specification"));
	XmlElement^ pXMLQuants = pXMLDoc->CreateElement("quantity_definitions");
	for(unsigned int i=0; i<csp->quantities.size(); i++){
		QuantityType* qt = csp->quantities[i];
		XmlElement^qdNode = this->Save_XMLBuildQuantityTypeNode(qt);
		pXMLQuants->AppendChild(qdNode);
		}
	psElem->AppendChild(pXMLQuants);

	XmlElement^ pXMLConfSpaces = pXMLDoc->CreateElement("configuration_spaces");
	XmlElement^ csNode = this->Save_XMLBuildConfigurationSpaceNode(csp);
	pXMLConfSpaces->AppendChild(csNode);
	psElem->AppendChild(pXMLConfSpaces);

	if(o->isConfigurationSet()){
		ConfigurationSet* cse = dynamic_cast<ConfigurationSet*>(o);
		XmlElement^ cseNode = this->Save_XMLBuildConfigurationSetNode(cse);
		XmlElement^ pXMLConfSets = pXMLDoc->CreateElement("configuration_sets");
		pXMLConfSets->AppendChild(cseNode);
		psElem->AppendChild(pXMLConfSets);
	}

	this->Save_OutputFile(f);

}

void ParetoParser::Save_PrepareXMLDoc(){
	pXMLDoc = gcnew System::Xml::XmlDocument();
	XmlDeclaration^ pXMLDecl = pXMLDoc->CreateXmlDeclaration( "1.0", nullptr, nullptr );

	pXMLDoc->AppendChild(pXMLDecl);
	XmlElement^ pXMLRootElem = pXMLDoc->CreateElement("pareto_specification");
	pXMLRootElem->SetAttribute("schemaVersion", "1.0");
	pXMLRootElem->SetAttribute("xmlns", "pareto");
	pXMLRootElem->SetAttribute("xmlns:xsi", "http://www.w3.org/2001/XMLSchema-instance");
	pXMLRootElem->SetAttribute("xsi:schemaLocation", "pareto paretospec.xsd");

	pXMLDoc->AppendChild(pXMLRootElem);
}

void ParetoParser::Save_OutputFile(wstring f){
//	Save the XML document to file
	System::Xml::XmlTextWriter^ writer = gcnew XmlTextWriter( gcnew System::String(f.c_str()), nullptr );
	writer->Formatting = Formatting::Indented;
	pXMLDoc->Save(writer);
}

void ParetoParser::LoadQuantityTypes(){
	// Load Quantity types from XML document

	// Select the quantity definition nodes
	XmlNodeList^ pXMLDomNodeList = pXMLDoc->DocumentElement->SelectNodes("//pa:pareto_specification/pa:quantity_definitions/pa:quantity_definition", nsmgr);
	// note1: Do I really need to insert the namespace everywhere...?
	// note2: I could maybe use more advanced selectNodes queries to select the different types separately?

	IEnumerator^ myEnum = pXMLDomNodeList->GetEnumerator();
	while (myEnum->MoveNext()){
		QuantityType* qn;
		System::String^ pTypeStr;
		bool typeDetermined = false;

		// for eacht quantity_definition node pQuantNode ...
		XmlNode^ pQuantNode = safe_cast<XmlNode^>(myEnum->Current);

		// get the type 
		pTypeStr = pQuantNode->Attributes->GetNamedItem("type")->InnerXml;	

		// real quantities
		if(pTypeStr=="real"){
			typeDetermined = true;
			qn = new QuantityType_Real(system_to_std_string(pQuantNode->Attributes->GetNamedItem("name")->InnerXml));
		}

		//integer quantities
		if(pTypeStr=="integer"){
			typeDetermined = true;
			qn = new QuantityType_Integer(system_to_std_string(pQuantNode->Attributes->GetNamedItem("name")->InnerXml));
		}

		// totally ordered and unordered enumeration type
		bool ordered;
		if((ordered=(pTypeStr=="enumeration"))||(pTypeStr=="unordered")){
			typeDetermined = true;
			QuantityType_Enum *qne;
			XmlNodeList^ pXMLDomValuesList;

			// make a QuantityType of the appropriate kind
			if(ordered) 
				qne = new QuantityType_Enum(system_to_std_string(pQuantNode->Attributes->GetNamedItem("name")->InnerXml));
			else
				qne = new QuantityType_Enum_Unordered(system_to_std_string(pQuantNode->Attributes->GetNamedItem("name")->InnerXml));

			// get all the values of the enumeration type and add them
			pXMLDomValuesList = pQuantNode->SelectNodes("pa:values/pa:value", nsmgr);
			IEnumerator^ valueEnum = pXMLDomValuesList->GetEnumerator();
			while (valueEnum->MoveNext()){
				XmlNode^ pValueNode = safe_cast<XmlNode^>(valueEnum->Current);
				qne->addQuantity(system_to_std_string(pValueNode->InnerXml));
			}
			qn = qne; // temp qne is to avoid type casting
		}
		if(!typeDetermined){
			throw *new EParetoCalculatorError(string("Quantity of unknown type "+system_to_std_string(pTypeStr)));
		}
		pc->store(*qn);
	}
}


XmlElement^ ParetoParser::Save_XMLBuildQuantityTypeNode(QuantityType* qt){
	XmlElement^qdNode = pXMLDoc->CreateElement("quantity_definition");
	qdNode->SetAttribute("name", gcnew System::String(qt->name.c_str()));
	qdNode->SetAttribute("type", gcnew System::String(qt->xmlTypeString()->c_str()));
	if(qt->isQuantityTypeEnum()){
		QuantityType_Enum* qte = dynamic_cast<QuantityType_Enum*>(qt);
		XmlElement^ valuesElem = pXMLDoc->CreateElement("values");
		qdNode->AppendChild(valuesElem);
		for(unsigned int j=0; j<qte->names.size(); j++){
			XmlElement^ valueElem = pXMLDoc->CreateElement("value");
			valuesElem->AppendChild(valueElem);
			valueElem->AppendChild(pXMLDoc->CreateTextNode(gcnew System::String(qte->names[j].c_str())));
		}
	}
	return qdNode;
}

void ParetoParser::SaveQuantityTypes(){
	XmlElement^ pXMLQuants = pXMLDoc->CreateElement("quantity_definitions");
	XmlElement^ psElem = safe_cast<XmlElement^>(pXMLDoc->DocumentElement->SelectSingleNode("//pa:pareto_specification", nsmgr));
	psElem->AppendChild(pXMLQuants);

	StorageMap::iterator i;
	for(i=pc->memory.begin(); i!=pc->memory.end(); i++){
		StorableObject* q= (*i).second;
		if(q->isQuantityType()){
			QuantityType* qt = dynamic_cast<QuantityType*>(q);
			XmlElement^qdNode = this->Save_XMLBuildQuantityTypeNode(qt);
			pXMLQuants->AppendChild(qdNode);
		}
	}

}




void ParetoParser::SaveConfigurationSpaces(){
	XmlElement^ pXMLConfSpaces = pXMLDoc->CreateElement("configuration_spaces");
	XmlElement^ psElem = safe_cast<XmlElement^>(pXMLDoc->DocumentElement->SelectSingleNode("//pa:pareto_specification", nsmgr));
	psElem->AppendChild(pXMLConfSpaces);

	StorageMap::iterator i;
	for(i=pc->memory.begin(); i!=pc->memory.end(); i++){
		StorableObject* q= (*i).second;
		if(q->isConfigurationSet()){
			ConfigurationSet* cs = dynamic_cast<ConfigurationSet*>(q);
			q = cs->confspace;
		}
		if(q->isConfigurationSpace()){
			ConfigurationSpace* cs = dynamic_cast<ConfigurationSpace*>(q);
			XmlElement^csNode = this->Save_XMLBuildConfigurationSpaceNode(cs);
			pXMLConfSpaces->AppendChild(csNode);
		}		
	}
}


void ParetoParser::LoadConfigurationSpaces(){
	// Load Configuration Spaces from XML document

	// Select the quantity definition nodes
	XmlNodeList^ pXMLDomNodeList = pXMLDoc->DocumentElement->SelectNodes("//pa:pareto_specification/pa:configuration_spaces/pa:space", nsmgr);

	IEnumerator^ myEnum = pXMLDomNodeList->GetEnumerator();
	while (myEnum->MoveNext()){
		// for each configuration space node pSpaceNode ...
		XmlNode^ pSpaceNode = safe_cast<XmlNode^>(myEnum->Current);

		ConfigurationSpace* cs = new ConfigurationSpace(system_to_std_string(pSpaceNode->Attributes->GetNamedItem("name")->InnerXml));

		// get all the quantities and add them to the space
		XmlNodeList^ pXMLDomQuantitiesList = pSpaceNode->SelectNodes("pa:quantity", nsmgr);
		IEnumerator^ quantEnum = pXMLDomQuantitiesList->GetEnumerator();
		while (quantEnum->MoveNext()){
			XmlNode^ pQuantityNode = safe_cast<XmlNode^>(quantEnum->Current);
			QuantityType& qu = safe_cast<QuantityType&>(*pc->retrieve(system_to_std_string(pQuantityNode->Attributes->GetNamedItem("name")->InnerXml)));
			QuantityName qn;
			XmlNode ^rn;
			if(rn = pQuantityNode->Attributes->GetNamedItem("referBy")){
				qn = system_to_std_string(rn->InnerXml);
			}
			else {
				qn = qu.name;
			}
			cs->addQuantityAs(qu,qn);
		}
		pc->store(*cs);
	}
}


void ParetoParser::LoadConfigurationSets(){
	// Configuration Sets from XML document

	// Select the configuration set nodes
	XmlNodeList^ pXMLDomNodeList = pXMLDoc->DocumentElement->SelectNodes("//pa:pareto_specification/pa:configuration_sets/pa:configuration_set", nsmgr);

	IEnumerator^ myEnum = pXMLDomNodeList->GetEnumerator();

	while (myEnum->MoveNext()){
		// for each configuration set node pConfSetNode ...
		XmlNode^ pConfSetNode = safe_cast<XmlNode^>(myEnum->Current);
		
		ConfigurationSpace* sp = dynamic_cast<ConfigurationSpace*>(pc->retrieve(system_to_std_string(pConfSetNode->Attributes->GetNamedItem("space_id")->InnerXml)));
		ConfigurationSet* cs = new ConfigurationSet(sp, system_to_std_string(pConfSetNode->Attributes->GetNamedItem("name")->InnerXml));
		vector<QuantityType*> *qts = &sp->quantities;

		// get all the configurations and add them to the set
		XmlNodeList^ pXMLDomConfsList = pConfSetNode->SelectNodes("pa:configurations/pa:configuration", nsmgr);

		IEnumerator^ confsEnum = pXMLDomConfsList->GetEnumerator();
		while (confsEnum->MoveNext()){

			// for each configuration node pConfNode ...
			XmlNode^ pConfNode = safe_cast<XmlNode^>(confsEnum->Current);
			Configuration* cf = new Configuration(sp);

			// get all the values and add them to the configuration
			XmlNodeList^ pXMLDomValuesList = pConfNode->SelectNodes("pa:value", nsmgr);

			IEnumerator^ valueEnum = pXMLDomValuesList->GetEnumerator();
			unsigned int k=0;
			while (valueEnum->MoveNext()){

				// for each value node pValueNode ...
				XmlNode^ pValueNode = safe_cast<XmlNode^>(valueEnum->Current);

				if(k>=qts->size()){
					throw *new EParetoCalculatorError("Wrong number of values in configuration\n");
				}
				QuantityValue& qv = (*qts)[k]->valueFromString(system_to_std_string(pValueNode->InnerXml));
				cf->addQuantity(qv);

				k=k+1;
			}
			cs->addConfiguration(*cf);
		}
		pc->store(*cs);
	}
}

XmlElement^ ParetoParser::Save_XMLBuildConfigurationSetNode(ConfigurationSet* cs){
	XmlElement^csNode = pXMLDoc->CreateElement("configuration_set");
	csNode->SetAttribute("name", gcnew System::String(cs->name.c_str()));
	csNode->SetAttribute("space_id", gcnew System::String(cs->confspace->name.c_str()));
	XmlElement^ confsNode = pXMLDoc->CreateElement("configurations");
	SetOfConfigurations::iterator k;
	for(k=cs->confs.begin(); k!=cs->confs.end(); k++){
		const Configuration& conf = *k;
		XmlElement^ confNode = pXMLDoc->CreateElement("configuration");
		ListOfQuantityValues::const_iterator l;
		for(l=conf.quantities.begin(); l!=conf.quantities.end(); l++){
			XmlElement^ valueNode = pXMLDoc->CreateElement("value");
			QuantityValue* v = *l;
			string& vs = v->asString();
			valueNode->AppendChild(pXMLDoc->CreateTextNode(gcnew System::String(vs.c_str())));
			confNode->AppendChild(valueNode);
		}
		confsNode->AppendChild(confNode);
	}
	csNode->AppendChild(confsNode);
	return csNode;
}


XmlElement^ ParetoParser::Save_XMLBuildConfigurationSpaceNode(ConfigurationSpace* cs){
	XmlElement^csNode = pXMLDoc->CreateElement("space");
	csNode->SetAttribute("name", gcnew System::String(cs->name.c_str()));
	for(unsigned int j=0; j<cs->quantities.size(); j++){
		QuantityType* qu = cs->quantities[j];
		XmlElement^ quNode = pXMLDoc->CreateElement("quantity");
		quNode->SetAttribute("name", gcnew System::String(qu->name.c_str()));
		csNode->AppendChild(quNode);
	}
	return csNode;
}

void ParetoParser::SaveConfigurationSets(){
	XmlElement^ pXMLConfSets = pXMLDoc->CreateElement("configuration_sets");
	XmlElement^ psElem = safe_cast<XmlElement^>(pXMLDoc->DocumentElement->SelectSingleNode("//pa:pareto_specification", nsmgr));
	psElem->AppendChild(pXMLConfSets);

	StorageMap::iterator i;
	for(i=pc->memory.begin(); i!=pc->memory.end(); i++){
		StorableObject* q= (*i).second;
		if(q->isConfigurationSet()){
			ConfigurationSet* cs = dynamic_cast<ConfigurationSet*>(q);
			XmlElement^ csNode = this->Save_XMLBuildConfigurationSetNode(cs);
			pXMLConfSets->AppendChild(csNode);
		}		
	}
}




void ParetoParser::LoadOperations() throw(...){
	// Load the operations from XML document

	if(!pXMLDoc) return;

	// Select the configuration set nodes
	XmlNodeList^ pXMLDomNodeList = pXMLDoc->DocumentElement->SelectNodes("//pa:pareto_specification/pa:calculation/pa:*", nsmgr);

	IEnumerator^ myEnum = pXMLDomNodeList->GetEnumerator();
	while (myEnum->MoveNext()){

		// for each operation pOperationNode ...
		XmlNode^ pOperationNode = safe_cast<XmlNode^>(myEnum->Current);

		if(pOperationNode->Name=="join"){
			pc->verbose("join on multiple attributes\n");

			JoinMap qm;

			// Select the configuration set nodes
			XmlNodeList^ pXMLQuantNodeList = pOperationNode->SelectNodes("pa:between", nsmgr);
			IEnumerator^ qEnum = pXMLQuantNodeList->GetEnumerator();
			while (qEnum->MoveNext()){
				// for each quantity pair ...
				XmlNode^ pJoinNode = safe_cast<XmlNode^>(qEnum->Current);
				std::string qa = system_to_std_string(pJoinNode->Attributes->GetNamedItem("quanta")->InnerXml);
				std::string qb = system_to_std_string(pJoinNode->Attributes->GetNamedItem("quantb")->InnerXml);
				qm[qa]=qb;
			}
			POperation_Join *op = new POperation_Join(&qm);
			op->executeOn(*pc);
			delete op;
		}
		if(pOperationNode->Name=="join_eff"){
			pc->verbose("efficient join on multiple attributes\n");

			JoinMap qm;

			// Select the configuration set nodes
			XmlNodeList^ pXMLQuantNodeList = pOperationNode->SelectNodes("pa:between", nsmgr);
			IEnumerator^ qEnum = pXMLQuantNodeList->GetEnumerator();
			while (qEnum->MoveNext()){
				// for each quantity pair ...
				XmlNode^ pJoinNode = safe_cast<XmlNode^>(qEnum->Current);
				std::string qa = system_to_std_string(pJoinNode->Attributes->GetNamedItem("quanta")->InnerXml);
				std::string qb = system_to_std_string(pJoinNode->Attributes->GetNamedItem("quantb")->InnerXml);
				qm[qa]=qb;
			}
			POperation_EfficientJoin *op = new POperation_EfficientJoin(&qm);
			op->executeOn(*pc);
			delete op;
		}
		else if(pOperationNode->Name=="abstract"){
			pc->verbose("abstraction of quantities\n");

			ListOfQuantityNames lqn;

			// Select the quantity nodes
			XmlNodeList^ pXMLQuantNodeList = pOperationNode->SelectNodes("pa:quant", nsmgr);
			IEnumerator^ qEnum = pXMLQuantNodeList->GetEnumerator();
			while (qEnum->MoveNext()){
				// for each quantity ...
				XmlNode^ pQuantNode = safe_cast<XmlNode^>(qEnum->Current);
				std::string q = system_to_std_string(pQuantNode->InnerXml);
				lqn.push_back(q);
			}
			POperation_Abstract *op = new POperation_Abstract(&lqn);
			op->executeOn(*pc);
			delete op;
		}
		else if(pOperationNode->Name=="hide"){
			pc->verbose("hiding quantities\n");

			ListOfQuantityNames lqn;

			// Select the quantity nodes
			XmlNodeList^ pXMLQuantNodeList = pOperationNode->SelectNodes("pa:quant", nsmgr);
			IEnumerator^ qEnum = pXMLQuantNodeList->GetEnumerator();
			while (qEnum->MoveNext()){
				// for each quantity ...
				XmlNode^ pQuantNode = safe_cast<XmlNode^>(qEnum->Current);
				std::string q = system_to_std_string(pQuantNode->InnerXml);
				lqn.push_back(q);
			}
			POperation_Hide *op = new POperation_Hide(&lqn);
			op->executeOn(*pc);
			delete op;
		}
		else if(pOperationNode->Name=="aggregate"){
			pc->verbose("aggregating attributes\n");

			ListOfQuantityNames lqn;

			// Select the quantity nodes
			XmlNodeList^ pXMLQuantNodeList = pOperationNode->SelectNodes("pa:quant", nsmgr);
			IEnumerator^ qEnum = pXMLQuantNodeList->GetEnumerator();
			while (qEnum->MoveNext()){
				// for each quantity ...
				XmlNode^ pQuantNode = safe_cast<XmlNode^>(qEnum->Current);
				std::string q = system_to_std_string(pQuantNode->InnerXml);
				lqn.push_back(q);
			}

			// Select the new name node
			XmlNode^ pXMLNameNode = pOperationNode->SelectSingleNode("pa:name", nsmgr);
			std::string newname = system_to_std_string(pXMLNameNode->InnerXml);
			
			POperation_Aggregate *op = new POperation_Aggregate(&lqn,newname);
			op->executeOn(*pc);
			delete op;
		}
		else if(pOperationNode->Name=="prodcons"){
			pc->verbose("applying producer consumer constraint\n");

			// Select the producer and consumer quantity nodes
			XmlNode^ pXMLProdNode = pOperationNode->SelectSingleNode("pa:producer_quant", nsmgr);
			XmlNode^ pXMLConsNode = pOperationNode->SelectSingleNode("pa:consumer_quant", nsmgr);
			std::string pq = system_to_std_string(pXMLProdNode->InnerXml);
			std::string cq = system_to_std_string(pXMLConsNode->InnerXml);
			ParetoCalculatorOperation *op = new POperation_ProdCons(pq,cq);
			op->executeOn(*pc);
			delete op;
		}
		else if(pOperationNode->Name=="prodcons_eff"){
			pc->verbose("applying producer consumer constraint\n");

			// Select the producer and consumer quantity nodes
			XmlNode^ pXMLProdNode = pOperationNode->SelectSingleNode("pa:producer_quant", nsmgr);
			XmlNode^ pXMLConsNode = pOperationNode->SelectSingleNode("pa:consumer_quant", nsmgr);
			std::string pq = system_to_std_string(pXMLProdNode->InnerXml);
			std::string cq = system_to_std_string(pXMLConsNode->InnerXml);
			ParetoCalculatorOperation *op = new POperation_EfficientProdCons(pq,cq);
			op->executeOn(*pc);
			delete op;
		}
		else if(pOperationNode->Name=="push"){
			pc->verbose("pushing object onto the stack\n");
			ParetoCalculatorOperation *op = new POperation_Push(system_to_std_string(pOperationNode->Attributes->GetNamedItem("name")->InnerXml));
			op->executeOn(*pc);
			delete op;
		}
		else if(pOperationNode->Name=="store"){
			pc->verbose("storing object from the stack\n");
			ParetoCalculatorOperation *op = new POperation_Store(system_to_std_string(pOperationNode->Attributes->GetNamedItem("name")->InnerXml));
			op->executeOn(*pc);
			delete op;
		}
		else if(pOperationNode->Name=="minimize"){
			pc->verbose("minimizing set of configurations\n");
			ParetoCalculatorOperation *op = new POperation_Minimize();
			op->executeOn(*pc);
			delete op;
		}
		else if(pOperationNode->Name=="minimize_eff"){
			pc->verbose("minimizing set of configurations\n");
			ParetoCalculatorOperation *op = new POperation_EfficientMinimize();
			op->executeOn(*pc);
			delete op;
		}
		else if(pOperationNode->Name=="pop"){
			pc->verbose("popping element from the stack\n");
			ParetoCalculatorOperation *op = new POperation_Pop();
			op->executeOn(*pc);
			delete op;
		}
		else if(pOperationNode->Name=="duplicate"){
			pc->verbose("duplicating element on top of the stack\n");
			ParetoCalculatorOperation *op = new POperation_Duplicate();
			op->executeOn(*pc);
			delete op;
		}
		else if(pOperationNode->Name=="print"){
			pc->verbose("printing element from the stack:\n");
			ParetoCalculatorOperation *op = new POperation_Print();
			op->executeOn(*pc);
			delete op;
		}
		else if(pOperationNode->Name=="product"){
			pc->verbose("computing product of sets of configurations\n");
			ParetoCalculatorOperation *op = new POperation_Product();
			op->executeOn(*pc);
			delete op;
		}
	}
}
	



vector<QuantityType*>& ParetoParser::getQuantityTypes(){
        vector<QuantityType*>* v = new(vector<QuantityType*>);
        return *v;
}