#pragma once

#include <calculator.h>

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
		/// Summary for SelectionDialog
		///
		/// WARNING: If you change the name of this class, you will need to change the
		///          'Resource File Name' property for the managed resource compiler tool
		///          associated with all .resx files this class depends on.  Otherwise,
		///          the designers will not be able to interact properly with localized
		///          resources associated with this form.
		/// </summary>
		public ref class SelectionDialog : public System::Windows::Forms::Form
		{
		public:
			SelectionDialog(ParetoCalculator* c)
			{
				InitializeComponent();
				//
				//TODO: Add the constructor code here
				//
				if(c){
					StorageMap::iterator i;
					for(i=c->memory.begin(); i!=c->memory.end(); i++){
						this->listBox1->Items->Add(gcnew System::String((*i).first.c_str()));
					}
				}
			}


		protected:
			/// <summary>
			/// Clean up any resources being used.
			/// </summary>
			~SelectionDialog()
			{
				if (components)
				{
					delete components;
				}
			}
		private: System::Windows::Forms::Label^  label1;
		public: System::Windows::Forms::ListBox^  listBox1;
		private: System::Windows::Forms::Button^  OKButton;
		private: System::Windows::Forms::Button^  CancelButton;
		protected: 

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
				this->label1 = (gcnew System::Windows::Forms::Label());
				this->listBox1 = (gcnew System::Windows::Forms::ListBox());
				this->OKButton = (gcnew System::Windows::Forms::Button());
				this->CancelButton = (gcnew System::Windows::Forms::Button());
				this->SuspendLayout();
				// 
				// label1
				// 
				this->label1->AutoSize = true;
				this->label1->Location = System::Drawing::Point(32, 13);
				this->label1->Name = L"label1";
				this->label1->Size = System::Drawing::Size(138, 13);
				this->label1->TabIndex = 0;
				this->label1->Text = L"Select Object for Inspection";
				// 
				// listBox1
				// 
				this->listBox1->FormattingEnabled = true;
				this->listBox1->Location = System::Drawing::Point(35, 48);
				this->listBox1->Name = L"listBox1";
				this->listBox1->Size = System::Drawing::Size(225, 186);
				this->listBox1->TabIndex = 1;
				// 
				// OKButton
				// 
				this->OKButton->DialogResult = System::Windows::Forms::DialogResult::OK;
				this->OKButton->Location = System::Drawing::Point(35, 259);
				this->OKButton->Name = L"OKButton";
				this->OKButton->Size = System::Drawing::Size(75, 23);
				this->OKButton->TabIndex = 2;
				this->OKButton->Text = L"OK";
				this->OKButton->UseVisualStyleBackColor = true;
				// 
				// CancelButton
				// 
				this->CancelButton->DialogResult = System::Windows::Forms::DialogResult::Cancel;
				this->CancelButton->Location = System::Drawing::Point(184, 258);
				this->CancelButton->Name = L"CancelButton";
				this->CancelButton->Size = System::Drawing::Size(75, 23);
				this->CancelButton->TabIndex = 3;
				this->CancelButton->Text = L"Cancel";
				this->CancelButton->UseVisualStyleBackColor = true;
				// 
				// SelectionDialog
				// 
				this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
				this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
				this->ClientSize = System::Drawing::Size(301, 303);
				this->Controls->Add(this->CancelButton);
				this->Controls->Add(this->OKButton);
				this->Controls->Add(this->listBox1);
				this->Controls->Add(this->label1);
				this->Name = L"SelectionDialog";
				this->Text = L"SelectionDialog";
				this->ResumeLayout(false);
				this->PerformLayout();

			}
#pragma endregion
		};
	}
}
