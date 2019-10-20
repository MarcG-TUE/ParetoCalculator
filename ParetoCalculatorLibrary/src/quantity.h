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
// Implements the various kinds of quantity types and values
//

#ifndef QUANTITY_H
#define QUANTITY_H

#include <vector>
#include <string>
#include <iostream>

#include "storage.h"


namespace Pareto {

	/// name of a quantity type
	typedef std::string QuantityName ;
	typedef std::shared_ptr<QuantityName> QuantityNamePtr;

	class QuantityValue;
	typedef std::shared_ptr<QuantityValue> QuantityValuePtr;

	/// abstract super class of all quantity types
	class QuantityType: public StorableObject {
	public:
		/// Constructor of a quantity type with name 'n'
		QuantityType(std::string n);

		/// compare (<=) two quantity values of this quantity type.
		virtual bool compare(const QuantityValue& q1, const QuantityValue& q2) const;

		/// test if two values of this quantity type are equal
		virtual bool equal(const QuantityValue& q1, const QuantityValue& q2) const;

		// Implements an arbitrary total order on the quantity values of this type
		virtual bool totalOrderSmaller(const QuantityValue& v1, const QuantityValue& v2) const;

		/// Stream a string representation of this quantity type to the stream 'os'
		virtual void streamOn(std::ostream& os) const;

		/// returns a default value for this quantity type
		virtual QuantityValuePtr defaultValue(void) const;

		/// converts a string into a value of this quantity type.
		virtual QuantityValuePtr valueFromString(const std::string s) const;

		/// returns a string representation of this quantity type.
		virtual const std::string& asString(){return this->name;}

		// Run-time type checking
		virtual bool isConfigurationSet(void) const { return false;}
		virtual bool isConfigurationSpace(void) const { return false;}
		virtual bool isString(void) const { return false;}
		virtual bool isQuantityType(void) const { return true;}

		/// test whether this quantity type is an enumerated type
		virtual bool isQuantityTypeEnum(void) const = 0;
		/// test whether this quantity type is totally ordered
		virtual bool isTotallyOrdered(void) const = 0;
		/// test whether this quantity type is unordered
		virtual bool isUnordered(void) const = 0;

		// XML support
		virtual std::string* xmlTypeString(void) = 0;
	};

	/// Pointer to QuantityType
	typedef std::shared_ptr<const QuantityType> QuantityTypePtr;

	/// QuantityValues represent the data in individual quantities of configurations
	///
	/// A quantity value has a quantity type ('qtype'). Subclasses implement values for specific kinds of quantity types.
	///
	class QuantityValue{

	public:
		/// Constructr of a quantity value of type 't'	
		QuantityValue(const QuantityType& t);

		virtual ~QuantityValue(){};

		/// Stream a string representation of the quantiy value on 'os'
		virtual void streamOn(std::ostream& os) const;

		/// return a string representation of the quantity value
		std::string asString(void) const;

		/// The quantity type of this quantity value
		const QuantityType& qtype;

		// an arbitrary total order on values
		bool totalOrderSmaller(const QuantityValue& v) const {
			return qtype.totalOrderSmaller(*this, v);
		}
	};


	class QuantityValue_Enum;
	typedef std::shared_ptr<QuantityValue_Enum> QuantityValue_EnumPtr;


	class ListOfQuantityValueEnum : public std::vector<QuantityValue_EnumPtr> {};

	/// Enumerated quantity type. The values in this type are assumed to be totally ordered.
	class QuantityType_Enum: public QuantityType {
	public:
		/// an STL vector of names of the values
		std::vector<std::string> names;
		///instances of each of the values of this type
		ListOfQuantityValueEnum quantities;

		/// Constructor 
		QuantityType_Enum(std::string n);

		/// add a value to the type, with name 's'
		virtual void addQuantity(std::string s);

		/// stream a string representation of the quantity type to 'os'
		virtual void streamOn(std::ostream& os) const;

		/// return a default value for this type
		virtual QuantityValuePtr defaultValue(void) const;

		/// create a value object from a string representation
		virtual QuantityValuePtr valueFromString(const std::string s) const;

		/// compare two values of this type (<=)
		virtual bool compare(const QuantityValue& q1, const QuantityValue& q2) const;

		/// test whether two values of this type are equal
		virtual bool equal(const QuantityValue& q1, const QuantityValue& q2) const;

		// implements an arbitrary total order on values of this type
		virtual bool totalOrderSmaller(const QuantityValue& v1, const QuantityValue& v2) const;

		// Run-time type checking
		virtual bool isQuantityTypeEnum(void) const {return true;}
		virtual bool isTotallyOrdered(void) const {return true;}
		virtual bool isUnordered(void) const {return false;}

		// copying
		virtual StorableObjectPtr copy(void) const;

		// XML support
		virtual std::string* xmlTypeString(void) {return new std::string("enum");}
	};


	/// An enumerated quantity value from any (ordered or unordered) enumerated type
	class QuantityValue_Enum: public QuantityValue{
	public:
		/// Constructor, provide enumerated type and string representation of the desired value
		QuantityValue_Enum(QuantityType& t, std::string v);

		/// stream a string representation of the value to 'os'
		virtual void streamOn(std::ostream& os) const;

		/// obtain an index of this values within the list of values of its type
		virtual unsigned int index(void) const;

		/// a string representing the enumerated value
		std::string value;
	};


	/// Enumerated quantity type. The values in this type are assumed to be unordered.
	class QuantityType_Enum_Unordered: public QuantityType_Enum {
	public:
		/// Constructor 
		QuantityType_Enum_Unordered(std::string n);
		virtual bool compare(const QuantityValue& q1, const QuantityValue& q2) const;
		virtual bool isTotallyOrdered(void) const {return false;}
		virtual bool isUnordered(void) const {return true;}

		// copying
		virtual StorableObjectPtr copy(void) const;

		// XML support
		virtual std::string* xmlTypeString(void) {return new std::string("unordered");}
	};


	/// A quantity type with integer values
	class QuantityType_Integer: public QuantityType {
	public:
		/// Constructor. name 'n'
		QuantityType_Integer(std::string n);

		/// stream a string representation of the type to 'os'
		virtual void streamOn(std::ostream& os) const;

		/// return some default value of this type
		virtual QuantityValue& defaultValue(void);

		/// create a value of this type from a string
		virtual QuantityValuePtr valueFromString(const std::string s) const;

		virtual bool compare(const QuantityValue& q1, const QuantityValue& q2) const;
		virtual bool equal(const QuantityValue& q1, const QuantityValue& q2) const;
		virtual bool totalOrderSmaller(const QuantityValue& v1, const QuantityValue& v2) const;

		// Run-time type checking
		virtual bool isQuantityTypeEnum(void) const {return false;}
		virtual bool isTotallyOrdered(void) const {return true;}
		virtual bool isUnordered(void) const {return false;}

		// copying
		virtual StorableObjectPtr copy(void) const;

		// XML support
		virtual std::string* xmlTypeString(void) {return new std::string("integer");}
	};


	/// A quantity value of an integer quantity type
	class QuantityValue_Integer: public QuantityValue{
	public:
		/// Constructor. Quantity type 't' and integer value 'n'
		QuantityValue_Integer(const QuantityType& t, int n);

		/// stream a string representation of the value to 'os'
		virtual void streamOn(std::ostream& os) const;

		/// the integer value
		int value;
	};

	/// A quantity type with real values
	class QuantityType_Real: public QuantityType {
	public:

		/// Constructor. name 'n'
		QuantityType_Real(std::string n);

		/// stream a string representation of the type to 'os'
		virtual void streamOn(std::ostream& os) const;

		/// return some default value of this type
		virtual QuantityValue& defaultValue(void);

		/// create a value of this type from a string
		virtual QuantityValuePtr valueFromString(const std::string s) const;

		virtual bool compare(const QuantityValue& q1, const QuantityValue& q2) const;
		virtual bool equal(const QuantityValue& q1, const QuantityValue& q2) const;
		virtual bool totalOrderSmaller(const QuantityValue& v1, const QuantityValue& v2) const;

		// Run-time type checking
		virtual bool isQuantityTypeEnum(void) const {return false;}
		virtual bool isTotallyOrdered(void) const {return true;}
		virtual bool isUnordered(void) const {return false;}

		// copying
		virtual StorableObjectPtr copy(void) const;

		// XML support
		virtual std::string* xmlTypeString(void) {return new std::string("real");}
	};


	/// A quantity value of an real quantity type
	class QuantityValue_Real: public QuantityValue{
	public:

		/// Constructor. Quantity type 't' and integer value 'r'
		QuantityValue_Real(const QuantityType& t, double r);

		/// stream a string representation of the value to 'os'
		virtual void streamOn(std::ostream& os) const;

		/// the real value
		double value;
	};

	typedef std::shared_ptr<QuantityValue_Real> QuantityValue_RealPtr;

    std::ostream& operator<<(std::ostream& os, const QuantityValue& v);
	std::ostream& operator<<(std::ostream& os, const QuantityValuePtr v);

	bool operator<=(const QuantityValue& q1, const QuantityValue& q2);
	bool operator==(const QuantityValue& q1, const QuantityValue& q2);
	bool operator<(const QuantityValue& q1, const QuantityValue& q2);

	bool operator==(const QuantityType& t1, const QuantityType& t2);
	bool operator!=(const QuantityType& t1, const QuantityType& t2);
}

#endif
