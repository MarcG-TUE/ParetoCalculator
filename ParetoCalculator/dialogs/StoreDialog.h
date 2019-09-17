#pragma once

#include <string>
#include "utils/utils_vs.h"

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;


namespace Pareto {
	namespace Calculator {

		/// <summary>
		/// Summary for StoreDialog
		///
		/// WARNING: If you change the name of this class, you will need to change the
		///          'Resource File Name' property for the managed resource compiler tool
		///          associated with all .resx files this class depends on.  Otherwise,
		///          the designers will not be able to interact properly with localized
		///          resources associated with this form.
		/// </summary>
		public ref class StoreDialog : public System::Windows::Forms::Form
		{
		public:
			StoreDialog(std::string& suggestedName)
			{
				InitializeComponent();
				//
				//TODO: Add the constructor code here
				//
				nameTextBox->Text = gcnew System::String(suggestedName.c_str());
			}

			std::string& getResult(void){return system_to_std_string(nameTextBox->Text);}

		protected:
			/// <summary>
			/// Clean up any resources being used.
			/// </summary>
			~StoreDialog()
			{
				if (components)
				{
					delete components;
				}
			}
		private: System::Windows::Forms::Button^  button1;
		protected: 
		private: System::Windows::Forms::Button^  button2;
		private: System::Windows::Forms::Label^  label1;
		private: System::Windows::Forms::TextBox^  nameTextBox;



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
				this->button1 = (gcnew System::Windows::Forms::Button());
				this->button2 = (gcnew System::Windows::Forms::Button());
				this->label1 = (gcnew System::Windows::Forms::Label());
				this->nameTextBox = (gcnew System::Windows::Forms::TextBox());
				this->SuspendLayout();
				// 
				// button1
				// 
				this->button1->DialogResult = System::Windows::Forms::DialogResult::OK;
				this->button1->Location = System::Drawing::Point(27, 79);
				this->button1->Name = L"button1";
				this->button1->Size = System::Drawing::Size(75, 23);
				this->button1->TabIndex = 0;
				this->button1->Text = L"Store";
				this->button1->UseVisualStyleBackColor = true;
				// 
				// button2
				// 
				this->button2->DialogResult = System::Windows::Forms::DialogResult::Cancel;
				this->button2->Location = System::Drawing::Point(108, 79);
				this->button2->Name = L"button2";
				this->button2->Size = System::Drawing::Size(75, 23);
				this->button2->TabIndex = 1;
				this->button2->Text = L"Cancel";
				this->button2->UseVisualStyleBackColor = true;
				// 
				// label1
				// 
				this->label1->AutoSize = true;
				this->label1->Location = System::Drawing::Point(24, 22);
				this->label1->Name = L"label1";
				this->label1->Size = System::Drawing::Size(105, 13);
				this->label1->TabIndex = 2;
				this->label1->Text = L"Name for new object";
				// 
				// nameTextBox
				// 
				this->nameTextBox->Location = System::Drawing::Point(27, 38);
				this->nameTextBox->Name = L"nameTextBox";
				this->nameTextBox->Size = System::Drawing::Size(300, 20);
				this->nameTextBox->TabIndex = 4;
				// 
				// StoreDialog
				// 
				this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
				this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
				this->ClientSize = System::Drawing::Size(355, 141);
				this->Controls->Add(this->nameTextBox);
				this->Controls->Add(this->label1);
				this->Controls->Add(this->button2);
				this->Controls->Add(this->button1);
				this->Name = L"StoreDialog";
				this->Text = L"Store Object";
				this->ResumeLayout(false);
				this->PerformLayout();

			}
#pragma endregion
		};
	}}
