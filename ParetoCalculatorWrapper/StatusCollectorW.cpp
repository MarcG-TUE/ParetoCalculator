#include <mutex>

#include "StatusCollectorW.h"



StatusCollector::StatusCollector() {
}

void StatusCollector::setStatus(const std::string& s) {
	this->stats.push_back(s);
}

void StatusCollector::verbose(const std::string& s) {
	this->verbs.push_back(s);
}
