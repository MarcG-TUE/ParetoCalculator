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
// Some infrastructure to store objects on a stack and in an 'associative memory'
// (dictionary).
//

#include <iostream>
#include <sstream>
#include "storage.h"

namespace Pareto {

	StorableObject::StorableObject(std::string n){
		name=n;
	}


	const void StorableObject::streamOn(std::ostream& os){
		
		os << "[" << this->name << "]";
	}

    std::string& StorableObject::asString(void) {
        std::ostringstream myString;
		this->streamOn(myString);
		return *new std::string(myString.str());
	}


    std::ostream& operator<<(std::ostream& os, StorableObject& o){
		o.streamOn(os);
		return os;
	}

	StorableObject* StackOfStorageObjects::pop(void){
		StorableObject* b = this->back();
		this->pop_back();
		return b;
	}

	void StackOfStorageObjects::push(StorableObject* o){
		this->push_back(o);
	}

	StorableObject* StackOfStorageObjects::peek(void){
		return this->back();
	}

	void StackOfStorageObjects::duplicate(void){
		this->push_back(this->back());
	}

};



