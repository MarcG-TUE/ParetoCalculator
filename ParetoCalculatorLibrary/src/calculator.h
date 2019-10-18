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
// Implementation of the ParetoCalculator class, implementing the Pareto Algebra operations
//

#ifndef PARETO_CALCULATOR_H
#define PARETO_CALCULATOR_H

#include <functional>
#include "operations.h"

namespace Pareto {

	class ParetoParser;

	/// An abstract superclass from which to derive objects to monitor
	/// information from the calculator about its activities.
	class StatusCallback {
	public:
		virtual ~StatusCallback(){};
		/// called by the calculator to indicate the status of its activities
		virtual void setStatus(const std::string& s) = 0;
		/// called by the calculator to generate verbose stream description of what it is doing.
		virtual void verbose(const std::string& s) = 0;
	};

	/// Type for a list of configuration sets
	typedef std::list<ConfigurationSetPtr> ListOfConfSet;
	typedef std::shared_ptr<ListOfConfSet> ListOfConfSetPtr;

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
		void store(const StorableObjectPtr o);

		/// store an StorableObject 'o'in memory under key
		void store(const StorableObject& o, const std::string& key);

		/// pop an object from the stack and store it in memory, filed under its name
		void store(void);

		/// pop an object from the stack and store it in memory, filed under 'key'
		void store(const std::string& key);

		/// retrieve an object from the memory 
		const StorableObject& retrieve(const std::string& oname); 

		/// retrieve a quantity type from the memory 
		const QuantityType& retrieveQuantityType(const std::string& oname); 

		/// push an object 'o' onto the calculator stack
		void push(const StorableObjectPtr o);

		/// retreive the object name 'oname' from memory and push it on the stack
		void push(const std::string& oname); 

		/// return an object popped from the stack
		StorableObjectPtr pop(); 

		/// pop a configuration set from the stack.
		/// Throws an exception if the object is not a configuration set
		ConfigurationSetPtr popConfigurationSet(); 

		/// pop a storable string from the stack.
		/// Throws an exception if the object is not a storable string
		StorableStringPtr popStorableString();

		/// return a pointer to the object on top of the stack without removing it.
		const StorableObject& peek(); 

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
		StatusCallback* statusObject = nullptr;

		/// Register a StatusCallBack object.
		void setStatusCallbackObject(StatusCallback& o);

		// called by the calculator when its status changes
		void setStatus(const std::string& newStatus);

		// called by the calculator to log its activities
		void verbose(const std::string& addText);

		//
		// static functions which can be used without an instance of the calculator
		//

		// compute the product of the two configuration sets
		static ConfigurationSetPtr product(ConfigurationSetPtr cs1, ConfigurationSetPtr cs2);

		// compute the product of the two configuration sets in an already defined predefined configuration space
		static ConfigurationSetPtr productInSpace(ConfigurationSetPtr cs1, ConfigurationSetPtr cs2, ConfigurationSpacePtr cspace);

		// abstract the n-th dimension from the configuration set
		static ConfigurationSetPtr abstraction(ConfigurationSetPtr cs, unsigned int n);

		// abstract a quantity named s, a 'StorableString'
		static ConfigurationSetPtr abstraction(const ConfigurationSetPtr cs, const StorableString& s);

		// abstract a quantity named qn, a QuantityName
		static ConfigurationSetPtr abstraction(ConfigurationSetPtr cs, const QuantityName& qn);

		// hide a collection of quantities, specified by the list of names lqn
		static ConfigurationSetPtr hiding(ConfigurationSetPtr cs, const ListOfQuantityNames& lqn);

		// hide a single quantity named qn
		static ConfigurationSetPtr hiding(const ConfigurationSetPtr cs, const QuantityName& qn);

		// compute the constraint (intersection) of two configuration sets
		static ConfigurationSetPtr constraint(ConfigurationSetPtr cs1, ConfigurationSetPtr cs2);
		static ConfigurationSetPtr constraint(ConfigurationSetPtr cs1, bool (*testConstraint)(const Pareto::Configuration&));
		static ConfigurationSetPtr alternative(ConfigurationSetPtr cs1, ConfigurationSetPtr cs2);
		static ConfigurationSetPtr minimize(ConfigurationSetPtr cs);
		static ConfigurationSetPtr minimize_SC(ConfigurationSetPtr cs);
		static const unsigned int MINIMIZE_THRESHOLD = 2048; // determine optimal value!
		static const unsigned int FILTER_THRESHOLD = 2048; // determine optimal value!
		static ConfigurationSetPtr efficient_minimize(ConfigurationSetPtr cs);

		/// execute block-based minimization
		void minimize(void);
		/// execute generalized multi-dimensional Divide-and-Conquer minimization
		void efficient_minimize(void);
		/// compute product
		void product(void);
		/// compute abstraction
		void abstract(void);
		/// hide quantities
		void hide(ListOfQuantityNames &lqn);

		/// apply a constraint.
		///
		/// The constraint is implemented as the characteristic function testConstraint
		/// which returns true when a configuration satisfies the constraint.
		void constraint(bool (*testConstraint)(const Configuration&));

		// parsing
		ParetoParser *parser = nullptr;
		
		/// call this function before using the parser function below.
		void initParser();

		/// load an XML file 'fn' into memory
		void LoadFile(const std::string& fn);

		/// load the quantity types specified in the loaded XML file into memory
		void LoadQuantityTypes();

		/// load the configuration spaces specified in the loaded XML file into memory
		void LoadConfigurationSpaces();

		/// load the configuration sets specified in the loaded XML file into memory
		void LoadConfigurationSets();

		/// load and execute operations
		void LoadOperations();

		void SaveAllFile(const std::string& fn);
		void SaveItemFile(const std::string& itemToSave, const std::string& fn);

	private:
		static ConfigurationSetPtr efficient_minimize_unordered(ConfigurationSetPtr cs, const QuantityName& qn);
		static ListOfConfSetPtr splitClasses(ConfigurationSetPtr cs, const QuantityName& qn);
		static ConfigurationSetPtr efficient_minimize_totally_ordered(ConfigurationSetPtr cs, const QuantityName& qn);
		static ConfigurationSetPtr efficient_minimize_totally_ordered_recursive(ConfigurationSetPtr cs, const QuantityName& qn);
		static ListOfConfSetPtr splitLowHigh(ConfigurationSetPtr cs, const QuantityName& qn, const QuantityValue **v);
		static ConfigurationSetPtr efficient_minimize_dcmerge(ConfigurationSetPtr csl, ConfigurationSetPtr csh, const QuantityName& qn, const QuantityValue& v);
		static ConfigurationSetPtr efficient_minimize_filter1(ConfigurationSetPtr csl, ConfigurationSetPtr csh, const QuantityName& qn);
		static ConfigurationSetPtr efficient_minimize_filter2(ConfigurationSetPtr csa, ConfigurationSetPtr csb);
		static ConfigurationSetPtr efficient_minimize_filter3(ConfigurationSetPtr csa, ConfigurationSetPtr csb);
		static ConfigurationSetPtr efficient_minimize_filter4(ConfigurationSetPtr csa, ConfigurationSetPtr csb);
		static const QuantityValue& efficient_minimize_getPivot(ConfigurationSetPtr cs, const QuantityName& qn);
		static void efficient_minimize_filter_split(ConfigurationSetPtr cs, const QuantityName& qn, const QuantityValue& pivot, ConfigurationSet* *csl, ConfigurationSet* *csh);


	};


}

#endif
