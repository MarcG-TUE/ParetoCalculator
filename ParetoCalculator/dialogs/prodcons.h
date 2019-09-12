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
		/// Summary for prodcons
		///
		/// WARNING: If you change the name of this class, you will need to change the
		///          'Resource File Name' property for the managed resource compiler tool
		///          associated with all .resx files this class depends on.  Otherwise,
		///          the designers will not be able to interact properly with localized
		///          resources associated with this form.
		/// </summary>
		public ref class prodcons : public System::Windows::Forms::Form
		{
		public:
			prodcons(ConfigurationSet* cs)
			{
				InitializeComponent();
				//
				//TODO: Add the constructor code here
				//
				QuantityIntMap& qm = cs->confspace->quantityNames;
				QuantityIntMap::iterator i;
				for(i = qm.begin(); i!=qm.end(); i++){
					const std::string ns = (*i).first;
					producerList->Items->Add(gcnew System::String(ns.c_str()));
					consumerList->Items->Add(gcnew System::String(ns.c_str()));
				}
			}

		protected:
			/// <summary>
			/// Clean up any resources being used.
			/// </summary>
			~prodcons()
			{
				if (components)
				{
					delete components;
				}
			}
		private: System::Windows::Forms::Label^  label1;
		public:  System::Windows::Forms::ComboBox^  producerList;
		protected: 

		private: System::Windows::Forms::Label^  label2;
		public:  System::Windows::Forms::ComboBox^  consumerList;
		private: System::Windows::Forms::Button^  OK;
		private: System::Windows::Forms::Button^  Cancel;




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
				this->producerList = (gcnew System::Windows::Forms::ComboBox());
				this->label2 = (gcnew System::Windows::Forms::Label());
				this->consumerList = (gcnew System::Windows::Forms::ComboBox());
				this->OK = (gcnew System::Windows::Forms::Button());
				this->Cancel = (gcnew System::Windows::Forms::Button());
				this->SuspendLayout();
				// 
				// label1
				// 
				this->label1->AutoSize = true;
				this->label1->Location = System::Drawing::Point(35, 35);
				this->label1->Name = L"label1";
				this->label1->Size = System::Drawing::Size(92, 13);
				this->label1->TabIndex = 0;
				this->label1->Text = L"Producer Quantity";
				// 
				// producerList
				// 
				this->producerList->FormattingEnabled = true;
				this->producerList->Location = System::Drawing::Point(38, 51);
				this->producerList->Name = L"producerList";
				this->producerList->Size = System::Drawing::Size(329, 21);
				this->producerList->TabIndex = 1;
				// 
				// label2
				// 
				this->label2->AutoSize = true;
				this->label2->Location = System::Drawing::Point(35, 108);
				this->label2->Name = L"label2";
				this->label2->Size = System::Drawing::Size(96, 13);
				this->label2->TabIndex = 2;
				this->label2->Text = L"Consumer Quantity";
				// 
				// consumerList
				// 
				this->consumerList->FormattingEnabled = true;
				this->consumerList->Location = System::Drawing::Point(38, 136);
				this->consumerList->Name = L"consumerList";
				this->consumerList->Size = System::Drawing::Size(329, 21);
				this->consumerList->TabIndex = 3;
				// 
				// OK
				// 
				this->OK->DialogResult = System::Windows::Forms::DialogResult::OK;
				this->OK->Location = System::Drawing::Point(38, 181);
				this->OK->Name = L"OK";
				this->OK->Size = System::Drawing::Size(75, 23);
				this->OK->TabIndex = 4;
				this->OK->Text = L"OK";
				this->OK->UseVisualStyleBackColor = true;
				// 
				// Cancel
				// 
				this->Cancel->DialogResult = System::Windows::Forms::DialogResult::Cancel;
				this->Cancel->Location = System::Drawing::Point(194, 181);
				this->Cancel->Name = L"Cancel";
				this->Cancel->Size = System::Drawing::Size(75, 23);
				this->Cancel->TabIndex = 5;
				this->Cancel->Text = L"Cancel";
				this->Cancel->UseVisualStyleBackColor = true;
				// 
				// prodcons
				// 
				this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
				this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
				this->ClientSize = System::Drawing::Size(470, 220);
				this->Controls->Add(this->Cancel);
				this->Controls->Add(this->OK);
				this->Controls->Add(this->consumerList);
				this->Controls->Add(this->label2);
				this->Controls->Add(this->producerList);
				this->Controls->Add(this->label1);
				this->Name = L"prodcons";
				this->Text = L"prodcons";
				this->ResumeLayout(false);
				this->PerformLayout();

			}
#pragma endregion
		};
	}
}
