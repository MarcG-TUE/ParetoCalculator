#pragma once

#include "calculator.h"

using namespace System;
using namespace System::Collections;


using namespace Pareto;


namespace Pareto {



	public class StatusCollector : public StatusCallback {

	private:

		StatusCollector() {};

		std::vector<std::string> stats;
		std::vector<std::string> verbs;
	public:

		virtual void setStatus(const std::string& s) {
			this->stats.push_back(s);
		};

		virtual void verbose(const std::string& s) {
			this->verbs.push_back(s);
		};
	};

}