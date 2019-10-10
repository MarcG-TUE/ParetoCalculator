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
// Implementation of the ParetoCalculator class, implementing the Pareto Algebra operations
//

#include "calculator.h"
#include <sstream>
#include <math.h>

#include "paretoparser_libxml.h"


using namespace Pareto;



ParetoCalculator::ParetoCalculator() {
	statusObject = NULL;
	parser = NULL;
}

ConfigurationSet& ParetoCalculator::productInSpace(const ConfigurationSet& cs1, const ConfigurationSet& cs2, ConfigurationSpace* cspace) {

	ConfigurationSet* prod = new ConfigurationSet(cspace, "Product(" + cs1.name + ", " + cs2.name + ")");
	SetOfConfigurations::iterator i;
	for (i = cs1.confs.begin(); i != cs1.confs.end(); i++) {
		const Configuration* c1 = &(*i);
		SetOfConfigurations::iterator j;
		for (j = cs2.confs.begin(); j != cs2.confs.end(); j++) {
			const Configuration* c2 = &(*j);
			Configuration* c = new Configuration(cspace);
			c->addQuantitiesOf(*c1);
			c->addQuantitiesOf(*c2);
			prod->addUniqueConfiguration(*c);
		}
	}
	return *prod;
}

ConfigurationSet& ParetoCalculator::product(const ConfigurationSet& cs1, const ConfigurationSet& cs2) {
	// Create a configuration space
	ConfigurationSpace* cspace = new ConfigurationSpace("Product(" + cs1.confspace->name + ";" + cs2.confspace->name + ")");
	cspace->addQuantitiesOf(*cs1.confspace);
	cspace->addQuantitiesOf(*cs2.confspace);

	return ParetoCalculator::productInSpace(cs1, cs2, cspace);
}


ConfigurationSet& ParetoCalculator::constraint(const ConfigurationSet& cs1, const ConfigurationSet& cs2) {
	// not yet tested!!!
	// assumptions: 
	// - ConfigurationSet is a sorted collection according to the order defined
	//   as ConfigurationSpace::LexicographicCompare
	// - iterating over SetOfConfigurations provides the configurations in sorted order.

	// Make a new set to hold the result
	ConfigurationSet* res = new ConfigurationSet(cs1.confspace, "Constraint");

	SetOfConfigurations::iterator i1 = cs1.confs.begin();
	SetOfConfigurations::iterator i2 = cs2.confs.begin();

	while ((i1 != cs1.confs.end()) && (i2 != cs2.confs.end())) {
		const Configuration& c1 = *i1;
		const Configuration& c2 = *i2;
		if (ConfigurationSpace::LexicographicCompare(c1, c2)) {
			// forget c1
			i1++;
		}
		else if (ConfigurationSpace::LexicographicCompare(c2, c1)) {
			// forget c2
			i2++;
		}
		else {
			// they are equal, insert
			res->addUniqueConfiguration(c1);
			i1++; i2++;
		}
	}
	return *res;
}


ConfigurationSet& ParetoCalculator::constraint(const ConfigurationSet& cs, bool (*testConstraint)(const Pareto::Configuration&)) {
	ConfigurationSet* res = new ConfigurationSet(cs.confspace, "Constraint");
	SetOfConfigurations::iterator i;
	for (i = cs.confs.begin(); i != cs.confs.end(); i++) {
		const Configuration& c = *i;
		if ((*testConstraint)(c)) { res->addUniqueConfiguration(c); }
	}
	return *res;
}

ConfigurationSet& ParetoCalculator::alternative(const ConfigurationSet& cs1, const ConfigurationSet& cs2) {
	ConfigurationSet* res = new ConfigurationSet(cs1.confspace, "Alternative");

	SetOfConfigurations::iterator i;
	for (i = cs1.confs.begin(); i != cs1.confs.end(); i++) {
		const Configuration& c = *i;
		res->addUniqueConfiguration(c);
	}
	for (i = cs2.confs.begin(); i != cs2.confs.end(); i++) {
		Configuration* nc = new Configuration(res->confspace);
		nc->addQuantitiesOf(*i);
		res->addConfiguration(*nc);
	}
	return *res;
}


Pareto::Configuration& conf_abstraction(ConfigurationSpace* cs, const Pareto::Configuration& c, unsigned int n) {
	Pareto::Configuration* nc = new Pareto::Configuration(cs);

	// temporary fix for porting
	Pareto::Configuration* d = const_cast<Pareto::Configuration*>(&c);

	ListOfQuantityValues::iterator i;
	unsigned int k = 0;
	for (i = d->quantities.begin(); i != d->quantities.end(); i++, k++) {
		if (k != n) {
			nc->addQuantity(d->getQuantity(k));
		}
	}
	return *nc;
}

ConfigurationSet& ParetoCalculator::abstraction(const ConfigurationSet& cs, unsigned int n) {
	// Create a configuration space
	ConfigurationSpace* cspace = new ConfigurationSpace("Abstract(" + cs.confspace->name + "," + cs.confspace->nameOfQuantityNr(n) + ")");

	unsigned int k = 0;
	for (ListOfQuantityTypes::const_iterator i = cs.confspace->quantities.begin(); i != cs.confspace->quantities.end(); i++, k++) {
		if (k != n) {
			cspace->addQuantityAsVisibility(*(*i), cs.confspace->nameOfQuantityNr(k), cs.confspace->quantityVisibility[k]);
		}
	}

	ConfigurationSet* res = new ConfigurationSet(cspace, ("Abstraction(" + cs.name + ", ").append(n, 'a') + ")");
	// add configurations
	SetOfConfigurations::iterator i;
	for (i = cs.confs.begin(); i != cs.confs.end(); i++) {
		const Configuration& nc = conf_abstraction(cspace, (*i), n);
		res->addConfiguration(nc);
	}
	return *res;
}


ConfigurationSet& ParetoCalculator::abstraction(const ConfigurationSet& cs, const StorableString& s) {
	return ParetoCalculator::abstraction(cs, s.name);
}

ConfigurationSet& ParetoCalculator::abstraction(const ConfigurationSet& cs, const QuantityName& qn) {

	if (!cs.confspace->includesQuantityNamed(qn)) {
		throw EParetoCalculatorError("Quantity " + qn + "does not exist in ParetoCalculator::abstraction");
	}

	unsigned int n = cs.confspace->quantityNames.at(qn);
	return ParetoCalculator::abstraction(cs, n);
}



void ParetoCalculator::abstract(void) {
	const StorableObject& s = *this->pop();
	if (!s.isString()) {
		throw EParetoCalculatorError("Abstraction requires quantity name and configuration set on the stack");
		return;
	}
	const StorableString& ss = dynamic_cast<const StorableString&>(s);
	const StorableObject& o = *this->pop();
	if (!o.isConfigurationSet()) {
		throw EParetoCalculatorError("Abstraction requires quantity name and configuration set on the stack");
		return;
	}
	const ConfigurationSet& cs = dynamic_cast<const ConfigurationSet&>(o);
	this->push(this->abstraction(cs, ss));
}


void ParetoCalculator::hide(ListOfQuantityNames* lqn) {
	const StorableObject& so = *this->pop();
	if (!so.isConfigurationSet()) {
		throw EParetoCalculatorError("Hiding requires a configuration set on the stack");
		return;
	}
	const ConfigurationSet& cs = dynamic_cast<const ConfigurationSet&>(so);
	ConfigurationSet& ncs = ParetoCalculator::hiding(cs, lqn);
	this->push(ncs);
}

ConfigurationSet& ParetoCalculator::hiding(const ConfigurationSet& cs, const ListOfQuantityNames* lqn) {
	const ConfigurationSpace* csp = cs.confspace->hide(lqn);
	std::string nm = "Hide of " + cs.name;
	ConfigurationSet* ncs = new ConfigurationSet(csp, nm);
	SetOfConfigurations::iterator i;
	for (i = cs.confs.begin(); i != cs.confs.end(); i++) {
		Configuration* cf = new Configuration(csp);
		cf->addQuantitiesOf(*i);
		ncs->addConfiguration(*cf);
	}
	return *ncs;
}

ConfigurationSet& ParetoCalculator::hiding(const ConfigurationSet& cs, const QuantityName& qn) {
	ListOfQuantityNames lqn;
	lqn.push_back(qn);
	return ParetoCalculator::hiding(cs, &lqn);
}



ConfigurationSet& ParetoCalculator::minimize(const ConfigurationSet& cs) {
	// The naive way

	ConfigurationSet* res = new ConfigurationSet(cs.confspace, "min(" + cs.name + ")");

	// Note that the following gives a copy of the set of configurations
	SetOfConfigurations confs = cs.confs;

	// invariants of the loop: 
	// - the union of confs and res is Pareto equivalent to the original set cs.confs
	// - res in Pareto minimal
	while (!confs.empty()) {
		// remove arbitrary element from the set
		SetOfConfigurations::iterator cfp = confs.begin();
		Configuration c = *cfp;
		confs.erase(cfp);

		// walk throught the other configurations
		// find one configuration that is Pareto optimal with no points dominated by it left in the set.
		// invariant of the loop: configuration c is optimal among the configurations up to the iterator and up to the iterator there are no configurations in the
		// set that c dominates.
		SetOfConfigurations::iterator i = confs.begin();
		while (i != confs.end()) {
			const Configuration* cc = &(*i);
			if (c <= *cc) { // remove cc as it is dominated and continue
				// for port to linux, which does not support (apparently) the erase returning a new iterator
				// some people on the internet claim the following code is safe.
				SetOfConfigurations::iterator j;
				j = i;
				j++;
				confs.erase(i);
				i = j;
			}
			else {
				if (*cc <= c) {
					// forget about c, it is dominated by cc. Continue with cc, but 
					// start from the beginning to make sure all points it dominates will be removed.
					c = *cc;
					confs.erase(i);
					i = confs.begin();
				}
				else { i++; }
			}
		}
		res->addUniqueConfiguration(c);
	}
	return *res;
}

ConfigurationSet& ParetoCalculator::minimize_SC(ConfigurationSet& cs) {
	// Simple Cull

	ConfigurationSet* res = new ConfigurationSet(cs.confspace, "min(" + cs.name + ")");

	SetOfConfigurations::iterator i;
	for (i = cs.confs.begin(); i != cs.confs.end(); i++) {
		const Configuration& c = *i;
		res->addConfigurationAndFilter(c);
	}
	return *res;
}

ConfigurationSet* ParetoCalculator::efficient_minimize_unordered(const ConfigurationSet* cs, const QuantityName& qn) {
	ConfigurationSet* res = new ConfigurationSet(cs->confspace, "min(" + cs->name + ")");

	// split confset into classes with same value for qn
	ListOfConfSet* cls = splitClasses(cs, qn);

	// minimize each class separately and add to result
	ListOfConfSet::iterator i;
	for (i = cls->begin(); i != cls->end(); i++) {
		ConfigurationSet* cx = *i;

		// abstract from quantity qn
		ConfigurationSet* cxa = &ParetoCalculator::abstraction(*cx, qn);

		//minimize *i after abstraction of qn
		const ConfigurationSet* mcxa = &ParetoCalculator::efficient_minimize(*cxa);

		//add configurations of the result after adding qn again

		// add all configurations of mcxa
		unsigned int p = cs->confspace->indexOfQuantity(qn);
		SetOfConfigurations::iterator j;
		for (j = mcxa->confs.begin(); j != mcxa->confs.end(); j++) {
			Configuration* c = new Configuration(cs->confspace);
			const Configuration& c_mcxa = *j;
			const QuantityValue& x = (*cx->confs.begin()).getQuantity(p);
			//c-> add quantities with x at the right place ....
			for (unsigned int k = 0; k < mcxa->confspace->quantities.size(); k++) {
				if (k == p) c->addQuantity(x);
				c->addQuantity(c_mcxa.getQuantity(k));
			}
			res->addConfiguration(*c);
		}
	}
	return res;
}


ListOfConfSet* ParetoCalculator::splitClasses(const ConfigurationSet* cs, const QuantityName& qn) {
	ListOfConfSet* lcs = new ListOfConfSet();

	//	create unordered index on cs,qn
	IndexOnUnorderedConfigurationSet i(qn, cs);

	unsigned int l, u;
	unsigned int k = 0;
	while (k < i.size()) {
		ConfigurationIndexReference& r = i.at(k);
		l = i.lower(r);
		u = i.upper(r);
		ConfigurationSet* ncs = new ConfigurationSet(cs->confspace, "class " + r.value().asString());
		for (unsigned int n = l; n <= u; n++) {
			ncs->addConfiguration(*(i.at(n).conf));
		}
		lcs->push_back(ncs);
		k = u + 1;
	}
	return lcs;
}


const ConfigurationSet* ParetoCalculator::efficient_minimize_filter1(const ConfigurationSet* csl, const ConfigurationSet* csh, const QuantityName* qn) {
	// assume: csl is minimal set of configurations and csh is minimal set of configurations. Return 
	// set of configurations with configurations from csh removed that are strictly dominated by configurations
	// of csl.
		// note: there cannot be any unordered quantities

	if (csl->confs.size() == 0) { return csh; }
	if (csh->confs.size() == 0) { return csh; }

	ConfigurationSet* csla = &ParetoCalculator::hiding(*csl, *qn);
	ConfigurationSet* csha = &ParetoCalculator::hiding(*csh, *qn);
	const ConfigurationSet* filtered = ParetoCalculator::efficient_minimize_filter2(csla, csha);
	ConfigurationSet* res = new ConfigurationSet(csl->confspace, "temp");
	SetOfConfigurations::iterator i;
	for (i = filtered->confs.begin(); i != filtered->confs.end(); i++) {
		Configuration* c = new Configuration(res->confspace);
		c->addQuantitiesOf(*i);
		res->addConfiguration(*c);
	}

	return res;
}

const QuantityValue& ParetoCalculator::efficient_minimize_getPivot(const ConfigurationSet* cs, const QuantityName* qn) {
	IndexOnTotalOrderConfigurationSet i = IndexOnTotalOrderConfigurationSet(*qn, cs);
	return i.at(i.size() / 2).conf->getQuantity(*qn);
}

void ParetoCalculator::efficient_minimize_filter_split(const ConfigurationSet* cs, const QuantityName* qn, const QuantityValue& pivot, ConfigurationSet** csl, ConfigurationSet** csh) {
	IndexOnTotalOrderConfigurationSet i = IndexOnTotalOrderConfigurationSet(*qn, cs);
	*csl = new ConfigurationSet(cs->confspace, "temp");
	*csh = new ConfigurationSet(cs->confspace, "temp");
	IndexOnTotalOrderConfigurationSet::iterator j;
	for (j = i.begin(); j != i.end(); j++) {
		const Configuration* c = (*j).conf;
		if (c->getQuantity(*qn) < pivot) { (*csl)->addConfiguration(*c); }
		else break;
	}
	for (; j != i.end(); j++) {
		const Configuration* c = (*j).conf;
		(*csh)->addConfiguration(*c);
	}
}



const ConfigurationSet* ParetoCalculator::efficient_minimize_filter2(const ConfigurationSet* csa, const ConfigurationSet* csb) {
	// assume: csl is minimal set of configurations and csh is minimal set of configurations. Return 
	// set of configurations with configurations from csh removed that are strictly dominated by configurations
	// of csl.
		// note: there cannot be any unordered quantities

	if (csa->confs.size() == 0) { return csb; }
	if (csb->confs.size() == 0) { return csb; }

	if (csa->confs.size() < FILTER_THRESHOLD) {
		const ConfigurationSet* res = ParetoCalculator::efficient_minimize_filter3(csa, csb);
		return res;
	}

	const QuantityName* qn = csa->confspace->getTotallyOrderedQuantity();
	if (qn) {
		if (csb->confspace->nrOfVisibleQuantities() == 1) {
			// there is only one, totally ordered quantity
			const ConfigurationSet* res = ParetoCalculator::efficient_minimize_filter4(csa, csb);
			return res;
		}
		ConfigurationSet* total = &ParetoCalculator::alternative(*csa, *csb);
		const QuantityValue& pivot = ParetoCalculator::efficient_minimize_getPivot(total, qn);
		ConfigurationSet* csal, * csah;
		ConfigurationSet* csbl, * csbh;
		ParetoCalculator::efficient_minimize_filter_split(csa, qn, pivot, &csal, &csah);
		ParetoCalculator::efficient_minimize_filter_split(csb, qn, pivot, &csbl, &csbh);
		const ConfigurationSet* csblf = ParetoCalculator::efficient_minimize_filter2(csal, csbl);
		const ConfigurationSet* csbhft = ParetoCalculator::efficient_minimize_filter2(csah, csbh);
		const ConfigurationSet* csbhf = ParetoCalculator::efficient_minimize_filter1(csal, csbhft, qn);
		ConfigurationSet* res = &ParetoCalculator::alternative(*csblf, *csbhf);
		return res;
	}
	else {
		return ParetoCalculator::efficient_minimize_filter3(csa, csb); // the simple way.
	}
}

const ConfigurationSet* ParetoCalculator::efficient_minimize_filter3(const ConfigurationSet* csa, const ConfigurationSet* csb) {
	if (csa->confs.size() == 0) { return csb; }
	if (csb->confs.size() == 0) { return csb; }

	ConfigurationSet* res = new ConfigurationSet(*csb);
	SetOfConfigurations::iterator i, j;
	for (i = csa->confs.begin(); i != csa->confs.end(); i++) {
		for (j = res->confs.begin(); j != res->confs.end();) {
			const Configuration* c = &(*j);
			if ((*i) <= (*c)) { // remove cc as it is dominated and continue
				// port to linux/gcc, set erase does not return a new iterator here
				// some people say the following is safe...
				SetOfConfigurations::iterator k;
				k = j;
				k++;
				res->confs.erase(j);
				j = k;
			}
			else j++;
		}
	}
	return res;
}


const ConfigurationSet* ParetoCalculator::efficient_minimize_filter4(const ConfigurationSet* csa, const ConfigurationSet* csb) {
	// assume: csa and csb are 1-dimensional spaces with a total order
		//find min of csa

	if (csa->confs.size() == 0) { return csb; }
	if (csb->confs.size() == 0) { return csb; }

	const QuantityValue* v = NULL, * w;
	if (csa->confs.size() == 0) return csb;
	unsigned int n = csa->confspace->firstVisibleQuantity();
	SetOfConfigurations::iterator i;
	for (i = csa->confs.begin(); i != csa->confs.end(); i++) {
		if (!v) { v = &(*i).getQuantity(n); }
		else if ((*(w = &(*i).getQuantity(n))) <= *v) { v = w; }
	}
	//	return all confs of b which are smaller than a.
	ConfigurationSet* res = new ConfigurationSet(csb->confspace, "temp");
	for (i = csb->confs.begin(); i != csb->confs.end(); i++) {
		res->addConfiguration(*i);
	}
	return res;
}



const ConfigurationSet* ParetoCalculator::efficient_minimize_dcmerge(const ConfigurationSet* csl, const ConfigurationSet* csh, const QuantityName& qn, const QuantityValue& v) {
	// csl and csh are minimized and sorted on qn, such that for any cl in csl and ch in csh, cl(qn)<=ch(qn)
	// project on cut plane "hide qn"


	if (csl->confspace->nrOfVisibleQuantities() == 1) {
		// csl should contain only one element which dominates the element of csh
#ifdef _DEBUG
		if (csl->confs.size() > 1) { // for debugging, just checking... shouldn't happen
			throw EParetoCalculatorError("Should not be reachable. minimize_dcmerge()\n");
		}
#endif
		return csl;
	}

	const ConfigurationSet* cshf = ParetoCalculator::efficient_minimize_filter1(csl, csh, &qn);

	ConfigurationSet* csp = &ParetoCalculator::alternative(*csl, *cshf);

	return csp;
}


const ConfigurationSet* ParetoCalculator::efficient_minimize_totally_ordered_recursive(ConfigurationSet* cs, const QuantityName& qn) {
	// Is called in the N-recursive part of totall_ordered

	// Base case of recursion, when problem size small enough use
	// ordinary minimization
	if (cs->confs.size() < MINIMIZE_THRESHOLD) {
		return &ParetoCalculator::minimize(*cs);
	}

	const QuantityValue* v;
	ListOfConfSet* l = ParetoCalculator::splitLowHigh(cs, qn, &v);
	ListOfConfSet::iterator p = l->begin();
	const ConfigurationSet* mcsl = ParetoCalculator::efficient_minimize_totally_ordered_recursive(*p, qn);
	p++;
	const ConfigurationSet* mcsh = ParetoCalculator::efficient_minimize_totally_ordered_recursive(*p, qn);

	const ConfigurationSet* res = ParetoCalculator::efficient_minimize_dcmerge(mcsl, mcsh, qn, *v);

	return res;
}


const ConfigurationSet* ParetoCalculator::efficient_minimize_totally_ordered(const ConfigurationSet* cs, const QuantityName& qn) {
	const QuantityValue* v;
	ListOfConfSet* l = ParetoCalculator::splitLowHigh(cs, qn, &v);
	ListOfConfSet::iterator p = l->begin();
	const ConfigurationSet* mcsl = ParetoCalculator::efficient_minimize_totally_ordered_recursive(*p, qn);
	p++;
	const ConfigurationSet* mcsh = ParetoCalculator::efficient_minimize_totally_ordered_recursive(*p, qn);

	const ConfigurationSet* res = ParetoCalculator::efficient_minimize_dcmerge(mcsl, mcsh, qn, *v);

	return res;
}

ListOfConfSet* ParetoCalculator::splitLowHigh(const ConfigurationSet* cs, const QuantityName& qn, const QuantityValue** v) {
	// pre: size of cs is at least two.

	ListOfConfSet* lcs = new ListOfConfSet();

	//	create index on cs,qn
	IndexOnTotalOrderConfigurationSet i(qn, cs);

	unsigned int mid = i.size() / 2;
	*v = const_cast<QuantityValue*>(&(i.at(mid).conf->getQuantity(qn)));  // try to get rid of the const_cast...

	lcs->push_back(i.copyFromTo(0, mid - 1));
	lcs->push_back(i.copyFromTo(mid, i.size() - 1));
	return lcs;
}

//ListOfConfSet* ParetoCalculator::splitLowHigh_presorted(ConfigurationSet* cs, const QuantityName& qn, QuantityValue **  v){
//	// pre: size of cs is at least two and the configurations in cs
//	// are sorted on qn.
//
//	ListOfConfSet* lcs = new ListOfConfSet();
//
//	unsigned int mid = cs->confs.size() / 2;
//
//	ConfigurationSet* cslow = new ConfigurationSet(cs->confspace, "temp");
//	ConfigurationSet* cshigh = new ConfigurationSet(cs->confspace, "temp");
//
//	SetOfConfigurations::iterator i;
//	unsigned int k;
//	for(k=0, i = cs->confs.begin(); k<mid; i++, k++){
//		cslow->addUniqueConfiguration(*i);
//	}
//
//	*v = const_cast<QuantityValue*>(&((*i).getQuantity(qn)));
//
//	for(; i != cs->confs.end(); i++){
//		cshigh->addUniqueConfiguration(*i);
//	}
//
//	lcs->push_back(cslow);
//	lcs->push_back(cshigh);
//	return lcs;
//}
//
const ConfigurationSet& ParetoCalculator::efficient_minimize(const ConfigurationSet& cs) {

	// The efficient way

	// Base case of recursion, when problem size small enough use
	// ordinary minimization
	if (cs.confs.size() < MINIMIZE_THRESHOLD) {
		return ParetoCalculator::minimize(cs);
	}

	const QuantityName* qn = cs.confspace->getUnorderedQuantity();
	if (qn) {
		return *ParetoCalculator::efficient_minimize_unordered(&cs, *qn);
	}
	else {
		qn = cs.confspace->getTotallyOrderedQuantity();
		if (qn) {
			return *ParetoCalculator::efficient_minimize_totally_ordered(&cs, *qn);
		}
		else {
			return ParetoCalculator::minimize(cs);
		}
	}
}


void ParetoCalculator::store(const StorableObject& o, const std::string& key) {
	// put the object in memory
	memory.insert(std::pair<const std::string, const StorableObject*>(key, &o));
}

void ParetoCalculator::store(const StorableObject& o) {
	this->store(o, o.name);
}

void ParetoCalculator::store(void) {
	// store object from the stack
	this->store(*this->pop());
}

void ParetoCalculator::store(const std::string& key) {
	// store object from the stack
	this->store(*this->pop(), key);
}


const StorableObject* ParetoCalculator::retrieve(const std::string& oname)
{
	// find object in memory
	StorageMap::iterator p = memory.find(oname);
	if (p == memory.end()) {
		//this->listStorage(cout);
		throw EParetoCalculatorError("object not found in ParetoCalculator::retrieve");
	}
	else {
		return (*p).second;
	}
}

const QuantityType* ParetoCalculator::retrieveQuantityType(const std::string& oname) //throw (EParetoCalculatorError)
{
	return dynamic_cast<const QuantityType*>(this->retrieve(oname));
}


void ParetoCalculator::push(const StorableObject& o) {
	stack.push(&o);
}

void ParetoCalculator::push(const std::string& oname) //throw(EParetoCalculatorError)
{
	const StorableObject* o = this->retrieve(oname);
	if (o) {
		this->push(*o);
	}
	else {
		throw EParetoCalculatorError("object not found in ParetoCalculator::push");
	}
}

const StorableObject* ParetoCalculator::pop() //throw(EParetoCalculatorError)
{
	if (stack.size() > 0) {
		return stack.pop();
	}
	else {
		throw EParetoCalculatorError("Stack is empty in ParetoCalculator::pop()");
		return NULL;
	}
}

const ConfigurationSet* ParetoCalculator::popConfigurationSet() //throw(EParetoCalculatorError)
{
	const StorableObject* so = this->pop();
	if (!so->isConfigurationSet()) {
		throw EParetoCalculatorError("Configuration set expected in ParetoCalculator::popConfigurationSet()");
		return NULL;
	}
	return dynamic_cast<const ConfigurationSet*>(so);
}

const StorableObject* ParetoCalculator::peek() //throw(EParetoCalculatorError)
{
	if (stack.size() > 0) {
		return stack.peek();
	}
	else {
		throw EParetoCalculatorError("Stack is empty in ParetoCalculator::peek()");
		return NULL;
	}
}

void ParetoCalculator::duplicate() {
	if (stack.size() > 0) {
		stack.duplicate();
	}
	else {
		throw EParetoCalculatorError("Stack is empty in ParetoCalculator::duplicate()");
	}
}

void ParetoCalculator::minimize(void) {
	// operate on the stack
	const StorableObject& so = *this->pop();
	if (!so.isConfigurationSet()) {
		throw EParetoCalculatorError("Minimization requires configuration set on the stack");
		return;
	}
	const ConfigurationSet& cs = dynamic_cast<const ConfigurationSet&>(so);
	ConfigurationSet* csn = &this->minimize(cs);
	this->push(*csn);
}

void ParetoCalculator::efficient_minimize(void) {
	// operate on the stack
	const StorableObject& so = *this->pop();
	if (!so.isConfigurationSet()) {
		throw EParetoCalculatorError("Minimization requires configuration set on the stack");
		return;
	}
	const ConfigurationSet& cs = dynamic_cast<const ConfigurationSet&>(so);
	const ConfigurationSet* csn = &this->efficient_minimize(cs);
	this->push(*csn);
}

void ParetoCalculator::product(void) {
	// operate on the stack
	const StorableObject& o1 = *this->pop();
	const StorableObject& o2 = *this->pop();
	if (!(o1.isConfigurationSet() && o2.isConfigurationSet())) {
		throw EParetoCalculatorError("Product requires two configuration sets");
		return;
	}
	const ConfigurationSet& cs1 = dynamic_cast<const ConfigurationSet&>(o1);
	const ConfigurationSet& cs2 = dynamic_cast<const ConfigurationSet&>(o2);
	ConfigurationSet* csn = &this->product(cs1, cs2);
	this->push(*csn);
}


void ParetoCalculator::listStorage(std::ostream& os) {
	StorageMap::iterator i;
	for (i = memory.begin(); i != memory.end(); i++) {
		os << (*i).first << std::endl;
	}
}

void ParetoCalculator::printOn(std::ostream& os) {
	os << *this->pop() << std::endl;
}

void ParetoCalculator::print() {
	std::ostringstream myString;
	this->pop()->streamOn(myString);
	this->verbose(myString.str());
}


void ParetoCalculator::constraint(bool (*testConstraint)(const Pareto::Configuration&)) {
	// operate on the stack
	const StorableObject& so = *this->pop();
	if (!so.isConfigurationSet()) {
		throw EParetoCalculatorError("Constraint requires configuration set on the stack");
		return;
	}
	const ConfigurationSet& cs = dynamic_cast<const ConfigurationSet&>(so);
	ConfigurationSet* csn = &this->constraint(cs, testConstraint);
	this->push(*csn);
}


void ParetoCalculator::setStatus(const std::string& newStatus) {
	status = newStatus;
	if (statusObject) {
		statusObject->setStatus(status);
	}
}

void ParetoCalculator::verbose(const std::string& addText) {
	if (statusObject) {
		statusObject->verbose(addText);
	}
}


void ParetoCalculator::setStatusCallbackObject(StatusCallback* o) {
	statusObject = o;
}

void ParetoCalculator::initParser() {
	if (parser == NULL) {
		this->setStatus("Creating XML Parser");
		this->verbose("Creating XML Parser\n");
		parser = new ParetoParser(this);
	}
}

void ParetoCalculator::LoadFile(const std::wstring& fn) {
	this->initParser();
	this->setStatus("Loading XML input");
	this->verbose("Loading XML input...");
	parser->LoadFile(fn);
	this->setStatus("XML input loaded");
	this->verbose("loaded\n");
}

void ParetoCalculator::LoadQuantityTypes() {
	this->setStatus("Loading quantity types from input");
	this->verbose("Loading quantity types from input...");
	parser->LoadQuantityTypes();
	this->setStatus("Quantity types loaded");
	this->verbose("loaded\n");
}

void ParetoCalculator::LoadConfigurationSpaces() {
	this->setStatus("Loading configuration spaces from input");
	this->verbose("Loading configuration spaces from input...");
	parser->LoadConfigurationSpaces();
	this->setStatus("Configuration spaces loaded");
	this->verbose("loaded\n");
}

void ParetoCalculator::LoadConfigurationSets() {
	this->setStatus("Loading configuration sets from input");
	this->verbose("Loading configuration sets from input...");
	parser->LoadConfigurationSets();
	this->setStatus("Configuration sets loaded");
	this->verbose("loaded\n");
}

void ParetoCalculator::LoadOperations() {
	this->setStatus("Executing operations from input");
	this->verbose("Executing operations from input\n");
	parser->LoadOperations();
	this->setStatus("Operations from input executed");
	this->verbose("Operations from input executed\n");
}

//#ifdef _MSC_VER
void ParetoCalculator::SaveAllFile(const std::wstring& fn) {
	this->setStatus("Saving XML file");
	this->verbose("Saving XML file...");
	parser->SaveAll(fn);
	this->setStatus("XML input saved");
	this->verbose("saved\n");
}

void ParetoCalculator::SaveItemFile(const std::string& itemToSave, const std::wstring& fn) {
	this->setStatus("Saving XML item ");
	this->verbose("Saving XML item...");
	parser->Save(itemToSave, fn);
	this->setStatus("XML item saved");
	this->verbose("saved\n");
}
//#endif

long int testComplexity(long int N, long int d) {
	if (N <= 1) return 1;
	if (d == 1) return N;
	long int a = (long int)(log((double)(N)) * ((double)N));
	long int b = testComplexity(N / 2, d);
	long int c = testComplexity(N, d - 1);
	return a + 2 * b + c;
}

void test(void) {
	long int a;
	a = testComplexity(100, 4);
	a = testComplexity(1000, 4);
	a = testComplexity(10000, 4);
	a = testComplexity(100000, 4);
	a = testComplexity(1000000, 4);

	a = testComplexity(100, 6);
	a = testComplexity(1000, 6);
	a = testComplexity(10000, 6);
	a = testComplexity(100000, 6);
	a = testComplexity(1000000, 6);
}
