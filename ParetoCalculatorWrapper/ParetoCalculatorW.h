#pragma once

#include "calculator.h"
#include "StatusCollectorW.h"

using namespace System;
using namespace System::Collections;
using namespace System::Threading;

public delegate void PCSetStatus(String^ s);
public delegate void PCVerbose(String^ s);

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

	void duplicate();

	void product();

	void minimize();

	// new wrapper functions 
	ArrayList^ getStackItemStrings();
	ArrayList^ getMemoryItemStrings();
	int stackSize();
	String^ pop();
	String^ peek();
	String^ peek(int n);
	bool stackEmpty();
	void storePop(String^ name);
	void executeProdCons(String^ pq, String^ cq);
	void executeAbstract(String^ qn);
	void executeJoin(String^ qn);
	void executeSum(String^ qa, String^ qb);
	void executeMin(String^ qa, String^ qb);
	void executeMax(String^ qa, String^ qb);
	void executeMultiply(String^ qa, String^ qb);
	ArrayList^ getScattterPoints(String^ qh, String^ qv);


	ArrayList^ confsetConfspaceQuantityNames();
	void StatusChecker(System::Object^ o);

	void setStatusCallbacks(PCSetStatus^ s, PCVerbose^ v, int pollingTimeMs);

private:
	ParetoCalculator* pc;
	StatusCollector* sc;
	PCSetStatus^ cbstat;
	PCVerbose^ cbverb;
	Timer^ callbackTimer;
};

