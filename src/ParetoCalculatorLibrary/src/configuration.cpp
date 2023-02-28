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
// Implementation of the Configuration and ConfigurationSpace, ConfigurationSet 
// classes and supporting classes, for instance to create a sorted index on a set 
// configurations
//

#include "configuration.h"
#include "exceptions.h"
#include <sstream> 
#include <algorithm>

namespace Pareto {

	/// ConfigurationSpace constructor
	ConfigurationSpace::ConfigurationSpace(std::string n) : StorableObject(n) {
	}

	/// add a quantity type to the configuration space under its own name
	/// it will be visible
	void ConfigurationSpace::addQuantity(QuantityTypePtr q) {
		this->addQuantityAsVisibility(q, q->name, true);
	}

	/// add a quantity type to the configuration space under a specified name
	/// it will be visible
	void ConfigurationSpace::addQuantityAs(QuantityTypePtr q, const QuantityName qname) {
		this->addQuantityAsVisibility(q, qname, true);
	}

	/// add a quantity type to the configuration space under a specified name and with specified visibility
	void ConfigurationSpace::addQuantityAsVisibility(QuantityTypePtr q, const QuantityName qname, bool vis) {
		quantities.push_back(q);
		quantityNames[qname] = (unsigned int) (quantities.size() - 1);
		this->quantityVisibility.push_back(vis);
	}

	void ConfigurationSpace::addQuantitiesOf(const ConfigurationSpace& cs) {
		for (unsigned int i = 0; i < cs.quantities.size(); i++) {
			this->addQuantityAsVisibility(cs.quantities[i], cs.nameOfQuantityNr(i), cs.quantityVisibility[i]);
		}
	}

	/// create a product configuration space
	ConfigurationSpacePtr ConfigurationSpace::productWith(ConfigurationSpacePtr cs) const
	{
		ConfigurationSpacePtr cspace = std::make_shared<ConfigurationSpace>("Product(" + this->name + ";" + cs->name + ")");
		cspace->addQuantitiesOf(*this);
		cspace->addQuantitiesOf(*cs);
		return cspace;
	}


	/// get quantity type by name
	QuantityTypePtr ConfigurationSpace::getQuantity(const QuantityName& qn) const
	{
		if (quantityNames.find(qn) != quantityNames.end()) return quantities.at(quantityNames.at(qn));
		ListOfQuantityTypes::const_iterator i;
		for (i = quantities.begin(); i != quantities.end(); i++) {
			if ((*i)->name == qn) return *i;
		}
		throw EParetoCalculatorError("quantity " + qn + " not found in ConfigurationSpace::getQuantity");
	}

	/// count the number of visible quantities in the space
	unsigned int ConfigurationSpace::nrOfVisibleQuantities(void) const {
		unsigned int count = 0;
		BoolVector::const_iterator i;
		for (i = this->quantityVisibility.begin(); i != this->quantityVisibility.end(); i++) {
			if (*i) count++;
		}
		return count;
	}

	/// get the index of the quantity type with the given name
	unsigned int ConfigurationSpace::indexOfQuantity(const QuantityName& qn) const
	{
		if (quantityNames.find(qn) == quantityNames.end()) {
			throw EParetoCalculatorError("quantity " + qn + " not found in ConfigurationSpace::indexOfQuantity");
		}
		return quantityNames.at(qn);
	}

	/// get the first unordered quantity type in the space that is visible
	/// return nullptr if no such quantity type is found.
	QuantityNamePtr ConfigurationSpace::getUnorderedQuantity() const {
		QuantityIntMap::const_iterator i;
		unsigned int k;
		for (k = 0, i = this->quantityNames.begin(); i != this->quantityNames.end(); i++, k++) {
			QuantityTypePtr t = this->quantities[(*i).second];
			if ((this->quantityVisibility[k]) && (t->isUnordered())) {
				QuantityNamePtr qn = std::make_shared<QuantityName>((*i).first);
				return qn;
			}
		}
		return nullptr;
	}

	/// get the first totally ordered quantity type in the space that is visible
	/// return nullptr if no such quantity type is found.
	QuantityNamePtr ConfigurationSpace::getTotallyOrderedQuantity() const {
		QuantityIntMap::const_iterator i;
		unsigned int k;
		for (k = 0, i = this->quantityNames.begin(); i != this->quantityNames.end(); i++, k++) {
			QuantityTypePtr t = this->quantities[(*i).second];
			if ((this->quantityVisibility[k]) && (t->isTotallyOrdered())) {
				QuantityNamePtr qn = std::make_shared<QuantityName>((*i).first);
				return qn;
			}
		}
		return nullptr;
	}

	/// check if the space has a quantity type with the given name
	bool ConfigurationSpace::includesQuantityNamed(const QuantityName& qn) const {
		QuantityIntMap::const_iterator i;
		for (i = quantityNames.begin(); i != quantityNames.end(); i++) {
			if ((*i).first == qn) return true;
		}
		return false;
	}

	/// get the index of the first visible quantity in the space
	/// throws an exception if there is no visible quantity
	const unsigned int ConfigurationSpace::firstVisibleQuantity() const {
		BoolVector::const_iterator i;
		unsigned int k;
		for (i = this->quantityVisibility.begin(), k = 0; i != this->quantityVisibility.end(); i++, k++) {
			if (*i) return k;
		}
		throw EParetoCalculatorError("no visible quantity found in ConfigurationSpace::firstVisibleQuantity");
		return 0;
	}

	/// compare two configurations according to Pareto dominance in this configuration space
	/// Note that invisible quantities are not compared
	bool ConfigurationSpace::compare(const Configuration& c1, const Configuration& c2) const {
		unsigned int i = 0;

		for (ListOfQuantityTypes::const_iterator j = this->quantities.begin(); j != this->quantities.end(); i++, j++) 
		{
			if (this->quantityVisibility[i] && !((*c1.getQuantity(i)) <= (*c2.getQuantity(i)))) 
			{ 
				return false;
			}
		}
		return true;
	}

	/// compare two configurations for equality in this configuration space
	/// Note that invisible quantities are not compared
	bool ConfigurationSpace::equal(const Configuration& c1, const Configuration& c2) const {
		for (unsigned int i = 0; i < this->quantities.size(); i++) {
			if (this->quantityVisibility[i] && !((*c1.getQuantity(i)) == (*c2.getQuantity(i)))) { return false; }
		}
		return true;
	}

	/// compare two configurations according to lexicograhical total order in this configuration space
	/// Note that invisible quantities are included in the comparisson
	bool ConfigurationSpace::LexicographicCompare(const Configuration& c1, const Configuration& c2)
		// provides strict ordering for sorted storage of configurationset, hence ignores visibility
	{
		unsigned int k = (unsigned int) c1.confspace->quantities.size();
		for (unsigned int i = 0; i < k; i++) {
			QuantityValuePtr v1 = c1.getQuantity(i);
			QuantityValuePtr v2 = c2.getQuantity(i);
			if (v1->totalOrderSmaller(*v2)) { return true; }
			if (v2->totalOrderSmaller(*v1)) { return false; }
		}
		return false;
	}

	/// represent the configuraiton space as text on a stream 
	void ConfigurationSpace::streamOn(std::ostream& os) const {
		os << name << " = ";
		unsigned int n;
		for (n = 0; n < this->quantities.size(); n++) {
			if (this->quantityVisibility[n]) {
				os << this->nameOfQuantityNr(n);
			}
			else {
				os << "[" << this->nameOfQuantityNr(n) << "]";
			}
			if (n < quantities.size() - 1) os << " x ";
		}
	}

	/// create a copy of the configuration space
	StorableObjectPtr ConfigurationSpace::copy(void) const
	{
		return std::make_shared<ConfigurationSpace>(*this);
	}

	bool ConfigurationSpace::isIdenticalTo(ConfigurationSpacePtr cs) const
	{
		return this == &(*cs);
	}


	// return a string representation of the configuration space
	std::unique_ptr<std::string> Configuration::asString(void) {
		std::ostringstream myString;
		this->streamOn(myString);
		//to avoid C++14 requirement: return std::make_unique<std::string>(myString.str());
		return std::unique_ptr<std::string>(new std::string(myString.str()));
	}

	void Configuration::adoptConfigurationSpace(ConfigurationSpacePtr cs)
	{
		this->confspace = cs;
	}


	/// perform hiding of the given list of quantity types by making them invisible in the resulting new configuration space
	ConfigurationSpacePtr ConfigurationSpace::hide(const ListOfQuantityNames& lqn) const {
		// name for the new configuration space
		std::string n = "Hide of " + this->name;
		// make the new configuration space
		ConfigurationSpacePtr cs = std::make_shared<ConfigurationSpace>(n);
		// add the quantity types according to visibility
		for (unsigned int i = 0; i < this->quantities.size(); i++) {
			bool v = this->quantityVisibility[i];
			if (lqn.includes(this->nameOfQuantityNr(i))) {
				v = false;
			}
			cs->addQuantityAsVisibility(this->quantities[i], this->nameOfQuantityNr(i), v);
		}
		// return the result
		return cs;
	}

	/// returns a new configuration space in which single quantity from the space is hidden.
	ConfigurationSpacePtr ConfigurationSpace::hide(const QuantityName& qn) {
		ListOfQuantityNames l;
		l.push_back(qn);
		return this->hide(l);
	}

	/// perform unhiding of the given list of quantity types by making them invisible in the resulting new configuration space
	ConfigurationSpacePtr ConfigurationSpace::unhide(const ListOfQuantityNames& lqn) const {
		std::string n = "Unhide of " + this->name;
		ConfigurationSpacePtr cs = std::make_shared<ConfigurationSpace>(n);
		for (unsigned int i = 0; i < this->quantities.size(); i++) {
			bool v = this->quantityVisibility[i];
			if (lqn.includes(this->nameOfQuantityNr(i))) {
				v = true;
			}
			cs->addQuantityAsVisibility(this->quantities[i], this->nameOfQuantityNr(i), v);
		}
		return cs;
	}

	/// returns a new configuration space in which single quantity from the space is unhidden.
	ConfigurationSpacePtr ConfigurationSpace::unhide(const QuantityName& qn) const {
		ListOfQuantityNames l;
		l.push_back(qn);
		return this->unhide(l);
	}

	/// get the name of the quantity type with the given index
	const QuantityName ConfigurationSpace::nameOfQuantityNr(const unsigned int n) const {
		QuantityIntMap::const_iterator i;
		for (i = this->quantityNames.begin(); i != this->quantityNames.end(); i++) {
			if ((*i).second == n) { return (*i).first; }
		}
		throw EParetoCalculatorError("Error: quantity not found in function nameOfQuantitynr.");
	}

	/// create a new configuration in this configuration space with default values for the quantities
	ConfigurationPtr ConfigurationSpace::newConfiguration(void) {

		ConfigurationPtr c = std::make_shared<Configuration>(this->shared_from_this());

		// add default values
		ListOfQuantityTypes::iterator i;
		for (i = quantities.begin(); i != quantities.end(); i++) {
			QuantityValuePtr v = (*i)->defaultValue();
			c->addQuantity(v);
		}
		return c;
	}

	// check configuration spaces on identity, not content
	bool operator!=(ConfigurationSpace& cs1, ConfigurationSpace& cs2) {
		// maybe needs to be made more advanced later (a real comparison)
		return &cs1 != &cs2;
	}

	/////////////////// Configuration //////////////////////

	/// constructor Configuration
	Configuration::Configuration(ConfigurationSpacePtr cs) :confspace(cs) {
	}

	/// 'copy constructor' from pointer
	Configuration::Configuration(ConfigurationPtr c) :
		confspace(c->confspace)
	{
		// copy quantity values
		ListOfQuantityValues::iterator i;
		for (i = c->quantities.begin(); i != c->quantities.end(); i++) {
			this->quantities.push_back(*i);
		}
	}

	// add a quantitu value to the end of the configuration
	void Configuration::addQuantity(QuantityValuePtr q) {
		quantities.push_back(q);
	}

	// add all quantity values from configuration c to this configuration
	void Configuration::addQuantitiesOf(ConfigurationPtr c) {
		ListOfQuantityValues::iterator i;
		for (i = c->quantities.begin(); i != c->quantities.end(); i++) {
			this->addQuantity(*i);
		}
	}

	// produce a textual representaiton of the configuration on the output stream
	void Configuration::streamOn(std::ostream& os) const {
		os << "(";
		ListOfQuantityValues::const_iterator i;
		unsigned int n = 0;
		for (i = quantities.begin(); i != quantities.end(); i++, n++) {
			if (this->confspace->quantityVisibility[n]) {
				QuantityValuePtr v = (*i);
				os << v;
			}
			else {
				os << "[" << (*i) << "]";
			}
			if (n < quantities.size() - 1) os << ", ";
		}
		os << ")";
	}

	/// set the value of quantity n to v
	void Configuration::setQuantity(int n, QuantityValuePtr v) {
		quantities[n] = v;
	}

	/// retrieve quantity with index n
	QuantityValuePtr Configuration::getQuantity(const unsigned int n) const {
#ifdef _DEBUG
		if (n > quantities.size()) {
			throw EParetoCalculatorError("Index out of bounds in Configuration::getQuantity(int).");
			return quantities[0];
		}
#endif
		return quantities[n];
	}

	// retrieve a quantity by its name in the configuration space
	QuantityValuePtr Configuration::getQuantity(const QuantityName& n) const {
		// find the index
		unsigned int k = this->confspace->indexOfQuantity(n);
		// return the value
		return this->quantities[k];
	}

	/// check Pareto dominance on two configurations, returns true if c1 domaintes c2
	bool operator<=(const Configuration& c1, const Configuration& c2) {
#ifdef _DEBUG
		// convenient for debugging, too slow for real
		if (c1.confspace != c2.confspace) {
			throw EParetoCalculatorError("Error: cannot compare configurations of different configuration spaces.");
			return 0;
		}
#endif
		return c1.confspace->compare(c1, c2);
	}

	/// check equality of two configurations
	bool operator==(const Configuration& c1, const Configuration& c2) {
#ifdef _DEBUG
		if (c1.confspace != c2.confspace) {
			throw EParetoCalculatorError("Error: cannot compare configurations of different configuration spaces.");
			return 0;
		}
#endif
		return c1.confspace->equal(c1, c2);
	}

	/// a strict (non-reflexive) Pareto domaince check
	bool operator<(const Configuration& c1, const Configuration& c2) { return (c1 <= c2) && !(c1 == c2); };


	///////////////// ConfigurationSet ///////////

	/// constructor of ConfigurationSet, given a configuration space and a name
	ConfigurationSet::ConfigurationSet(ConfigurationSpacePtr cs, const std::string n) : 
		StorableObject(n), 
		confspace(cs) 
	{
	}


	/// copy constructor for ConfigurationSet 
	ConfigurationSet::ConfigurationSet(ConfigurationSetPtr cs) :
		StorableObject(cs->name),
		confspace(cs->confspace)
	{
		SetOfConfigurations::const_iterator i;
		for (i = cs->confs.begin(); i != cs->confs.end(); i++)
		{
			ConfigurationPtr c = std::make_shared<Configuration>(*i);
			this->confs.insert(c);
		}

	}


	/// add configuration c to the configuration set
	void ConfigurationSet::addConfiguration(ConfigurationPtr c) {
#ifdef _DEBUG
		if ((c->confspace) != (this->confspace)) {
			throw EParetoCalculatorError("Error: configuration is of wrong type in ConfigurationSet::addConfiguration");
		}
#endif
		confs.insert(c);
	}

	/// Add a new configuration to the configuration set.
	/// Important: Use this method only if you are sure that the configuration does not already occur in the set!
	void ConfigurationSet::addUniqueConfiguration(ConfigurationPtr c) {
#ifdef _DEBUG
		if ((c->confspace) != (this->confspace)) {
			throw EParetoCalculatorError("Error: configuration is of wrong type in ConfigurationSet::addConfiguration");
		}
#endif
		confs.insert(c);
	}

	/// insert a new configuration while maintaining Pareto minimality
	/// assumes the configuraiton set is Pareto minimal to start from
	/// after the funciton completes the configuration set is again Pareto minimal and
	/// equivalen to the original set extended with the new configuration c
	void ConfigurationSet::addConfigurationAndFilter(ConfigurationPtr c) {
		// assumes the configurations set is minimal
		// post: set is minimal version of set plus new configuration
		SetOfConfigurations::iterator i = this->confs.begin();
		while (i != this->confs.end()) {
			ConfigurationPtr a = *i;
			if (*a <= *c) {
				// c is dominated by a conf in this -> don't use c
				return;
			}
			else if (*c <= *a) {
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

	/// add all configurations from configuration set cs as unique (non-existing configurations
	/// Note: assumes that the new configurations do not yet exist in the set
	void ConfigurationSet::addUniqueConfigurationsOf(ConfigurationSetPtr cs) {
		for (SetOfConfigurations::iterator i = cs->confs.begin(); i != cs->confs.end(); i++) {
			ConfigurationPtr c = (*i);
			this->addUniqueConfiguration(c);
		}
	}

	/// check if the set contains configuration c
	bool ConfigurationSet::containsConfiguration(ConfigurationPtr c) {
		return confs.find(c) != confs.end();
	}

	/// produce a textual representation of the configuration set on the stream os
	void ConfigurationSet::streamOn(std::ostream& os) const {
		os << "{";
		SetOfConfigurations::iterator i;
		unsigned int n = 0;
		for (i = confs.begin(); i != confs.end(); i++, n++) {
			ConfigurationPtr c = *i;
			os << c;
			if (n < confs.size() - 1) os << ", " << std::endl;
		}
		os << "}";
	}

	bool ConfigurationSet::checkConfSpaceConsistency(void) const
	{
		SetOfConfigurations::const_iterator i;
		for (i = this->confs.begin(); i != this->confs.end(); i++)
		{
			if (!(*i)->confspace->isIdenticalTo(this->confspace)) {
				return false;
			}
		}
		return true;
	}

	void ConfigurationSet::adoptConfigurationSpaceOf(ConfigurationSetPtr cs)
	{
		this->confspace = cs->confspace;
		SetOfConfigurations::iterator i = this->confs.begin();
		while (i != this->confs.end())
		{
			(*i)->adoptConfigurationSpace(this->confspace);
			i++;
		}
	}


	/// make a copy of the configuration set
	StorableObjectPtr ConfigurationSet::copy(void) const
	{
		return std::make_shared<ConfigurationSet>(*this);
	}

	/// return the quantity value the index reference points to
	QuantityValuePtr ConfigurationIndexReference::value(void) const {
		return (this->conf)->getQuantity(index.quantity);
	}

	/// compare two index references for equality by the values they refer to
	bool ConfigurationIndexReference::operator==(const ConfigurationIndexReference& right) const {
		return *(this->value()) == *(right.value());
	}

	/// compare two index references of the totally ordered reference for strict inequality by the values they refer to
	bool ConfigurationIndexOnTotalOrderReference::operator<(const ConfigurationIndexReference& right) const {
		return *(this->value()) < *(right.value());
	}

	/// compare two index references for strict greater inequality by the values they refer to
	bool ConfigurationIndexReference::operator>(const ConfigurationIndexReference& right) const {
		// delegate to smaller than relation
		return right < *this;
	}

	/// constructor of IndexOnConfigurationSet
	IndexOnConfigurationSet::IndexOnConfigurationSet(const QuantityName& qn, ConfigurationSetPtr cs) : 
		quantity(qn), 
		confset(cs)
	{
	}

	/// constructor of IndexOnTotalOrderConfigurationSet from quantity name and configuraton set 
	IndexOnTotalOrderConfigurationSet::IndexOnTotalOrderConfigurationSet(const QuantityName& qn, ConfigurationSetPtr cs) : 
		IndexOnConfigurationSet(qn, cs) 
	{
		// make a list of references for the index
		SetOfConfigurations::iterator i;
		for (i = cs->confs.begin(); i != cs->confs.end(); i++) {
			ConfigurationIndexOnTotalOrderReferencePtr r = std::make_shared<ConfigurationIndexOnTotalOrderReference>(*i, *this);
			this->push_back(r);
		}
		std::sort(this->begin(), this->end());
	}

	/// lower returns the smallest index such that S[i,Q]>=v
	int IndexOnConfigurationSet::lower(const ConfigurationIndexReference& v) {
		int a, b, m;
		// use a binary search
		a = -1; // S[a,Q]<v
		b = (int)  this->confset->confs.size(); // S[b,Q]>=v;
		while (b - a > 1) {
			m = (a + b) / 2;
			if (*(this->get(m)) < v) {
				a = m;
			}
			else {
				b = m;
			}
		}
		return b;
	}

	/// upper returns the largest index such that S[i,Q]<=v
	int IndexOnConfigurationSet::upper(const ConfigurationIndexReference& v) {
		int a, b, m;
		// use a binary search
		a = -1; // S[a,Q]<=v
		b = (int) this->confset->confs.size(); // S[b,Q]>v;
		while (b - a > 1) {
			m = (a + b) / 2;
			if (*(this->get(m)) > v) {
				b = m;
			}
			else {
				a = m;
			}
		}
		return a;
	}

	/// copy a range from f to t from the index as a new configuration set
	ConfigurationSetPtr IndexOnTotalOrderConfigurationSet::copyFromTo(int f, int t) {
		// make the resulting configuration set in the same configuration space
		ConfigurationSetPtr res = std::make_shared<ConfigurationSet>(this->confset->confspace, this->confset->name + " range");
		// advance the index to position f
		// TODO: linearly? can we make this more efficient?
		IndexOnTotalOrderConfigurationSet::iterator i = this->begin();
		for (int j = 0; j < f; j++, i++);
		// copy the configurations in the range to the result set
		for (int j = f; j <= t; j++, i++) {
			res->addConfiguration((*i)->conf);
		}
		return res;
	}

	bool operator<(const ConfigurationIndexOnTotalOrderReferencePtr pa, const ConfigurationIndexOnTotalOrderReferencePtr pb)
	{
		return *pa < *pb;
	}

	/// copy a range from f to t from the index as a new configuration set
	ConfigurationSetPtr IndexOnUnorderedConfigurationSet::copyFromTo(int f, int t) {
		// make the resulting configuration set in the same configuration space
		ConfigurationSetPtr res = std::make_shared<ConfigurationSet>(this->confset->confspace, this->confset->name + " range");
		// advance the index to position f
		// TODO: linearly? can we make this more efficient?
		IndexOnUnorderedConfigurationSet::iterator i = this->begin();
		for (int j = 0; j < f; j++, i++);
		// copy the configurations in the range to the result set
		for (int j = f; j <= t; j++, i++) {
			res->addConfiguration((*i)->conf);
		}
		return res;
	}

	/// a total order to organize the unordered quantities
	bool ConfigurationIndexOnUnorderedReference::operator<(const ConfigurationIndexReference& right) const {
		QuantityValuePtr addrA = this->value();
		QuantityValuePtr addrB = right.value();
		// compare the pointers instead
		return addrA < addrB;
		// TODO: check, it this always safe. maybe replace by efficient direct comparison of strings without making too many copies.
		// was:	return this->value().asString()<right.value().asString();
	}


	/// constructor of IndexOnUnorderedConfigurationSet, given a name of the quantity and a configuration set to index
	IndexOnUnorderedConfigurationSet::IndexOnUnorderedConfigurationSet(const QuantityName& qn, ConfigurationSetPtr cs) : IndexOnConfigurationSet(qn, cs) {
		// create the references
		SetOfConfigurations::iterator i;
		for (i = cs->confs.begin(); i != cs->confs.end(); i++) {
			std::shared_ptr<ConfigurationIndexOnUnorderedReference> r = std::make_shared<ConfigurationIndexOnUnorderedReference>(*i, *this);
			this->push_back(r);
		}
		// sort the index
		std::sort(this->begin(), this->end());
	}


	///////////////// functions /////////////////


	/// stream a configuration through a pointer
	std::ostream& operator<<(std::ostream& os, ConfigurationPtr c) {
		c->streamOn(os);
		return os;
	}

	/// stream a configuration
	std::ostream& operator<<(std::ostream& os, const Configuration& c) {
		c.streamOn(os);
		return os;
	}

	std::ostream& operator<<(std::ostream& os, ConfigurationSetPtr cs) {
		cs->streamOn(os);
		return os;
	}

	std::ostream& operator<<(std::ostream& os, const ConfigurationSet& cs) {
		cs.streamOn(os);
		return os;
	}


}
