#ifndef STATUSMEDIATOR_H
#define STATUSMEDIATOR_H

#include "calculator.h"

#include <vcclr.h>
#using <mscorlib.dll>
#using <System.Xml.dll>

using namespace Pareto;

namespace Pareto {
	namespace Calculator {

		ref class MainForm;

		class StatusMediator : public StatusCallback {
			gcroot<MainForm^> myForm;
		public:
			StatusMediator( MainForm^ const forForm);	
			virtual void setStatus(const std::string& s);
			virtual void verbose(const std::string& s);
		};

	}
}
#endif