#pragma once

#include "calculator.h"

using namespace System;
using namespace System::Collections;

public ref class ParetoCalculatorW
{
public:
	ParetoCalculatorW();

	/// load an XML file 'fn' into memory
	void LoadFile(String^ fn);

	/// load the quantity types specified in the loaded XML file into memory
	void LoadQuantityTypes();

	/// load the configuration spaces specified in the loaded XML file into memory
	void LoadConfigurationSpaces();

	/// load the configuration sets specified in the loaded XML file into memory
	void LoadConfigurationSets();

	/// not yet implemented in the current version, sorry
	void LoadOperations();

	/// push an object named s onto the calculator stack
	void push(String^ s);

	// new wrapper functions 
	ArrayList^ getStackItemStrings();
	ArrayList^ getMemoryItemStrings();
	int stackSize();
	String^ pop();
	String^ peek();
	bool stackEmpty();
	void storePop(String^ name);

private:
	Pareto::ParetoCalculator* pc;
};

