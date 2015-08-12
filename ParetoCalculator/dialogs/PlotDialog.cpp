#include "../stdafx.h"
#include "../calculator/utils_vs.h"
#include "PlotDialog.h"

using namespace Pareto::Calculator;

System::Void PlotDialog::Plot_Click(System::Object^  sender, System::EventArgs^  e) {
	ExcelAutomation^ ea = gcnew ExcelAutomation();
	String^ sx = safe_cast<System::String^>(this->xAxisBox->SelectedItem);
	if(!sx){sx = gcnew System::String("");}
	String^ sy = safe_cast<System::String^>(this->yAxisBox->SelectedItem);
	if(!sy){sy = gcnew System::String("");}
	String^ sc = safe_cast<System::String^>(this->colorBox->SelectedItem);
	if(!sc){sc = gcnew System::String("");}
	String^ ss = safe_cast<System::String^>(this->sizeBox->SelectedItem);
	if(!ss){ss = gcnew System::String("");}
	ea->createTable(confset, system_to_std_string(sx), system_to_std_string(sy), system_to_std_string(sc), system_to_std_string(ss));
	ea->plotGraph();
}
