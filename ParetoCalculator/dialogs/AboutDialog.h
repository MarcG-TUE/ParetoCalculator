#pragma once

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;


namespace Pareto {
	namespace Calculator {

		/// <summary>
		/// Summary for About
		///
		/// WARNING: If you change the name of this class, you will need to change the
		///          'Resource File Name' property for the managed resource compiler tool
		///          associated with all .resx files this class depends on.  Otherwise,
		///          the designers will not be able to interact properly with localized
		///          resources associated with this form.
		/// </summary>
		public ref class AboutDialog : public System::Windows::Forms::Form
		{
		public:
			AboutDialog(void)
			{
				InitializeComponent();
				//
				//TODO: Add the constructor code here
				//
			}

		protected:
			/// <summary>
			/// Clean up any resources being used.
			/// </summary>
			~AboutDialog()
			{
				if (components)
				{
					delete components;
				}
			}
		private: System::Windows::Forms::RichTextBox^  aboutTextBox;
		private: System::Windows::Forms::Button^  okButton;
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
				this->aboutTextBox = (gcnew System::Windows::Forms::RichTextBox());
				this->okButton = (gcnew System::Windows::Forms::Button());
				this->SuspendLayout();
				// 
				// aboutTextBox
				// 
				this->aboutTextBox->BackColor = System::Drawing::SystemColors::Window;
				this->aboutTextBox->Location = System::Drawing::Point(21, 22);
				this->aboutTextBox->Name = L"aboutTextBox";
				this->aboutTextBox->ReadOnly = true;
				this->aboutTextBox->Size = System::Drawing::Size(248, 176);
				this->aboutTextBox->TabIndex = 0;
				this->aboutTextBox->Text = L"Pareto Calculator\n(c)2007, Eindhoven University of Technology\ncontact: \nMarc Geil" 
					L"en\nm.c.w.geilen@tue.nl\n\nWWW: http://www.es.ele.tue.nl/pareto";
				// 
				// okButton
				// 
				this->okButton->DialogResult = System::Windows::Forms::DialogResult::OK;
				this->okButton->Location = System::Drawing::Point(21, 225);
				this->okButton->Name = L"okButton";
				this->okButton->Size = System::Drawing::Size(248, 23);
				this->okButton->TabIndex = 1;
				this->okButton->Text = L"O.K.";
				this->okButton->UseVisualStyleBackColor = true;
				// 
				// AboutDialog
				// 
				this->ClientSize = System::Drawing::Size(292, 266);
				this->Controls->Add(this->okButton);
				this->Controls->Add(this->aboutTextBox);
				this->Name = L"AboutDialog";
				this->ResumeLayout(false);

			}
#pragma endregion
		};
	}}
