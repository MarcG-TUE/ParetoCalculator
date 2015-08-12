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
// Implements the various kinds of quantity types and values
//


#include <sstream>
#include "quantity.h"
#include "calculator.h"

using namespace std;

namespace Pareto {

QuantityType::QuantityType(string n): StorableObject(n){
}

bool QuantityType::compare(const QuantityValue& q1, const QuantityValue& q2) const {
	throw *new EParetoCalculatorError("abstract class QuantityType cannot compare quantities");
	return 0;
}

bool QuantityType::equal(const QuantityValue& q1, const QuantityValue& q2) const {
	throw *new EParetoCalculatorError("abstract class QuantityType cannot compare quantities");
	return 0;
}

bool QuantityType::totalOrderSmaller(const QuantityValue& v1, const QuantityValue& v2){
	throw *new EParetoCalculatorError("abstract class QuantityType cannot compare quantities");
	return 0;
}

void QuantityType::streamOn(ostream& os) const {
os << name;
}

QuantityValue& QuantityType::defaultValue(void){
	throw *new EParetoCalculatorError("defaultValue in abstract class QuantityType should not be called");
	QuantityValue& v = *new QuantityValue(*this);
	return v;
}

QuantityValue& QuantityType::valueFromString(const string s){
	throw *new EParetoCalculatorError("valueFromString in abstract class QuantityType should not be called");
	return this->defaultValue();
}


bool operator==(QuantityType& t1, QuantityType& t2){
// for now, assume that every quantity type has a unique instance
// maybe refine with an equality test later
//return &t1 == &t2;
return t1.name == t2.name;
}

bool operator!=(QuantityType& t1, QuantityType& t2){
	return !(Pareto::operator ==(t1,t2));
}

/////////////// QuantityType_Enum ///////////////////

QuantityType_Enum::QuantityType_Enum(string n): QuantityType(n) {
}

void QuantityType_Enum::addQuantity(string s){
names.push_back(s);
quantities.push_back(new QuantityValue_Enum(*this, s));
};


void QuantityType_Enum::streamOn(ostream& os) const {
QuantityType::streamOn(os);
os << "{";
for(unsigned int i=0; i<quantities.size(); i++){
        os << *quantities[i];
        if(i<quantities.size()-1) os << ", ";
        }
os << "}";
}


QuantityValue& QuantityType_Enum::defaultValue(void){
return **quantities.begin();
}

QuantityValue& QuantityType_Enum::valueFromString(const string qn){
vector<string>::iterator n_iter;
vector<QuantityValue_Enum*>::iterator q_iter;

for (n_iter=names.begin(), q_iter=quantities.begin(); n_iter != names.end(); n_iter++, q_iter++)
  {
    if((*n_iter) == qn) {return **q_iter;}
  }
	throw *new EParetoCalculatorError("Quantity" + qn + " unknown in quantity type " + name +". default quantity returned in function getQuantity.");
	return this->defaultValue();
}


bool QuantityType_Enum::compare(const QuantityValue& q1, const QuantityValue& q2) const {
	unsigned int s = quantities.size();
	for(unsigned int i=0; i<s; i++){
		if(*quantities[i]==q1) return true;
		if(*quantities[i]==q2) return false;
	}
	throw *new EParetoCalculatorError("Error in QuantityType_Enum::compare");
	return false;
}

bool QuantityType_Enum::equal(const QuantityValue& q1, const QuantityValue& q2) const {
#ifdef _DEBUG
	const QuantityValue_Enum qe1 = dynamic_cast<const QuantityValue_Enum&>(q1);
	const QuantityValue_Enum qe2 = dynamic_cast<const QuantityValue_Enum&>(q2);
#else
	const QuantityValue_Enum qe1 = (const QuantityValue_Enum&)(q1);
	const QuantityValue_Enum qe2 = (const QuantityValue_Enum&)(q2);
#endif
	return (qe1.value==qe2.value);
}

bool QuantityType_Enum::totalOrderSmaller(const QuantityValue& v1, const QuantityValue& v2){
	//ListOfQuantityValueEnum::iterator i;
	//for(i = this->quantities.begin(); i != this->quantities.end(); i++){
	//	if((**i==v1) && !(**i==v2)) { return true;}
	//	if(**i==v2) { return false;}
	//}
	//throw *new EParetoCalculatorError("Error in QuantityType_Enum::totalOrderSmaller");
	//return false;
#ifdef _DEBUG
	const QuantityValue_Enum qe1 = dynamic_cast<const QuantityValue_Enum&>(v1);
	const QuantityValue_Enum qe2 = dynamic_cast<const QuantityValue_Enum&>(v2);
	return (qe1.value<qe2.value);
#else
	return (((const QuantityValue_Enum&)(v1)).value<((const QuantityValue_Enum&)(v2)).value);
#endif
}


////////////////// QuantityType_Enum_Unordered //////////////////

QuantityType_Enum_Unordered::QuantityType_Enum_Unordered(string n): QuantityType_Enum(n) {
}

bool QuantityType_Enum_Unordered::compare(const QuantityValue& q1, const QuantityValue& q2) const {
if(q1==q2) return true;
else return false;
}



///////////////// QuantityValue //////////////////
QuantityValue::QuantityValue(QuantityType& t): qtype(t){
}

void QuantityValue::streamOn(ostream& os) const {
os << "a value of " << qtype;
}

string& QuantityValue::asString(void) const {
	ostringstream myString;
	this->streamOn(myString);
	return *new string(myString.str());
}

bool operator<=(const QuantityValue& q1, const QuantityValue& q2){
#ifdef _DEBUG
	// only in debug mode, takes too much time otherwise
	if(&q1.qtype!=&q2.qtype){
		throw *new EParetoCalculatorError("Cannot compare quantities of different types.");
		return 0;}
#endif
	return q1.qtype.compare(q1,q2);
}

bool operator==(const QuantityValue& q1, const QuantityValue& q2){
	if(&q1.qtype!=&q2.qtype){
		throw *new EParetoCalculatorError("Cannot compare quantities of different types.");
		return 0;}
	return q1.qtype.equal(q1,q2);
}

bool operator<(const QuantityValue& q1, const QuantityValue& q2) {
		return q1<=q2 && !(q1==q2);}


///////////////// QuantityValue_Enum //////////////////

QuantityValue_Enum::QuantityValue_Enum(QuantityType& t, string v): QuantityValue(t){
value = v;
}

void QuantityValue_Enum::streamOn(ostream& os) const {
os << value;
}


///////////////// QuantityType_Integer ///////////////////

QuantityType_Integer::QuantityType_Integer(string n)
:QuantityType(n)
{
}

void QuantityType_Integer::streamOn(ostream& os) const {
os << name ;
}

QuantityValue& QuantityType_Integer::defaultValue(void){
QuantityValue* v = new QuantityValue_Integer(*this, 0);
return *v;
}

QuantityValue& QuantityType_Integer::valueFromString(const string s){
	int number = std::strtol(s.c_str(), 0, 10);
	return * new QuantityValue_Integer(*this, number); 
}


bool QuantityType_Integer::compare(const QuantityValue& q1, const QuantityValue& q2) const {
#ifdef _DEBUG
	return dynamic_cast<const QuantityValue_Integer&>(q1).value<=dynamic_cast<const QuantityValue_Integer&>(q2).value;
#else
	return ((const QuantityValue_Integer&)(q1)).value<=((const QuantityValue_Integer&)(q2)).value;
#endif
}

bool QuantityType_Integer::equal(const QuantityValue& q1, const QuantityValue& q2) const {
#ifdef _DEBUG
	return dynamic_cast<const QuantityValue_Integer&>(q1).value == dynamic_cast<const QuantityValue_Integer&>(q2).value;
#else
	return ((const QuantityValue_Integer&)(q1)).value == ((const QuantityValue_Integer&)(q2)).value;
#endif
}

bool QuantityType_Integer::totalOrderSmaller(const QuantityValue& v1, const QuantityValue& v2){
#ifdef _DEBUG
	const QuantityValue_Integer& i1 = dynamic_cast<const QuantityValue_Integer&>(v1);
	const QuantityValue_Integer& i2 = dynamic_cast<const QuantityValue_Integer&>(v2);
	return i1.value<i2.value;
#else
	return ((const QuantityValue_Integer&)(v1)).value<((const QuantityValue_Integer&)(v2)).value;
#endif
}

///////////////// QuantityValue_Integer ///////////////////


QuantityValue_Integer::QuantityValue_Integer(QuantityType& t, int n)
:QuantityValue(t), value(n)
{
}

void QuantityValue_Integer::streamOn(ostream& os) const {
os << value;
}





///////////////// QuantityType_Real ///////////////////

QuantityType_Real::QuantityType_Real(string n)
:QuantityType(n)
{
}

void QuantityType_Real::streamOn(ostream& os) const {
os << name ;
}

QuantityValue& QuantityType_Real::defaultValue(void){
QuantityValue* v = new QuantityValue_Real(*this, 0.0);
return *v;
}

QuantityValue& QuantityType_Real::valueFromString(const string s){
	return * new QuantityValue_Real(*this, strtod(s.c_str(),NULL));
}

bool QuantityType_Real::compare(const QuantityValue& q1, const QuantityValue& q2) const {
#ifdef _DEBUG
	return dynamic_cast<const QuantityValue_Real&>(q1).value<=dynamic_cast<const QuantityValue_Real&>(q2).value;
#else
	return ((const QuantityValue_Real&)(q1)).value<=((const QuantityValue_Real&)(q2)).value;
#endif
}

bool QuantityType_Real::equal(const QuantityValue& q1, const QuantityValue& q2) const {
#ifdef _DEBUG
	return dynamic_cast<const QuantityValue_Real&>(q1).value == dynamic_cast<const QuantityValue_Real&>(q2).value;
#else
	return ((const QuantityValue_Real&)(q1)).value == ((const QuantityValue_Real&)(q2)).value;
#endif
};

bool QuantityType_Real::totalOrderSmaller(const QuantityValue& v1, const QuantityValue& v2){
#ifdef _DEBUG
	const QuantityValue_Real& i1 = dynamic_cast<const QuantityValue_Real&>(v1);
	const QuantityValue_Real& i2 = dynamic_cast<const QuantityValue_Real&>(v2);
	return i1.value<i2.value;
#else
	return ((const QuantityValue_Real&)(v1)).value<((const QuantityValue_Real&)(v2)).value;
#endif
}


///////////////// QuantityValue_Real ///////////////////


QuantityValue_Real::QuantityValue_Real(QuantityType& t, double n)
:QuantityValue(t), value(n)
{
}

void QuantityValue_Real::streamOn(ostream& os) const {
os << value;
}






//////////////////// functions //////////////////

//ostream& operator<<(ostream& os, QuantityType& et){
// et.streamOn(os);
// return os;
//}

ostream& operator<<(ostream& os, QuantityValue& v){
 v.streamOn(os);
 return os;
}


unsigned int QuantityValue_Enum::index(void) const {
	unsigned int s = dynamic_cast<QuantityType_Enum&>(qtype).quantities.size();
	for(unsigned int i=0; i<s; i++){
		if(dynamic_cast<QuantityType_Enum&>(qtype).quantities[i]==this){return i;}
	}
	throw *new EParetoCalculatorError("Error: value not found in QuantitityType in QuantityValue_Enum::index");
	return 0;
}

}
