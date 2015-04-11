#pragma once
#include <iostream>
#include <string>
#include <cstdlib>
#include <vector>
#include <Windows.h>
#include <msclr\marshal_cppstd.h>
#include <boost\date_time.hpp>
#include "TMExecutor.h"
#include "CurrentDateAndTime.h"
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
			//Thread^ splash = gcnew Thread( gcnew ThreadStart (this,&TMGUI :: TMGUserInterface :: SplashStart));
			//splash->Start();
			//Thread::Sleep(2450);
			InitializeComponent();
			//splash->Abort();
			//this->Show();
			//Activate();
		}

	public:
		static String^ DISPLAY_DEFAULT = "Default display";
		static String^ DISPLAY_DEADLINED = "Deadlined Tasks";
		static String^ DISPLAY_UNDATED = "Undated Tasks";
		static String^ DISPLAY_ARCHIVED = "Archived Tasks";
		static String^ DISPLAY_SEARCH_RESULTS = "Search Results";
		static String^ MESSAGE_ADD = "Adding the following task ";
		static String^ MESSAGE_PROCESSING_IDS = "Task ID(s) to be processed: ";
		static String^ MESSAGE_TASK_ID = "Task ID: ";
		static String^ MESSAGE_CONFIRMED = "Yes";
		static String^ MESSAGE_UNCONFIRMED = "No";
		static String^ DISPLAY_TASK_DESCRIPTION = "Task Description: ";
		static String^ DISPLAY_START_DATE = "Start Date: ";
		static String^ DISPLAY_START_TIME = "Start Time: ";
		static String^ DISPLAY_END_DATE = "End Date: ";
		static String^ DISPLAY_END_TIME = "End Time: ";
		static String^ DISPLAY_DUE_DATE = "Due Date: ";
		static String^ DISPLAY_DUE_TIME = "Due Time: ";
		static String^ DISPLAY_INVALID = "Invalid time, please re-enter task time.";
		static String^ ICON_PASSED_DEADLINE = "!";
		static String^ DISPLAY_BLANK = "";
		
		

		void SplashStart(){
			Application::Run(gcnew TMSplash);
		}

		void processRealTime(){
			
			 TMParser* inputParser = TMParser :: getInstance();
			 String ^ str = userInput->Text;
			 
			 std::string unmanaged = msclr::interop::marshal_as<std::string>(str);

			 inputParser->initialize(unmanaged);
					
			 std::string command = inputParser->extractCommand();
			 TMParser :: CommandTypes commandType = inputParser->determineCommandType(command);
			 
				if(commandType == TMParser :: CommandTypes :: Add){
					TMTask task = inputParser->parseTaskInfo();
					statusDisplay -> Text = MESSAGE_ADD + "\n" + printResultRealTime(task);
				}

				else if(commandType == TMParser :: CommandTypes :: Delete || commandType == TMParser :: CommandTypes :: Complete || commandType == TMParser :: CommandTypes :: Incomplete){
					std::vector<int> tasksID = inputParser->parseTaskPositionNo();
					std::vector<int>::iterator iter;
					std::string idNumbers;
					for (iter = tasksID.begin(); iter != tasksID.end(); ++iter){
						idNumbers = idNumbers + std :: to_string (*iter) + ", ";
					} 

					statusDisplay->Text = MESSAGE_PROCESSING_IDS + gcnew String(idNumbers.c_str());
				}

				else if(commandType == TMParser :: CommandTypes :: Edit){
					std :: string taskID;
					taskID = inputParser->extractTokenAfterCommand();
					statusDisplay -> Text = MESSAGE_TASK_ID;
					if(taskID != "") {
						TMTask newTask = inputParser->parseTaskInfo();
						statusDisplay -> Text = MESSAGE_TASK_ID + gcnew String(taskID.c_str()) + "\n" + printResultRealTime(newTask);
					}
				}
		}
		
		void saveAndQuit(){
			TMTaskList taskList = initiateTaskList();
			taskList.leaveReferenceUponExit();
		}


		String^ printResultRealTime(TMTask task){
			
			String^ taskDetails=
			DISPLAY_TASK_DESCRIPTION + gcnew String(task.getTaskDescription().c_str()) + "\n"+
			DISPLAY_START_DATE + gcnew String(task.getTaskTime().getStartDate().c_str()) + "\t\t\t" + 
			DISPLAY_START_TIME + gcnew String(task.getTaskTime().getStartTime().c_str()) + "\n"+
			DISPLAY_END_DATE + gcnew String(task.getTaskTime().getEndDate().c_str()) + "\t\t\t" + 
			DISPLAY_END_TIME + gcnew String(task.getTaskTime().getEndTime().c_str());
		
			if (task.getTaskType() == TaskType :: WithEndDateTime){
				taskDetails = 
				DISPLAY_TASK_DESCRIPTION + gcnew String(task.getTaskDescription().c_str()) + "\n"+
				DISPLAY_DUE_DATE + gcnew String(task.getTaskTime().getEndDate().c_str()) + "\t\t\t" + 
				DISPLAY_DUE_TIME + gcnew String(task.getTaskTime().getEndTime().c_str());
			}
			
			if(task.getTaskType() == TaskType ::WithStartDateTime){
				taskDetails = 
				DISPLAY_TASK_DESCRIPTION + gcnew String(task.getTaskDescription().c_str()) + "\n"+
				DISPLAY_START_DATE + gcnew String(task.getTaskTime().getStartDate().c_str()) + "\t\t\t" + 
				DISPLAY_START_TIME + gcnew String(task.getTaskTime().getStartTime().c_str());
			}

			if(task.getTaskType() == TaskType :: Invalid){
				taskDetails = DISPLAY_INVALID;
			}

			return taskDetails;
		}

		//defaulTasks do not include archived tasks as these will not be displayed by default
		std::vector<TMTask> initiateDefaultTasks(TMTaskList taskList){
			
			std::vector<TMTask> dated = taskList.getDated();
			std::vector<TMTask> undated = taskList.getUndated();
			std::vector<TMTask> defaultTasks;

			defaultTasks.reserve( dated.size() + undated.size());
			defaultTasks.insert( defaultTasks.end(), dated.begin(), dated.end() );
			defaultTasks.insert( defaultTasks.end(), undated.begin(), undated.end() );

			return defaultTasks;
		}

		std::vector<TMTask> initiateAllTasks(TMTaskList taskList){
			
			std::vector<TMTask> archived = taskList.getArchived();
			std::vector<TMTask> defaultTasks = initiateDefaultTasks(taskList);
			std::vector<TMTask> allTasks;

			allTasks.reserve( defaultTasks.size() + archived.size());
			allTasks.insert( allTasks.end(), defaultTasks.begin(), defaultTasks.end() );
			allTasks.insert( allTasks.end(), archived.begin(), archived.end() );
					
			return allTasks;
		}
		
		TMTaskList initiateTaskList(){
			TMTaskListStates *taskListStates = TMTaskListStates::getInstance();
			TMTaskList taskList = taskListStates->getCurrentTaskList();

			return taskList;
		}

		void clearListView(){
			defaultView->Items->Clear();
		}

		void displayTasks(std::vector<TMTask> taskList, int index, int taskPosition){
			
			TMExecutor* exe = TMExecutor::getInstance();
			std::vector<int> indexes = exe->getPositionIndexes();	

			ListViewItem^ defaultEntry;
			defaultEntry = gcnew ListViewItem(Convert::ToString(index));
			defaultEntry->SubItems->Add(gcnew String(( (taskList[taskPosition].getTaskDescription()).c_str() )));
			if(taskList[taskPosition].getTaskType() == TaskType ::WithEndDateTime){
				defaultEntry->SubItems->Add(DISPLAY_BLANK);
				defaultEntry->SubItems->Add(DISPLAY_BLANK); 
			} else{
				defaultEntry->SubItems->Add(gcnew String(( (taskList[taskPosition].getTaskTime().getStartDate()).c_str() )));
				defaultEntry->SubItems->Add(gcnew String(( (taskList[taskPosition].getTaskTime().getStartTime()).c_str() )));
			}
				
			if(taskList[taskPosition].getTaskType() == TaskType ::WithStartDateTime){
				defaultEntry->SubItems->Add(DISPLAY_BLANK);
				defaultEntry->SubItems->Add(DISPLAY_BLANK); 
			} else{
				defaultEntry->SubItems->Add(gcnew String(( (taskList[taskPosition].getTaskTime().getEndDate()).c_str() )));
				defaultEntry->SubItems->Add(gcnew String(( (taskList[taskPosition].getTaskTime().getEndTime()).c_str() )));
				}

			if(taskList[taskPosition].getTaskType() == TaskType ::WithEndDateTime){
				defaultEntry->ForeColor = Color :: Red;
			}

			std::string confirmationStatus;
						
			if (taskList[taskPosition].isConfirmed()) {
				defaultEntry->SubItems->Add(MESSAGE_CONFIRMED);
			} else {
				defaultEntry->SubItems->Add(MESSAGE_UNCONFIRMED);
				defaultEntry->ForeColor = Color :: Gray;
			}
				
			if (taskList[taskPosition].isClashed()) {
				defaultEntry->ForeColor = Color :: Blue;
			} 
				
			if (taskList[taskPosition].isCompleted()) {
				defaultEntry->ForeColor = Color :: ForestGreen;
			} 
				
			boost::gregorian::date dateToday = currentDate();
			std::string timeNow = currentTime();
			
			if (taskList[taskPosition].getTaskTime().getEndBoostDate() < dateToday){
				defaultEntry->SubItems->Add(ICON_PASSED_DEADLINE);
				defaultEntry->Font = gcnew System::Drawing::Font ("Corbel",11,FontStyle :: Bold);
			}

			if (taskList[taskPosition].getTaskTime().getEndBoostDate() == dateToday){
				if(taskList[taskPosition].getTaskTime().getEndTime() < timeNow){
					defaultEntry->SubItems->Add(ICON_PASSED_DEADLINE);
					defaultEntry->Font = gcnew System::Drawing::Font ("Corbel",11,FontStyle :: Bold);
				}
			}
				
			std::vector<int>::iterator iter;
			for (iter = indexes.begin(); iter != indexes.end(); ++iter){
				if(index == (*iter)){
					defaultEntry -> BackColor = Color :: Yellow;
				}
			}

			defaultView->Items->Add(defaultEntry);
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
	private: System::Windows::Forms::Label^  displayTime;
	private: System::Windows::Forms::Timer^  timer1;
	private: System::Windows::Forms::Label^  DisplayState;
	private: System::Windows::Forms::Label^  todayIs;
	private: System::Windows::Forms::Label^  nowShowing;
	private: System::Windows::Forms::Label^  label1;
	private: System::Windows::Forms::ColumnHeader^  hasPassed;
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
			System::ComponentModel::ComponentResourceManager^  resources = (gcnew System::ComponentModel::ComponentResourceManager(TMGUserInterface::typeid));
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
			this->hasPassed = (gcnew System::Windows::Forms::ColumnHeader());
			this->displayTime = (gcnew System::Windows::Forms::Label());
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
			this->userInput->Location = System::Drawing::Point(52, 845);
			this->userInput->Margin = System::Windows::Forms::Padding(2);
			this->userInput->Name = L"userInput";
			this->userInput->Size = System::Drawing::Size(1868, 50);
			this->userInput->TabIndex = 0;
			this->userInput->KeyDown += gcnew System::Windows::Forms::KeyEventHandler(this, &TMGUserInterface::userInput_KeyDown);
			this->userInput->KeyPress += gcnew System::Windows::Forms::KeyPressEventHandler(this, &TMGUserInterface::userInput_KeyPress);
			// 
			// welcomeMessage
			// 
			this->welcomeMessage->AutoSize = true;
			this->welcomeMessage->Font = (gcnew System::Drawing::Font(L"Corbel", 12, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->welcomeMessage->ForeColor = System::Drawing::SystemColors::ButtonFace;
			this->welcomeMessage->Location = System::Drawing::Point(44, 804);
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
			this->statusDisplay->Location = System::Drawing::Point(51, 629);
			this->statusDisplay->Margin = System::Windows::Forms::Padding(4, 3, 4, 3);
			this->statusDisplay->Name = L"statusDisplay";
			this->statusDisplay->ReadOnly = true;
			this->statusDisplay->Size = System::Drawing::Size(1869, 172);
			this->statusDisplay->TabIndex = 6;
			this->statusDisplay->Text = L"";
			// 
			// defaultView
			// 
			this->defaultView->BackColor = System::Drawing::SystemColors::HighlightText;
			this->defaultView->Columns->AddRange(gcnew cli::array< System::Windows::Forms::ColumnHeader^  >(8) {this->taskID, this->taskDescription, 
				this->startDate, this->startTime, this->endDate, this->endTime, this->confirmation, this->hasPassed});
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
			this->taskDescription->Width = 360;
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
			// hasPassed
			// 
			this->hasPassed->Text = L"";
			this->hasPassed->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
			this->hasPassed->Width = 25;
			// 
			// displayTime
			// 
			this->displayTime->AutoSize = true;
			this->displayTime->Font = (gcnew System::Drawing::Font(L"Corbel", 12, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->displayTime->ForeColor = System::Drawing::SystemColors::ButtonFace;
			this->displayTime->Location = System::Drawing::Point(44, 51);
			this->displayTime->Margin = System::Windows::Forms::Padding(2, 0, 2, 0);
			this->displayTime->Name = L"displayTime";
			this->displayTime->Size = System::Drawing::Size(182, 39);
			this->displayTime->TabIndex = 10;
			this->displayTime->Text = L"currentTime";
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
			this->DisplayState->Location = System::Drawing::Point(283, 582);
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
			this->nowShowing->Location = System::Drawing::Point(46, 586);
			this->nowShowing->Name = L"nowShowing";
			this->nowShowing->Size = System::Drawing::Size(214, 33);
			this->nowShowing->TabIndex = 13;
			this->nowShowing->Text = L"The above shows";
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
			this->Controls->Add(this->displayTime);
			this->Controls->Add(this->defaultView);
			this->Controls->Add(this->statusDisplay);
			this->Controls->Add(this->welcomeMessage);
			this->Controls->Add(this->userInput);
			this->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 8.25F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedSingle;
			this->Icon = (cli::safe_cast<System::Drawing::Icon^  >(resources->GetObject(L"$this.Icon")));
			this->Margin = System::Windows::Forms::Padding(2);
			this->MaximizeBox = false;
			this->MinimizeBox = false;
			this->Name = L"TMGUserInterface";
			this->Padding = System::Windows::Forms::Padding(0, 0, 30, 30);
			this->StartPosition = System::Windows::Forms::FormStartPosition::CenterScreen;
			this->Text = L"TimeMaster";
			this->Deactivate += gcnew System::EventHandler(this, &TMGUserInterface::TMGUserInterface_Deactivate);
			this->Load += gcnew System::EventHandler(this, &TMGUserInterface::TMGUserInterface_Load);
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
	

private: System::Void userInput_KeyPress(System::Object^  sender, System::Windows::Forms::KeyPressEventArgs^  e) {
			
			 if(e->KeyChar == (char)13){
				 if (userInput->Text == "quit" || userInput->Text =="exit" || userInput->Text =="q" || userInput->Text =="close"){
					 saveAndQuit();
					 Application :: Exit();
				 }
				 else {
					 statusDisplay->Clear();
					 String ^ entry = userInput->Text;
					 std::string originalEntry = msclr::interop::marshal_as<std::string>(entry);
					
					 TMExecutor* exe = TMExecutor::getInstance();
					
					 exe->executeMain(originalEntry);
					 
					 statusDisplay->Text = gcnew String(exe->returnResultOfExecution().c_str());
				
					 TMDisplay display = exe->getCurrentDisplay();
					
					 TMTaskList taskList = initiateTaskList();
					
					 std::vector<TMTask> dated = taskList.getDated();
					 std::vector<TMTask> undated = taskList.getUndated();
					 std::vector<TMTask> archived = taskList.getArchived();

					 std::vector<TMTask> defaultTasks = initiateDefaultTasks(taskList);
					 std::vector<TMTask> allTasks = initiateAllTasks(taskList);
			
					 int defaultCountStart = 0;
					 int defaultCountEnd = defaultTasks.size();
					
					 int undatedCountStart = dated.size();
					 int undatedCountEnd = dated.size() + undated.size();

					 int archivedCountStart = dated.size() + undated.size();
					 int archivedCountEnd = dated.size() + undated.size() + archived.size();

					 std::vector<TMTask>::iterator iter;
	
					 switch (display) {
					 case Default:
						 DisplayState->Text = DISPLAY_DEFAULT;
						 clearListView();

						 for(int i = defaultCountStart; i != defaultCountEnd ; i++){
							 int defaultTaskPosition = i - defaultCountStart;
							 int defaultIndex = i + 1;
							 displayTasks(defaultTasks,defaultIndex,defaultTaskPosition);
						 }
						 break;

					 case  DeadlineTasks:
						 DisplayState->Text = DISPLAY_DEADLINED;
						 clearListView();
						 for (int l = 0; l != dated.size(); l++){
							 if(dated[l].getTaskType() == TaskType ::WithEndDateTime){
								 int deadlinedTaskPosition = l;
								 int deadlinedIndex = l+1;
								 displayTasks(dated,deadlinedIndex,deadlinedTaskPosition);
							 }
						 }
						 break;
					
					case UndatedTasks:
						DisplayState->Text = DISPLAY_UNDATED;
						clearListView();
						
						for(int j = undatedCountStart; j != undatedCountEnd ; j++){
							int undatedTaskPosition = j - undatedCountStart;
							int undatedIndex = j + 1;
							displayTasks(undated,undatedIndex,undatedTaskPosition);
						}
						break;
					
					case ArchivedTasks:
						DisplayState->Text = DISPLAY_ARCHIVED;
						clearListView();
						
						for(int k = archivedCountStart; k != archivedCountEnd ; k++){
							int archivedTaskPosition = k - archivedCountStart;
							int archivedIndex = k + 1;
							displayTasks(archived,archivedIndex,archivedTaskPosition);
						}
						break;

					case SearchResults:
						DisplayState->Text = DISPLAY_SEARCH_RESULTS;
						clearListView();
						
						std::vector<int> indexes = exe->getPositionIndexes();
						std::vector<int>::iterator iter;
						
						for (iter = indexes.begin(); iter != indexes.end(); ++iter) {
							int searchedTaskPosition = *(iter) - 1;
							int searchedIndex = *iter;
							displayTasks(allTasks,searchedIndex,searchedTaskPosition);
						}

						exe->setCurrentDisplay(Default);
						break;
					}
					userInput->Clear();
				 } 
			 }
		}
			
private: System::Void timer1_Tick(System::Object^  sender, System::EventArgs^  e) {
			 DateTime time = DateTime::Now;
			 String^ format = "f";
			 
			 displayTime->Text = time.ToString(format);	

			 //processRealTime();
			
		 }

private: System::Void TMGUserInterface_Load(System::Object^  sender, System::EventArgs^  e) {
			clearListView();
	
			TMTaskList taskList = initiateTaskList();

			std::vector<TMTask> defaultTasks = initiateDefaultTasks (taskList);

			int defaultCountStart = 0;
			int defaultCountEnd = defaultTasks.size();

			for(int i = defaultCountStart; i != defaultCountEnd ; i++){
						int defaultTaskPosition = i - defaultCountStart;
						int defaultIndex = i + 1;
						displayTasks(defaultTasks,defaultIndex,defaultTaskPosition);
			}
		 }

private: System::Void userInput_KeyDown(System::Object^  sender, System::Windows::Forms::KeyEventArgs^  e) {
			
			 if(e->KeyCode == Keys:: PageDown){
					defaultView->Focus();
					SendKeys :: SendWait ("{PGDN}");
					userInput->Focus();
			 } else if(e->KeyCode == Keys:: PageUp){
					defaultView->Focus();
					SendKeys :: SendWait ("{PGUP}");
					userInput->Focus();
			 } else if(e->KeyCode == Keys::F1){
					ShellExecuteA(NULL,"open","..\\readme.pdf",NULL,NULL,0);
			 } 
		 }


private: System::Void TMGUserInterface_Deactivate(System::Object^  sender, System::EventArgs^  e) {
			 saveAndQuit();
		 }
};
}
