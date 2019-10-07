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

#ifndef UTILS_LIBXML_H
#define UTILS_LIBXML_H

#include <stdlib.h>
#include <string>
#include <libxml/parser.h>
#include <libxml/xpath.h>
#include <libxml/xpathInternals.h>


std::string wstring_to_string(std::wstring& wstr);
std::wstring string_to_wstring(std::string& str);
std::string xml_to_std_string(const xmlChar* s);

xmlXPathObjectPtr getNodeSetXPath (xmlDocPtr doc, xmlChar *xpath, xmlXPathContextPtr xpathCtx);
// execute an xpath query on the document

xmlXPathObjectPtr getNodeSetXPathNode (xmlDocPtr doc, xmlNodePtr node, xmlChar *xpath, xmlXPathContextPtr xpathCtx);
// execute an xpath query within a node

xmlNodePtr getNodeXPathNode(xmlDocPtr doc, xmlNodePtr node, xmlChar* xpath, xmlXPathContextPtr xpathCtx);
// execute an xpath query to get a single node result


std::string getNodeAttribute(xmlNodePtr n, xmlChar* attr);

std::string getNodeType(xmlNodePtr n);

bool hasNodeAttribute(xmlNodePtr n, xmlChar* attr);

std::string getNodeText(xmlDocPtr doc, xmlNodePtr n);

#endif
