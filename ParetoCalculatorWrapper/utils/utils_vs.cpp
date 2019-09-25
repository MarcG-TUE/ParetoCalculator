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
// Visual Studio specific utility functions
//

#include "utils_vs.h"

// Strings

//std::string wstring_to_string(std::wstring& wstr) {
//	std::string str;
//	size_t len = wstr.length();
//	char* psz = new char[len + 1];
//	wcstombs_s(&len, psz, len+1, wstr.c_str(),len + 1);
//	str = psz;
//	delete [] psz;
//	return str;
//}

bool To_string( String^ source, std::string &target ) {
	pin_ptr<const wchar_t> wch = PtrToStringChars( source );
	int len = (( source->Length+1) * 2);
	char *ch = new char[ len ];
	size_t nr;
	bool result = wcstombs_s(&nr,ch,len,wch,len) != -1;
	target = ch;
	delete[] ch;
	return result;
}

bool To_wstring(String^ source, std::wstring& target) {
	pin_ptr<const wchar_t> wch = PtrToStringChars(source);
	target = wch;
	return true;
}


std::string& system_to_std_string(String^ source) {
	std::string* s = new std::string;
	To_string(source, *s);
	return *s;
}

std::wstring& system_to_std_wstring(String^ source) {
	std::wstring* s = new std::wstring;
	To_wstring(source, *s);
	return *s;
}

String^ std_to_system_string(std::string& source) {
	return gcnew String(source.c_str());
}
