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
// Implementation of the ParetoCalculatorOperation class and its subclasses
// Representing the operations to be performed by the calculator.
//

#include "operations.h"
#include "calculator.h"
#include <string>
#include <sstream>

using namespace Pareto;


/// POperation_Minimize constructor
POperation_Minimize::POperation_Minimize() : ParetoCalculatorOperation() {
}

/// execute the operation represented by the object
void POperation_Minimize::executeOn(ParetoCalculator& c) {
	c.efficient_minimize();
}

/// POperation_EfficientMinimize constructor
POperation_EfficientMinimize::POperation_EfficientMinimize() : ParetoCalculatorOperation() {
}

/// execute the operation represented by the object
void POperation_EfficientMinimize::executeOn(ParetoCalculator& c) {
	c.efficient_minimize();
}

/// POperation_Product constructor
POperation_Product::POperation_Product() : ParetoCalculatorOperation() {
}

/// execute the operation represented by the object
void POperation_Product::executeOn(ParetoCalculator& c) {
	c.product();
};

/// POperation_Push constructor
POperation_Push::POperation_Push(const std::string& oname) : ParetoCalculatorOperation() {
	o = oname;
}

/// execute the operation represented by the object
void POperation_Push::executeOn(ParetoCalculator& c) {
	c.push(o);
}

/// POperation_Store constructor
POperation_Store::POperation_Store(const std::string& oname) : ParetoCalculatorOperation() {
	o = oname;
}

/// execute the operation represented by the object
void POperation_Store::executeOn(ParetoCalculator& c) {
	StorableObjectPtr ob = c.pop();
	StorableObjectPtr nob = ob->copy();
	nob->name = o;
	c.store(nob);
}

/// POperation_Duplicate constructor
POperation_Duplicate::POperation_Duplicate() : ParetoCalculatorOperation() {
}

/// execute the operation represented by the object
void POperation_Duplicate::executeOn(ParetoCalculator& c) {
	c.duplicate();
}

/// POperation_Pop constructor
POperation_Pop::POperation_Pop() : ParetoCalculatorOperation() {
}

/// execute the operation represented by the object
void POperation_Pop::executeOn(ParetoCalculator& c) {
	c.pop();
}

/// POperation_Print constructor
POperation_Print::POperation_Print() : ParetoCalculatorOperation() {
}

/// execute the operation represented by the object
void POperation_Print::executeOn(ParetoCalculator& c) {
	c.print();
}

/// POperation_ProdCons constructor
POperation_ProdCons::POperation_ProdCons(const std::string& pqname, const std::string& cqname) : ParetoCalculatorOperation(),
	p_quant(pqname),
	c_quant(cqname)
{
}


/// execute the operation represented by the object
void POperation_ProdCons::executeOn(ParetoCalculator& c) {
	ConfigurationSetPtr cs = c.popConfigurationSet();
	ConfigurationSetPtr ncs = c.constraint(cs, 
		[this](const Pareto::Configuration& c) {
			return POperation_ProdCons::testConstraint(c, this->p_quant, this->c_quant);
		}
		);
	c.push(ncs);
}

/// evaluate the producer-consumer constraint on the configuration
bool POperation_ProdCons::testConstraint(const Pareto::Configuration& c, const std::string& p_quant, const std::string& c_quant) {
	QuantityValue_RealPtr pq = std::dynamic_pointer_cast<QuantityValue_Real>(c.getQuantity(p_quant));
	QuantityValue_RealPtr cq = std::dynamic_pointer_cast<QuantityValue_Real>(c.getQuantity(c_quant));
	return cq->value <= (1.0 / pq->value);
}

/// POperation_Derived constructor
POperation_Derived::POperation_Derived(const std::string& aqname, const std::string& bqname) : ParetoCalculatorOperation(),
	a_quant(aqname),
	b_quant(bqname)
{
}

/// execute the operation represented by the object
void POperation_Derived::executeOn(ParetoCalculator& c) {

	ConfigurationSetPtr cs = c.popConfigurationSet();

	// define new sum quantity type
	std::ostringstream sqn;
	sqn << this->description() << " of " << a_quant << " and " << b_quant;
	std::shared_ptr<QuantityType_Real> sqt = std::make_shared<QuantityType_Real>(sqn.str());

	// build new configuration space
	std::ostringstream sqsn;
	sqsn << this->description() << " (" << cs->confspace->name << ")";
	ConfigurationSpacePtr scs = std::make_shared<ConfigurationSpace>(sqsn.str());
	scs->addQuantitiesOf(*(cs->confspace));
	scs->addQuantity(*sqt);

	// build new set of configurations
	std::ostringstream sconfsn;
	sconfsn << this->description() << " (" << cs->name << ", " << a_quant << ", " << b_quant << ")";

	ConfigurationSetPtr sconfs = std::make_shared<ConfigurationSet>(scs, sconfsn.str());

	SetOfConfigurations::iterator i;
	for (i = cs->confs.begin(); i != cs->confs.end(); i++) {
		ConfigurationPtr c = *i;
		ConfigurationPtr scf = std::make_shared<Configuration>(*scs);
		scf->addQuantitiesOf(c);

		double va = (*(std::dynamic_pointer_cast<QuantityValue_RealPtr>(c->getQuantity(a_quant))))->value;
		double vb = (*(std::dynamic_pointer_cast<QuantityValue_RealPtr>(c->getQuantity(b_quant))))->value;

		QuantityValue_RealPtr sqv = std::make_shared<QuantityValue_Real>(*sqt, this->derive(va, +vb));
		scf->addQuantity(sqv);
		sconfs->addUniqueConfiguration(scf);
	}
	c.push(sconfs);
}

/// POperation_Agggregate constructor
POperation_Aggregate::POperation_Aggregate(ListOfQuantityNames& ag_quants, std::string& agname):
	aggregate_quants(ag_quants),
	newName(agname)
{
}

/// execute the operation represented by the object
void POperation_Aggregate::executeOn(ParetoCalculator& c)
{
	// get the configuratoin set to aggregate on
	ConfigurationSetPtr cs = c.popConfigurationSet();

	// define new aggregate quantity type
	std::shared_ptr<QuantityType_Real> sqt = std::make_shared<QuantityType_Real>(this->newName);

	// build new configuration space
	std::ostringstream sqsn;
	sqsn << "Aggregation" << " (" << cs->confspace->name << ")";
	ConfigurationSpacePtr scs = std::make_shared<ConfigurationSpace>(sqsn.str());
	scs->addQuantitiesOf(*(cs->confspace));
	scs->addQuantity(*sqt);

	// build new set of configurations
	std::ostringstream sconfsn;
	sconfsn << "Aggregation" << " (" << this->newName << ")";
	ConfigurationSetPtr sconfs = std::make_shared<ConfigurationSet>(scs, sconfsn.str());

	// find positions of quantities to aggregate
	std::vector<unsigned int> idx;
	for (ListOfQuantityNames::iterator i = this->aggregate_quants.begin(); i != this->aggregate_quants.end(); i++) {
		std::string& qn = *i;
		idx.push_back(cs->confspace->indexOfQuantity(qn));
	}

	// create the new configurations
	SetOfConfigurations::iterator i;
	for (i = cs->confs.begin(); i != cs->confs.end(); i++) {
		ConfigurationPtr cf = *i;

		// create new configuration with all values of cf
		ConfigurationPtr scf = std::make_shared<Configuration>(*scs);
		scf->addQuantitiesOf(cf);

		// compute the aggregated value
		double sum = 0.0;
		std::vector<unsigned int>::iterator j;
		for (j = idx.begin(); j != idx.end(); j++) {
			unsigned int k = *j;
			QuantityValue_RealPtr qvr = std::dynamic_pointer_cast<QuantityValue_Real>(cf->getQuantity(k));
			sum = sum + qvr->value;

		}
		// add the aggregated value to the new configuration
		QuantityValue_RealPtr sqv = std::make_shared<QuantityValue_Real>(*sqt, sum);
		scf->addQuantity(sqv);

		// add the new configuraiton to the result set
		sconfs->addUniqueConfiguration(scf);
	}
	// push the result on the stack
	c.push(sconfs);
}


/// POperation_Sum constructor
POperation_Sum::POperation_Sum(const std::string& aqname, const std::string& bqname) : POperation_Derived(aqname, bqname) {
}

// Sum's derived operation
double POperation_Sum::derive(double a, double b) {
	return a + b;
}

// provide a description of the derived operator
std::string POperation_Sum::description() {
	return "Sum";
}

/// POperation_Max constructor
POperation_Max::POperation_Max(const std::string& aqname, const std::string& bqname) : POperation_Derived(aqname, bqname) {
}

// Max's derived operation
double POperation_Max::derive(double a, double b) {
	if (a > b) { return a; }
	else { return b; }
}

// provide a description of the derived operator
std::string POperation_Max::description() {
	return "Max";
}

/// POperation_Min constructor
POperation_Min::POperation_Min(const std::string& aqname, const std::string& bqname) : POperation_Derived(aqname, bqname) {
}

// Min's derived operation
double POperation_Min::derive(double a, double b) {
	if (a > b) { return b; }
	else { return a; }
}

// provide a description of the derived operator
std::string POperation_Min::description() {
	return "Min";
}

/// POperation_Multiply constructor
POperation_Multiply::POperation_Multiply(const std::string& aqname, const std::string& bqname) : POperation_Derived(aqname, bqname) {
}

// Multiply's derived operation
double POperation_Multiply::derive(double a, double b) {
	return a * b;
}

// provide a description of the derived operator
std::string POperation_Multiply::description() {
	return "Multiply";
}

/// POperation_Abstract constructor
POperation_Abstract::POperation_Abstract(ListOfQuantityNames& qnames):
	lqn(qnames)
{
}

/// execute the operation represented by the object
void POperation_Abstract::executeOn(ParetoCalculator& c) {
	ConfigurationSetPtr cs = c.popConfigurationSet();
	// TODO: Surely, this can be done a lot more efficiently...
	c.push(cs);
	ListOfQuantityNames::iterator i;
	for (i = this->lqn.begin(); i != this->lqn.end(); i++) {
		c.push(std::make_shared<StorableString>(*i));
		c.abstract();
	}
}

/// POperation_Hide constructor
POperation_Hide::POperation_Hide(ListOfQuantityNames& qnames): lqn(qnames) {
}

/// execute the operation represented by the object
void POperation_Hide::executeOn(ParetoCalculator& c) {
	ConfigurationSetPtr cs = c.popConfigurationSet();
	c.push(cs);
	c.hide(lqn);
}

/// POperation_Join constructor
POperation_Join::POperation_Join(const JoinMap& jqnamemap) :
	j_quants(jqnamemap)
{
}

// evaluate the join constraint on configuration c
bool POperation_Join::testConstraint(const Pareto::Configuration& c, const std::vector<int>& qan, const std::vector<int>& qbn) {
	bool diff = false;
	std::vector<int>::const_iterator i = qan.begin();
	std::vector<int>::const_iterator j = qbn.begin();
	for (; !diff && i != qan.end(); i++, j++) {
		unsigned int k = *i;
		unsigned int m = *j;
		QuantityValuePtr qva = c.getQuantity(k);
		QuantityValuePtr qvb = c.getQuantity(m);

		diff = diff || !(*qva == *qvb);
	}
	return !diff;
}

/// execute the operation represented by the object
void POperation_Join::executeOn(ParetoCalculator& c) {
	if (c.stack.size() < 2) {
		throw EParetoCalculatorError("Not enough configuration spaces on stack to perform join.");
	}

	// pop the configuraiton sets to be joined
	ConfigurationSetPtr csa = c.popConfigurationSet();
	ConfigurationSetPtr csb = c.popConfigurationSet();

	// get the indices of the quantities to be joined
	std::vector<int> qan, qbn;
	JoinMap::const_iterator i;
	for (i = j_quants.begin(); i != j_quants.end(); i++) {
		qan.push_back(csa->confspace->indexOfQuantity(i->first));
		qbn.push_back(csb->confspace->indexOfQuantity(i->second) + (int)csa->confspace->quantities.size());
	}

	// compute the free product first
	c.push(csb);
	c.push(csa);
	c.product();

	// apply the join constraint
	ConfigurationSetPtr cs = c.popConfigurationSet();
	ConfigurationSetPtr ncs = c.constraint(cs, 
		[qan,qbn](const Configuration& c) {
			return POperation_Join::testConstraint(c, qan, qbn);
		}
	);

	// push the result on the stack
	c.push(ncs);
};

/// POperation_EfficientJoin constructor
POperation_EfficientJoin::POperation_EfficientJoin(JoinMap& jqnamemap): j_quants(jqnamemap) {
}

// execute efficient join operation
ConfigurationSetPtr recursiveEfficientJoin(ParetoCalculator& c, JoinMap& jqnamemap, ConfigurationSetPtr A, ConfigurationSetPtr B, ConfigurationSpacePtr productspace) {
	// check if anything needs to be joined
	// the base case of the recursion
	if (jqnamemap.size() == 0) {
		// return the product of A and B
		return ParetoCalculator::productInSpace(A, B, productspace);
	}
	// if either of the configuration sets is empty
	else if (A->confs.size() == 0 || B->confs.size() == 0) {
		// return an empty set in the product space
		ConfigurationSetPtr result = std::make_shared<ConfigurationSet>(productspace, "result");
		return result;
	}
	else {
		QuantityName qa = jqnamemap.begin()->first;
		QuantityName qb = jqnamemap.begin()->second;
		std::shared_ptr<IndexOnConfigurationSet> ia, ib;
		// check if the quantity to be joined is totally ordered 
		if (A->confspace->getQuantity(qa).isTotallyOrdered()) {
			// make an index on the ordered quantity qa
			ia = std::make_shared<IndexOnTotalOrderConfigurationSet>(qa, A);
		}
		else {
			// make an index on the unordered quantity qa
			ia = std::make_shared<IndexOnUnorderedConfigurationSet>(qa, A);
		}
		if (B->confspace->getQuantity(qb).isTotallyOrdered()) {
			// make an index on the ordered quantity qb
			ib = std::make_shared<IndexOnTotalOrderConfigurationSet>(qb, B);
		}
		else {
			// make an index on the unordered quantity qb
			ib = std::make_shared<IndexOnUnorderedConfigurationSet>(qb, B);
		}

		// create a copy of the joinmap, excluding the first pair (which is handled in this call)
		JoinMap newnamemap;
		JoinMap::iterator i = jqnamemap.begin();
		i++;
		for (; i != jqnamemap.end(); i++) {
			newnamemap[i->first] = i->second;
		}

		// create the result configuraiton set
		ConfigurationSetPtr result = std::make_shared<ConfigurationSet>(productspace, "result");

		// until all configurations in index ia have been covered...
		unsigned int j = 0;
		while (j < ia->confset->confs.size()) {
			int l_x, u_x, v_x, w_x;
			// get the value v (refered to by x) for index ia on position j
			std::shared_ptr<ConfigurationIndexReference> x = ia->get(j);
			
			// determine the ranges [l_x,u_x] and [v_x,w_x] in indices ia and ib, respectively, for which the value is equal to v
			l_x = ia->lower(*x);
			u_x = ia->upper(*x);
			v_x = ib->lower(*x);
			w_x = ib->upper(*x);

			// copy the configurations from both sets in which the value to be joined is equal to v
			ConfigurationSetPtr Asub = ia->copyFromTo(l_x, u_x);
			ConfigurationSetPtr Bsub = ib->copyFromTo(v_x, w_x);

			// recursively join the remaining join pairs in newnamemap
			ConfigurationSetPtr temp = recursiveEfficientJoin(c, newnamemap, Asub, Bsub, productspace);

			// add the result to the result set
			result->addUniqueConfigurationsOf(temp);

			// continue with the first index with a value different from v
			j = u_x + 1;
		}
		return result;
	}
}

/// execute the operation represented by the object
void POperation_EfficientJoin::executeOn(ParetoCalculator& c) {
	// get the configuraiton sets
	ConfigurationSetPtr csa = c.popConfigurationSet();
	ConfigurationSetPtr csb = c.popConfigurationSet();
	// create the resulting configuraiton space
	ConfigurationSpacePtr productspace = csa->confspace->productWith(csb->confspace);
	// compute the join operation recursively
	ConfigurationSetPtr result = recursiveEfficientJoin(c, this->j_quants, csa, csb, productspace);
	// push the result on the stack
	c.push(result);
};


/// POperation_EfficientProdCons constructor
POperation_EfficientProdCons::POperation_EfficientProdCons(const std::string& pqname, const std::string& cqname) : 
	p_quant(pqname), 
	c_quant(cqname) 
{
}

/// evaluate the producer-consumer matching between given quantity values
/// assumes that the QuantityValues vp and vc are in fact of type QuantityValue_Real
bool POperation_EfficientProdCons::default_match(const QuantityValue& vp, const QuantityValue& vc) const {
#ifdef _DEBUG
	const QuantityValue_Real& rp = dynamic_cast<const QuantityValue_Real&>(vp);
	const QuantityValue_Real& rc = dynamic_cast<const QuantityValue_Real&>(vc);
#else
	const QuantityValue_Real& rp = (const QuantityValue_Real&)(vp);
	const QuantityValue_Real& rc = (const QuantityValue_Real&)(vc);
#endif
	return rc.value <= (1.0 / rp.value);
}

/// execute the operation represented by the object
/// assumes that the quantities to be constrained as producer-consumer are totally ordered quantities
void POperation_EfficientProdCons::executeOn(ParetoCalculator& c) {

	// get the configuration sets
	const ConfigurationSetPtr csc = c.popConfigurationSet();
	const ConfigurationSetPtr csp = c.popConfigurationSet();

	// check if the quantities are totally ordered
	if (!(csp->confspace->getQuantity(p_quant).isTotallyOrdered() && csc->confspace->getQuantity(c_quant).isTotallyOrdered())) {
		throw EParetoCalculatorError("Dimensions must be totally ordered for efficient producer-consumer");
	}

	// create the product space for the result
	ConfigurationSpacePtr nspace = csp->confspace->productWith(csc->confspace);	

	// create the result configuration set
	std::string name = "Producer-Consumer ( " + csp->name + ", " + csc->name + ", " + p_quant + ", " + c_quant + ")";
	ConfigurationSetPtr ns = std::make_shared<ConfigurationSet>(nspace, name);

	// create sorted indices (from best to worst) on the quantities to be constrained as producer-consumer
	IndexOnTotalOrderConfigurationSet ip(p_quant, csp);
	IndexOnTotalOrderConfigurationSet ic(c_quant, csc);

	// start from the best (lowest) value in the consuming quantitity...
	unsigned int i = 0;
	// and from the worst (also lowest!) value in the producing quantity
	int j = ip.size() - 1;
	// until all configurations from either of the sets have been exhausted...
	// loop tracing the border between matching and non-matching configurations
	// loop invariants:
	// - (i,j+1) is non-matching or j+1 >= ip.size()
	// - (i-1, j) is matching or i<=0
	// - all matching configurations (k,m) with 0<=k<i and j<=m<ip.size() have been added to the result set
	while (i < ic.size() && j >= 0) {
		// check if the current pair (i,j) of configurations match the producer-consumer constraint
		if (default_match(*(ip.at(j)->value()), *(ic.at(i)->value()))) {
			// they match, so create a resulting configuration
			ConfigurationPtr c = std::make_shared<Configuration>(*nspace);
			c->addQuantitiesOf(ic.at(i)->conf);
			c->addQuantitiesOf(ip.at(j)->conf);
			// and add it to the result set
			ns->addConfiguration(c);
			// move to the next (higher) consuming configuration
			i = i + 1;
		}
		else {
			// they do not match, go to the next (higher) producing configuration
			j = j - 1;
			// if we have not exhausted all producing configurations yet
			if (j >= 0) {
				// add configurations for all pairs (k,j) with 0<=k<i, because we know they must match
				// because (i-1, j+1) is matching
				for (unsigned int k = 0; k < i; k++) {
					// make a configuration for the pair (k,j)
					ConfigurationPtr c = std::make_shared<Configuration>(*nspace);
					c->addQuantitiesOf(ic.at(k)->conf);
					c->addQuantitiesOf(ip.at(j)->conf);
					// add the configuration to the result set
					ns->addConfiguration(c);
				}
			}
		}
	}
	// we know the loop invariant holds: all matching configurations (k,m) with 0<=k<i and j<=m<ip.size() have been added to the result set
	// and we know that i==ic.size() or j==-1
	// if j==-1:
	//		- all matching configurations (k,m) with 0<=k<i and 0<=m<ip.size() have been added to the result set
	//		- (i,0) is non-matching and all matching configurations have been added
	// if i==ic.size():
	//		- all matching configurations (k,m) with 0<=k<ic.size() and j<=m<ip.size() have been added to the result set
	//      - (ic.size()-1, j) is matching , so all pairs (k, m) for 0<=k<ic.size() and 0<=m<j are also matching and still need to be added to the result

	if (i == ic.size()) {
		// add all pairs (k, m) for 0<=k<ic.size() and 0<=m<j to the results
		// for each of the remaining producing configurations 0<=l<j
		for (int l = 0; l < j; l++) {
			// for all consuming configurations k
			for (unsigned int k = 0; k < ic.size(); k++) {
				// create resulting configuration (k,l)
				ConfigurationPtr c = std::make_shared<Configuration>(*nspace);
				c->addQuantitiesOf(ic.at(k)->conf);
				c->addQuantitiesOf(ip.at(l)->conf);
				// add it to the result set
				ns->addConfiguration(c);
			}
		}
	}
	// push the result to the stack
	c.push(ns);
}
