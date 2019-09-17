#pragma once

#include <calculator.h>
#include "utils/utils_vs.h"

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
		/// Summary for SaveDialog
		///
		/// WARNING: If you change the name of this class, you will need to change the
		///          'Resource File Name' property for the managed resource compiler tool
		///          associated with all .resx files this class depends on.  Otherwise,
		///          the designers will not be able to interact properly with localized
		///          resources associated with this form.
		/// </summary>
		public ref class SaveDialog : public System::Windows::Forms::Form
		{
		public:
			SaveDialog(ParetoCalculator* c)
			{
				InitializeComponent();
				//
				//TODO: Add the constructor code here
				//
				paretocalculator = c;
				if(c){
					StorageMap::iterator i;
					for(i=c->memory.begin(); i!=c->memory.end(); i++){
						this->memListBox->Items->Add(gcnew System::String((*i).first.c_str()));
					}
				}
			}

		public:
			ParetoCalculator* paretocalculator;

		protected:
			/// <summary>
			/// Clean up any resources being used.
			/// </summary>
			~SaveDialog()
			{
				if (components)
				{
					delete components;
				}
			}
		private: System::Windows::Forms::Button^  cancelButton;
		protected: 

		private: System::Windows::Forms::Button^  saveButton;
		private: System::Windows::Forms::Button^  saveAllButton;
		private: System::Windows::Forms::ListBox^  memListBox;
		protected: 



		private: System::Windows::Forms::Label^  label1;

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
				this->cancelButton = (gcnew System::Windows::Forms::Button());
				this->saveButton = (gcnew System::Windows::Forms::Button());
				this->saveAllButton = (gcnew System::Windows::Forms::Button());
				this->memListBox = (gcnew System::Windows::Forms::ListBox());
				this->label1 = (gcnew System::Windows::Forms::Label());
				this->SuspendLayout();
				// 
				// cancelButton
				// 
				this->cancelButton->DialogResult = System::Windows::Forms::DialogResult::Cancel;
				this->cancelButton->Location = System::Drawing::Point(361, 243);
				this->cancelButton->Name = L"cancelButton";
				this->cancelButton->Size = System::Drawing::Size(75, 23);
				this->cancelButton->TabIndex = 0;
				this->cancelButton->Text = L"Cancel";
				this->cancelButton->UseVisualStyleBackColor = true;
				// 
				// saveButton
				// 
				this->saveButton->Location = System::Drawing::Point(361, 41);
				this->saveButton->Name = L"saveButton";
				this->saveButton->Size = System::Drawing::Size(75, 23);
				this->saveButton->TabIndex = 1;
				this->saveButton->Text = L"Save";
				this->saveButton->UseVisualStyleBackColor = true;
				this->saveButton->Click += gcnew System::EventHandler(this, &SaveDialog::saveButton_Click);
				// 
				// saveAllButton
				// 
				this->saveAllButton->Location = System::Drawing::Point(361, 214);
				this->saveAllButton->Name = L"saveAllButton";
				this->saveAllButton->Size = System::Drawing::Size(75, 23);
				this->saveAllButton->TabIndex = 2;
				this->saveAllButton->Text = L"Save All";
				this->saveAllButton->UseVisualStyleBackColor = true;
				this->saveAllButton->Click += gcnew System::EventHandler(this, &SaveDialog::saveAllButton_Click);
				// 
				// memListBox
				// 
				this->memListBox->FormattingEnabled = true;
				this->memListBox->Location = System::Drawing::Point(29, 41);
				this->memListBox->Name = L"memListBox";
				this->memListBox->SelectionMode = System::Windows::Forms::SelectionMode::MultiExtended;
				this->memListBox->Size = System::Drawing::Size(303, 225);
				this->memListBox->TabIndex = 3;
				// 
				// label1
				// 
				this->label1->AutoSize = true;
				this->label1->Location = System::Drawing::Point(29, 22);
				this->label1->Name = L"label1";
				this->label1->Size = System::Drawing::Size(58, 13);
				this->label1->TabIndex = 4;
				this->label1->Text = L"Save Item:";
				// 
				// SaveDialog
				// 
				this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
				this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
				this->ClientSize = System::Drawing::Size(470, 301);
				this->Controls->Add(this->label1);
				this->Controls->Add(this->memListBox);
				this->Controls->Add(this->saveAllButton);
				this->Controls->Add(this->saveButton);
				this->Controls->Add(this->cancelButton);
				this->Name = L"SaveDialog";
				this->Text = L"Save Model";
				this->ResumeLayout(false);
				this->PerformLayout();

			}
#pragma endregion
		private: System::Void saveAllButton_Click(System::Object^  sender, System::EventArgs^  e) {
					 paretocalculator->SaveAllFile(L"test.xml");
				 }
		private: System::Void saveButton_Click(System::Object^  sender, System::EventArgs^  e) {
					 // ToDo: check if anything is selected
					 paretocalculator->SaveItemFile(system_to_std_string(memListBox->Text), L"test.xml");
				 }
		};
	}
}
