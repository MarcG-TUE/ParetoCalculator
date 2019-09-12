#pragma once

#include "../excel/ExcelAutomation.h"
#include "../calculator/configuration.h"

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;

using namespace Pareto;

namespace Pareto {
	namespace Calculator {

		/// <summary>
		/// Summary for PlotDialog
		///
		/// WARNING: If you change the name of this class, you will need to change the
		///          'Resource File Name' property for the managed resource compiler tool
		///          associated with all .resx files this class depends on.  Otherwise,
		///          the designers will not be able to interact properly with localized
		///          resources associated with this form.
		/// </summary>
		public ref class PlotDialog : public System::Windows::Forms::Form
		{
		private:
			ConfigurationSet* confset;
		public:
			PlotDialog(ConfigurationSet* cs)
			{
				InitializeComponent();
				//
				//TODO: Add the constructor code here
				//
				confset = cs;

				QuantityIntMap qm = cs->confspace->quantityNames;
				QuantityIntMap::iterator i;
				for(i = qm.begin(); i!=qm.end(); i++){
					const std::string s = (*i).first;
					this->xAxisBox->Items->Add(gcnew System::String(s.c_str()));
					this->yAxisBox->Items->Add(gcnew System::String(s.c_str()));
					this->colorBox->Items->Add(gcnew System::String(s.c_str()));
					this->sizeBox->Items->Add(gcnew System::String(s.c_str()));
				}
			}

		protected:
			/// <summary>
			/// Clean up any resources being used.
			/// </summary>
			~PlotDialog()
			{
				if (components)
				{
					delete components;
				}
			}
		private: System::Windows::Forms::Button^  Plot;
		protected: 
		private: System::Windows::Forms::Button^  CancelButton;
		private: System::Windows::Forms::ComboBox^  xAxisBox;
		private: System::Windows::Forms::ComboBox^  yAxisBox;
		private: System::Windows::Forms::ComboBox^  colorBox;



		private: System::Windows::Forms::Label^  label1;
		private: System::Windows::Forms::Label^  label2;
		private: System::Windows::Forms::Label^  label3;
		private: System::Windows::Forms::ComboBox^  sizeBox;

		private: System::Windows::Forms::Label^  label4;

		private:
			/// <summary>
			/// Required designer variable.
			/// </summary>
			System::ComponentModel::Container ^components;

#pragma region Windows Form Designer generated code
			/// <summary>
			/// Required method for Designer support - do not modify
			/// the contents of this method with the code editor.
			/// </summary>
			void InitializeComponent(void)
			{
				this->Plot = (gcnew System::Windows::Forms::Button());
				this->CancelButton = (gcnew System::Windows::Forms::Button());
				this->xAxisBox = (gcnew System::Windows::Forms::ComboBox());
				this->yAxisBox = (gcnew System::Windows::Forms::ComboBox());
				this->colorBox = (gcnew System::Windows::Forms::ComboBox());
				this->label1 = (gcnew System::Windows::Forms::Label());
				this->label2 = (gcnew System::Windows::Forms::Label());
				this->label3 = (gcnew System::Windows::Forms::Label());
				this->sizeBox = (gcnew System::Windows::Forms::ComboBox());
				this->label4 = (gcnew System::Windows::Forms::Label());
				this->SuspendLayout();
				// 
				// Plot
				// 
				this->Plot->Location = System::Drawing::Point(63, 239);
				this->Plot->Name = L"Plot";
				this->Plot->Size = System::Drawing::Size(75, 23);
				this->Plot->TabIndex = 0;
				this->Plot->Text = L"Plot";
				this->Plot->UseVisualStyleBackColor = true;
				this->Plot->Click += gcnew System::EventHandler(this, &PlotDialog::Plot_Click);
				// 
				// CancelButton
				// 
				this->CancelButton->DialogResult = System::Windows::Forms::DialogResult::Cancel;
				this->CancelButton->Location = System::Drawing::Point(337, 239);
				this->CancelButton->Name = L"CancelButton";
				this->CancelButton->Size = System::Drawing::Size(75, 23);
				this->CancelButton->TabIndex = 1;
				this->CancelButton->Text = L"Cancel";
				this->CancelButton->UseVisualStyleBackColor = true;
				// 
				// xAxisBox
				// 
				this->xAxisBox->FormattingEnabled = true;
				this->xAxisBox->Location = System::Drawing::Point(63, 45);
				this->xAxisBox->Name = L"xAxisBox";
				this->xAxisBox->Size = System::Drawing::Size(349, 21);
				this->xAxisBox->TabIndex = 2;
				// 
				// yAxisBox
				// 
				this->yAxisBox->FormattingEnabled = true;
				this->yAxisBox->Location = System::Drawing::Point(63, 96);
				this->yAxisBox->Name = L"yAxisBox";
				this->yAxisBox->Size = System::Drawing::Size(349, 21);
				this->yAxisBox->TabIndex = 3;
				// 
				// colorBox
				// 
				this->colorBox->FormattingEnabled = true;
				this->colorBox->Location = System::Drawing::Point(63, 147);
				this->colorBox->Name = L"colorBox";
				this->colorBox->Size = System::Drawing::Size(349, 21);
				this->colorBox->TabIndex = 4;
				// 
				// label1
				// 
				this->label1->AutoSize = true;
				this->label1->Location = System::Drawing::Point(60, 29);
				this->label1->Name = L"label1";
				this->label1->Size = System::Drawing::Size(75, 13);
				this->label1->TabIndex = 5;
				this->label1->Text = L"X-axis quantity";
				// 
				// label2
				// 
				this->label2->AutoSize = true;
				this->label2->Location = System::Drawing::Point(60, 80);
				this->label2->Name = L"label2";
				this->label2->Size = System::Drawing::Size(75, 13);
				this->label2->TabIndex = 6;
				this->label2->Text = L"Y-axis quantity";
				// 
				// label3
				// 
				this->label3->AutoSize = true;
				this->label3->Location = System::Drawing::Point(60, 131);
				this->label3->Name = L"label3";
				this->label3->Size = System::Drawing::Size(71, 13);
				this->label3->TabIndex = 7;
				this->label3->Text = L"Color quantity";
				// 
				// sizeBox
				// 
				this->sizeBox->FormattingEnabled = true;
				this->sizeBox->Location = System::Drawing::Point(63, 198);
				this->sizeBox->Name = L"sizeBox";
				this->sizeBox->Size = System::Drawing::Size(349, 21);
				this->sizeBox->TabIndex = 8;
				// 
				// label4
				// 
				this->label4->AutoSize = true;
				this->label4->Location = System::Drawing::Point(63, 179);
				this->label4->Name = L"label4";
				this->label4->Size = System::Drawing::Size(67, 13);
				this->label4->TabIndex = 9;
				this->label4->Text = L"Size quantity";
				// 
				// PlotDialog
				// 
				this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
				this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
				this->ClientSize = System::Drawing::Size(487, 290);
				this->Controls->Add(this->label4);
				this->Controls->Add(this->sizeBox);
				this->Controls->Add(this->label3);
				this->Controls->Add(this->label2);
				this->Controls->Add(this->label1);
				this->Controls->Add(this->colorBox);
				this->Controls->Add(this->yAxisBox);
				this->Controls->Add(this->xAxisBox);
				this->Controls->Add(this->CancelButton);
				this->Controls->Add(this->Plot);
				this->Name = L"PlotDialog";
				this->Text = L"Make a plot";
				this->ResumeLayout(false);
				this->PerformLayout();

			}
#pragma endregion
		private: System::Void Plot_Click(System::Object^  sender, System::EventArgs^  e);};
	}
}