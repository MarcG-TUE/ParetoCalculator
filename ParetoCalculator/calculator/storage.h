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
// Some infrastructure to store objects on a stack and in an 'associative memory'
// (dictionary).
//

#ifndef PARETO_STORAGE_H
#define PARETO_STORAGE_H

#include <string>
#include <vector>
#include <map>

using namespace std;

namespace Pareto {

	/// StorableObject is an abstract superclass of anything we want to 
	/// put on a stack or in the memory of the calculator.
	///
	class StorableObject {
	public:
		StorableObject(string n);
		virtual ~StorableObject(){};
		
		/// name of the object
		string name;

		virtual const void streamOn(ostream& os);
		string& asString(void);

		// Run-time type checking. The price we pay...
		
		/// test whether the object is a configuration set
		virtual bool isConfigurationSet(void) = 0;

		/// test whether the object is a configuration space
		virtual bool isConfigurationSpace(void) = 0;

		/// test whether the object is a quantity type
		virtual bool isQuantityType(void) = 0;

		/// test whether the object is a string
		virtual bool isString(void) = 0;
	};

	class StorageMap : public map<string,StorableObject*>{};

	/// A string that can be stored in calculator storage
	class StorableString : public StorableObject {
	public:
		StorableString(string n): StorableObject(n){};
		virtual const void streamOn(ostream& os){ os << name ;}
		string& asString(void){return name;}
		virtual bool isConfigurationSet(void) {return false;}
		virtual bool isConfigurationSpace(void) {return false;}
		virtual bool isQuantityType(void) {return false;}
		virtual bool isString(void) {return true;}
	};


	ostream& operator<<(ostream& os, StorableObject& o);

	/// A stack of StorableObjects
	class StackOfStorageObjects: public vector<StorableObject*>{
	public:
		StorableObject* pop(void);
		void push(StorableObject* o);
		StorableObject* peek(void);
		void duplicate(void);
	};
}

#endif
