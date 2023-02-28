#include <thread>

#include "StatusCollectorW.h"
#include "ParetoCalculatorExceptionW.h"



StatusCollector::StatusCollector() {
}

void StatusCollector::setStatus(const std::string& s) {
	if (!this->get_lock()) {
		throw gcnew ParetoCalculatorExceptionW("Could not acquire lock.");
		return;
	};

	this->stats.push_back(s);

	if (!this->release_lock()) {
		throw gcnew ParetoCalculatorExceptionW("Lock unexpectedly not taken.");
	};
}

void StatusCollector::verbose(const std::string& s) {

	if (!this->get_lock()) {
		throw gcnew ParetoCalculatorExceptionW("Could not acquire lock.");
		return;
	};

	this->verbs.push_back(s);

	if (!this->release_lock()) {
		throw gcnew ParetoCalculatorExceptionW("Lock unexpectedly not taken.");
	};
}

bool StatusCollector::get_lock(void)
{
	// TODO: make a proper timeout, or better: a proper mutex
	// (problem is that I couldn't find a mutex solution that is compatible with mixed managed / unmanaged code)
	uint32_t timeoutcnt = 0;
	while (timeoutcnt++ < 10000 && this->lock) {
		std::this_thread::sleep_for(std::chrono::milliseconds(60));
	}
	if (this->lock) {
		// lock still taken, timeout has occurred
		return false;
	}
	this->lock = true;

	return true;
}

bool StatusCollector::release_lock(void)
{
	if (!this->lock) {
		return false;
	}
	this->lock = false;

	return true;
}