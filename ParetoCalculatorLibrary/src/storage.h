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
// Some infrastructure to store objects on a stack and in an 'associative memory'
// (dictionary).
//

#ifndef PARETO_STORAGE_H
#define PARETO_STORAGE_H

#include <string>
#include <vector>
#include <map>
#include <memory>


namespace Pareto {

	/// StorableObject is an abstract superclass of anything we want to 
	/// put on a stack or in the memory of the calculator.
	///
	class StorableObject {
	public:
		StorableObject(std::string n);
		virtual ~StorableObject(){};
		
		/// name of the object
        std::string name;

		virtual void streamOn(std::ostream& os) const;
        std::string& asString(void) const;

		// Run-time type checking. The price we pay...
		
		/// test whether the object is a configuration set
		virtual bool isConfigurationSet(void) const = 0;

		/// test whether the object is a configuration space
		virtual bool isConfigurationSpace(void) const = 0;

		/// test whether the object is a quantity type
		virtual bool isQuantityType(void) const = 0;

		/// test whether the object is a string
		virtual bool isString(void) const = 0;

		/// copy the object
		virtual StorableObject& copy(void) const = 0;
	};

	typedef std::shared_ptr<const StorableObject> StorableObjectPtr;

	class StorageMap : public std::map<const std::string,const StorableObject*>{};

	/// A string that can be stored in calculator storage
	class StorableString : public StorableObject {
	public:
		StorableString(std::string n): StorableObject(n){};
		virtual void streamOn(std::ostream& os) const { os << name ;}
        std::string& asString(void){return name;}
		virtual bool isConfigurationSet(void) const {return false;}
		virtual bool isConfigurationSpace(void) const {return false;}
		virtual bool isQuantityType(void) const {return false;}
		virtual bool isString(void) const {return true;}
		virtual StorableObject& copy(void) const;
	};


    std::ostream& operator<<(std::ostream& os, const StorableObject& o);

	/// A stack of StorableObjects
	class StackOfStorageObjects: public std::vector<std::shared_ptr<const StorableObject>>{
	public:
		StorableObjectPtr pop(void);
		void push(StorableObjectPtr o);
		StorableObjectPtr peek(void);
		void duplicate(void);
	};
}

#endif
