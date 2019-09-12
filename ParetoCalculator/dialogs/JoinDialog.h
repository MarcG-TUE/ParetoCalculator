#pragma once

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
		/// Summary for JoinDialog
		///
		/// WARNING: If you change the name of this class, you will need to change the
		///          'Resource File Name' property for the managed resource compiler tool
		///          associated with all .resx files this class depends on.  Otherwise,
		///          the designers will not be able to interact properly with localized
		///          resources associated with this form.
		/// </summary>
		public ref class JoinDialog : public System::Windows::Forms::Form
		{
		public:
			JoinDialog(ConfigurationSet* cs)
			{
				InitializeComponent();
				//
				//TODO: Add the constructor code here
				//
                std::vector<QuantityType*> qtl = cs->confspace->quantities;
                std::vector<QuantityType*>::iterator i;
				for(i = qtl.begin(); i!=qtl.end(); i++){
					quantitiesList->Items->Add(gcnew System::String((*i)->name.c_str()));
				}
			}

		protected:
			/// <summary>
			/// Clean up any resources being used.
			/// </summary>
			~JoinDialog()
			{
				if (components)
				{
					delete components;
				}
			}
		private: System::Windows::Forms::Label^  label1;
		public: System::Windows::Forms::ComboBox^  quantitiesList;
		private: System::Windows::Forms::Button^  button1;
		private: System::Windows::Forms::Button^  button2;
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
				this->quantitiesList = (gcnew System::Windows::Forms::ComboBox());
				this->button1 = (gcnew System::Windows::Forms::Button());
				this->button2 = (gcnew System::Windows::Forms::Button());
				this->SuspendLayout();
				// 
				// label1
				// 
				this->label1->AutoSize = true;
				this->label1->Location = System::Drawing::Point(22, 18);
				this->label1->Name = L"label1";
				this->label1->Size = System::Drawing::Size(108, 13);
				this->label1->TabIndex = 0;
				this->label1->Text = L"Select quantity to join";
				// 
				// quantitiesList
				// 
				this->quantitiesList->FormattingEnabled = true;
				this->quantitiesList->Location = System::Drawing::Point(25, 34);
				this->quantitiesList->Name = L"quantitiesList";
				this->quantitiesList->Size = System::Drawing::Size(347, 21);
				this->quantitiesList->TabIndex = 1;
				// 
				// button1
				// 
				this->button1->DialogResult = System::Windows::Forms::DialogResult::OK;
				this->button1->Location = System::Drawing::Point(25, 61);
				this->button1->Name = L"button1";
				this->button1->Size = System::Drawing::Size(75, 23);
				this->button1->TabIndex = 2;
				this->button1->Text = L"OK";
				this->button1->UseVisualStyleBackColor = true;
				// 
				// button2
				// 
				this->button2->DialogResult = System::Windows::Forms::DialogResult::Cancel;
				this->button2->Location = System::Drawing::Point(297, 61);
				this->button2->Name = L"button2";
				this->button2->Size = System::Drawing::Size(75, 23);
				this->button2->TabIndex = 3;
				this->button2->Text = L"Cancel";
				this->button2->UseVisualStyleBackColor = true;
				// 
				// JoinDialog
				// 
				this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
				this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
				this->ClientSize = System::Drawing::Size(406, 119);
				this->Controls->Add(this->button2);
				this->Controls->Add(this->button1);
				this->Controls->Add(this->quantitiesList);
				this->Controls->Add(this->label1);
				this->Name = L"JoinDialog";
				this->Text = L"Join";
				this->ResumeLayout(false);
				this->PerformLayout();

			}
#pragma endregion
		};
	}
}
