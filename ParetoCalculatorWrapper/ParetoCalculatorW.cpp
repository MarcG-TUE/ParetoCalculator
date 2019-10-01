#include "ParetoCalculatorW.h"
#include "ParetoCalculatorExceptionW.h"
#include "exceptions.h"

#include "utils/utils_vs.h"

using namespace Pareto;
using namespace System::Collections;
using namespace System::Threading;

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

void ParetoCalculatorW::duplicate()
{
	this->pc->duplicate();
}

void ParetoCalculatorW::product()
{
	this->pc->product();
}

void ParetoCalculatorW::minimize()
{
	this->pc->minimize();
}

ArrayList^ ParetoCalculatorW::getStackItemStrings()
{
	ArrayList^ res = gcnew ArrayList();
	std::vector<const StorableObject*> stack = this->pc->stack;
	std::vector<const StorableObject*>::iterator i;
	for (i = stack.begin(); i != stack.end(); i++)
	{
		res->Insert(0, gcnew System::String((*i)->asString().c_str()));
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
	const StorableObject* so = this->pc->pop();
	return gcnew System::String(so->asString().c_str());
}

String^ ParetoCalculatorW::peek()
{
	const StorableObject* so = this->pc->peek();
	return gcnew System::String(so->asString().c_str());
}

String^ ParetoCalculatorW::peek(int n)
{
	const StorableObject* so = this->pc->stack[n];
	return gcnew System::String(so->asString().c_str());
}

bool ParetoCalculatorW::stackEmpty()
{
	return this->pc->stack.empty();
}

void ParetoCalculatorW::storePop(String^ name)
{
	StorableObject* so = &this->pc->pop()->copy();
	so->name = system_to_std_string(name);
	this->pc->store(*so);
}

void ParetoCalculatorW::executeProdCons(String^ pq, String^ cq)
{
	POperation_ProdCons& pco = *new POperation_ProdCons(system_to_std_string(pq), system_to_std_string(cq));
	pco.executeOn(*this->pc);
}

void ParetoCalculatorW::executeAbstract(String^ qn)
{
	ListOfQuantityNames lqn;
	lqn.push_back(system_to_std_string(qn));
	POperation_Abstract& ao = *new POperation_Abstract(&lqn);
	ao.executeOn(*this->pc);
}

void ParetoCalculatorW::executeJoin(String^ qn)
{
	JoinMap jm;
	std::string q = system_to_std_string(qn);
	jm[q] = q;
	POperation_EfficientJoin& jo = *new POperation_EfficientJoin(&jm);
	jo.executeOn(*this->pc);
}

void ParetoCalculatorW::executeSum(String^ qa, String^ qb)
{
	POperation_Sum& so = *new POperation_Sum(system_to_std_string(qa), system_to_std_string(qb));
	so.executeOn(*this->pc);
	delete& so;
}

void ParetoCalculatorW::executeMin(String^ qa, String^ qb)
{
	POperation_Min& so = *new POperation_Min(system_to_std_string(qa), system_to_std_string(qb));
	so.executeOn(*this->pc);
	delete& so;
}

void ParetoCalculatorW::executeMax(String^ qa, String^ qb)
{
	POperation_Max& so = *new POperation_Max(system_to_std_string(qa), system_to_std_string(qb));
	so.executeOn(*this->pc);
	delete& so;
}

void ParetoCalculatorW::executeMultiply(String^ qa, String^ qb)
{
	POperation_Max& so = *new POperation_Max(system_to_std_string(qa), system_to_std_string(qb));
	so.executeOn(*this->pc);
	delete& so;
}


ArrayList^ ParetoCalculatorW::confsetConfspaceQuantityNames()
{
	ArrayList^ res = gcnew ArrayList();
	const ConfigurationSet* cs = dynamic_cast<const ConfigurationSet*>(this->pc->peek());
	const QuantityIntMap& qm = cs->confspace->quantityNames;
	QuantityIntMap::const_iterator i;
	for (i = qm.begin(); i != qm.end(); i++) {
		const std::string ns = (*i).first;
		res->Add(gcnew System::String(ns.c_str()));
	}
	return res;
}

void ParetoCalculatorW::StatusChecker(System::Object^ o)
{
	return;
	if (this->sc != NULL) {
		while (! this->sc->stats.empty()) {
			(this->cbstat)(std_to_system_string(*this->sc->stats.begin()));
			this->sc->stats.erase(this->sc->stats.begin());
		}
		while (!this->sc->verbs.empty()) {
			(this->cbverb)(std_to_system_string(*this->sc->verbs.begin()));
			this->sc->verbs.erase(this->sc->verbs.begin());
		}
	}
}

void ParetoCalculatorW::setStatusCallbacks(PCSetStatus^ s, PCVerbose^ v, int pollingTimeMs)
{
	this->cbstat = s;
	this->cbverb = v;
	this->sc = new StatusCollector();
	this->pc->setStatusCallbackObject(this->sc);
	TimerCallback^ tcb = gcnew TimerCallback(this, &ParetoCalculatorW::StatusChecker);
	this->callbackTimer = gcnew Timer(tcb, NULL, 5* pollingTimeMs, pollingTimeMs);

}
