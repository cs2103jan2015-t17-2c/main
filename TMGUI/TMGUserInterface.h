#pragma once
#include <iostream>
#include <string>
#include <cstdlib>
#include <vector>
#include <Windows.h>
#include <msclr\marshal_cppstd.h>

#include "TMExecutor.h"
#include "TMParser.h"
#include "TMTaskList.h"
#include "TMCommandCreator.h"
#include "TMCommand.h"
#include "TMSplash.h"
#include <shellapi.h>

namespace TMGUI {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;
	using namespace System::Threading;
	using namespace System::Diagnostics;

	/// <summary>
	/// Summary for TMGUserInterface
	/// </summary>
	public ref class TMGUserInterface : public System::Windows::Forms::Form
	{
	public:
		TMGUserInterface(void)
		{
			Thread^ splash = gcnew Thread( gcnew ThreadStart (this,&TMGUI :: TMGUserInterface :: SplashStart));
			splash->Start();
			Thread::Sleep(2450);
			InitializeComponent();
			splash->Abort();
			this->Show();
			Activate();
		}

	public:
		void SplashStart(){
			Application::Run(gcnew TMSplash);
	}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~TMGUserInterface()
		{
			if (components)
			{
				delete components;
			}
		}

	protected: 

	private: System::Windows::Forms::TextBox^  userInput;
	private: System::Windows::Forms::Label^  welcomeMessage;




	private: System::Windows::Forms::RichTextBox^  statusDisplay;



	private: System::Windows::Forms::ListView^  defaultView;

	private: System::Windows::Forms::ColumnHeader^  taskID;
	private: System::Windows::Forms::ColumnHeader^  taskDescription;
	private: System::Windows::Forms::ColumnHeader^  startDate;
	private: System::Windows::Forms::ColumnHeader^  startTime;
	private: System::Windows::Forms::ColumnHeader^  endDate;
	private: System::Windows::Forms::ColumnHeader^  endTime;
	private: System::Windows::Forms::ColumnHeader^  confirmation;
	private: System::Windows::Forms::Label^  label4;
	private: System::Windows::Forms::Timer^  timer1;
	private: System::Windows::Forms::ColumnHeader^  isClash;
	private: System::Windows::Forms::ColumnHeader^  isDone;
	private: System::Windows::Forms::Label^  DisplayState;
	private: System::Windows::Forms::Label^  todayIs;
	private: System::Windows::Forms::Label^  nowShowing;
	private: System::Windows::Forms::Label^  label1;













	private: System::ComponentModel::IContainer^  components;


	protected: 



	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>


#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			this->components = (gcnew System::ComponentModel::Container());
			this->userInput = (gcnew System::Windows::Forms::TextBox());
			this->welcomeMessage = (gcnew System::Windows::Forms::Label());
			this->statusDisplay = (gcnew System::Windows::Forms::RichTextBox());
			this->defaultView = (gcnew System::Windows::Forms::ListView());
			this->taskID = (gcnew System::Windows::Forms::ColumnHeader());
			this->taskDescription = (gcnew System::Windows::Forms::ColumnHeader());
			this->startDate = (gcnew System::Windows::Forms::ColumnHeader());
			this->startTime = (gcnew System::Windows::Forms::ColumnHeader());
			this->endDate = (gcnew System::Windows::Forms::ColumnHeader());
			this->endTime = (gcnew System::Windows::Forms::ColumnHeader());
			this->confirmation = (gcnew System::Windows::Forms::ColumnHeader());
			this->isClash = (gcnew System::Windows::Forms::ColumnHeader());
			this->isDone = (gcnew System::Windows::Forms::ColumnHeader());
			this->label4 = (gcnew System::Windows::Forms::Label());
			this->timer1 = (gcnew System::Windows::Forms::Timer(this->components));
			this->DisplayState = (gcnew System::Windows::Forms::Label());
			this->todayIs = (gcnew System::Windows::Forms::Label());
			this->nowShowing = (gcnew System::Windows::Forms::Label());
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->SuspendLayout();
			// 
			// userInput
			// 
			this->userInput->BackColor = System::Drawing::SystemColors::Window;
			this->userInput->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
			this->userInput->Font = (gcnew System::Drawing::Font(L"Segoe UI", 12, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->userInput->Location = System::Drawing::Point(51, 775);
			this->userInput->Margin = System::Windows::Forms::Padding(2);
			this->userInput->Name = L"userInput";
			this->userInput->Size = System::Drawing::Size(1868, 50);
			this->userInput->TabIndex = 0;
			this->userInput->TextChanged += gcnew System::EventHandler(this, &TMGUserInterface::userInput_TextChanged);
			this->userInput->KeyDown += gcnew System::Windows::Forms::KeyEventHandler(this, &TMGUserInterface::userInput_KeyDown);
			this->userInput->KeyPress += gcnew System::Windows::Forms::KeyPressEventHandler(this, &TMGUserInterface::userInput_KeyPress);
			// 
			// welcomeMessage
			// 
			this->welcomeMessage->AutoSize = true;
			this->welcomeMessage->Font = (gcnew System::Drawing::Font(L"Corbel", 12, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->welcomeMessage->ForeColor = System::Drawing::SystemColors::ButtonFace;
			this->welcomeMessage->Location = System::Drawing::Point(44, 734);
			this->welcomeMessage->Margin = System::Windows::Forms::Padding(2, 0, 2, 0);
			this->welcomeMessage->Name = L"welcomeMessage";
			this->welcomeMessage->Size = System::Drawing::Size(391, 39);
			this->welcomeMessage->TabIndex = 2;
			this->welcomeMessage->Text = L"What would you like to do\?";
			// 
			// statusDisplay
			// 
			this->statusDisplay->BackColor = System::Drawing::SystemColors::InactiveCaption;
			this->statusDisplay->BorderStyle = System::Windows::Forms::BorderStyle::None;
			this->statusDisplay->Font = (gcnew System::Drawing::Font(L"Corbel", 12, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->statusDisplay->ForeColor = System::Drawing::SystemColors::InfoText;
			this->statusDisplay->Location = System::Drawing::Point(52, 599);
			this->statusDisplay->Margin = System::Windows::Forms::Padding(4, 3, 4, 3);
			this->statusDisplay->Name = L"statusDisplay";
			this->statusDisplay->ReadOnly = true;
			this->statusDisplay->Size = System::Drawing::Size(1574, 132);
			this->statusDisplay->TabIndex = 6;
			this->statusDisplay->Text = L"";
			// 
			// defaultView
			// 
			this->defaultView->BackColor = System::Drawing::SystemColors::HighlightText;
			this->defaultView->Columns->AddRange(gcnew cli::array< System::Windows::Forms::ColumnHeader^  >(9) {this->taskID, this->taskDescription, 
				this->startDate, this->startTime, this->endDate, this->endTime, this->confirmation, this->isClash, this->isDone});
			this->defaultView->Font = (gcnew System::Drawing::Font(L"Corbel", 10.875F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->defaultView->FullRowSelect = true;
			this->defaultView->Location = System::Drawing::Point(52, 98);
			this->defaultView->Margin = System::Windows::Forms::Padding(2);
			this->defaultView->Name = L"defaultView";
			this->defaultView->Size = System::Drawing::Size(1867, 482);
			this->defaultView->TabIndex = 9;
			this->defaultView->UseCompatibleStateImageBehavior = false;
			this->defaultView->View = System::Windows::Forms::View::Details;
			// 
			// taskID
			// 
			this->taskID->Text = L"ID";
			this->taskID->Width = 25;
			// 
			// taskDescription
			// 
			this->taskDescription->Text = L"Task Description";
			this->taskDescription->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
			this->taskDescription->Width = 245;
			// 
			// startDate
			// 
			this->startDate->Text = L"Start Date";
			this->startDate->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
			this->startDate->Width = 90;
			// 
			// startTime
			// 
			this->startTime->Text = L"Start Time";
			this->startTime->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
			this->startTime->Width = 90;
			// 
			// endDate
			// 
			this->endDate->Text = L"End Date";
			this->endDate->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
			this->endDate->Width = 90;
			// 
			// endTime
			// 
			this->endTime->Text = L"End Time";
			this->endTime->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
			this->endTime->Width = 90;
			// 
			// confirmation
			// 
			this->confirmation->Text = L"Confirmed";
			this->confirmation->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
			this->confirmation->Width = 80;
			// 
			// isClash
			// 
			this->isClash->Text = L"Clashes";
			this->isClash->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
			this->isClash->Width = 70;
			// 
			// isDone
			// 
			this->isDone->Text = L"Completed";
			this->isDone->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
			this->isDone->Width = 80;
			// 
			// label4
			// 
			this->label4->AutoSize = true;
			this->label4->Font = (gcnew System::Drawing::Font(L"Corbel", 12, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->label4->ForeColor = System::Drawing::SystemColors::ButtonFace;
			this->label4->Location = System::Drawing::Point(44, 51);
			this->label4->Margin = System::Windows::Forms::Padding(2, 0, 2, 0);
			this->label4->Name = L"label4";
			this->label4->Size = System::Drawing::Size(182, 39);
			this->label4->TabIndex = 10;
			this->label4->Text = L"currentTime";
			// 
			// timer1
			// 
			this->timer1->Enabled = true;
			this->timer1->Interval = 1;
			this->timer1->Tick += gcnew System::EventHandler(this, &TMGUserInterface::timer1_Tick);
			// 
			// DisplayState
			// 
			this->DisplayState->AutoSize = true;
			this->DisplayState->Font = (gcnew System::Drawing::Font(L"Corbel", 12, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->DisplayState->ForeColor = System::Drawing::SystemColors::ButtonFace;
			this->DisplayState->Location = System::Drawing::Point(1630, 636);
			this->DisplayState->Margin = System::Windows::Forms::Padding(4, 0, 4, 0);
			this->DisplayState->Name = L"DisplayState";
			this->DisplayState->Size = System::Drawing::Size(213, 39);
			this->DisplayState->TabIndex = 11;
			this->DisplayState->Text = L"Default display";
			// 
			// todayIs
			// 
			this->todayIs->AutoSize = true;
			this->todayIs->Font = (gcnew System::Drawing::Font(L"Corbel", 10.875F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->todayIs->ForeColor = System::Drawing::SystemColors::ButtonFace;
			this->todayIs->Location = System::Drawing::Point(45, 15);
			this->todayIs->Name = L"todayIs";
			this->todayIs->Size = System::Drawing::Size(117, 36);
			this->todayIs->TabIndex = 12;
			this->todayIs->Text = L"Today is";
			// 
			// nowShowing
			// 
			this->nowShowing->AutoSize = true;
			this->nowShowing->Font = (gcnew System::Drawing::Font(L"Corbel", 10.125F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->nowShowing->ForeColor = System::Drawing::SystemColors::ButtonFace;
			this->nowShowing->Location = System::Drawing::Point(1632, 603);
			this->nowShowing->Name = L"nowShowing";
			this->nowShowing->Size = System::Drawing::Size(118, 33);
			this->nowShowing->TabIndex = 13;
			this->nowShowing->Text = L"Showing";
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Font = (gcnew System::Drawing::Font(L"Corbel", 10.125F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->label1->ForeColor = System::Drawing::SystemColors::ButtonFace;
			this->label1->Location = System::Drawing::Point(1715, 57);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(194, 33);
			this->label1->TabIndex = 14;
			this->label1->Text = L"Press F1 for help";
			// 
			// TMGUserInterface
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(13, 26);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->AutoSize = true;
			this->BackColor = System::Drawing::Color::LightSlateGray;
			this->ClientSize = System::Drawing::Size(1949, 1032);
			this->Controls->Add(this->label1);
			this->Controls->Add(this->nowShowing);
			this->Controls->Add(this->todayIs);
			this->Controls->Add(this->DisplayState);
			this->Controls->Add(this->label4);
			this->Controls->Add(this->defaultView);
			this->Controls->Add(this->statusDisplay);
			this->Controls->Add(this->welcomeMessage);
			this->Controls->Add(this->userInput);
			this->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 8.25F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedSingle;
			this->Margin = System::Windows::Forms::Padding(2);
			this->MaximizeBox = false;
			this->MinimizeBox = false;
			this->Name = L"TMGUserInterface";
			this->Padding = System::Windows::Forms::Padding(0, 0, 30, 30);
			this->StartPosition = System::Windows::Forms::FormStartPosition::CenterScreen;
			this->Text = L"TimeMaster";
			//this->Load += gcnew System::EventHandler(this, &TMGUserInterface::TMGUserInterface_Load);
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
	
	private: System::Void userInput_KeyPress(System::Object^  sender, System::Windows::Forms::KeyPressEventArgs^  e) {

				 

				 if(e->KeyChar == (char)13){
				
					if (userInput->Text == "quit" || userInput->Text =="exit" || userInput->Text =="q" || userInput->Text =="close"){
						TMTaskListStates *taskListStates = TMTaskListStates::getInstance();
						TMTaskList taskList = taskListStates->getCurrentTaskList();
						taskList.leaveReferenceUponExit();
						Application :: Exit();
					}
					else{
					

					statusDisplay->Text = "";
					String ^ str = userInput->Text;
					std::string unmanaged = msclr::interop::marshal_as<std::string>(str);
					
					TMExecutor* exe = TMExecutor::getInstance();
					String ^ displayString;
					
					exe->executeMain(unmanaged);
					statusDisplay->Text = gcnew String(exe->returnResultOfExecution().c_str());
					
					
					TMDisplay display = exe->getCurrentDisplay();
					
					
					TMTaskListStates *taskListStates = TMTaskListStates::getInstance();
					TMTaskList taskList = taskListStates->getCurrentTaskList();
					
					
					std::vector<TMTask> dated = taskList.getDated();
					std::vector<TMTask> undated = taskList.getUndated();
					std::vector<TMTask> archived = taskList.getArchived();
					std::vector<TMTask> allTasks;
					allTasks.reserve( dated.size() + undated.size());
					allTasks.insert( allTasks.end(), dated.begin(), dated.end() );
					allTasks.insert( allTasks.end(), undated.begin(), undated.end() );
			
					std::vector<TMTask>::iterator iter;
	
					switch (display) {
					case Default:
					DisplayState->Text = "Default display";
					ListViewItem^ defaultEntry;
					defaultView->Items->Clear();
					for(int i=0; i != allTasks.size() ; ++i){
						
						defaultEntry = gcnew ListViewItem(Convert::ToString(i+1));
						defaultEntry->SubItems->Add(gcnew String(( (allTasks[i].getTaskDescription()).c_str() )));
						if(allTasks[i].getTaskType() == TaskType ::WithEndDateTime){
							defaultEntry->SubItems->Add("");
							defaultEntry->SubItems->Add(""); 
						}
						else{
						defaultEntry->SubItems->Add(gcnew String(( (allTasks[i].getTaskTime().getStartDate()).c_str() )));
						defaultEntry->SubItems->Add(gcnew String(( (allTasks[i].getTaskTime().getStartTime()).c_str() )));
						}
						
						defaultEntry->SubItems->Add(gcnew String(( (allTasks[i].getTaskTime().getEndDate()).c_str() )));
						defaultEntry->SubItems->Add(gcnew String(( (allTasks[i].getTaskTime().getEndTime()).c_str() )));
						

						if(allTasks[i].getTaskType() == TaskType ::WithEndDateTime){
							defaultEntry->ForeColor = Color :: Red;
						}

						if(allTasks[i].getTaskType() == TaskType ::Undated){
							defaultEntry->ForeColor = Color :: RosyBrown;
						}

						std::string confirmationStatus;
						if (allTasks[i].isConfirmed()) {
							confirmationStatus = "Yes";
						} else {
							confirmationStatus = "No";
							defaultEntry->ForeColor = Color :: Gray;
						}
						defaultEntry->SubItems->Add(gcnew String(( (confirmationStatus.c_str()) )));

						std::string clashStatus;
						if (allTasks[i].isClashed()) {
							clashStatus = "Yes";
							defaultEntry->ForeColor = Color :: Blue;
						} else {
							clashStatus = "No";
						}
						defaultEntry->SubItems->Add(gcnew String(( (clashStatus.c_str()) )));

						std::string completionStatus;
						if (allTasks[i].isCompleted()) {
							completionStatus = "Yes";
							defaultEntry->ForeColor = Color :: ForestGreen;
						} else {
							completionStatus = "No";
						}
						defaultEntry->SubItems->Add(gcnew String(( (completionStatus.c_str()) )));

						defaultView->Items->Add(defaultEntry);
					}
						break;

					case  DeadlineTasks:
						DisplayState->Text = "Deadlined Tasks";
						ListViewItem^ deadlineEntry;
						defaultView->Items->Clear();
						for (int j = 0; j != dated.size(); ++j){
							if(dated[j].getTaskType() == TaskType ::WithEndDateTime){
							
							deadlineEntry = gcnew ListViewItem(Convert::ToString(j+1));
							deadlineEntry->SubItems->Add(gcnew String(( (dated[j].getTaskDescription()).c_str() )));
							deadlineEntry->SubItems->Add("");
							deadlineEntry->SubItems->Add("");
							deadlineEntry->SubItems->Add(gcnew String(( (dated[j].getTaskTime().getEndDate()).c_str() )));
							deadlineEntry->SubItems->Add(gcnew String(( (dated[j].getTaskTime().getEndTime()).c_str() )));
						
							std::string confirmationStatus;
							if (dated[j].isConfirmed()) {
								confirmationStatus = "Yes";
							} else {
								confirmationStatus = "No";
							}
							deadlineEntry->SubItems->Add(gcnew String(( (confirmationStatus.c_str()) )));

							std::string clashStatus = "No";
							deadlineEntry->SubItems->Add(gcnew String(( (clashStatus.c_str()) )));

							std::string completionStatus= "No";
							deadlineEntry->SubItems->Add(gcnew String(( (completionStatus.c_str()) )));
							
							deadlineEntry->ForeColor = Color :: Red;
							defaultView->Items->Add(deadlineEntry);

							}	
						}
						exe->setCurrentDisplay(Default);
						break;
					
					case UndatedTasks:
						DisplayState->Text = "Undated Tasks";
						ListViewItem^ floatingEntry;
						defaultView->Items->Clear();
						for (int k = dated.size(); k != dated.size() + undated.size(); ++k){
							floatingEntry = gcnew ListViewItem(Convert::ToString(k+1));
							floatingEntry->SubItems->Add(gcnew String(( (undated[k-dated.size()].getTaskDescription()).c_str() )));
							floatingEntry->SubItems->Add("");
							floatingEntry->SubItems->Add("");
							floatingEntry->SubItems->Add("");
							floatingEntry->SubItems->Add("");

							std::string confirmationStatus;
							if (undated[k-dated.size()].isConfirmed()) {
								confirmationStatus = "Yes";
							} else {
								confirmationStatus = "No";
							}
							floatingEntry->SubItems->Add(gcnew String(( (confirmationStatus.c_str()) )));

							std::string clashStatus = "No";
							floatingEntry->SubItems->Add(gcnew String(( (clashStatus.c_str()) )));

							std::string completionStatus= "No";
							floatingEntry->SubItems->Add(gcnew String(( (completionStatus.c_str()) )));
							
							floatingEntry->ForeColor = Color :: RosyBrown;
							defaultView->Items->Add(floatingEntry);
							exe->setCurrentDisplay(Default);
						}
						break;
					
					case ArchivedTasks:
						DisplayState->Text = "Archived Tasks";
						ListViewItem^ archivedEntry;
						defaultView->Items->Clear();
						for (int l = dated.size()+undated.size(); l != dated.size() + undated.size()+archived.size(); ++l){
							archivedEntry = gcnew ListViewItem(Convert::ToString(l+1));
							archivedEntry->SubItems->Add(gcnew String(( (archived[l-dated.size()-undated.size()].getTaskDescription()).c_str() )));
							if(archived[l-dated.size()-undated.size()].getTaskType() == TaskType ::WithEndDateTime){
								archivedEntry->SubItems->Add("");
								archivedEntry->SubItems->Add("");
							}
							else{
							
							archivedEntry->SubItems->Add(gcnew String(( (archived[l-dated.size()-undated.size()].getTaskTime().getStartDate()).c_str() )));
							archivedEntry->SubItems->Add(gcnew String(( (archived[l-dated.size()-undated.size()].getTaskTime().getStartTime()).c_str() )));
							
							}
							
							archivedEntry->SubItems->Add(gcnew String(( (archived[l-dated.size()-undated.size()].getTaskTime().getEndDate()).c_str() )));
							archivedEntry->SubItems->Add(gcnew String(( (archived[l-dated.size()-undated.size()].getTaskTime().getEndTime()).c_str() )));
						
							std::string confirmationStatus;
							if (archived[l-dated.size()-undated.size()].isConfirmed()) {
								confirmationStatus = "Yes";
							} else {
								confirmationStatus = "No";
							}
							archivedEntry->SubItems->Add(gcnew String(( (confirmationStatus.c_str()) )));

							std::string clashStatus = "No";
							archivedEntry->SubItems->Add(gcnew String(( (clashStatus.c_str()) )));

							std::string completionStatus= "Yes";
							archivedEntry->SubItems->Add(gcnew String(( (completionStatus.c_str()) )));
							
							archivedEntry->ForeColor = Color :: ForestGreen;
							defaultView->Items->Add(archivedEntry);
							exe->setCurrentDisplay(Default);
						}
						break;

					
					case SearchResults:
						DisplayState->Text = "Search results";
						ListViewItem^ searchResult;
						defaultView->Items->Clear();
						
						std::vector<int> indexes = exe->getPositionIndexes();
						std::vector<int>::iterator iter;
						
						for (iter = indexes.begin(); iter != indexes.end(); ++iter) {
							TMTask task = taskList.getTaskFromPositionIndex(*iter);
							searchResult = gcnew ListViewItem(Convert::ToString(*iter));
							searchResult->SubItems->Add(gcnew String(( (task.getTaskDescription()).c_str() )));
							if(task.getTaskType() == TaskType ::WithEndDateTime){
								searchResult->SubItems->Add("");
								searchResult->SubItems->Add("");
							}
							else{
								searchResult->SubItems->Add(gcnew String(( (task.getTaskTime().getStartDate()).c_str() )));
								searchResult->SubItems->Add(gcnew String(( (task.getTaskTime().getStartTime()).c_str() )));
							}
							searchResult->SubItems->Add(gcnew String(( (task.getTaskTime().getEndDate()).c_str() )));
							searchResult->SubItems->Add(gcnew String(( (task.getTaskTime().getEndTime()).c_str() )));
						
							if(task.getTaskType() == TaskType ::WithEndDateTime){
								searchResult->ForeColor = Color :: Red;
							}

							if(task.getTaskType() == TaskType ::Undated){
								searchResult->ForeColor = Color :: RosyBrown;
							}


							std::string confirmationStatus;
							if (task.isConfirmed()) {
								confirmationStatus = "Yes";
							} else {
								confirmationStatus = "No";
								searchResult->ForeColor = Color :: Gray;
							}
							searchResult->SubItems->Add(gcnew String(( (confirmationStatus.c_str()) )));

							std::string clashStatus;
							if (task.isClashed()) {
								clashStatus = "Yes";
								searchResult->ForeColor = Color :: Blue;
							} else {
								clashStatus = "No";
							}
							searchResult->SubItems->Add(gcnew String(( (clashStatus.c_str()) )));

							std::string completionStatus;
							if (task.isCompleted()) {
								completionStatus = "Yes";
								searchResult->ForeColor = Color :: ForestGreen;
							} else {
								completionStatus = "No";
							}
							searchResult->SubItems->Add(gcnew String(( (completionStatus.c_str()) )));

							
							defaultView->Items->Add(searchResult);
							
						}
						exe->setCurrentDisplay(Default);
						break;
					}
					userInput->Clear();
				 }
				 
			 }
			
			 
			 
}
			
			 
			 
	
	
	private: System::Void userInput_TextChanged(System::Object^  sender, System::EventArgs^  e) {

				 if (userInput->Text == "a") {
					statusDisplay->Text = "add <task description> {{<time markers> <time/time period>} {<date markers> <date/date period>}/{day}}";
				 }

				 if (userInput->Text == "d") {
					statusDisplay->Text = "delete <ID>";
				 }
				 
				 if (userInput->Text == "e") {
					statusDisplay->Text = "edit <task number> <task component> <new task description or timing or completion status or confirmation status>";
				 }
					 
				if (userInput->Text == "s") {
					statusDisplay->Text = "search <keyword(s)>";
				}
			 }



private: System::Void timer1_Tick(System::Object^  sender, System::EventArgs^  e) {
			 DateTime time = DateTime::Now;
			 String^ format = "f";
			 
			 label4->Text = time.ToString(format);	
		 }
/*private: System::Void TMGUserInterface_Load(System::Object^  sender, System::EventArgs^  e) {
		 TMTaskListStates *taskListStates = TMTaskListStates::getInstance();
		 }*/

private: System::Void userInput_KeyDown(System::Object^  sender, System::Windows::Forms::KeyEventArgs^  e) {
			
			 if(e->KeyCode == Keys:: Down){
					defaultView->Focus();
					SendKeys :: SendWait ("{PGDN}");
					userInput->Focus();
			 }

			 if(e->KeyCode == Keys:: Up){
					defaultView->Focus();
					SendKeys :: SendWait ("{PGUP}");
					userInput->Focus();
			 }
			 
			 if(e->KeyCode == Keys::F1){
				 ShellExecuteA(NULL,"open","..\\readme.pdf",NULL,NULL,0);
			 }

		 }
};
}
