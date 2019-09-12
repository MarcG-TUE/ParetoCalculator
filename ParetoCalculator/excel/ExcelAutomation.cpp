#include "ExcelAutomation.h"
#include <strstream>


ExcelAutomation::ExcelAutomation(void){
	this->startExcel();
}

void ExcelAutomation::startExcel(){
	this->excelApp = gcnew Excel::ApplicationClass();   
	
	// Make Excel Visible
	this->excelApp->Visible = true;
    }

void ExcelAutomation::preExcelInteraction(){
	oldCI = System::Threading::Thread::CurrentThread->CurrentCulture;
	System::Threading::Thread::CurrentThread->CurrentCulture = 
		gcnew System::Globalization::CultureInfo("en-US");
}

void ExcelAutomation::postExcelInteraction(){
	System::Threading::Thread::CurrentThread->CurrentCulture = oldCI;
}


void ExcelAutomation::createTable(ConfigurationSet* cs, std::string& qx, std::string& qy, std::string& qc, std::string& qs){
	System::Object^ missing = System::Type::Missing;

	confset = cs;

	this->preExcelInteraction();
	this->excelWorkbook = this->excelApp->Workbooks->Add(System::Type::Missing);
	this->postExcelInteraction();

	this->excelSheets = this->excelWorkbook->Worksheets;
	this->excelWorksheet = safe_cast<Excel::Worksheet^>(this->excelWorkbook->ActiveSheet);

	int qix, qiy, qic, qis;
	ConfigurationSpace* csp = cs->confspace;
	if(csp->includesQuantityNamed(qx)){
		qix = cs->confspace->indexOfQuantity(qx);
	} else {qix = -1;}
	if(csp->includesQuantityNamed(qy)){
		qiy = cs->confspace->indexOfQuantity(qy);
	} else {qiy = -1;}
	if(csp->includesQuantityNamed(qc)){
		qic = cs->confspace->indexOfQuantity(qc);
	} else {qic = -1;}
	if(csp->includesQuantityNamed(qs)){
		qis = cs->confspace->indexOfQuantity(qs);
	} else {qis = -1;}

	SetOfConfigurations::iterator k;
	unsigned int i=0;
	for(k=cs->confs.begin(); k!=cs->confs.end(); k++, i++){
		const Pareto::Configuration& c = *k; 
		const QuantityValue& vx = c.getQuantity(qix);
		const QuantityValue& vy = c.getQuantity(qiy);

        std::ostrstream vxString;
		vx.streamOn(vxString);
		vxString << std::ends;
		this->putNumberInSheet(i+1,1,std::string(vxString.str()));

        std::ostrstream vyString;
		vy.streamOn(vyString);
		vyString << std::ends;
		this->putNumberInSheet(i+1,2,std::string(vyString.str()));
	}
}

void ExcelAutomation::addConfiguration(Pareto::Configuration* c){

}

void ExcelAutomation::putNumberInSheet(int x, int y, std::string& v){
	this->preExcelInteraction();
	this->excelWorksheet->Cells->Item[x,y]=gcnew System::String(v.c_str());
	this->postExcelInteraction();
}


void ExcelAutomation::plotGraph(){
	this->preExcelInteraction();
	this->excelWorkbook->Charts->Add(System::Type::Missing,System::Type::Missing,System::Type::Missing,System::Type::Missing);
	this->excelWorkbook->ActiveChart->Location(Excel::XlChartLocation::xlLocationAsNewSheet, "Pareto Points");
	this->postExcelInteraction();

	Excel::Chart^ ac = this->excelWorkbook->ActiveChart;

	ac->ChartType=Excel::XlChartType::xlXYScatter;
    std::ostrstream rnString;
	rnString << "A" << 1 << ":B" << confset->confs.size() << std::ends;

	Excel::Range^ rn  = this->excelWorksheet->Range::get( gcnew System::String(rnString.str()), System::Type::Missing);
	ac->SetSourceData(rn,Excel::XlRowCol::xlColumns);
}
