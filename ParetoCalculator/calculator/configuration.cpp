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
// Implementation of the Configuration and ConfigurationSpace, ConfigurationSet 
// classes and supporting classes, for instance to create a sorted index on a set 
// configurations
//

#include "configuration.h"
#include "exceptions.h"
#include <sstream> 
#include <algorithm>

namespace Pareto {

ConfigurationSpace::ConfigurationSpace(string n): StorableObject(n){
}

void ConfigurationSpace::addQuantity(QuantityType& q){
	this->addQuantityAsVisibility(q, q.name, true);
}

void ConfigurationSpace::addQuantityAs(QuantityType& q, QuantityName qname){
	this->addQuantityAsVisibility(q, qname, true);
}

void ConfigurationSpace::addQuantityAsVisibility(QuantityType& q, QuantityName qname, bool vis){
	quantities.push_back(&q);
	quantityNames[qname]=quantities.size()-1;
	this->quantityVisibility.push_back(vis);
}

void ConfigurationSpace::addQuantitiesOf(ConfigurationSpace& cs){
	for(unsigned int i=0; i<cs.quantities.size(); i++){
		this->addQuantityAsVisibility(*cs.quantities[i], cs.nameOfQuantityNr(i), cs.quantityVisibility[i]);
	}
}



QuantityType& ConfigurationSpace::getQuantity(const QuantityName& qn) throw(EParetoCalculatorError){
	if(quantityNames.find(qn)!=quantityNames.end()) return *quantities[quantityNames[qn]];
	ListOfQuantityTypes::iterator i;
	for(i=quantities.begin(); i!=quantities.end(); i++){
        if((*i)->name==qn) return *(*i);
        }
	throw *new EParetoCalculatorError("quantity "+qn+" not found in ConfigurationSpace::getQuantity");
}

unsigned int ConfigurationSpace::nrOfVisibleQuantities(void){
	unsigned int count = 0;
	BoolVector::iterator i;
	for(i = this->quantityVisibility.begin(); i!=this->quantityVisibility.end(); i++){
		if(*i) count++;
	}
	return count;
}


unsigned int ConfigurationSpace::indexOfQuantity(const QuantityName& qn) throw(EParetoCalculatorError){
	if(quantityNames.find(qn)==quantityNames.end()) {
		throw *new EParetoCalculatorError("quantity "+qn+" not found in ConfigurationSpace::indexOfQuantity");
	}
	return quantityNames[qn];
}

const QuantityName* ConfigurationSpace::getUnorderedQuantity(){
	QuantityIntMap::iterator i;
	unsigned int k;
	for(k=0, i = this->quantityNames.begin(); i!=this->quantityNames.end(); i++, k++){
		QuantityType* t = this->quantities[(*i).second];
		if((this->quantityVisibility[k]) && (t->isUnordered())){
			string *qn = new string;
			*qn = (*i).first;
			return qn;
		}
	}
	return NULL;
}

const QuantityName* ConfigurationSpace::getTotallyOrderedQuantity(){
	QuantityIntMap::iterator i;
	unsigned int k;
	for(k=0, i = this->quantityNames.begin(); i!=this->quantityNames.end(); i++, k++){
		QuantityType* t = this->quantities[(*i).second];
		if((this->quantityVisibility[k]) && (t->isTotallyOrdered())){
			string *qn = new string;
			*qn = (*i).first;
			return qn;
		}
	}
	return NULL;
}


bool ConfigurationSpace::includesQuantityNamed(const QuantityName& qn){
	QuantityIntMap::iterator i;
	for(i=quantityNames.begin(); i!=quantityNames.end(); i++){
		if((*i).first==qn) return true;
        }
	return false;
}

const unsigned int ConfigurationSpace::firstVisibleQuantity(){
	BoolVector::iterator i;
	unsigned int k;
	for(i=this->quantityVisibility.begin(), k=0; i!=this->quantityVisibility.end(); i++, k++){
		if(*i) return k;
	}
	throw *new EParetoCalculatorError("no visible quantity found in ConfigurationSpace::firstVisibleQuantity");
	return 0;
}


bool ConfigurationSpace::compare(const Configuration& c1, const Configuration& c2){
	unsigned int i=0;
	for(ListOfQuantityTypes::iterator j=this->quantities.begin(); j!=this->quantities.end(); i++, j++){
		if(this->quantityVisibility[i] && !(c1.getQuantity(i)<=c2.getQuantity(i))){return false;}
	}
	return true;
}

bool ConfigurationSpace::equal(const Configuration& c1, const Configuration& c2){
	for(unsigned int i=0; i<this->quantities.size(); i++){
		if(this->quantityVisibility[i] && !(c1.getQuantity(i)==c2.getQuantity(i))){return false;}
	}
	return true;
}

bool ConfigurationSpace::LexicographicCompare(const Configuration& c1, const Configuration& c2)
// provides strict ordering for sorted storage of configurationset, hence ignores visibility
{
	unsigned int k = c1.confspace->quantities.size();
	for(unsigned int i=0; i<k; i++){
		const QuantityValue *v1 = &(c1.getQuantity(i));
		const QuantityValue *v2 = &(c2.getQuantity(i));
		if(v1->totalOrderSmaller(*v2)){return true;}
		if(v2->totalOrderSmaller(*v1)){return false;}
	}
	return false;
}



void const ConfigurationSpace::streamOn(ostream& os){
	os << name << " = ";
	unsigned int n;
	for (n = 0; n < this->quantities.size(); n++){
		if(this->quantityVisibility[n]) {
			os << this->nameOfQuantityNr(n);
		} else {
			os << "[" << this->nameOfQuantityNr(n) << "]";
		}
		if(n<quantities.size()-1) os<< " x ";
	}
}

string& Configuration::asString(void) {
	ostringstream myString;
	this->streamOn(myString);
	return *new string(myString.str());
}


ConfigurationSpace* ConfigurationSpace::hide(ListOfQuantityNames *lqn){
	string n = "Hide of " + this->name;
	ConfigurationSpace* cs = new ConfigurationSpace(n);
	for(unsigned int i=0; i<this->quantities.size(); i++){
		bool v = this->quantityVisibility[i];
		if(lqn->includes(this->nameOfQuantityNr(i))) { 
			v=false; }
		cs->addQuantityAsVisibility(*this->quantities[i], this->nameOfQuantityNr(i), v);
	}
	return cs;
}

ConfigurationSpace* ConfigurationSpace::hide(const QuantityName& qn){
	ListOfQuantityNames l;
	l.push_back(qn);
	return this->hide(&l);
}


ConfigurationSpace* ConfigurationSpace::unhide(ListOfQuantityNames *lqn){
	string n = "Unhide of " + this->name;
	ConfigurationSpace* cs = new ConfigurationSpace(n);
	for(unsigned int i=0; i<this->quantities.size(); i++){
		bool v = this->quantityVisibility[i];
		if(lqn->includes(this->nameOfQuantityNr(i))) { 
			v=true; }
		cs->addQuantityAsVisibility(*this->quantities[i], this->nameOfQuantityNr(i), v);
	}
	return cs;
}

ConfigurationSpace* ConfigurationSpace::unhide(const QuantityName& qn){
	ListOfQuantityNames l;
	l.push_back(qn);
	return this->unhide(&l);
}



const QuantityName ConfigurationSpace::nameOfQuantityNr(const unsigned int n){
	QuantityIntMap::iterator i;
	for(i = this->quantityNames.begin(); i!=this->quantityNames.end(); i++){
		if((*i).second==n) { return (*i).first;}
	}
	throw *new EParetoCalculatorError("Error: quantity not found in function nameOfQuantitynr.");
}


Configuration* ConfigurationSpace::newConfiguration(void){

	Configuration* c=new Configuration(this);

	ListOfQuantityTypes::iterator i;
	for (i = quantities.begin(); i != quantities.end(); i++){
		QuantityValue& v = (*i)->defaultValue();
		c->addQuantity(v);
	}
	return c;
}

bool operator!=(ConfigurationSpace& cs1, ConfigurationSpace& cs2){
// maybe needs to be made more advanced later (a real comparison)
return &cs1 != &cs2;
}

/////////////////// Configuration //////////////////////

Configuration::Configuration(ConfigurationSpace* cs):confspace(cs){
}

void Configuration::addQuantity(const QuantityValue& q){
	Pareto::QuantityValue* qp = const_cast<Pareto::QuantityValue*>(&q);
	quantities.push_back(qp);
}

void Configuration::addQuantitiesOf(const Configuration& c){
	Configuration* cc = const_cast<Pareto::Configuration*>(&c); // temporary (I hope) workaround
	ListOfQuantityValues::iterator i;
	for(i=cc->quantities.begin(); i!=cc->quantities.end(); i++){
        this->addQuantity(*(*i));
        }
}


void Configuration::streamOn(ostream& os) const {
	os << "(";
	ListOfQuantityValues::const_iterator i;
	unsigned int n=0;
	for (i = quantities.begin(); i != quantities.end(); i++, n++){
		if(this->confspace->quantityVisibility[n]){
			QuantityValue* v = (*i);	
			os << *v;
		} else {
			os << "[" << *(*i) << "]";
		}
		if(n<quantities.size()-1) os<< ", ";
	}
	os << ")";
}

void Configuration::setQuantity(int n, QuantityValue* v){
quantities[n]=v;
}


const QuantityValue& Configuration::getQuantity(const unsigned int n) const {
#ifdef _DEBUG
	if(n>quantities.size()){
		throw *new EParetoCalculatorError("Index out of bounds in Configuration::getQuantity(int).");
        return *quantities[0];
        }
#endif
	return *quantities[n];
}

const QuantityValue& Configuration::getQuantity(const QuantityName& n) const {
	unsigned int k = this->confspace->indexOfQuantity(n);
	return *this->quantities[k];
}


bool operator<=(const Configuration& c1, const Configuration& c2){
#ifdef _DEBUG
	// convenient for debugging, too slow for real
	if(c1.confspace!=c2.confspace){ 
		throw *new EParetoCalculatorError("Error: cannot compare configurations of different configuration spaces.");
		return 0;
	}
#endif
	return c1.confspace->compare(c1,c2);
}

bool operator==(const Configuration& c1, const Configuration& c2){
#ifdef _DEBUG
	if(c1.confspace!=c2.confspace){ 
		throw *new EParetoCalculatorError("Error: cannot compare configurations of different configuration spaces.");
		return 0;}
#endif
	return c1.confspace->equal(c1,c2);
}

bool operator<(const Configuration& c1, const Configuration& c2){ return (c1<=c2)&& !(c1==c2);};


///////////////// ConfigurationSet ///////////

ConfigurationSet::ConfigurationSet(ConfigurationSpace* cs, string n): StorableObject(n), confspace(cs) {
}


void ConfigurationSet::addConfiguration(const Configuration& c){
#ifdef _DEBUG
	if(c.confspace!=this->confspace){
		throw *new EParetoCalculatorError("Error: configuration is of wrong type in ConfigurationSet::addConfiguration");
	}
#endif
	confs.insert(c);
}

void ConfigurationSet::addUniqueConfiguration(const Configuration& c){
	// Note: Use this method only if you are sure that the configuration does not already occur in the set!
#ifdef _DEBUG
	if(c.confspace!=this->confspace){
		throw *new EParetoCalculatorError("Error: configuration is of wrong type in ConfigurationSet::addConfiguration");
	}
#endif
	confs.insert(c);
}

void ConfigurationSet::addConfigurationAndFilter(const Configuration& c){
	// assumes the configurations set is minimal
	// post: set is minimal version of set plus new configuration
	SetOfConfigurations::iterator i = this->confs.begin();
	while(i!= this->confs.end()){
		const Configuration& a = *i;
		if (a<=c) {
			// c is dominated by a conf in this -> don't use c
			return;
		}
		else if (c<=a) {
			// remove a
			SetOfConfigurations::iterator it = i;
			i++;
			this->confs.erase(it);
		}
		else
			i++;
	}
	// c is not dominated by any conf in this -> add c to this
	this->confs.insert(c);
}

void ConfigurationSet::addUniqueConfigurationsOf(ConfigurationSet& cs){
	for(SetOfConfigurations::iterator i= cs.confs.begin(); i!= cs.confs.end(); i++){
		const Configuration* c = &(*i);
		this->addUniqueConfiguration(*c);
	}
}


bool ConfigurationSet::containsConfiguration(Configuration& c){
	return confs.find(c)!=confs.end();
}

void const ConfigurationSet::streamOn(ostream& os){
	os << "{";
	SetOfConfigurations::iterator i;
	unsigned int n=0;
	for(i=confs.begin(); i!=confs.end(); i++, n++){
		const Configuration* c = &(*i);
        os << c;
        if(n<confs.size()-1) os << ", " << endl;
        }
	os << "}";
}

const QuantityValue& ConfigurationIndexReference::value(void) const {
	return (this->conf)->getQuantity(index->quantity);
}


bool ConfigurationIndexReference::operator==(const ConfigurationIndexReference& right) const {
	return this->value()==right.value();
}

bool ConfigurationIndexOnTotalOrderReference::operator<(const ConfigurationIndexReference& right) const {
	return this->value()<right.value();
}

bool ConfigurationIndexReference::operator>(const ConfigurationIndexReference& right) const {
	return right < *this;
}

bool ConfigurationIndexReference::operator<(const ConfigurationIndexReference& right) const {
	throw *new EParetoCalculatorError("Error: < should not be called on abstract ConfigurationIndexReference");
}

ConfigurationIndexReference* IndexOnConfigurationSet::get(int n){
	throw *new EParetoCalculatorError("Error: 'get' should not be called on abstract ConfigurationIndexReference");
}

ConfigurationSet* IndexOnConfigurationSet::copyFromTo(int f, int t){
	throw *new EParetoCalculatorError("Error: 'copyFromTo' should not be called on abstract ConfigurationIndexReference");
}



const ConfigurationIndexOnTotalOrderReference& ConfigurationIndexOnTotalOrderReference::operator= (const ConfigurationIndexOnTotalOrderReference& right) {
	this->conf = right.conf;
	this->index = right.index;
	return *this;
}


IndexOnConfigurationSet::IndexOnConfigurationSet(const QuantityName& qn, ConfigurationSet* cs):quantity(qn),confset(cs){
}


IndexOnTotalOrderConfigurationSet::IndexOnTotalOrderConfigurationSet(const QuantityName& qn, ConfigurationSet* cs):IndexOnConfigurationSet(qn,cs){
	SetOfConfigurations::iterator i;
	for(i = cs->confs.begin(); i != cs->confs.end(); i++){
		ConfigurationIndexOnTotalOrderReference* r = new ConfigurationIndexOnTotalOrderReference(&(*i), this);
		this->push_back(*r);
		delete r; 
	}
	std::sort(this->begin(), this->end());
}

// lower returns the smallest index such that S[i,Q]>=v
int IndexOnConfigurationSet::lower(const ConfigurationIndexReference& v){
	int a,b,m;
	a=-1; // S[a,Q]<v
	b=this->confset->confs.size(); // S[b,Q]>=v;
	while(b-a>1){
		m = (a+b)/2;
		if(*(this->get(m))<v){
			a = m;
		} else {
			b = m;
		}
	}
	return b;
}


// upper returns the largest index such that S[i,Q]<=v
int IndexOnConfigurationSet::upper(const ConfigurationIndexReference& v){
	int a,b,m;
	a=-1; // S[a,Q]<=v
	b=this->confset->confs.size(); // S[b,Q]>v;
	while(b-a>1){
		m = (a+b)/2;
		if((*this->get(m))>v){
			b = m;
		} else {
			a = m;
		}
	}
	return a;
}


ConfigurationSet* IndexOnTotalOrderConfigurationSet::copyFromTo(int f, int t){
	ConfigurationSet* res = new ConfigurationSet(this->confset->confspace, this->confset->name + " range");
	IndexOnTotalOrderConfigurationSet::iterator i = this->begin();
	for (int j = 0; j<f; j++, i++);
	for (int j = f; j<=t; j++, i++){
		res->addConfiguration(*(*i).conf);
	}
	return res;
}


ConfigurationSet* IndexOnUnorderedConfigurationSet::copyFromTo(int f, int t){
	ConfigurationSet* res = new ConfigurationSet(this->confset->confspace, this->confset->name + " range");
	IndexOnUnorderedConfigurationSet::iterator i = this->begin();
	for (int j = 0; j<f; j++, i++);
	for (int j = f; j<=t; j++, i++){
		res->addConfiguration(*(*i).conf);
	}
	return res;
}


bool ConfigurationIndexOnUnorderedReference::operator<(const ConfigurationIndexReference& right) const {
	const QuantityValue* addrA = &(this->value());
	const QuantityValue* addrB = &(right.value());
	return addrA<addrB;
// Todo: check, it this always safe. maybe replace by efficient direct comparison of strings without making too many copies.
// was:	return this->value().asString()<right.value().asString();
}


const ConfigurationIndexOnUnorderedReference& ConfigurationIndexOnUnorderedReference::operator= (const ConfigurationIndexOnUnorderedReference& right) {
	this->conf = right.conf;
	this->index = right.index;
	return *this;
}


IndexOnUnorderedConfigurationSet::IndexOnUnorderedConfigurationSet(const QuantityName& qn, ConfigurationSet* cs): IndexOnConfigurationSet(qn, cs){
	SetOfConfigurations::iterator i;
	for(i = cs->confs.begin(); i != cs->confs.end(); i++){
		ConfigurationIndexOnUnorderedReference* r = new ConfigurationIndexOnUnorderedReference(&(*i), this);
		this->push_back(*r);
		delete r; 
	}
	std::sort(this->begin(), this->end());
}



///////////////// functions /////////////////



ostream& operator<<(ostream& os, const Configuration* c){
	c->streamOn(os);
	return os;
}

}
