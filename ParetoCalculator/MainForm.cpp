#include "stdafx.h"
#include "MainForm.h"
#include "dialogs/SelectionDialog.h"
#include "dialogs/AbstractionDialog.h"
#include "dialogs/AboutDialog.h"
#include "dialogs/SumDialog.h"
#include "dialogs/JoinDialog.h"
#include "dialogs/PlotDialog.h"
#include "dialogs/SaveDialog.h"
#include "dialogs/StoreDialog.h"
#include "dialogs/prodcons.h"
#include <operations.h>

#include <math.h>
#include <strstream>
#include <sstream>

#include <time.h>

using namespace System::Windows::Forms;

namespace Pareto {
	namespace Calculator {

		System::Void MainForm::selectfileButton_Click(System::Object^  sender, System::EventArgs^  e) {
			OpenFileDialog^ openFileDialog1 = gcnew OpenFileDialog();
			openFileDialog1->Filter = "xml files (*.xml)|*.xml|All files (*.*)|*.*" ;
			openFileDialog1->FilterIndex = 2 ;
			openFileDialog1->RestoreDirectory = true ;

			Forms::DialogResult result = openFileDialog1->ShowDialog();
			if ( result == Forms::DialogResult::OK ){
				try {
					std::wstring s;
					MarshalString(openFileDialog1->FileName, s);
					paretocalculator->LoadFile(s);
					paretocalculator->LoadQuantityTypes();
					paretocalculator->LoadConfigurationSpaces();
					paretocalculator->LoadConfigurationSets();
				}
				catch (EParetoCalculatorError& e) {
					this->ParetoCalculatorExceptionOccurred(e);
				}
			}
			this->updateStack();
		}

		System::Void MainForm::executeButton_Click(System::Object^  sender, System::EventArgs^  e) {
#ifdef _DEBUG
			try {
#endif
				paretocalculator->LoadOperations();
				this->updateStack();
#ifdef _DEBUG
			}
			catch (EParetoCalculatorError& e) {
				this->ParetoCalculatorExceptionOccurred(e);
				this->updateStack();
			}
#endif
		}


		System::Void MainForm::SelectButton_Click(System::Object^  sender, System::EventArgs^  e) {
			SelectionDialog^ sd = gcnew SelectionDialog(paretocalculator); 
			Forms::DialogResult r = sd->ShowDialog();
			if(r==Forms::DialogResult::OK){
				String^ s = safe_cast<System::String^>(sd->listBox1->SelectedItem);
				std::string so = system_to_std_string(s);
				paretocalculator->push(so);
			}
			this->updateStack();
		}


		void MainForm::updateStack() {
			this->stackListBox->Items->Clear();
            std::vector<StorableObject*> stack = paretocalculator->stack;
            std::vector<StorableObject*>::iterator i;
			for(i=stack.begin(); i!=stack.end(); i++){
				this->stackListBox->Items->Add(gcnew System::String((*i)->asString().c_str()));
			}
		}

		void MainForm::ParetoCalculatorExceptionOccurred(EParetoCalculatorError& e){
			this->verbose("An exception occurred: " + e.errorMsg + "\n");
			this->updateStack();
		}

		System::Void MainForm::printButton_Click(System::Object^  sender, System::EventArgs^  e) {
			try{
				// print selected item from stack
				unsigned int n = this->stackListBox->SelectedIndex;
                std::vector<StorableObject*> stack = paretocalculator->stack;
				if((n>=0)&&(n<stack.size())){
					StorableObject* o = stack[n];
					this->verbose(o->asString().c_str());
				}
			}
			catch (EParetoCalculatorError& e) {
				this->ParetoCalculatorExceptionOccurred(e);
			}
		}


		System::Void MainForm::popButton_Click(System::Object^  sender, System::EventArgs^  e) {
			try{
				if(paretocalculator->stack.size()>0){
					StorableObject* o = paretocalculator->pop();
					this->verbose(o->asString().c_str());
					this->updateStack();
				}
			}
			catch (EParetoCalculatorError& e) {
				this->ParetoCalculatorExceptionOccurred(e);
			}
		}


		System::Void MainForm::productButton_Click(System::Object^  sender, System::EventArgs^  e) {
			try {		
				if(paretocalculator->stack.size()>=2){
					paretocalculator->product();
					this->updateStack();
				}
			}
			catch (EParetoCalculatorError& e) {
				this->ParetoCalculatorExceptionOccurred(e);
			}
		}


		System::Void MainForm::prodconsButton_Click(System::Object^  sender, System::EventArgs^  e) {
			try {
				if(!paretocalculator->stack.empty()){
					prodcons^ pc = gcnew prodcons(dynamic_cast<ConfigurationSet*>(paretocalculator->peek())); 
					Forms::DialogResult r = pc->ShowDialog();
					if(r==Forms::DialogResult::OK) {
						std::string& pq = system_to_std_string(safe_cast<System::String^>(pc->producerList->SelectedItem));
						std::string& cq = system_to_std_string(safe_cast<System::String^>(pc->consumerList->SelectedItem));
						POperation_ProdCons& pco = *new POperation_ProdCons(pq,cq);
						pco.executeOn(*paretocalculator);
						this->updateStack();
						delete &pco;
					}
				}
			}
			catch (EParetoCalculatorError& e) {
				this->ParetoCalculatorExceptionOccurred(e);
			}
		}

		System::Void MainForm::abstractionButton_Click(System::Object^  sender, System::EventArgs^  e) {
			try {
				if(paretocalculator->stack.size()>0){
					AbstractionDialog^ ad = gcnew AbstractionDialog(dynamic_cast<ConfigurationSet*>(paretocalculator->peek())); 
					Forms::DialogResult r = ad->ShowDialog();
					if(r==Forms::DialogResult::OK) {
						std::string& q = system_to_std_string(safe_cast<System::String^>(ad->quantitiesBox->SelectedItem));
						ListOfQuantityNames lqn;
						lqn.push_back(q);
						POperation_Abstract& ao = *new POperation_Abstract(&lqn);
						ao.executeOn(*paretocalculator);
						this->updateStack();
						delete &ao;
					}
				}
			}
			catch (EParetoCalculatorError& e) {
				this->ParetoCalculatorExceptionOccurred(e);
			}
		}

		System::Void MainForm::SumButton_Click(System::Object^  sender, System::EventArgs^  e) {
			try {
				if(paretocalculator->stack.size()>0){
					SumDialog^ sd = gcnew SumDialog(dynamic_cast<ConfigurationSet*>(paretocalculator->peek()), "Sum of quantities"); 
					Forms::DialogResult r = sd->ShowDialog();
					if(r==Forms::DialogResult::OK) {
						std::string& qa = system_to_std_string(safe_cast<System::String^>(sd->quantABox->SelectedItem));
						std::string& qb = system_to_std_string(safe_cast<System::String^>(sd->quantBBox->SelectedItem));
						POperation_Sum& so = *new POperation_Sum(qa,qb);
						so.executeOn(*paretocalculator);
						this->updateStack();
						delete &so;
					}
				}
			}
			catch (EParetoCalculatorError& e) {
				this->ParetoCalculatorExceptionOccurred(e);
			}
		}

		System::Void MainForm::MaxButton_Click(System::Object^  sender, System::EventArgs^  e) {
			try {
				if(paretocalculator->stack.size()>0){
					SumDialog^ sd = gcnew SumDialog(dynamic_cast<ConfigurationSet*>(paretocalculator->peek()), "Maximum of quantities"); 
					Forms::DialogResult r = sd->ShowDialog();
					if(r==Forms::DialogResult::OK) {
						std::string& qa = system_to_std_string(safe_cast<System::String^>(sd->quantABox->SelectedItem));
						std::string& qb = system_to_std_string(safe_cast<System::String^>(sd->quantBBox->SelectedItem));
						POperation_Max& so = *new POperation_Max(qa,qb);
						so.executeOn(*paretocalculator);
						this->updateStack();
						delete &so;
					}
				}
			}
			catch (EParetoCalculatorError& e) {
				this->ParetoCalculatorExceptionOccurred(e);
			}
		}

		System::Void MainForm::MinButton_Click(System::Object^  sender, System::EventArgs^  e) {
			try {
				if(paretocalculator->stack.size()>0){
					SumDialog^ sd = gcnew SumDialog(dynamic_cast<ConfigurationSet*>(paretocalculator->peek()), "Minimum of quantities"); 
					Forms::DialogResult r = sd->ShowDialog();
					if(r==Forms::DialogResult::OK) {
						std::string& qa = system_to_std_string(safe_cast<System::String^>(sd->quantABox->SelectedItem));
						std::string& qb = system_to_std_string(safe_cast<System::String^>(sd->quantBBox->SelectedItem));
						POperation_Min& so = *new POperation_Min(qa,qb);
						so.executeOn(*paretocalculator);
						this->updateStack();
						delete &so;
					}
				}
			}
			catch (EParetoCalculatorError& e) {
				this->ParetoCalculatorExceptionOccurred(e);
			}
		}

		System::Void MainForm::MultiplyButton_Click(System::Object^  sender, System::EventArgs^  e) {
			try {
				if(paretocalculator->stack.size()>0){
					SumDialog^ sd = gcnew SumDialog(dynamic_cast<ConfigurationSet*>(paretocalculator->peek()), "Product of quantities"); 
					Forms::DialogResult r = sd->ShowDialog();
					if(r==Forms::DialogResult::OK) {
						std::string& qa = system_to_std_string(safe_cast<System::String^>(sd->quantABox->SelectedItem));
						std::string& qb = system_to_std_string(safe_cast<System::String^>(sd->quantBBox->SelectedItem));
						POperation_Multiply& so = *new POperation_Multiply(qa,qb);
						so.executeOn(*paretocalculator);
						this->updateStack();
						delete &so;
					}
				}
			}
			catch (EParetoCalculatorError& e) {
				this->ParetoCalculatorExceptionOccurred(e);
			}
		}

		System::Void MainForm::JoinButton_Click(System::Object^  sender, System::EventArgs^  e) {
			try {
				if(!paretocalculator->stack.empty()){
					JoinDialog^ jd = gcnew JoinDialog(dynamic_cast<ConfigurationSet*>(paretocalculator->peek())); 
					Forms::DialogResult r = jd->ShowDialog();
					if(r==Forms::DialogResult::OK) {
						std::string& q = system_to_std_string(safe_cast<System::String^>(jd->quantitiesList->SelectedItem));
						JoinMap jm;
						jm[q]=q;
						POperation_EfficientJoin& jo = *new POperation_EfficientJoin(&jm);
						jo.executeOn(*paretocalculator);
						this->updateStack();
						delete &jo;
					}
				}
			}
			catch (EParetoCalculatorError& e) {
				this->ParetoCalculatorExceptionOccurred(e);
			}
		}

		System::Void MainForm::MinimizeButton_Click(System::Object^  sender, System::EventArgs^  e) {
			try {
				if(!paretocalculator->stack.empty()){		
					paretocalculator->minimize();
					this->updateStack();
				}
			}
			catch (EParetoCalculatorError& e) {
				this->ParetoCalculatorExceptionOccurred(e);
			}
		}

		System::Void MainForm::PlotButton_Click(System::Object^  sender, System::EventArgs^  e) {	
			try {
				if(paretocalculator->stack.size()>0){
					PlotDialog^ pd = gcnew PlotDialog(dynamic_cast<ConfigurationSet*>(paretocalculator->peek())); 
					Forms::DialogResult r = pd->ShowDialog();
					if(r==Forms::DialogResult::OK) {
					}
				}
			}
			catch (EParetoCalculatorError& e) {
				this->ParetoCalculatorExceptionOccurred(e);
			}
		}

		System::Void MainForm::saveButton_Click(System::Object^  sender, System::EventArgs^  e) {
			SaveDialog^ sd = gcnew SaveDialog(paretocalculator); 
			Forms::DialogResult r = sd->ShowDialog();
		}

		System::Void MainForm::DuplicateButton_Click(System::Object^  sender, System::EventArgs^  e) {
			try {
				if(!paretocalculator->stack.empty()){
					paretocalculator->duplicate();
					this->updateStack();
				}
			}
			catch (EParetoCalculatorError& e) {
				this->ParetoCalculatorExceptionOccurred(e);
			}
		}

		System::Void MainForm::storeButton_Click(System::Object^  sender, System::EventArgs^  e) {
			try {
				if(!paretocalculator->stack.empty()){
					StoreDialog^ sd = gcnew StoreDialog(paretocalculator->peek()->name); 
					Forms::DialogResult r = sd->ShowDialog();
					if(r==Forms::DialogResult::OK){
						StorableObject& o = *paretocalculator->pop();
						o.name=sd->getResult();
						paretocalculator->store(o);
						this->updateStack();
					}
				}
			}
			catch (EParetoCalculatorError& e) {
				this->ParetoCalculatorExceptionOccurred(e);
			}
		}


		System::Void MainForm::aboutButton_Click(System::Object^  sender, System::EventArgs^  e) {
			AboutDialog^ aboutDialog1 = gcnew AboutDialog();
			aboutDialog1->ShowDialog();
		}

		bool resourceConstraint(const Configuration& c){
			const int R=5; // number of resources
//			int RL[R] = {75, 75, 75, 75, 75, 75, 75, 75, 75, 75}; //3
			int RL[R] = {35, 35, 35, 35, 35}; //2
//			int RL[R] = {25, 25, 25, 25, 25}; //1
			if(dynamic_cast<const QuantityValue_Integer&>(c.getQuantity(1)).value>RL[0]) return false;
			if(dynamic_cast<const QuantityValue_Integer&>(c.getQuantity(2)).value>RL[1]) return false;
			if(dynamic_cast<const QuantityValue_Integer&>(c.getQuantity(3)).value>RL[2]) return false;
			if(dynamic_cast<const QuantityValue_Integer&>(c.getQuantity(4)).value>RL[3]) return false;
			if(dynamic_cast<const QuantityValue_Integer&>(c.getQuantity(5)).value>RL[4]) return false;
			return true;
		}

		System::Void MainForm::testButton_Click(System::Object^  sender, System::EventArgs^  e) {
			time_t start, finish; 
			double elapsed_time; 
			time( &start ); 
			
			const int N=7; // number of 'tasks'
			const int L=5; // number of options per task
			const int R=5; // number of resources;

			ConfigurationSet* cs[N];

			for(int k=0; k<N; k++){
				std::stringstream ss;
				std::string str;
				ss << (k+1);
				ss >> str;
				cs[k] = dynamic_cast<ConfigurationSet*>(paretocalculator->retrieve("Set"+ str));
			}

			QuantityType& qtv = *paretocalculator->retrieveQuantityType("Value");
			QuantityType& qtr1 = *paretocalculator->retrieveQuantityType("Resource1");
			QuantityType& qtr2 = *paretocalculator->retrieveQuantityType("Resource2");
			QuantityType& qtr3 = *paretocalculator->retrieveQuantityType("Resource3");
			QuantityType& qtr4 = *paretocalculator->retrieveQuantityType("Resource4");
			QuantityType& qtr5 = *paretocalculator->retrieveQuantityType("Resource5");

			ConfigurationSpace* rsp = new ConfigurationSpace("SSetResult");
			rsp->addQuantityAs(qtv,"TotalValue");
			rsp->addQuantity(*(paretocalculator->retrieveQuantityType("Resource1")));
			rsp->addQuantity(*(paretocalculator->retrieveQuantityType("Resource2")));
			rsp->addQuantity(*(paretocalculator->retrieveQuantityType("Resource3")));
			rsp->addQuantity(*(paretocalculator->retrieveQuantityType("Resource4")));
			rsp->addQuantity(*(paretocalculator->retrieveQuantityType("Resource5")));

			ConfigurationSet* intermRes = cs[0];

			SetOfConfigurations::iterator i,j;

			for(int k=1; k<N-1; k++){
				ConfigurationSet* newIntermRes = new ConfigurationSet(rsp, "IntermSet");
				int debugCount=0;
				for( i=intermRes->confs.begin(); i!=intermRes->confs.end(); i++){
					debugCount++;
					for( j=cs[k]->confs.begin(); j!=cs[k]->confs.end(); j++){
						const Configuration& c1 = *i;
						const Configuration& c2 = *j;
						Configuration *c = new Configuration(rsp);
						double v;
						v = ((const QuantityValue_Real&)(c1.getQuantity(0))).value;
						v += ((const QuantityValue_Real&)(c2.getQuantity(0))).value;
						c->addQuantity(*new QuantityValue_Real(qtv, v));
						int r1,r2,r3,r4,r5;
						r1 = ((const QuantityValue_Integer&)(c1.getQuantity(1))).value;
						r1 += ((const QuantityValue_Integer&)(c2.getQuantity(1))).value;
						c->addQuantity(*new QuantityValue_Integer(qtr1, r1));

						r2 = ((const QuantityValue_Integer&)(c1.getQuantity(2))).value;
						r2 += ((const QuantityValue_Integer&)(c2.getQuantity(2))).value;
						c->addQuantity(*new QuantityValue_Integer(qtr2, r2));

						r3 = ((const QuantityValue_Integer&)(c1.getQuantity(3))).value;
						r3 += ((const QuantityValue_Integer&)(c2.getQuantity(3))).value;
						c->addQuantity(*new QuantityValue_Integer(qtr3, r3));

						r4 = ((const QuantityValue_Integer&)(c1.getQuantity(4))).value;
						r4 += ((const QuantityValue_Integer&)(c2.getQuantity(4))).value;
						c->addQuantity(*new QuantityValue_Integer(qtr4, r4));

						r5 = ((const QuantityValue_Integer&)(c1.getQuantity(5))).value;
						r5 += ((const QuantityValue_Integer&)(c2.getQuantity(5))).value;
						c->addQuantity(*new QuantityValue_Integer(qtr5, r5));

						if(resourceConstraint(*c))newIntermRes->addConfigurationAndFilter(*c);
					}
				}

				intermRes = newIntermRes;
				
				std::stringstream ss;
				std::string str;
				ss << (intermRes->confs.size());
				ss >> str;				
				paretocalculator->verbose(str+"\n");
			}


			ListOfQuantityNames* lqn = new ListOfQuantityNames();
			lqn->push_back("Resource1");
			lqn->push_back("Resource2");
			lqn->push_back("Resource3");
			lqn->push_back("Resource4");
			lqn->push_back("Resource5");

			ConfigurationSpace* hrsp = rsp->hide(lqn);

			ConfigurationSet* finalRes = new ConfigurationSet(hrsp,"FinalSet");

			for( i=intermRes->confs.begin(); i!=intermRes->confs.end(); i++){
				for( j=cs[N-1]->confs.begin(); j!=cs[N-1]->confs.end(); j++){
					const Configuration& c1 = *i;
					const Configuration& c2 = *j;
					Configuration *c = new Configuration(hrsp);
					double v;
					v = dynamic_cast<const QuantityValue_Real&>(c1.getQuantity(0)).value;
					v += dynamic_cast<const QuantityValue_Real&>(c2.getQuantity(0)).value;
					c->addQuantity(*new QuantityValue_Real(qtv, v));
					int r1,r2,r3,r4,r5;
					r1 = dynamic_cast<const QuantityValue_Integer&>(c1.getQuantity(1)).value;
					r1 += dynamic_cast<const QuantityValue_Integer&>(c2.getQuantity(1)).value;
					c->addQuantity(*new QuantityValue_Integer(qtr1, r1));

					r2 = dynamic_cast<const QuantityValue_Integer&>(c1.getQuantity(2)).value;
					r2 += dynamic_cast<const QuantityValue_Integer&>(c2.getQuantity(2)).value;
					c->addQuantity(*new QuantityValue_Integer(qtr2, r2));

					r3 = dynamic_cast<const QuantityValue_Integer&>(c1.getQuantity(3)).value;
					r3 += dynamic_cast<const QuantityValue_Integer&>(c2.getQuantity(3)).value;
					c->addQuantity(*new QuantityValue_Integer(qtr3, r3));

					r4 = dynamic_cast<const QuantityValue_Integer&>(c1.getQuantity(4)).value;
					r4 += dynamic_cast<const QuantityValue_Integer&>(c2.getQuantity(4)).value;
					c->addQuantity(*new QuantityValue_Integer(qtr4, r4));

					r5 = dynamic_cast<const QuantityValue_Integer&>(c1.getQuantity(5)).value;
					r5 += dynamic_cast<const QuantityValue_Integer&>(c2.getQuantity(5)).value;
					c->addQuantity(*new QuantityValue_Integer(qtr5, r5));

					if(resourceConstraint(*c))finalRes->addConfigurationAndFilter(*c);
				}
			}


			paretocalculator->store(*finalRes);

			time( &finish ); 
			elapsed_time = difftime( finish, start ); 

			std:: stringstream str;
			str<<elapsed_time;
            std::string buffer(str.str());
			paretocalculator->verbose(buffer);
		}


//		System::Void MainForm::testButton_Click(System::Object^  sender, System::EventArgs^  e) {
//
//			ConfigurationSpace *sp = dynamic_cast<ConfigurationSpace*>(paretocalculator->retrieve("S_Available_BitRate"));
//			ConfigurationSet *cs = new ConfigurationSet(sp, "M_Available_BitRate");
//
//			Configuration *c = new Configuration(sp);
//			QuantityType *qt = dynamic_cast<QuantityType*>(paretocalculator->retrieve("Q_BitRate_Inv"));
//			QuantityValue_Real *v = new QuantityValue_Real(*qt, 1.0/100.0);  // <-use the bitrate as input
//
//			// Here, the quality input Parameter might be checked also, in order to be used as an additional constraint
//			//	QuantityValue_Real *q = NULL;
//			// if (quality)
//			//	  q = new QuantityValue_Real(*??, T_FLOAT(quality));  // <-use also the quality as input ??
//
//
//			c->addQuantity(*v);
//			cs->addConfiguration(*c);
//
//			paretocalculator->push("M_MPEG-4_Transport_BitRate");
//			paretocalculator->push(*cs);
//
//			paretocalculator->product();
//
//			ParetoCalculatorOperation *o = new POperation_ProdCons("Q_TR_BitRate_Inv","Q_TR_BitRate");
//			o->executeOn(*paretocalculator);
//			delete o;
//
//			paretocalculator->push("M_MPEG-4_Stream");
//
//			JoinMap jm;
//			jm["Q_IP"]="Q_TR_IP";
//			jm["Q_QP"]="Q_TR_QP";
//			jm["Q_Resolution"]="Q_TR_Resolution";
//			jm["Q_FrameRate"]="Q_TR_FrameRate";
//
//			o = new POperation_EfficientJoin(&jm);
//			//o = new POperation_Join(&jm);
//			o->executeOn(*paretocalculator);
//			delete o;
//
//			ConfigurationSet* tempc = paretocalculator->popConfigurationSet();
//
//			std::ostringstream ss;
//			std::string str;
//			ss << "Configurations: ";
//			ss << tempc->confs.size();
//			str = ss.str();
//			paretocalculator->verbose(str);
//			paretocalculator->push(*tempc);
//
//			ListOfQuantityNames aq;
//			aq.push_back("Q_TR_IP");
//			aq.push_back("Q_TR_QP");
//			aq.push_back("Q_TR_Resolution");
//			aq.push_back("Q_TR_FrameRate");
//			aq.push_back("Q_TR_BitRate");  //CK: comment for feedback
//			aq.push_back("Q_TR_BitRate_Inv");
//
//			o = new POperation_Abstract(&aq);
//			o->executeOn(*paretocalculator);
//			delete o;
//
//			ListOfQuantityNames hq;
//			hq.push_back("Q_IP");
//			hq.push_back("Q_QP");
//			hq.push_back("Q_Resolution");
//			hq.push_back("Q_FrameRate");
//			hq.push_back("Q_BitRate");
//			//	hq.push_back("Q_TR_BitRate");  // CK: added for feedback
//
//
//			o = new POperation_Hide(&hq);
//			o->executeOn(*paretocalculator);
//			delete o;
//
//			paretocalculator->minimize();
//
//			// there should be only one configuration left.
//
//			ConfigurationSet *rcs = dynamic_cast<ConfigurationSet*>(paretocalculator->pop());
//
//			//test if there is only one (..and at least one) configuration
//			if(rcs->confs.size()!=1){
//				// available bit rate cannot be matched. Default to lowest possible bit rate ???
//				cout << "resulting set should contain at most one configuration in H1Query" << endl;
////				return NO_RESULT;
//			}
//			const Configuration *rc = &(*rcs->confs.begin());
//
//			const QuantityValue_Enum &qv_ip = dynamic_cast<const QuantityValue_Enum &>(rc->getQuantity("Q_IP"));
//			const QuantityValue_Enum &qv_qp = dynamic_cast<const QuantityValue_Enum &>(rc->getQuantity("Q_QP"));
//			const QuantityValue_Enum &qv_res = dynamic_cast<const QuantityValue_Enum &>(rc->getQuantity("Q_Resolution"));
//			const QuantityValue_Enum &qv_fr = dynamic_cast<const QuantityValue_Enum &>(rc->getQuantity("Q_FrameRate"));
//			const QuantityValue_Real &qv_br = dynamic_cast<const QuantityValue_Real &>(rc->getQuantity("Q_BitRate"));
//			const QuantityValue_Real &qv_q = dynamic_cast<const QuantityValue_Real &>(rc->getQuantity("Q_PNSR"));
//		}

	}} // namespace