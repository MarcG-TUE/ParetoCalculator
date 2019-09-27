#ifndef EXCELAUTOMATION_H
#define EXCELAUTOMATION_H

#include "configuration.h"
#include "quantity.h"

// On some machines / version of Visual Studio the following 3 lines
// need to be commented out; on others they should not. Don't know why...
using namespace Microsoft::Office;
using namespace Microsoft::Office::Interop;
using namespace Microsoft::Office::Interop::Excel;

using namespace Pareto;

public ref class ExcelAutomation{
private: 
		Excel::Application^	excelApp;
		Excel::Workbook^	excelWorkbook;
		Excel::Sheets^		excelSheets;
		Excel::Worksheet^	excelWorksheet;

		const ConfigurationSet* confset;

public:
	ExcelAutomation();

	void createTable(const ConfigurationSet* cs, std::string& qx, std::string& qy, std::string& qc, std::string& qs);
	void plotGraph();

private:
	void addConfiguration(Pareto::Configuration* c);
	void putNumberInSheet(int x, int y, std::string& v);

private:

	void startExcel();

	// The following methods are a workaround for a bug in Excel 
	// (see Microsoft knowledge base article 320369)
	// where certain Excel interactions fail if the language
	// version of Office does noet match that of the OS.
    void preExcelInteraction();
    void postExcelInteraction();
	System::Globalization::CultureInfo^ oldCI;


};

#endif