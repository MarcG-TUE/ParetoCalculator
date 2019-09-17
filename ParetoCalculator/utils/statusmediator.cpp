#include "statusmediator.h"

#include "MainForm.h"

namespace Pareto {
	namespace Calculator {

		StatusMediator::StatusMediator( MainForm^ const forForm){
			myForm = forForm;
		}

		void StatusMediator::setStatus(const std::string& s){
			myForm->setStatus(s);
		}

		void StatusMediator::verbose(const std::string& s){
			myForm->verbose(s);
		}

	}
}