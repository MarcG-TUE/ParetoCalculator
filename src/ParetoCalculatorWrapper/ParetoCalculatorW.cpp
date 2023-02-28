#include "ParetoCalculatorW.h"
#include "ParetoCalculatorExceptionW.h"
#include "exceptions.h"

#include "utils/utils_vs.h"
#include <sstream>

using namespace Pareto;
using namespace System::Collections;
using namespace System::Threading;

ParetoCalculatorW::ParetoCalculatorW() {
	this->pc = new ParetoCalculator();
}

void ParetoCalculatorW::LoadFile(String^ fn)
{
	try {
		this->pc->LoadFile(system_to_std_string(fn));
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
	std::vector<StorableObjectPtr>& stack = this->pc->stack;
	std::vector<StorableObjectPtr>::iterator i;
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
	return (int) this->pc->stack.size();
}

String^ ParetoCalculatorW::pop()
{
	StorableObjectPtr so = this->pc->pop();
	return gcnew System::String(so->asString().c_str());
}

String^ ParetoCalculatorW::peek()
{
	StorableObjectPtr so = this->pc->peek();
	return gcnew System::String(so->asString().c_str());
}

String^ ParetoCalculatorW::peek(int n)
{
	StorableObjectPtr so = this->pc->stack[n];
	return gcnew System::String(so->asString().c_str());
}

bool ParetoCalculatorW::stackEmpty()
{
	return this->pc->stack.empty();
}

void ParetoCalculatorW::storePop(String^ name)
{
	StorableObjectPtr so = this->pc->pop()->copy();
	so->name = system_to_std_string(name);
	this->pc->store(so);
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
	POperation_Abstract ao(lqn);
	ao.executeOn(*this->pc);
}

void ParetoCalculatorW::executeJoin(String^ qn)
{
	JoinMap jm;
	std::string q = system_to_std_string(qn);
	jm[q] = q;
	POperation_EfficientJoin jo(jm);
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


ArrayList^ ParetoCalculatorW::getScattterPoints(String^ qxs, String^ qys)
{
	int qix, qiy;
	std::string qx = system_to_std_string(qxs);
	std::string qy = system_to_std_string(qys);
	ConfigurationSetPtr cs = std::dynamic_pointer_cast<ConfigurationSet> (this->pc->peek());
	ConfigurationSpacePtr csp = cs->confspace;
	if (csp->includesQuantityNamed(qx)) {
		qix = cs->confspace->indexOfQuantity(qx);
	}
	else { qix = -1; }
	if (csp->includesQuantityNamed(qy)) {
		qiy = cs->confspace->indexOfQuantity(qy);
	}
	else { qiy = -1; }

	ArrayList^ result = gcnew ArrayList();
	SetOfConfigurations::iterator k;
	unsigned int i = 0;
	for (k = cs->confs.begin(); k != cs->confs.end(); k++, i++) {
		const Pareto::Configuration& c = *k;
		QuantityValuePtr vx = c.getQuantity(qix);
		QuantityValuePtr vy = c.getQuantity(qiy);

		ArrayList^ point = gcnew ArrayList();
		std::ostringstream vxString;
		vx->streamOn(vxString);
		vxString << std::ends;
		point->Add(std_to_system_string(std::string(vxString.str())));

		std::ostringstream vyString;
		vy->streamOn(vyString);
		vyString << std::ends;
		point->Add(std_to_system_string(std::string(vyString.str())));

		result->Add(point);
	}
	return result;
}


ArrayList^ ParetoCalculatorW::confsetConfspaceQuantityNames()
{
	ArrayList^ res = gcnew ArrayList();
	ConfigurationSetPtr cs = std::dynamic_pointer_cast<ConfigurationSet>(this->pc->peek());
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
	if (this->sc != NULL) {
		// acquire lock
		if (!this->sc->get_lock()) {
			throw gcnew ParetoCalculatorExceptionW("Could not acquire lock.");
			return;
		};
		// get status messages
		while (! this->sc->stats.empty()) {
			String^ msg = std_to_system_string(this->sc->verbs.front());
			this->sc->stats.pop_front();
			this->sc->release_lock();
			(this->cbstat)(msg);
			this->sc->get_lock();
		}
		// get verbose log messages
		while (!this->sc->verbs.empty()) {
			String^ msg = std_to_system_string(this->sc->verbs.front());
			this->sc->verbs.pop_front();
			this->sc->release_lock();
			(this->cbverb)(msg);
			this->sc->get_lock();
		}
		// release lock
		if (!this->sc->release_lock()) {
			throw gcnew ParetoCalculatorExceptionW("Lock unexpectedly not not taken.");
		};

	}
}

void ParetoCalculatorW::setStatusCallbacks(PCSetStatus^ s, PCVerbose^ v, int pollingTimeMs)
{
	this->cbstat = s;
	this->cbverb = v;
	this->sc = new StatusCollector();
	this->pc->setStatusCallbackObject(this->sc);
	TimerCallback^ tcb = gcnew TimerCallback(this, &ParetoCalculatorW::StatusChecker);
	this->callbackTimer = gcnew Timer(tcb, nullptr, 5* pollingTimeMs, pollingTimeMs);

}
