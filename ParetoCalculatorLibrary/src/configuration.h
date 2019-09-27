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
// Implementation of the Configuration and ConfigurationSpace, ConfigurationSet 
// classes and supporting classes, for instance to create a sorted index on a set 
// configurations
//

#ifndef PARETO_CONFIGURATION_H
#define PARETO_CONFIGURATION_H

#ifdef _MSC_VER
#pragma warning( disable : 4290 ) // to ignore the exception specification warnings in Visual Studio
#endif

#include <vector>
#include <list>
#include <set>
#include <string>
#include <map>
#include <iostream>
#include "exceptions.h"
#include "quantity.h"
#include "storage.h"


namespace Pareto {

	class Configuration;

	// An STL vector of pointers to quantity types
	// 
	class ListOfQuantityTypes : public std::vector<const QuantityType*> {};

	/// An STL vector of quantity names
	/// 
	class ListOfQuantityNames: public std::vector<QuantityName>{
	public:
		bool includes(const QuantityName& s) const {
			for(const_iterator i=this->begin(); i!=this->end(); i++){
				if((*i)==s) return true;
			}
			return false;
		}
	};

	// An STL map from QuantityNames to integers
	// 
	class QuantityIntMap: public std::map<const QuantityName, unsigned int> {};

	// An STL vector of bools
	// 
	class BoolVector: public std::vector<bool> {};


	///
	/// ConfigurationSpace represents a configuration space of Pareto Algebra.
	///
	/// Member 'quantities' is a list of quantity types that build up the space
	/// 'quantityNames' maps names of the quantity types (in this particular space)
	/// to a corresponding index in the list of quantity types.
	/// Additionally, quantities in the space may be hidden or visible as indicated
	/// by 'quantityVisibility'.
	///
	class ConfigurationSpace: public StorableObject {
	public:
		ListOfQuantityTypes quantities;
		QuantityIntMap quantityNames;
		BoolVector quantityVisibility;

		/// Constructor of configuration space with name 'n'
		ConfigurationSpace(std::string n);

		/// compare tests the dominance relation between configurations
		bool compare(const Configuration& c1, const Configuration& c2) const;

		/// equal tests whether the configurations are the same
		bool equal(const Configuration& c1, const Configuration& c2) const;

		/// make a lexicographical <= comparison between two configurations
		static bool LexicographicCompare(const Configuration& c1, const Configuration& c2);

		/// add a single quantity to the space, with the default name
		void addQuantity(const QuantityType& q);

		/// add a quantity to the space using the name 'qname'
		void addQuantityAs(const QuantityType& q, const QuantityName qname);

		/// add a quantity to the space using the name 'qname' and explicitly
		/// specify visibility.
		void addQuantityAsVisibility(const QuantityType& q, const QuantityName qname, bool vis);
		
		/// copy quantities of another configuration space to this configuration space
		void addQuantitiesOf(const ConfigurationSpace& cs);

		/// test if the configuration space includes a quantity by the name 'qn'
		bool includesQuantityNamed(const QuantityName& qn) const;

		/// retrieve a quantity of the configuration space by its name
		const QuantityType& getQuantity(const QuantityName& qn) const; // throw(EParetoCalculatorError);

		/// create a new configuration in this configuration space
		Configuration* newConfiguration(void);

		/// returns the number of visible (not hidden) quantities in the space
		unsigned int nrOfVisibleQuantities(void) const;

		/// returns the index of quantity with name 'qn' in the list of quantities
		unsigned int indexOfQuantity(const QuantityName& qn) const; // throw(EParetoCalculatorError);

		/// returns an arbitrary unordered quantity from the configuration space if one exists.
		///
		/// returns NULL otherwise
		const QuantityName* getUnorderedQuantity() const;

		/// returns an arbitrary totally ordered quantity from the configuration space if one exists.
		///
		/// returns NULL otherwise
		const QuantityName* getTotallyOrderedQuantity() const;

		/// retrieve the name of quantity number 'n'
		const QuantityName nameOfQuantityNr(const unsigned int n) const;

		/// returns the first visible quantity of the space. Assumes there is at least one. 
		/// Throws exception otherwise
		const unsigned int firstVisibleQuantity() const;

		/// generates a NEW configuration space in which quantity 'qn' is hidden
		ConfigurationSpace* hide(const QuantityName& qn);

		/// generates a NEW configuration space in which quantities 'lqn' are hidden
		ConfigurationSpace* hide(const ListOfQuantityNames *lqn) const;

		/// generates a NEW configuration space in which quantity 'qn' is unhidden
		ConfigurationSpace* unhide(const QuantityName& qn) const;

		/// generates a NEW configuration space in which quantities 'lqn' are unhidden
		ConfigurationSpace* unhide(const ListOfQuantityNames *lqn) const;

		/// stream a string representation of the configuration space to 'os'
		const void streamOn(std::ostream& os);


		// Run-time type checking.

		virtual bool isConfigurationSet(void) const { return false;}  // override
		virtual bool isConfigurationSpace(void) const { return true;} // override
		virtual bool isQuantityType(void) const { return false;} // override
		virtual bool isString(void) const { return false;} // override

		// copying
		virtual StorableObject& copy(void) const;

	};

	bool operator!=(ConfigurationSpace& cs1, ConfigurationSpace& cs2);

	// An STL vector of QuantityValues
	class ListOfQuantityValues : public std::vector<QuantityValue*> {
	};

	/// A configuration of Pareto Algebra.
	///
	/// A configuration is an element of a configuration space.
	/// The member 'confspace' refers to this space
	class Configuration{
	public:
		/// The member 'confspace' refers to this space
		const ConfigurationSpace* confspace;
		/// 'quantities' is a list of quantity values that defines the configuration.
		ListOfQuantityValues quantities;

		/// Use ConfigurationSpace::newConfiguration() to create configurations!
		Configuration(const ConfigurationSpace* cs);

		virtual ~Configuration(){};

		// Add a quantity value to the configuration
		virtual void addQuantity(const QuantityValue& q);

		// Copy quantity values of another configurwtion to this configuration
		virtual void addQuantitiesOf(const Configuration& c);

		/// retrieve quantity value of quantity with index number 'n'
		virtual const QuantityValue& getQuantity(const unsigned int n) const;

		/// retrieve quantity value of quantity with name 'n'
		virtual const QuantityValue& getQuantity(const QuantityName& n) const;

		/// set quantity value of quantity with index number 'n'
		virtual void setQuantity(int n, QuantityValue* v);

		/// stream a string representation of the configuration to 'os'
		virtual void streamOn(std::ostream& os) const;

		/// returns a string representation of the configuration
        std::string& asString();
	};

    std::ostream& operator<<(std::ostream& os, const Configuration* c);

	bool operator<=(const Configuration& c1, const Configuration& c2);
	bool operator==(const Configuration& c1, const Configuration& c2);
	bool operator<(const Configuration& c1, const Configuration& c2);

	class IndexOnTotalOrderConfigurationSet;
	class IndexOnUnorderedConfigurationSet;
	class IndexOnConfigurationSet;
	class ConfigurationIndexReference;
	class ConfigurationIndexOnTotalOrderReference;
	class ConfigurationIndexOnUnorderedReference;
	class ConfigurationSet;

	class ConfigurationIndexReference {
	public:
		const Configuration* conf;
		IndexOnConfigurationSet* index;
		ConfigurationIndexReference(const Configuration* c, IndexOnConfigurationSet* i): conf(c), index(i){}
		virtual ~ConfigurationIndexReference(){}
		const QuantityValue& value(void) const;
		bool operator==(const ConfigurationIndexReference& right) const;
		bool operator>(const ConfigurationIndexReference& right) const ;
		virtual bool operator<(const ConfigurationIndexReference& right) const;
	};

	class ConfigurationIndexOnTotalOrderReference : public ConfigurationIndexReference {
	public:
		ConfigurationIndexOnTotalOrderReference(const Configuration* c, IndexOnConfigurationSet* i): ConfigurationIndexReference(c, i){}
		virtual bool operator<(const ConfigurationIndexReference& right) const ;

		const ConfigurationIndexOnTotalOrderReference& operator= (const ConfigurationIndexOnTotalOrderReference& right);
	};

	class ConfigurationIndexOnUnorderedReference : public ConfigurationIndexReference {
	public:
		ConfigurationIndexOnUnorderedReference(const Configuration* c, IndexOnConfigurationSet* i): ConfigurationIndexReference(c, i){}
		virtual bool operator<(const ConfigurationIndexReference& right) const ;

		const ConfigurationIndexOnUnorderedReference& operator= (const ConfigurationIndexOnUnorderedReference& right);
	};

	class IndexOnConfigurationSet {
	public:
		const QuantityName& quantity;
		const ConfigurationSet* confset;
		IndexOnConfigurationSet(const QuantityName& qn, const ConfigurationSet* cs);
		virtual ~IndexOnConfigurationSet(){}
		virtual ConfigurationSet* copyFromTo(int f, int t);
		int lower(const ConfigurationIndexReference& v);
		int upper(const ConfigurationIndexReference& v);
		virtual ConfigurationIndexReference* get(int n);
	};

	// implements an index on the configurations of a configurationset
	// used for sorting the configurations w.r.t. different quantities.
	class IndexOnTotalOrderConfigurationSet: public std::vector<ConfigurationIndexOnTotalOrderReference>, public IndexOnConfigurationSet {
	public:
		IndexOnTotalOrderConfigurationSet(const QuantityName& qn, const ConfigurationSet* cs);
		virtual ConfigurationIndexReference* get(int n) {return &(this->at(n));}
		virtual ConfigurationSet* copyFromTo(int f, int t);
	};

	// Make an index on an unordered quantity, based on a total order derived from the string
	// representation of the quantity values
	class IndexOnUnorderedConfigurationSet: public std::vector<ConfigurationIndexOnUnorderedReference>, public IndexOnConfigurationSet {
	public:
		IndexOnUnorderedConfigurationSet(const QuantityName& qn, const ConfigurationSet* cs);
		virtual ConfigurationIndexReference* get(int n) {return &(this->at(n));}
		virtual ConfigurationSet* copyFromTo(int f, int t);
	};





	struct CompareConfiguration
	{
	  bool operator()(const Configuration& c1, const Configuration& c2) const
	  {
		  return ConfigurationSpace::LexicographicCompare(c1, c2);
	  }
	};


	class SetOfConfigurations: public std::set<Configuration, CompareConfiguration> {};


	/// Represents a set of configurations
	class ConfigurationSet : public StorableObject {
	public:
		/// constructor of a set of configurations on configuration space 'cs' and with name 'n'
		ConfigurationSet(const ConfigurationSpace* cs, const std::string n);

		/// add a configuration to the set
		void addConfiguration(const Configuration& c);

		/// add a configuration to the set.
		/// Assumes that 'c' does not yet occur in the set.
		void addUniqueConfiguration(const Configuration& c);

		/// add a configuration to a minimal set and keep minimal
		/// Assumes the set is minimal 
		/// Post: set is the minimal equivalent to the original set plus the new conf
		void addConfigurationAndFilter(const Configuration& c);

		// add all configurations of cs to this configuration set
		// Assumes that the new configurations are not already included 
		void addUniqueConfigurationsOf(ConfigurationSet& cs);

		/// test whether the configuration is included in the set
		bool containsConfiguration(Configuration& c);

		/// stream a string representation of the configuration set to 'os'
		virtual const void streamOn(std::ostream& os);

		// Run-time type checking.
		virtual bool isConfigurationSet(void) const { return true; }
		virtual bool isConfigurationSpace(void) const { return false; }
		virtual bool isQuantityType(void) const { return false; }
		virtual bool isString(void) const { return false; }

		// copying
		virtual StorableObject& copy(void) const;
	

		// the actual configurations
		SetOfConfigurations confs;
		
		/// reference to the configuration space of the configurations in this set.
		const ConfigurationSpace* confspace;
	};
}

#endif
