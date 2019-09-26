#include "ParetoCalculatorW.h"
#include "ParetoCalculatorExceptionW.h"
#include "exceptions.h"

#include "utils/utils_vs.h"

using namespace Pareto;
using namespace System::Collections;

ParetoCalculatorW::ParetoCalculatorW() {
	this->pc = new ParetoCalculator();
}

void ParetoCalculatorW::LoadFile(String^ fn)
{
	try {
		this->pc->LoadFile(system_to_std_wstring(fn));
	}
	catch (EParetoCalculatorError e) {
		throw gcnew ParetoCalculatorExceptionW(std_to_system_string(e.errorMsg));
	}
}

void ParetoCalculatorW::LoadQuantityTypes()
{
	this->pc->LoadQuantityTypes();
}

void ParetoCalculatorW::LoadConfigurationSpaces()
{
	this->pc->LoadConfigurationSpaces();
}

void ParetoCalculatorW::LoadConfigurationSets()
{
	this->pc->LoadConfigurationSets();
}

void ParetoCalculatorW::LoadOperations()
{
	this->pc->LoadOperations();
}

ArrayList^ ParetoCalculatorW::getStackItemStrings()
{
	ArrayList^ res = gcnew ArrayList();
	std::vector<StorableObject*> stack = this->pc->stack;
	std::vector<StorableObject*>::iterator i;
	for (i = stack.begin(); i != stack.end(); i++)
	{
		res->Add(gcnew System::String((*i)->asString().c_str()));
	}
	return res;
}
