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

#ifndef PARETO_CALCULATOR_H
#define PARETO_CALCULATOR_H

#include "operations.h"

namespace Pareto {

	class ParetoParser;

	/// An abstract superclass from which to derive objects to monitor
	/// information from the calculator about its activities.
	class StatusCallback {
	public:
		virtual ~StatusCallback(){};
		/// called by the calculator to indicite the status of its activities
		virtual void setStatus(const std::string& s) = 0;
		/// called by the calculator to generate verbose stream description of what it is doing.
		virtual void verbose(const std::string& s) = 0;
	};

	typedef std::list<ConfigurationSet*> ListOfConfSet;

	/// The Pareto Calculator object
	///
	/// The calculator can load data and calculation instructions from XML files.
	/// You can create operations and execute them on the calculator.
	/// You can store and retrieve data in memory and on the stack of the calculator
	/// In future version it should be possible to store results back to XML files.
	class ParetoCalculator {
	public:
		/// Constructor of the ParetoCalculator
		ParetoCalculator();

		/// The stack of the calculator, where it finds operands for the operations and stores the results.
		StackOfStorageObjects stack;

		/// A memory of the calculator for filing objects of type StorableObject.
		StorageMap memory;

		/// store an StorableObject 'o'in memory under its name
		void store(const StorableObject& o);

		/// store an StorableObject 'o'in memory under key
		void store(const StorableObject& o, const std::string& key);

		/// pop an object from the stack and store it in memory, filed under its name
		void store(void);

		/// pop an object from the stack and store it in memory, filed under 'key'
		void store(const std::string& key);

		/// retrieve an object from the memory 
		const StorableObject* retrieve(const std::string& oname); 

		/// retrieve a quantity type from the memory 
		const QuantityType* retrieveQuantityType(const std::string& oname); 

		/// push an object 'o' onto the calculator stack
		void push(const StorableObject& o);

		/// retreive the object name 'oname' from memory and push it on the stack
		void push(const std::string& oname); 

		/// return an object popped from the stack
		const StorableObject* pop(); 

		/// pop a configuration set from the set.
		/// Throws an exception if the object is not a configuration set
		const ConfigurationSet* popConfigurationSet(); 

		/// return a pointer to the object on top of the stack without removing it.
		const StorableObject* peek(); 

		/// duplicate the object on top of the stack
		void duplicate();

		/// Put the names of the object in memory on the stream 'os'
		void listStorage(std::ostream& os);

		/// pop an object from the stack and show it on the stream 'os'
		void printOn(std::ostream& os);

		/// pop an object from the stack and show it on the StatusCallBack-verbose
		void print();

		// to integrate in GUI, allow status information to be monitored

		/// contains a description of the current status of the calculator
		std::string status;

		/// reference to a StatusCallBack object. Use setStatusCallbackObject to register 
		/// the object.
		StatusCallback* statusObject;

		/// Register a StatusCallBack object.
		void setStatusCallbackObject(StatusCallback* o);

		// called by the calculator when its status changes
		void setStatus(const std::string& newStatus);

		// called by the calculator to log its activities
		void verbose(const std::string& addText);

		//
		// static functions which can be used without an instance of the calculator
		//

		// compute the product of the two configuration sets
		static ConfigurationSet& product(const ConfigurationSet& cs1, const ConfigurationSet& cs2);

		// compute the product of the two configuration sets in an already defined predefined configuration space
		static ConfigurationSet& productInSpace(const ConfigurationSet& cs1, const ConfigurationSet& cs2, ConfigurationSpace* cspace);

		// abstract the n-th dimension from the configuration set
		static ConfigurationSet& abstraction(const ConfigurationSet& cs, unsigned int n);

		// abstract a quantity named s, a 'StorableString'
		static ConfigurationSet& abstraction(const ConfigurationSet& cs, const StorableString& s);

		// abstract a quantity named qn, a QuantityName
		static ConfigurationSet& abstraction(const ConfigurationSet& cs, const QuantityName& qn);

		// hide a collection of quantities, specified by the list of names lqn
		static ConfigurationSet& hiding(const ConfigurationSet& cs, const ListOfQuantityNames *lqn);

		// hide a single quantity named qn
		static ConfigurationSet& hiding(const ConfigurationSet& cs, const QuantityName& qn);

		// compute the constraint (intersection) of two configuration sets
		static ConfigurationSet& constraint(const ConfigurationSet& cs1, const ConfigurationSet& cs2);
		static ConfigurationSet& constraint(const ConfigurationSet& cs1, bool (*testConstraint)(const Pareto::Configuration&));
		static ConfigurationSet& alternative(const ConfigurationSet& cs1, const ConfigurationSet& cs2);
		static ConfigurationSet& minimize(const ConfigurationSet& cs);
		static ConfigurationSet& minimize_SC(ConfigurationSet& cs);
		static const unsigned int MINIMIZE_THRESHOLD = 2048; // determine optimal value!
		static const unsigned int FILTER_THRESHOLD = 2048; // determine optimal value!
		static const ConfigurationSet& efficient_minimize(const ConfigurationSet& cs);

		/// execute block-based minimization
		void minimize(void);
		/// execute generalized multi-dimensional Divide-and-Conquer minimization
		void efficient_minimize(void);
		/// compute product
		void product(void);
		/// compute abstraction
		void abstract(void);
		/// hide quantities
		void hide(ListOfQuantityNames *lqn);

		/// apply a constraint.
		///
		/// The constraint is implemented as the characteristic function testConstraint
		/// which returns true when a configuration satisfies the constraint.
		void constraint(bool (*testConstraint)(const Configuration&));

		// parsing
		ParetoParser *parser;
		
		/// call this function before using the parser function below.
		void initParser();

		/// load an XML file 'fn' into memory
		void LoadFile(const std::wstring& fn);

		/// load the quantity types specified in the loaded XML file into memory
		void LoadQuantityTypes();

		/// load the configuration spaces specified in the loaded XML file into memory
		void LoadConfigurationSpaces();

		/// load the configuration sets specified in the loaded XML file into memory
		void LoadConfigurationSets();

		/// not yet implemented in the current version, sorry
		void LoadOperations();

#ifdef _MSC_VER
		void SaveAllFile(const std::wstring& fn);
		void SaveItemFile(const std::string& itemToSave, const std::wstring& fn);
#else
		// porting: to be done
		// void SaveAllFile(const std::wstring& fn);
		// void SaveItemFile(const std::string& itemToSave, const std::wstring& fn);
#endif

	private:
		static ConfigurationSet* efficient_minimize_unordered(const ConfigurationSet* cs, const QuantityName& qn);
		static ListOfConfSet* splitClasses(const ConfigurationSet* cs, const QuantityName& qn);
		static const ConfigurationSet* efficient_minimize_totally_ordered(const ConfigurationSet* cs, const QuantityName& qn);
		static const ConfigurationSet* efficient_minimize_totally_ordered_recursive(ConfigurationSet* cs, const QuantityName& qn);
		static ListOfConfSet* splitLowHigh(const ConfigurationSet* cs, const QuantityName& qn, const QuantityValue **v);
		static const ConfigurationSet* efficient_minimize_dcmerge(const ConfigurationSet* csl, const ConfigurationSet* csh, const QuantityName& qn, const QuantityValue& v);
		static const ConfigurationSet* efficient_minimize_filter1(const ConfigurationSet* csl, const ConfigurationSet* csh, const QuantityName* qn);
		static const ConfigurationSet* efficient_minimize_filter2(const ConfigurationSet* csa, const ConfigurationSet* csb);
		static const ConfigurationSet* efficient_minimize_filter3(const ConfigurationSet* csa, const ConfigurationSet* csb);
		static const ConfigurationSet* efficient_minimize_filter4(const ConfigurationSet* csa, const ConfigurationSet* csb);
		static const QuantityValue& efficient_minimize_getPivot(const ConfigurationSet* cs, const QuantityName* qn);
		static void efficient_minimize_filter_split(const ConfigurationSet* cs, const QuantityName* qn, const QuantityValue& pivot, ConfigurationSet* *csl, ConfigurationSet* *csh);


	};


}

#endif
