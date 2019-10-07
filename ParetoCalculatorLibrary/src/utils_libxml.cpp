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
// LibXML specific utility functions
//

#include "utils_libxml.h"

// Strings

std::string wstring_to_string(std::wstring& wstr) {
	std::string str;
	size_t ressz;
	size_t len = wstr.length();
	size_t outputsz = len + 1;
	char* psz = new char[outputsz];
	wcstombs_s(&ressz, psz, outputsz, wstr.c_str(), len);
	str = psz;
	delete[] psz;
	return str;
}

std::wstring string_to_wstring(std::string& str) {
	std::wstring wstr;
	size_t ressz;
	size_t len = str.length();
	size_t outputsz = len + 1;
	wchar_t* psz = new wchar_t[len + 1];
	//mbstowcs(psz, str.c_str(),len + 1);
	mbstowcs_s(&ressz, psz, outputsz, str.c_str(), len);
	wstr = psz;
	delete[] psz;
	return wstr;
}

std::string xml_to_std_string(xmlChar* s) {
	std::string* ns = new std::string((char*)s);
	return *ns;
}

xmlXPathObjectPtr getNodeSetXPath(xmlDocPtr doc, xmlChar* xpath, xmlXPathContextPtr xpathCtx) {

	xmlXPathObjectPtr result;

	result = xmlXPathEvalExpression(xpath, xpathCtx);
	if (result == NULL) {
		printf("Error in xmlXPathEvalExpression\n");
		return NULL;
	}
	if (xmlXPathNodeSetIsEmpty(result->nodesetval)) {
		xmlXPathFreeObject(result);
		printf("No result\n");
		return NULL;
	}
	return result;
}

xmlXPathObjectPtr getNodeSetXPathNode(xmlDocPtr doc, xmlNodePtr node, xmlChar* xpath, xmlXPathContextPtr xpathCtx) {

	xmlXPathObjectPtr result;

	xpathCtx->node = node;

	result = xmlXPathEvalExpression(xpath, xpathCtx);

	if (result == NULL) {
		printf("Error in xmlXPathEvalExpression\n");
		return NULL;
	}
	if (xmlXPathNodeSetIsEmpty(result->nodesetval)) {
		xmlXPathFreeObject(result);
		printf("No result\n");
		return NULL;
	}
	return result;
}

xmlNodePtr getNodeXPathNode(xmlDocPtr doc, xmlNodePtr node, xmlChar* xpath, xmlXPathContextPtr xpathCtx)
{
	xmlXPathObjectPtr nodeSet = getNodeSetXPathNode(doc, node, xpath, xpathCtx);
	xmlNodeSetPtr resultset = nodeSet->nodesetval;
	if (resultset->nodeNr == 0) return NULL;
	xmlNodePtr result = resultset->nodeTab[0];
	xmlXPathFreeObject(nodeSet);
	return result;
}


std::string getNodeAttribute(xmlNodePtr n, xmlChar* attr) {

	// porting, retrieve without namespace awareness for now
	xmlChar* a = xmlGetProp(n, attr);
	return xml_to_std_string(a);
}

bool hasNodeAttribute(xmlNodePtr n, xmlChar* attr) {

	// porting, retrieve without namespace awareness for now
	bool b = (xmlHasProp(n, attr) != NULL);
	return b;
}

std::string getNodeText(xmlDocPtr doc, xmlNodePtr n) {
	return xml_to_std_string(xmlNodeGetContent(n));
}
