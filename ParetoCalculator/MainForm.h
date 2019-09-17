#pragma once

#include <string>
#include "utils/globals.h"
#include "utils/statusmediator.h"
#include "utils/utils_vs.h"

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Windows;
using namespace System::Data;
using namespace System::Drawing;

using namespace Pareto;

namespace Pareto {
	namespace Calculator {

		//class StatusMediator;	


		/// <summary>
		/// Summary for MainForm
		///
		/// WARNING: If you change the name of this class, you will need to change the
		///          'Resource File Name' property for the managed resource compiler tool
		///          associated with all .resx files this class depends on.  Otherwise,
		///          the designers will not be able to interact properly with localized
		///          resources associated with this form.
		/// </summary>
		public ref class MainForm : public System::Windows::Forms::Form
		{
		private: StatusMediator* sm;
		public:
			MainForm(void)
			{
				InitializeComponent();
				//
				//TODO: Add the constructor code here
				//
				paretocalculator = new ParetoCalculator();
				sm = new StatusMediator(this); 
				paretocalculator->setStatusCallbackObject(sm);
			}

		protected:
			/// <summary>
			/// Clean up any resources being used.
			/// </summary>
			~MainForm()
			{
				if (components)
				{
					delete components;
				}
			}
		private: System::Windows::Forms::TextBox^  xmlBox;

		private: System::Windows::Forms::Button^  selectfileButton;


		private: System::Windows::Forms::Label^  label1;
		private: System::Windows::Forms::Label^  statusLabel;

		private: System::Windows::Forms::Button^  SelectButton;
		private: System::Windows::Forms::ListBox^  stackListBox;

		private: System::Windows::Forms::Label^  label2;
		private: System::Windows::Forms::Button^  printButton;
		private: System::Windows::Forms::Button^  executeButton;
		private: System::Windows::Forms::Button^  popButton;
		private: System::Windows::Forms::Button^  storeButton;
		private: System::Windows::Forms::Button^  productButton;
		private: System::Windows::Forms::Button^  prodconsButton;
		private: System::Windows::Forms::Button^  abstractionButton;
		private: System::Windows::Forms::Button^  JoinButton;
		private: System::Windows::Forms::Button^  MinimizeButton;
		private: System::Windows::Forms::Button^  PlotButton;
		private: System::Windows::Forms::Button^  DuplicateButton;
		private: System::Windows::Forms::Button^  SumButton;
		private: System::Windows::Forms::Button^  MaxButton;
		private: System::Windows::Forms::Button^  MinButton;
		private: System::Windows::Forms::Button^  MultiplyButton;
		private: System::Windows::Forms::Button^  saveButton;
		private: System::Windows::Forms::Button^  aboutButton;
		private: System::Windows::Forms::Button^  button1;


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
				System::ComponentModel::ComponentResourceManager^  resources = (gcnew System::ComponentModel::ComponentResourceManager(MainForm::typeid));
				this->xmlBox = (gcnew System::Windows::Forms::TextBox());
				this->selectfileButton = (gcnew System::Windows::Forms::Button());
				this->label1 = (gcnew System::Windows::Forms::Label());
				this->statusLabel = (gcnew System::Windows::Forms::Label());
				this->SelectButton = (gcnew System::Windows::Forms::Button());
				this->stackListBox = (gcnew System::Windows::Forms::ListBox());
				this->label2 = (gcnew System::Windows::Forms::Label());
				this->printButton = (gcnew System::Windows::Forms::Button());
				this->executeButton = (gcnew System::Windows::Forms::Button());
				this->popButton = (gcnew System::Windows::Forms::Button());
				this->storeButton = (gcnew System::Windows::Forms::Button());
				this->productButton = (gcnew System::Windows::Forms::Button());
				this->prodconsButton = (gcnew System::Windows::Forms::Button());
				this->abstractionButton = (gcnew System::Windows::Forms::Button());
				this->JoinButton = (gcnew System::Windows::Forms::Button());
				this->MinimizeButton = (gcnew System::Windows::Forms::Button());
				this->PlotButton = (gcnew System::Windows::Forms::Button());
				this->DuplicateButton = (gcnew System::Windows::Forms::Button());
				this->SumButton = (gcnew System::Windows::Forms::Button());
				this->MaxButton = (gcnew System::Windows::Forms::Button());
				this->MinButton = (gcnew System::Windows::Forms::Button());
				this->MultiplyButton = (gcnew System::Windows::Forms::Button());
				this->saveButton = (gcnew System::Windows::Forms::Button());
				this->aboutButton = (gcnew System::Windows::Forms::Button());
				this->button1 = (gcnew System::Windows::Forms::Button());
				this->SuspendLayout();
				// 
				// xmlBox
				// 
				this->xmlBox->Location = System::Drawing::Point(68, 56);
				this->xmlBox->Multiline = true;
				this->xmlBox->Name = L"xmlBox";
				this->xmlBox->ScrollBars = System::Windows::Forms::ScrollBars::Vertical;
				this->xmlBox->Size = System::Drawing::Size(405, 428);
				this->xmlBox->TabIndex = 0;
				// 
				// selectfileButton
				// 
				this->selectfileButton->Location = System::Drawing::Point(68, 27);
				this->selectfileButton->Name = L"selectfileButton";
				this->selectfileButton->Size = System::Drawing::Size(75, 23);
				this->selectfileButton->TabIndex = 2;
				this->selectfileButton->Text = L"Select File";
				this->selectfileButton->UseVisualStyleBackColor = true;
				this->selectfileButton->Click += gcnew System::EventHandler(this, &MainForm::selectfileButton_Click);
				// 
				// label1
				// 
				this->label1->AutoSize = true;
				this->label1->Location = System::Drawing::Point(65, 513);
				this->label1->Name = L"label1";
				this->label1->Size = System::Drawing::Size(37, 13);
				this->label1->TabIndex = 5;
				this->label1->Text = L"Status";
				// 
				// statusLabel
				// 
				this->statusLabel->AutoSize = true;
				this->statusLabel->Location = System::Drawing::Point(125, 513);
				this->statusLabel->Name = L"statusLabel";
				this->statusLabel->Size = System::Drawing::Size(87, 13);
				this->statusLabel->TabIndex = 6;
				this->statusLabel->Text = L"Calculator Status";
				// 
				// SelectButton
				// 
				this->SelectButton->Location = System::Drawing::Point(529, 285);
				this->SelectButton->Name = L"SelectButton";
				this->SelectButton->Size = System::Drawing::Size(75, 23);
				this->SelectButton->TabIndex = 8;
				this->SelectButton->Text = L"Push";
				this->SelectButton->UseVisualStyleBackColor = true;
				this->SelectButton->Click += gcnew System::EventHandler(this, &MainForm::SelectButton_Click);
				// 
				// stackListBox
				// 
				this->stackListBox->FormattingEnabled = true;
				this->stackListBox->Location = System::Drawing::Point(529, 56);
				this->stackListBox->Name = L"stackListBox";
				this->stackListBox->Size = System::Drawing::Size(318, 212);
				this->stackListBox->TabIndex = 9;
				// 
				// label2
				// 
				this->label2->AutoSize = true;
				this->label2->Location = System::Drawing::Point(526, 32);
				this->label2->Name = L"label2";
				this->label2->Size = System::Drawing::Size(35, 13);
				this->label2->TabIndex = 10;
				this->label2->Text = L"Stack";
				// 
				// printButton
				// 
				this->printButton->Location = System::Drawing::Point(772, 285);
				this->printButton->Name = L"printButton";
				this->printButton->Size = System::Drawing::Size(75, 23);
				this->printButton->TabIndex = 11;
				this->printButton->Text = L"Print";
				this->printButton->UseVisualStyleBackColor = true;
				this->printButton->Click += gcnew System::EventHandler(this, &MainForm::printButton_Click);
				// 
				// executeButton
				// 
				this->executeButton->Location = System::Drawing::Point(150, 27);
				this->executeButton->Name = L"executeButton";
				this->executeButton->Size = System::Drawing::Size(75, 23);
				this->executeButton->TabIndex = 12;
				this->executeButton->Text = L"Execute";
				this->executeButton->UseVisualStyleBackColor = true;
				this->executeButton->Click += gcnew System::EventHandler(this, &MainForm::executeButton_Click);
				// 
				// popButton
				// 
				this->popButton->Location = System::Drawing::Point(610, 285);
				this->popButton->Name = L"popButton";
				this->popButton->Size = System::Drawing::Size(75, 23);
				this->popButton->TabIndex = 13;
				this->popButton->Text = L"Pop";
				this->popButton->UseVisualStyleBackColor = true;
				this->popButton->Click += gcnew System::EventHandler(this, &MainForm::popButton_Click);
				// 
				// storeButton
				// 
				this->storeButton->Location = System::Drawing::Point(691, 285);
				this->storeButton->Name = L"storeButton";
				this->storeButton->Size = System::Drawing::Size(75, 23);
				this->storeButton->TabIndex = 14;
				this->storeButton->Text = L"Store";
				this->storeButton->UseVisualStyleBackColor = true;
				this->storeButton->Click += gcnew System::EventHandler(this, &MainForm::storeButton_Click);
				// 
				// productButton
				// 
				this->productButton->Location = System::Drawing::Point(529, 344);
				this->productButton->Name = L"productButton";
				this->productButton->Size = System::Drawing::Size(75, 23);
				this->productButton->TabIndex = 15;
				this->productButton->Text = L"Product";
				this->productButton->UseVisualStyleBackColor = true;
				this->productButton->Click += gcnew System::EventHandler(this, &MainForm::productButton_Click);
				// 
				// prodconsButton
				// 
				this->prodconsButton->Location = System::Drawing::Point(610, 344);
				this->prodconsButton->Name = L"prodconsButton";
				this->prodconsButton->Size = System::Drawing::Size(75, 23);
				this->prodconsButton->TabIndex = 16;
				this->prodconsButton->Text = L"Prod.Cons.";
				this->prodconsButton->UseVisualStyleBackColor = true;
				this->prodconsButton->Click += gcnew System::EventHandler(this, &MainForm::prodconsButton_Click);
				// 
				// abstractionButton
				// 
				this->abstractionButton->Location = System::Drawing::Point(691, 344);
				this->abstractionButton->Name = L"abstractionButton";
				this->abstractionButton->Size = System::Drawing::Size(75, 23);
				this->abstractionButton->TabIndex = 17;
				this->abstractionButton->Text = L"Abstract";
				this->abstractionButton->UseVisualStyleBackColor = true;
				this->abstractionButton->Click += gcnew System::EventHandler(this, &MainForm::abstractionButton_Click);
				// 
				// JoinButton
				// 
				this->JoinButton->Location = System::Drawing::Point(772, 344);
				this->JoinButton->Name = L"JoinButton";
				this->JoinButton->Size = System::Drawing::Size(75, 23);
				this->JoinButton->TabIndex = 18;
				this->JoinButton->Text = L"Join";
				this->JoinButton->UseVisualStyleBackColor = true;
				this->JoinButton->Click += gcnew System::EventHandler(this, &MainForm::JoinButton_Click);
				// 
				// MinimizeButton
				// 
				this->MinimizeButton->Location = System::Drawing::Point(529, 373);
				this->MinimizeButton->Name = L"MinimizeButton";
				this->MinimizeButton->Size = System::Drawing::Size(75, 23);
				this->MinimizeButton->TabIndex = 19;
				this->MinimizeButton->Text = L"Minimize";
				this->MinimizeButton->UseVisualStyleBackColor = true;
				this->MinimizeButton->Click += gcnew System::EventHandler(this, &MainForm::MinimizeButton_Click);
				// 
				// PlotButton
				// 
				this->PlotButton->Location = System::Drawing::Point(529, 431);
				this->PlotButton->Name = L"PlotButton";
				this->PlotButton->Size = System::Drawing::Size(75, 23);
				this->PlotButton->TabIndex = 20;
				this->PlotButton->Text = L"Plot";
				this->PlotButton->UseVisualStyleBackColor = true;
				this->PlotButton->Click += gcnew System::EventHandler(this, &MainForm::PlotButton_Click);
				// 
				// DuplicateButton
				// 
				this->DuplicateButton->Location = System::Drawing::Point(529, 315);
				this->DuplicateButton->Name = L"DuplicateButton";
				this->DuplicateButton->Size = System::Drawing::Size(75, 23);
				this->DuplicateButton->TabIndex = 21;
				this->DuplicateButton->Text = L"Duplicate";
				this->DuplicateButton->UseVisualStyleBackColor = true;
				this->DuplicateButton->Click += gcnew System::EventHandler(this, &MainForm::DuplicateButton_Click);
				// 
				// SumButton
				// 
				this->SumButton->Location = System::Drawing::Point(611, 374);
				this->SumButton->Name = L"SumButton";
				this->SumButton->Size = System::Drawing::Size(75, 23);
				this->SumButton->TabIndex = 22;
				this->SumButton->Text = L"Sum";
				this->SumButton->UseVisualStyleBackColor = true;
				this->SumButton->Click += gcnew System::EventHandler(this, &MainForm::SumButton_Click);
				// 
				// MaxButton
				// 
				this->MaxButton->Location = System::Drawing::Point(691, 374);
				this->MaxButton->Name = L"MaxButton";
				this->MaxButton->Size = System::Drawing::Size(75, 23);
				this->MaxButton->TabIndex = 23;
				this->MaxButton->Text = L"Max";
				this->MaxButton->UseVisualStyleBackColor = true;
				this->MaxButton->Click += gcnew System::EventHandler(this, &MainForm::MaxButton_Click);
				// 
				// MinButton
				// 
				this->MinButton->Location = System::Drawing::Point(772, 374);
				this->MinButton->Name = L"MinButton";
				this->MinButton->Size = System::Drawing::Size(75, 23);
				this->MinButton->TabIndex = 24;
				this->MinButton->Text = L"Min";
				this->MinButton->UseVisualStyleBackColor = true;
				this->MinButton->Click += gcnew System::EventHandler(this, &MainForm::MinButton_Click);
				// 
				// MultiplyButton
				// 
				this->MultiplyButton->Location = System::Drawing::Point(529, 402);
				this->MultiplyButton->Name = L"MultiplyButton";
				this->MultiplyButton->Size = System::Drawing::Size(75, 23);
				this->MultiplyButton->TabIndex = 25;
				this->MultiplyButton->Text = L"Multiply";
				this->MultiplyButton->UseVisualStyleBackColor = true;
				this->MultiplyButton->Click += gcnew System::EventHandler(this, &MainForm::MultiplyButton_Click);
				// 
				// saveButton
				// 
				this->saveButton->Location = System::Drawing::Point(529, 461);
				this->saveButton->Name = L"saveButton";
				this->saveButton->Size = System::Drawing::Size(75, 23);
				this->saveButton->TabIndex = 26;
				this->saveButton->Text = L"Save";
				this->saveButton->UseVisualStyleBackColor = true;
				this->saveButton->Click += gcnew System::EventHandler(this, &MainForm::saveButton_Click);
				// 
				// aboutButton
				// 
				this->aboutButton->Location = System::Drawing::Point(772, 461);
				this->aboutButton->Name = L"aboutButton";
				this->aboutButton->Size = System::Drawing::Size(75, 23);
				this->aboutButton->TabIndex = 27;
				this->aboutButton->Text = L"About";
				this->aboutButton->UseVisualStyleBackColor = true;
				this->aboutButton->Click += gcnew System::EventHandler(this, &MainForm::aboutButton_Click);
				// 
				// button1
				// 
				this->button1->Location = System::Drawing::Point(690, 460);
				this->button1->Name = L"button1";
				this->button1->Size = System::Drawing::Size(75, 23);
				this->button1->TabIndex = 28;
				this->button1->Text = L"Test";
				this->button1->UseVisualStyleBackColor = true;
				this->button1->Click += gcnew System::EventHandler(this, &MainForm::testButton_Click);
				// 
				// MainForm
				// 
				this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
				this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
				this->ClientSize = System::Drawing::Size(892, 566);
				this->Controls->Add(this->button1);
				this->Controls->Add(this->aboutButton);
				this->Controls->Add(this->saveButton);
				this->Controls->Add(this->MultiplyButton);
				this->Controls->Add(this->MinButton);
				this->Controls->Add(this->MaxButton);
				this->Controls->Add(this->SumButton);
				this->Controls->Add(this->DuplicateButton);
				this->Controls->Add(this->PlotButton);
				this->Controls->Add(this->MinimizeButton);
				this->Controls->Add(this->JoinButton);
				this->Controls->Add(this->abstractionButton);
				this->Controls->Add(this->prodconsButton);
				this->Controls->Add(this->productButton);
				this->Controls->Add(this->storeButton);
				this->Controls->Add(this->popButton);
				this->Controls->Add(this->executeButton);
				this->Controls->Add(this->printButton);
				this->Controls->Add(this->label2);
				this->Controls->Add(this->stackListBox);
				this->Controls->Add(this->SelectButton);
				this->Controls->Add(this->statusLabel);
				this->Controls->Add(this->label1);
				this->Controls->Add(this->selectfileButton);
				this->Controls->Add(this->xmlBox);
				this->Icon = (cli::safe_cast<System::Drawing::Icon^  >(resources->GetObject(L"$this.Icon")));
				this->Name = L"MainForm";
				this->Text = L"Pareto Calculator";
				this->ResumeLayout(false);
				this->PerformLayout();

			}
#pragma endregion

		private: System::Void selectfileButton_Click(System::Object^  sender, System::EventArgs^  e);

		public:		 
			void setStatus(const std::string &s){
				this->statusLabel->Text= gcnew System::String(s.c_str());
				this->statusLabel->Update();
			}

			void verbose(const std::string &s){
				this->xmlBox->AppendText(gcnew System::String(s.c_str()));
			}

			void MainForm::ParetoCalculatorExceptionOccurred(EParetoCalculatorError& e);

		private: void updateStack();

		private: System::Void SelectButton_Click(System::Object^  sender, System::EventArgs^  e);
		private: System::Void printButton_Click(System::Object^  sender, System::EventArgs^  e);
		private: System::Void executeButton_Click(System::Object^  sender, System::EventArgs^  e);
		private: System::Void popButton_Click(System::Object^  sender, System::EventArgs^  e);
		private: System::Void productButton_Click(System::Object^  sender, System::EventArgs^  e);
		private: System::Void prodconsButton_Click(System::Object^  sender, System::EventArgs^  e);
		private: System::Void abstractionButton_Click(System::Object^  sender, System::EventArgs^  e);
		private: System::Void JoinButton_Click(System::Object^  sender, System::EventArgs^  e) ;

		private: System::Void MinimizeButton_Click(System::Object^  sender, System::EventArgs^  e);
		private: System::Void PlotButton_Click(System::Object^  sender, System::EventArgs^  e);
		private: System::Void DuplicateButton_Click(System::Object^  sender, System::EventArgs^  e);
		private: System::Void SumButton_Click(System::Object^  sender, System::EventArgs^  e);
		private: System::Void MaxButton_Click(System::Object^  sender, System::EventArgs^  e);
		private: System::Void MinButton_Click(System::Object^  sender, System::EventArgs^  e);
		private: System::Void MultiplyButton_Click(System::Object^  sender, System::EventArgs^  e);
		private: System::Void saveButton_Click(System::Object^  sender, System::EventArgs^  e);
		private: System::Void storeButton_Click(System::Object^  sender, System::EventArgs^  e) ;
		private: System::Void aboutButton_Click(System::Object^  sender, System::EventArgs^  e) ;
		private: System::Void testButton_Click(System::Object^  sender, System::EventArgs^  e) ;
};

	}}