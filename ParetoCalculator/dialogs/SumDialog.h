#pragma once

#include "configuration.h"

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
		/// Summary for SumDialog
		///
		/// WARNING: If you change the name of this class, you will need to change the
		///          'Resource File Name' property for the managed resource compiler tool
		///          associated with all .resx files this class depends on.  Otherwise,
		///          the designers will not be able to interact properly with localized
		///          resources associated with this form.
		/// </summary>
		public ref class SumDialog : public System::Windows::Forms::Form
		{
		public:
			SumDialog(const ConfigurationSet* cs, System::String^ windowTitle)
			{
				InitializeComponent();
				//
				this->Text = windowTitle;

                std::vector<const QuantityType*> qtl = cs->confspace->quantities;
                std::vector<const QuantityType*>::iterator i;
				for(i = qtl.begin(); i!=qtl.end(); i++){
					quantABox->Items->Add(gcnew System::String((*i)->name.c_str()));
					quantBBox->Items->Add(gcnew System::String((*i)->name.c_str()));
				}
			}

		protected:
			/// <summary>
			/// Clean up any resources being used.
			/// </summary>
			~SumDialog()
			{
				if (components)
				{
					delete components;
				}
			}
		public: System::Windows::Forms::ComboBox^  quantABox;
		public: System::Windows::Forms::ComboBox^  quantBBox;
		private: System::Windows::Forms::Label^  label1;
		private: System::Windows::Forms::Label^  label2;
		private: System::Windows::Forms::Button^  button1;
		private: System::Windows::Forms::Button^  button2;

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
				this->quantABox = (gcnew System::Windows::Forms::ComboBox());
				this->quantBBox = (gcnew System::Windows::Forms::ComboBox());
				this->label1 = (gcnew System::Windows::Forms::Label());
				this->label2 = (gcnew System::Windows::Forms::Label());
				this->button1 = (gcnew System::Windows::Forms::Button());
				this->button2 = (gcnew System::Windows::Forms::Button());
				this->SuspendLayout();
				// 
				// quantABox
				// 
				this->quantABox->FormattingEnabled = true;
				this->quantABox->Location = System::Drawing::Point(44, 34);
				this->quantABox->Name = L"quantABox";
				this->quantABox->Size = System::Drawing::Size(342, 21);
				this->quantABox->TabIndex = 0;
				// 
				// quantBBox
				// 
				this->quantBBox->FormattingEnabled = true;
				this->quantBBox->Location = System::Drawing::Point(44, 85);
				this->quantBBox->Name = L"quantBBox";
				this->quantBBox->Size = System::Drawing::Size(342, 21);
				this->quantBBox->TabIndex = 1;
				// 
				// label1
				// 
				this->label1->AutoSize = true;
				this->label1->Location = System::Drawing::Point(41, 18);
				this->label1->Name = L"label1";
				this->label1->Size = System::Drawing::Size(56, 13);
				this->label1->TabIndex = 2;
				this->label1->Text = L"Quantity A";
				// 
				// label2
				// 
				this->label2->AutoSize = true;
				this->label2->Location = System::Drawing::Point(41, 69);
				this->label2->Name = L"label2";
				this->label2->Size = System::Drawing::Size(56, 13);
				this->label2->TabIndex = 3;
				this->label2->Text = L"Quantity B";
				// 
				// button1
				// 
				this->button1->DialogResult = System::Windows::Forms::DialogResult::OK;
				this->button1->Location = System::Drawing::Point(44, 130);
				this->button1->Name = L"button1";
				this->button1->Size = System::Drawing::Size(75, 23);
				this->button1->TabIndex = 4;
				this->button1->Text = L"OK";
				this->button1->UseVisualStyleBackColor = true;
				// 
				// button2
				// 
				this->button2->DialogResult = System::Windows::Forms::DialogResult::Cancel;
				this->button2->Location = System::Drawing::Point(311, 130);
				this->button2->Name = L"button2";
				this->button2->Size = System::Drawing::Size(75, 23);
				this->button2->TabIndex = 5;
				this->button2->Text = L"Cancel";
				this->button2->UseVisualStyleBackColor = true;
				// 
				// SumDialog
				// 
				this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
				this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
				this->ClientSize = System::Drawing::Size(442, 192);
				this->Controls->Add(this->button2);
				this->Controls->Add(this->button1);
				this->Controls->Add(this->label2);
				this->Controls->Add(this->label1);
				this->Controls->Add(this->quantBBox);
				this->Controls->Add(this->quantABox);
				this->Name = L"SumDialog";
				this->Text = L"window title";
				this->ResumeLayout(false);
				this->PerformLayout();

			}
#pragma endregion
		};
	}}
