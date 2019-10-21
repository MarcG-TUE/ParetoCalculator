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

	ConfigurationSpace::ConfigurationSpace(std::string n) : StorableObject(n) {
	}

	void ConfigurationSpace::addQuantity(const QuantityType& q) {
		this->addQuantityAsVisibility(q, q.name, true);
	}

	void ConfigurationSpace::addQuantityAs(const QuantityType& q, const QuantityName qname) {
		this->addQuantityAsVisibility(q, qname, true);
	}

	void ConfigurationSpace::addQuantityAsVisibility(const QuantityType& q, const QuantityName qname, bool vis) {
		quantities.push_back(&q);
		quantityNames[qname] = quantities.size() - 1;
		this->quantityVisibility.push_back(vis);
	}

	void ConfigurationSpace::addQuantitiesOf(const ConfigurationSpace& cs) {
		for (unsigned int i = 0; i < cs.quantities.size(); i++) {
			this->addQuantityAsVisibility(*cs.quantities[i], cs.nameOfQuantityNr(i), cs.quantityVisibility[i]);
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



	const QuantityType& ConfigurationSpace::getQuantity(const QuantityName& qn) const //throw(EParetoCalculatorError)
	{
		if (quantityNames.find(qn) != quantityNames.end()) return *quantities.at(quantityNames.at(qn));
		ListOfQuantityTypes::const_iterator i;
		for (i = quantities.begin(); i != quantities.end(); i++) {
			if ((*i)->name == qn) return *(*i);
		}
		throw EParetoCalculatorError("quantity " + qn + " not found in ConfigurationSpace::getQuantity");
	}

	unsigned int ConfigurationSpace::nrOfVisibleQuantities(void) const {
		unsigned int count = 0;
		BoolVector::const_iterator i;
		for (i = this->quantityVisibility.begin(); i != this->quantityVisibility.end(); i++) {
			if (*i) count++;
		}
		return count;
	}


	unsigned int ConfigurationSpace::indexOfQuantity(const QuantityName& qn) const  //throw(EParetoCalculatorError)
	{
		if (quantityNames.find(qn) == quantityNames.end()) {
			throw EParetoCalculatorError("quantity " + qn + " not found in ConfigurationSpace::indexOfQuantity");
		}
		return quantityNames.at(qn);
	}

	QuantityNamePtr ConfigurationSpace::getUnorderedQuantity() const {
		QuantityIntMap::const_iterator i;
		unsigned int k;
		for (k = 0, i = this->quantityNames.begin(); i != this->quantityNames.end(); i++, k++) {
			const QuantityType* t = this->quantities[(*i).second];
			if ((this->quantityVisibility[k]) && (t->isUnordered())) {
				QuantityNamePtr qn = std::make_shared<QuantityName>((*i).first);
				return qn;
			}
		}
		return nullptr;
	}

	QuantityNamePtr ConfigurationSpace::getTotallyOrderedQuantity() const {
		QuantityIntMap::const_iterator i;
		unsigned int k;
		for (k = 0, i = this->quantityNames.begin(); i != this->quantityNames.end(); i++, k++) {
			const QuantityType* t = this->quantities[(*i).second];
			if ((this->quantityVisibility[k]) && (t->isTotallyOrdered())) {
				QuantityNamePtr qn = std::make_shared<QuantityName>((*i).first);
				return qn;
			}
		}
		return nullptr;
	}


	bool ConfigurationSpace::includesQuantityNamed(const QuantityName& qn) const {
		QuantityIntMap::const_iterator i;
		for (i = quantityNames.begin(); i != quantityNames.end(); i++) {
			if ((*i).first == qn) return true;
		}
		return false;
	}

	const unsigned int ConfigurationSpace::firstVisibleQuantity() const {
		BoolVector::const_iterator i;
		unsigned int k;
		for (i = this->quantityVisibility.begin(), k = 0; i != this->quantityVisibility.end(); i++, k++) {
			if (*i) return k;
		}
		throw EParetoCalculatorError("no visible quantity found in ConfigurationSpace::firstVisibleQuantity");
		return 0;
	}


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

	bool ConfigurationSpace::equal(const Configuration& c1, const Configuration& c2) const {
		for (unsigned int i = 0; i < this->quantities.size(); i++) {
			if (this->quantityVisibility[i] && !((*c1.getQuantity(i)) == (*c2.getQuantity(i)))) { return false; }
		}
		return true;
	}

	bool ConfigurationSpace::LexicographicCompare(const Configuration& c1, const Configuration& c2)
		// provides strict ordering for sorted storage of configurationset, hence ignores visibility
	{
		unsigned int k = c1.confspace.quantities.size();
		for (unsigned int i = 0; i < k; i++) {
			QuantityValuePtr v1 = c1.getQuantity(i);
			QuantityValuePtr v2 = c2.getQuantity(i);
			if (v1->totalOrderSmaller(*v2)) { return true; }
			if (v2->totalOrderSmaller(*v1)) { return false; }
		}
		return false;
	}



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

	StorableObjectPtr ConfigurationSpace::copy(void) const
	{
		return std::make_shared<ConfigurationSpace>(*this);
	}

	std::unique_ptr<std::string> Configuration::asString(void) {
		std::ostringstream myString;
		this->streamOn(myString);
		return std::make_unique<std::string>(myString.str());
	}


	ConfigurationSpacePtr ConfigurationSpace::hide(const ListOfQuantityNames& lqn) const {
		std::string n = "Hide of " + this->name;
		ConfigurationSpacePtr cs = std::make_shared<ConfigurationSpace>(n);
		for (unsigned int i = 0; i < this->quantities.size(); i++) {
			bool v = this->quantityVisibility[i];
			if (lqn.includes(this->nameOfQuantityNr(i))) {
				v = false;
			}
			cs->addQuantityAsVisibility(*this->quantities[i], this->nameOfQuantityNr(i), v);
		}
		return cs;
	}

	ConfigurationSpacePtr ConfigurationSpace::hide(const QuantityName& qn) {
		ListOfQuantityNames l;
		l.push_back(qn);
		return this->hide(l);
	}


	ConfigurationSpacePtr ConfigurationSpace::unhide(const ListOfQuantityNames& lqn) const {
		std::string n = "Unhide of " + this->name;
		ConfigurationSpacePtr cs = std::make_shared<ConfigurationSpace>(n);
		for (unsigned int i = 0; i < this->quantities.size(); i++) {
			bool v = this->quantityVisibility[i];
			if (lqn.includes(this->nameOfQuantityNr(i))) {
				v = true;
			}
			cs->addQuantityAsVisibility(*this->quantities[i], this->nameOfQuantityNr(i), v);
		}
		return cs;
	}

	ConfigurationSpacePtr ConfigurationSpace::unhide(const QuantityName& qn) const {
		ListOfQuantityNames l;
		l.push_back(qn);
		return this->unhide(l);
	}



	const QuantityName ConfigurationSpace::nameOfQuantityNr(const unsigned int n) const {
		QuantityIntMap::const_iterator i;
		for (i = this->quantityNames.begin(); i != this->quantityNames.end(); i++) {
			if ((*i).second == n) { return (*i).first; }
		}
		throw EParetoCalculatorError("Error: quantity not found in function nameOfQuantitynr.");
	}


	ConfigurationPtr ConfigurationSpace::newConfiguration(void) {

		ConfigurationPtr c = std::make_shared<Configuration>(*this);

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

	Configuration::Configuration(const ConfigurationSpace& cs) :confspace(cs) {
	}

	// 'copy constructor' from pointer
	Configuration::Configuration(ConfigurationPtr c) :
		confspace(c->confspace)
	{
		ListOfQuantityValues::iterator i;
		for (i = c->quantities.begin(); i != c->quantities.end(); i++) {
			this->quantities.push_back(*i);
		}
	}

	void Configuration::addQuantity(QuantityValuePtr q) {
		quantities.push_back(q);
	}

	void Configuration::addQuantitiesOf(ConfigurationPtr c) {
		ListOfQuantityValues::iterator i;
		for (i = c->quantities.begin(); i != c->quantities.end(); i++) {
			this->addQuantity(*i);
		}
	}


	void Configuration::streamOn(std::ostream& os) const {
		os << "(";
		ListOfQuantityValues::const_iterator i;
		unsigned int n = 0;
		for (i = quantities.begin(); i != quantities.end(); i++, n++) {
			if (this->confspace.quantityVisibility[n]) {
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

	void Configuration::setQuantity(int n, QuantityValuePtr v) {
		quantities[n] = v;
	}


	QuantityValuePtr Configuration::getQuantity(const unsigned int n) const {
#ifdef _DEBUG
		if (n > quantities.size()) {
			throw EParetoCalculatorError("Index out of bounds in Configuration::getQuantity(int).");
			return quantities[0];
		}
#endif
		return quantities[n];
	}

	QuantityValuePtr Configuration::getQuantity(const QuantityName& n) const {
		unsigned int k = this->confspace.indexOfQuantity(n);
		return this->quantities[k];
	}


	bool operator<=(const Configuration& c1, const Configuration& c2) {
#ifdef _DEBUG
		// convenient for debugging, too slow for real
		if (&(c1.confspace) != &(c2.confspace)) {
			throw EParetoCalculatorError("Error: cannot compare configurations of different configuration spaces.");
			return 0;
		}
#endif
		return c1.confspace.compare(c1, c2);
	}

	bool operator==(const Configuration& c1, const Configuration& c2) {
#ifdef _DEBUG
		if (&(c1.confspace) != &(c2.confspace)) {
			throw EParetoCalculatorError("Error: cannot compare configurations of different configuration spaces.");
			return 0;
		}
#endif
		return c1.confspace.equal(c1, c2);
	}

	bool operator<(const Configuration& c1, const Configuration& c2) { return (c1 <= c2) && !(c1 == c2); };


	///////////////// ConfigurationSet ///////////

	ConfigurationSet::ConfigurationSet(ConfigurationSpacePtr cs, const std::string n) : StorableObject(n), confspace(cs) {
	}

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



	void ConfigurationSet::addConfiguration(ConfigurationPtr c) {
#ifdef _DEBUG
		if (&(c->confspace) != &(*(this->confspace))) {
			throw EParetoCalculatorError("Error: configuration is of wrong type in ConfigurationSet::addConfiguration");
		}
#endif
		confs.insert(c);
	}

	void ConfigurationSet::addUniqueConfiguration(ConfigurationPtr c) {
		// Note: Use this method only if you are sure that the configuration does not already occur in the set!
#ifdef _DEBUG
		if (&(c->confspace) != &(*(this->confspace))) {
			throw EParetoCalculatorError("Error: configuration is of wrong type in ConfigurationSet::addConfiguration");
		}
#endif
		confs.insert(c);
	}

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

	void ConfigurationSet::addUniqueConfigurationsOf(ConfigurationSetPtr cs) {
		for (SetOfConfigurations::iterator i = cs->confs.begin(); i != cs->confs.end(); i++) {
			ConfigurationPtr c = (*i);
			this->addUniqueConfiguration(c);
		}
	}


	bool ConfigurationSet::containsConfiguration(ConfigurationPtr c) {
		return confs.find(c) != confs.end();
	}

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

	StorableObjectPtr ConfigurationSet::copy(void) const
	{
		return std::make_shared<ConfigurationSet>(*this);
	}

	QuantityValuePtr ConfigurationIndexReference::value(void) const {
		return (this->conf)->getQuantity(index.quantity);
	}


	bool ConfigurationIndexReference::operator==(const ConfigurationIndexReference& right) const {
		return *(this->value()) == *(right.value());
	}

	bool ConfigurationIndexOnTotalOrderReference::operator<(const ConfigurationIndexReference& right) const {
		return this->value() < right.value();
	}

	bool ConfigurationIndexReference::operator>(const ConfigurationIndexReference& right) const {
		return right < *this;
	}

	bool ConfigurationIndexReference::operator<(const ConfigurationIndexReference& right) const {
		throw EParetoCalculatorError("Error: < should not be called on abstract ConfigurationIndexReference");
	}

	ConfigurationIndexReferencePtr IndexOnConfigurationSet::get(int n) {
		throw EParetoCalculatorError("Error: 'get' should not be called on abstract ConfigurationIndexReference");
	}

	ConfigurationSetPtr IndexOnConfigurationSet::copyFromTo(int f, int t) {
		throw EParetoCalculatorError("Error: 'copyFromTo' should not be called on abstract ConfigurationIndexReference");
	}


	//ConfigurationIndexOnTotalOrderReference& ConfigurationIndexOnTotalOrderReference::operator= (ConfigurationIndexOnTotalOrderReference& right) {
	//	this->conf = right.conf;
	//	this->index = right.index;
	//	return *this;
	//}


	IndexOnConfigurationSet::IndexOnConfigurationSet(const QuantityName& qn, ConfigurationSetPtr cs) : quantity(qn), confset(cs) {
	}


	IndexOnTotalOrderConfigurationSet::IndexOnTotalOrderConfigurationSet(const QuantityName& qn, ConfigurationSetPtr cs) : IndexOnConfigurationSet(qn, cs) {
		SetOfConfigurations::iterator i;
		for (i = cs->confs.begin(); i != cs->confs.end(); i++) {
			std::shared_ptr<ConfigurationIndexOnTotalOrderReference> r = std::make_shared<ConfigurationIndexOnTotalOrderReference>(*i, *this);
			this->push_back(r);
		}
		std::sort(this->begin(), this->end());
	}

	// lower returns the smallest index such that S[i,Q]>=v
	int IndexOnConfigurationSet::lower(const ConfigurationIndexReference& v) {
		int a, b, m;
		a = -1; // S[a,Q]<v
		b = this->confset->confs.size(); // S[b,Q]>=v;
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


	// upper returns the largest index such that S[i,Q]<=v
	int IndexOnConfigurationSet::upper(const ConfigurationIndexReference& v) {
		int a, b, m;
		a = -1; // S[a,Q]<=v
		b = this->confset->confs.size(); // S[b,Q]>v;
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


	ConfigurationSetPtr IndexOnTotalOrderConfigurationSet::copyFromTo(int f, int t) {
		ConfigurationSetPtr res = std::make_shared<ConfigurationSet>(this->confset->confspace, this->confset->name + " range");
		IndexOnTotalOrderConfigurationSet::iterator i = this->begin();
		for (int j = 0; j < f; j++, i++);
		for (int j = f; j <= t; j++, i++) {
			res->addConfiguration((*i)->conf);
		}
		return res;
	}


	ConfigurationSetPtr IndexOnUnorderedConfigurationSet::copyFromTo(int f, int t) {
		ConfigurationSetPtr res = std::make_shared<ConfigurationSet>(this->confset->confspace, this->confset->name + " range");
		IndexOnUnorderedConfigurationSet::iterator i = this->begin();
		for (int j = 0; j < f; j++, i++);
		for (int j = f; j <= t; j++, i++) {
			res->addConfiguration((*i)->conf);
		}
		return res;
	}


	bool ConfigurationIndexOnUnorderedReference::operator<(const ConfigurationIndexReference& right) const {
		QuantityValuePtr addrA = this->value();
		QuantityValuePtr addrB = right.value();
		return addrA < addrB;
		// TODO: check, it this always safe. maybe replace by efficient direct comparison of strings without making too many copies.
		// was:	return this->value().asString()<right.value().asString();
	}


	//ConfigurationIndexOnUnorderedReference& ConfigurationIndexOnUnorderedReference::operator= (ConfigurationIndexOnUnorderedReference& right) {
	//	this->conf = right.conf;
	//	this->index = right.index;
	//	return *this;
	//}


	IndexOnUnorderedConfigurationSet::IndexOnUnorderedConfigurationSet(const QuantityName& qn, ConfigurationSetPtr cs) : IndexOnConfigurationSet(qn, cs) {
		SetOfConfigurations::iterator i;
		for (i = cs->confs.begin(); i != cs->confs.end(); i++) {
			std::shared_ptr<ConfigurationIndexOnUnorderedReference> r = std::make_shared<ConfigurationIndexOnUnorderedReference>(*i, *this);
			this->push_back(r);
		}
		std::sort(this->begin(), this->end());
	}



	///////////////// functions /////////////////



	std::ostream& operator<<(std::ostream& os, ConfigurationPtr c) {
		c->streamOn(os);
		return os;
	}

	std::ostream& operator<<(std::ostream& os, const Configuration& c) {
		c.streamOn(os);
		return os;
	}

}
