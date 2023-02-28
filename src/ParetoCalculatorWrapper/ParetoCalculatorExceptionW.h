#pragma once


using namespace System;

public ref class ParetoCalculatorExceptionW :
	public Exception
{

public:
		ParetoCalculatorExceptionW()
		{
		}

		explicit ParetoCalculatorExceptionW(String^ message)
			: Exception(message)
		{
		}

		ParetoCalculatorExceptionW(String^ message, Exception^ inner)
			: Exception(message, inner)
		{
		}
	
};

