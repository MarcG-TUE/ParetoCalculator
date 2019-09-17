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
		/// Summary for AbstractionDialog
		///
		/// WARNING: If you change the name of this class, you will need to change the
		///          'Resource File Name' property for the managed resource compiler tool
		///          associated with all .resx files this class depends on.  Otherwise,
		///          the designers will not be able to interact properly with localized
		///          resources associated with this form.
		/// </summary>
		public ref class AbstractionDialog : public System::Windows::Forms::Form
		{
		public:
			AbstractionDialog(ConfigurationSet* cs)
			{
				InitializeComponent();
				//
				//TODO: Add the constructor code here
				//
				QuantityIntMap& qm = cs->confspace->quantityNames;
				QuantityIntMap::iterator i;
				for(i = qm.begin(); i!=qm.end(); i++){
					const std::string ns = (i->first);
					quantitiesBox->Items->Add(gcnew System::String(ns.c_str()));
				}
			}

		protected:
			/// <summary>
			/// Clean up any resources being used.
			/// </summary>
			~AbstractionDialog()
			{
				if (components)
				{
					delete components;
				}
			}
		public: System::Windows::Forms::ComboBox^  quantitiesBox;
		private: System::Windows::Forms::Label^  label1;
		private: System::Windows::Forms::Button^  OKButton;
		private: System::Windows::Forms::Button^  CancelButton;

		protected: 

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
				this->quantitiesBox = (gcnew System::Windows::Forms::ComboBox());
				this->label1 = (gcnew System::Windows::Forms::Label());
				this->OKButton = (gcnew System::Windows::Forms::Button());
				this->CancelButton = (gcnew System::Windows::Forms::Button());
				this->SuspendLayout();
				// 
				// quantitiesBox
				// 
				this->quantitiesBox->FormattingEnabled = true;
				this->quantitiesBox->Location = System::Drawing::Point(33, 49);
				this->quantitiesBox->Name = L"quantitiesBox";
				this->quantitiesBox->Size = System::Drawing::Size(285, 21);
				this->quantitiesBox->TabIndex = 0;
				// 
				// label1
				// 
				this->label1->AutoSize = true;
				this->label1->Location = System::Drawing::Point(30, 23);
				this->label1->Name = L"label1";
				this->label1->Size = System::Drawing::Size(130, 13);
				this->label1->TabIndex = 1;
				this->label1->Text = L"Select quantity to abstract";
				// 
				// OKButton
				// 
				this->OKButton->DialogResult = System::Windows::Forms::DialogResult::OK;
				this->OKButton->Location = System::Drawing::Point(33, 91);
				this->OKButton->Name = L"OKButton";
				this->OKButton->Size = System::Drawing::Size(75, 23);
				this->OKButton->TabIndex = 2;
				this->OKButton->Text = L"OK";
				this->OKButton->UseVisualStyleBackColor = true;
				// 
				// CancelButton
				// 
				this->CancelButton->DialogResult = System::Windows::Forms::DialogResult::Cancel;
				this->CancelButton->Location = System::Drawing::Point(242, 90);
				this->CancelButton->Name = L"CancelButton";
				this->CancelButton->Size = System::Drawing::Size(75, 23);
				this->CancelButton->TabIndex = 3;
				this->CancelButton->Text = L"Cancel";
				this->CancelButton->UseVisualStyleBackColor = true;
				// 
				// AbstractionDialog
				// 
				this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
				this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
				this->ClientSize = System::Drawing::Size(341, 152);
				this->Controls->Add(this->CancelButton);
				this->Controls->Add(this->OKButton);
				this->Controls->Add(this->label1);
				this->Controls->Add(this->quantitiesBox);
				this->Name = L"AbstractionDialog";
				this->Text = L"AbstractionDialog";
				this->ResumeLayout(false);
				this->PerformLayout();

			}
#pragma endregion
		};
	}
}
