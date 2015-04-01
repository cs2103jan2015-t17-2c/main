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

namespace TMGUI {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	/// <summary>
	/// Summary for TMGUserInterface
	/// </summary>
	public ref class TMGUserInterface : public System::Windows::Forms::Form
	{
	public:
		TMGUserInterface(void)
		{
			InitializeComponent();
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


	private: System::Windows::Forms::Label^  label1;
	private: System::Windows::Forms::Label^  label2;
	private: System::Windows::Forms::RichTextBox^  statusDisplay;

	private: System::Windows::Forms::Label^  label3;

	private: System::Windows::Forms::ListView^  defaultView;

	private: System::Windows::Forms::ColumnHeader^  taskID;
	private: System::Windows::Forms::ColumnHeader^  taskDescription;
	private: System::Windows::Forms::ColumnHeader^  startDate;
	private: System::Windows::Forms::ColumnHeader^  startTime;
	private: System::Windows::Forms::ColumnHeader^  endDate;
	private: System::Windows::Forms::ColumnHeader^  endTime;
	private: System::Windows::Forms::ColumnHeader^  confirmation;
<<<<<<< HEAD
	private: System::Windows::Forms::ColumnHeader^  isClash;
	private: System::Windows::Forms::Label^  label4;
=======

>>>>>>> 87cb880ffe07e168b65dc6460dd02d672526121e

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
			this->userInput = (gcnew System::Windows::Forms::TextBox());
			this->welcomeMessage = (gcnew System::Windows::Forms::Label());
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->label2 = (gcnew System::Windows::Forms::Label());
			this->statusDisplay = (gcnew System::Windows::Forms::RichTextBox());
			this->label3 = (gcnew System::Windows::Forms::Label());
			this->defaultView = (gcnew System::Windows::Forms::ListView());
			this->taskID = (gcnew System::Windows::Forms::ColumnHeader());
			this->taskDescription = (gcnew System::Windows::Forms::ColumnHeader());
			this->startDate = (gcnew System::Windows::Forms::ColumnHeader());
			this->startTime = (gcnew System::Windows::Forms::ColumnHeader());
			this->endDate = (gcnew System::Windows::Forms::ColumnHeader());
			this->endTime = (gcnew System::Windows::Forms::ColumnHeader());
			this->confirmation = (gcnew System::Windows::Forms::ColumnHeader());
<<<<<<< HEAD
			this->isClash = (gcnew System::Windows::Forms::ColumnHeader());
			this->label4 = (gcnew System::Windows::Forms::Label());
=======
>>>>>>> 87cb880ffe07e168b65dc6460dd02d672526121e
			this->SuspendLayout();
			// 
			// userInput
			// 
			this->userInput->BackColor = System::Drawing::SystemColors::Window;
			this->userInput->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
<<<<<<< HEAD
			this->userInput->Font = (gcnew System::Drawing::Font(L"Segoe UI", 9.75F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->userInput->Location = System::Drawing::Point(131, 551);
			this->userInput->Margin = System::Windows::Forms::Padding(2);
			this->userInput->Name = L"userInput";
			this->userInput->Size = System::Drawing::Size(674, 25);
=======
			this->userInput->Location = System::Drawing::Point(346, 982);
			this->userInput->Margin = System::Windows::Forms::Padding(4);
			this->userInput->Name = L"userInput";
			this->userInput->Size = System::Drawing::Size(1447, 31);
>>>>>>> 87cb880ffe07e168b65dc6460dd02d672526121e
			this->userInput->TabIndex = 1;
			this->userInput->TextChanged += gcnew System::EventHandler(this, &TMGUserInterface::userInput_TextChanged);
			this->userInput->KeyPress += gcnew System::Windows::Forms::KeyPressEventHandler(this, &TMGUserInterface::userInput_KeyPress);
			// 
			// welcomeMessage
			// 
			this->welcomeMessage->AutoSize = true;
			this->welcomeMessage->Font = (gcnew System::Drawing::Font(L"Segoe UI", 14.25F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
<<<<<<< HEAD
			this->welcomeMessage->Location = System::Drawing::Point(23, 547);
			this->welcomeMessage->Margin = System::Windows::Forms::Padding(2, 0, 2, 0);
=======
			this->welcomeMessage->Location = System::Drawing::Point(70, 952);
			this->welcomeMessage->Margin = System::Windows::Forms::Padding(4, 0, 4, 0);
>>>>>>> 87cb880ffe07e168b65dc6460dd02d672526121e
			this->welcomeMessage->Name = L"welcomeMessage";
			this->welcomeMessage->Size = System::Drawing::Size(103, 25);
			this->welcomeMessage->TabIndex = 2;
			this->welcomeMessage->Text = L"Command:";
			this->welcomeMessage->Click += gcnew System::EventHandler(this, &TMGUserInterface::welcomeMessage_Click);
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Font = (gcnew System::Drawing::Font(L"Segoe UI Symbol", 18, System::Drawing::FontStyle::Italic, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->label1->Location = System::Drawing::Point(706, 25);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(141, 32);
			this->label1->TabIndex = 4;
			this->label1->Text = L"TimeMaster";
			// 
			// label2
			// 
			this->label2->AutoSize = true;
			this->label2->Font = (gcnew System::Drawing::Font(L"Segoe UI Symbol", 11.25F, System::Drawing::FontStyle::Italic, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->label2->Location = System::Drawing::Point(563, 35);
			this->label2->Name = L"label2";
			this->label2->Size = System::Drawing::Size(153, 20);
			this->label2->TabIndex = 5;
			this->label2->Text = L"Master your time with";
			// 
			// statusDisplay
			// 
			this->statusDisplay->BackColor = System::Drawing::SystemColors::ActiveCaptionText;
			this->statusDisplay->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
			this->statusDisplay->Font = (gcnew System::Drawing::Font(L"Segoe UI", 12, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
<<<<<<< HEAD
			this->richTextBox2->ForeColor = System::Drawing::SystemColors::Window;
			this->richTextBox2->Location = System::Drawing::Point(131, 447);
			this->richTextBox2->Name = L"richTextBox2";
			this->richTextBox2->ReadOnly = true;
			this->richTextBox2->Size = System::Drawing::Size(674, 60);
			this->richTextBox2->TabIndex = 6;
			this->richTextBox2->Text = L"";
=======
			this->statusDisplay->ForeColor = System::Drawing::SystemColors::Window;
			this->statusDisplay->Location = System::Drawing::Point(346, 773);
			this->statusDisplay->Margin = System::Windows::Forms::Padding(6);
			this->statusDisplay->Name = L"statusDisplay";
			this->statusDisplay->ReadOnly = true;
			this->statusDisplay->Size = System::Drawing::Size(1447, 192);
			this->statusDisplay->TabIndex = 6;
			this->statusDisplay->Text = L"";
>>>>>>> 87cb880ffe07e168b65dc6460dd02d672526121e
			// 
			// label3
			// 
			this->label3->AutoSize = true;
			this->label3->Font = (gcnew System::Drawing::Font(L"Segoe UI", 14.25F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
<<<<<<< HEAD
			this->label3->Location = System::Drawing::Point(40, 463);
			this->label3->Margin = System::Windows::Forms::Padding(2, 0, 2, 0);
=======
			this->label3->Location = System::Drawing::Point(122, 823);
			this->label3->Margin = System::Windows::Forms::Padding(4, 0, 4, 0);
>>>>>>> 87cb880ffe07e168b65dc6460dd02d672526121e
			this->label3->Name = L"label3";
			this->label3->Size = System::Drawing::Size(67, 25);
			this->label3->TabIndex = 7;
			this->label3->Text = L"Status:";
			// 
			// defaultView
			// 
<<<<<<< HEAD
			this->defaultView->Columns->AddRange(gcnew cli::array< System::Windows::Forms::ColumnHeader^  >(8) {this->taskID, this->taskDescription, 
				this->startDate, this->startTime, this->endDate, this->endTime, this->confirmation, this->isClash});
			this->defaultView->Font = (gcnew System::Drawing::Font(L"Segoe UI", 9.75F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->defaultView->GridLines = true;
			this->defaultView->HeaderStyle = System::Windows::Forms::ColumnHeaderStyle::Nonclickable;
			this->defaultView->Location = System::Drawing::Point(28, 61);
			this->defaultView->Margin = System::Windows::Forms::Padding(2);
			this->defaultView->Name = L"defaultView";
			this->defaultView->Size = System::Drawing::Size(819, 362);
=======
			this->defaultView->Columns->AddRange(gcnew cli::array< System::Windows::Forms::ColumnHeader^  >(7) {this->taskID, this->taskDescription, 
				this->startDate, this->startTime, this->endDate, this->endTime, this->confirmation});
			this->defaultView->Font = (gcnew System::Drawing::Font(L"Rockwell", 10.875F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->defaultView->FullRowSelect = true;
			this->defaultView->Location = System::Drawing::Point(28, 106);
			this->defaultView->Name = L"defaultView";
			this->defaultView->Size = System::Drawing::Size(1765, 645);
>>>>>>> 87cb880ffe07e168b65dc6460dd02d672526121e
			this->defaultView->TabIndex = 9;
			this->defaultView->UseCompatibleStateImageBehavior = false;
			this->defaultView->View = System::Windows::Forms::View::Details;
			// 
			// taskID
			// 
<<<<<<< HEAD
			this->taskID->Text = L"Index";
			this->taskID->Width = 48;
=======
			this->taskID->Text = L"ID";
			this->taskID->Width = 30;
>>>>>>> 87cb880ffe07e168b65dc6460dd02d672526121e
			// 
			// taskDescription
			// 
			this->taskDescription->Text = L"Task Description";
<<<<<<< HEAD
			this->taskDescription->Width = 301;
=======
			this->taskDescription->Width = 260;
>>>>>>> 87cb880ffe07e168b65dc6460dd02d672526121e
			// 
			// startDate
			// 
			this->startDate->Text = L"Start Date";
			this->startDate->Width = 97;
			// 
			// startTime
			// 
			this->startTime->Text = L"Start Time";
<<<<<<< HEAD
			this->startTime->Width = 73;
=======
			this->startTime->Width = 120;
>>>>>>> 87cb880ffe07e168b65dc6460dd02d672526121e
			// 
			// endDate
			// 
			this->endDate->Text = L"End Date";
<<<<<<< HEAD
			this->endDate->Width = 94;
=======
			this->endDate->Width = 120;
>>>>>>> 87cb880ffe07e168b65dc6460dd02d672526121e
			// 
			// endTime
			// 
			this->endTime->Text = L"End Time";
<<<<<<< HEAD
			this->endTime->Width = 66;
			// 
			// confirmation
			// 
			this->confirmation->Text = L"Confirmed\?";
			this->confirmation->Width = 83;
			// 
			// isClash
			// 
			this->isClash->Text = L"Clash\?";
			this->isClash->Width = 53;
			// 
			// label4
			// 
			this->label4->AutoSize = true;
			this->label4->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 12, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->label4->Location = System::Drawing::Point(133, 519);
			this->label4->Name = L"label4";
			this->label4->Size = System::Drawing::Size(334, 20);
			this->label4->TabIndex = 10;
			this->label4->Text = L"Add (a), Undo (u), Redo (r), Delete (d), Edit (e)";
=======
			this->endTime->Width = 120;
			// 
			// confirmation
			// 
			this->confirmation->Text = L"Confirmation";
			this->confirmation->Width = 105;
>>>>>>> 87cb880ffe07e168b65dc6460dd02d672526121e
			// 
			// TMGUserInterface
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
<<<<<<< HEAD
			this->BackColor = System::Drawing::Color::Teal;
			this->ClientSize = System::Drawing::Size(879, 608);
			this->Controls->Add(this->label4);
=======
			this->AutoSize = true;
			this->BackColor = System::Drawing::Color::LightSteelBlue;
			this->ClientSize = System::Drawing::Size(1817, 1122);
>>>>>>> 87cb880ffe07e168b65dc6460dd02d672526121e
			this->Controls->Add(this->defaultView);
			this->Controls->Add(this->label3);
			this->Controls->Add(this->statusDisplay);
			this->Controls->Add(this->label2);
			this->Controls->Add(this->label1);
			this->Controls->Add(this->welcomeMessage);
			this->Controls->Add(this->userInput);
<<<<<<< HEAD
			this->Margin = System::Windows::Forms::Padding(2);
=======
			this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedSingle;
			this->Margin = System::Windows::Forms::Padding(4);
>>>>>>> 87cb880ffe07e168b65dc6460dd02d672526121e
			this->MaximizeBox = false;
			this->MaximumSize = System::Drawing::Size(1843, 1193);
			this->MinimizeBox = false;
			this->MinimumSize = System::Drawing::Size(1843, 1193);
			this->Name = L"TMGUserInterface";
			this->Padding = System::Windows::Forms::Padding(0, 0, 20, 20);
			this->Text = L"TMGUserInterface";
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
	
	private: System::Void userInput_KeyPress(System::Object^  sender, System::Windows::Forms::KeyPressEventArgs^  e) {
				
				 if(e->KeyChar == (char)13){
					
					String ^ str = userInput->Text;
					std::string unmanaged = msclr::interop::marshal_as<std::string>(str);
					
					TMExecutor* exe = TMExecutor::getInstance();
					String ^ displayString;
					
					exe->executeMain(unmanaged);
					statusDisplay->Text = gcnew String(exe->returnResultOfExecution().c_str());
					
					//displayString = displayString + gcnew String((*iter).c_str());
					
					TMTaskListStates *taskListStates = TMTaskListStates::getInstance();
					TMTaskList taskList = taskListStates->getCurrentTaskList();
					std::vector<TMTask> dated = taskList.getDated();
					std::vector<TMTask> undated = taskList.getUndated();
					std::vector<TMTask> archived = taskList.getArchived();

					std::vector<TMTask> allTasks;
					allTasks.reserve( dated.size() + undated.size() ); // preallocate memory
					allTasks.insert( allTasks.end(), dated.begin(), dated.end() );
					allTasks.insert( allTasks.end(), undated.begin(), undated.end() );

					
					std::vector<TMTask>::iterator iter;

					//displayString = displayString + gcnew String((*iter).c_str());
					
					ListViewItem^ entryTimed,entryFloating;

					
					defaultView->Items->Clear();
					for(int i=0; i != allTasks.size() ; ++i){
						
						entryTimed = gcnew ListViewItem(Convert::ToString(i+1));
						entryTimed->SubItems->Add(gcnew String(( (allTasks[i].getTaskDescription()).c_str() )));
						entryTimed->SubItems->Add(gcnew String(( (allTasks[i].getTaskTime().getStartDate()).c_str() )));
						entryTimed->SubItems->Add(gcnew String(( (allTasks[i].getTaskTime().getStartTime()).c_str() )));
						entryTimed->SubItems->Add(gcnew String(( (allTasks[i].getTaskTime().getEndDate()).c_str() )));
						entryTimed->SubItems->Add(gcnew String(( (allTasks[i].getTaskTime().getEndTime()).c_str() )));
						
						std::string confirmationStatus;
						if (allTasks[i].isConfirmed()) {
							confirmationStatus = "Yes";
						} else {
							confirmationStatus = "No";
							entryTimed->ForeColor = Color :: Gray;
						}
						entryTimed->SubItems->Add(gcnew String(( (confirmationStatus.c_str()) )));

						std::string clashStatus;
						if (allTasks[i].isClashed()) {
							clashStatus = "Yes";
							entryTimed->ForeColor = Color :: Blue;
						} else {
							clashStatus = "No";
						}
						entryTimed->SubItems->Add(gcnew String(( (clashStatus.c_str()) )));

						

						/*if(allTasks[i].isClashed()){
							entryTimed->ForeColor = Color :: Blue;
						}*/

						if(allTasks[i].getTaskType() == TaskType ::WithEndDateTime){
							entryTimed->ForeColor = Color :: Red;
						}

						if(allTasks[i].getTaskType() == TaskType ::Undated){
<<<<<<< HEAD
							//entryTimed->ForeColor = Color :: Gold;
=======
							entryTimed->ForeColor = Color :: RosyBrown;
>>>>>>> 87cb880ffe07e168b65dc6460dd02d672526121e
						}

						defaultView->Items->Add(entryTimed);

					}
					userInput->Clear();
				 }
			 }
			 

	
	private: System::Void welcomeMessage_Click(System::Object^  sender, System::EventArgs^  e) {
		 }

	private: System::Void dataGridView1_CellContentClick(System::Object^  sender, System::Windows::Forms::DataGridViewCellEventArgs^  e) {
		 }


	private: System::Void userInput_TextChanged(System::Object^  sender, System::EventArgs^  e) {
				 if (userInput->Text == "add") {
					statusDisplay->Text = "add <TIMEMARKER> <BLA>";
				 } else {
					// richTextBox2->Text = "";
				 }
			 }


};
}
