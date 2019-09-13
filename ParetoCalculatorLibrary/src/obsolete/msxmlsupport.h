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
// Some stuff to be able to use Microsoft's MSXML parser
//

#if !defined(MSXMLIncluded)
#define MSXMLIncluded

#include <TCHAR.H>
#include <stdio.h>
#include <time.h>

#import "msxml4.dll"

#include <msxml2.h>

#include <comutil.h>
#include <winreg.h>

#define TEMP_SIZE  _MAX_PATH               // size of short buffer
static _TCHAR   szTemp[TEMP_SIZE];         // multipurpose buffer on stack
static DWORD    dwLen;                     // buffer size

using namespace MSXML2;

inline void EVAL_HR( HRESULT _hr ) 
   { if FAILED(_hr) throw(_hr); }




#endif // !UtilsIncluded
