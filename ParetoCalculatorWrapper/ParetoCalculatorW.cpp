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

void ParetoCalculatorW::push(String^ s)
{
	this->pc->push(system_to_std_string(s));
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

ArrayList^ ParetoCalculatorW::getMemoryItemStrings()
{
	ArrayList^ res = gcnew ArrayList();
	StorageMap::iterator i;
	for (i = this->pc->memory.begin(); i != this->pc->memory.end(); i++)
	{
		res->Add(gcnew System::String((*i).first.c_str()));
	}
	return res;
}

int ParetoCalculatorW::stackSize()
{
	return this->pc->stack.size();
}

String^ ParetoCalculatorW::pop()
{
	StorableObject* so = this->pc->pop();
	return gcnew System::String(so->asString().c_str());
}

String^ ParetoCalculatorW::peek()
{
	StorableObject* so = this->pc->peek();
	return gcnew System::String(so->asString().c_str());
}

bool ParetoCalculatorW::stackEmpty()
{
	return this->pc->stack.empty();
}

void ParetoCalculatorW::storePop(String^ name)
{
	StorableObject* so = this->pc->pop();
	so->name = system_to_std_string(name);
	this->pc->store(*so);
}
