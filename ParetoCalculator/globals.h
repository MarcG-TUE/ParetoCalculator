#pragma once

#ifndef PARETOCALCULATORGLOBALS_H
#define PARETOCALCULATORGLOBALS_H

#include <string>
#include "calculator/calculator.h"

using namespace System;
using namespace Pareto;

// the 'static's are just a hack to get rid of some linker errors
// try to solve later...
static ParetoCalculator *paretocalculator;

static void MarshalString ( String ^ s, wstring& os ) {
   using namespace Runtime::InteropServices;
   const wchar_t* chars = 
      (const wchar_t*)(Marshal::StringToHGlobalUni(s)).ToPointer();
   os = chars;
   Marshal::FreeHGlobal(IntPtr((void*)chars));
}

#endif 