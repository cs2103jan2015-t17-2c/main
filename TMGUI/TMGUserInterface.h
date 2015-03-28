#pragma once
#include <iostream>
#include <string>
#include <cstdlib>
#include <vector>
#include <Windows.h>
#include <msclr\marshal_cppstd.h>

#include "TMUserInterface.h"
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
	private: System::Windows::Forms::RichTextBox^  richTextBox1;

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
			this->richTextBox1 = (gcnew System::Windows::Forms::RichTextBox());
			this->SuspendLayout();
			// 
			// userInput
			// 
			this->userInput->Location = System::Drawing::Point(99, 694);
			this->userInput->Margin = System::Windows::Forms::Padding(4);
			this->userInput->Name = L"userInput";
			this->userInput->Size = System::Drawing::Size(932, 31);
			this->userInput->TabIndex = 1;
			this->userInput->TextChanged += gcnew System::EventHandler(this, &TMGUserInterface::userInput_TextChanged);
			this->userInput->KeyPress += gcnew System::Windows::Forms::KeyPressEventHandler(this, &TMGUserInterface::userInput_KeyPress);
			// 
			// welcomeMessage
			// 
			this->welcomeMessage->AutoSize = true;
			this->welcomeMessage->Location = System::Drawing::Point(94, 653);
			this->welcomeMessage->Margin = System::Windows::Forms::Padding(4, 0, 4, 0);
			this->welcomeMessage->Name = L"welcomeMessage";
			this->welcomeMessage->Size = System::Drawing::Size(530, 25);
			this->welcomeMessage->TabIndex = 2;
			this->welcomeMessage->Text = L"Welcome to TimeMaster. Please enter your command:";
			// 
			// richTextBox1
			// 
			this->richTextBox1->AcceptsTab = true;
			this->richTextBox1->Font = (gcnew System::Drawing::Font(L"Calisto MT", 7.875F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->richTextBox1->Location = System::Drawing::Point(99, 54);
			this->richTextBox1->Name = L"richTextBox1";
			this->richTextBox1->ReadOnly = true;
			this->richTextBox1->Size = System::Drawing::Size(1120, 575);
			this->richTextBox1->TabIndex = 3;
			this->richTextBox1->Text = L"";
			// 
			// TMGUserInterface
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(12, 25);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(1337, 881);
			this->Controls->Add(this->richTextBox1);
			this->Controls->Add(this->welcomeMessage);
			this->Controls->Add(this->userInput);
			this->Margin = System::Windows::Forms::Padding(4);
			this->MaximizeBox = false;
			this->Name = L"TMGUserInterface";
			this->Text = L"TMGUserInterface";
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
	
	private: System::Void userInput_KeyPress(System::Object^  sender, System::Windows::Forms::KeyPressEventArgs^  e) {
				
				 if(e->KeyChar == (char)13){
					 richTextBox1->Text="";
					 std::ostringstream oss,HEADERS,DEADLINE,TIMED,FLOATING;
					 DEADLINE.clear();
					
				
					
					String ^ str = userInput->Text;
					std::string unmanaged = msclr::interop::marshal_as<std::string>(str);

					TMParser *parser = TMParser::getInstance(); 

					TMUserInterface *ui = TMUserInterface::getInstance();
					
					TMTaskListStates *taskListStates = TMTaskListStates::getInstance();

					TMCommandCreator cmdCreator;
				
				
					parser->initialize(unmanaged);
					
					std::string command = parser->extractCommand();
					TMCommand* commandObjPtr = cmdCreator.createNewCommandObj(parser->determineCommandType(command));
					commandObjPtr->execute();
					
					
					
					TMTaskList taskList = taskListStates->getCurrentTaskList();

					std::vector<TMTask> dated = taskList.getDated();
					std::vector<TMTask> undated = taskList.getUndated();
					std::vector<TMTask> archived = taskList.getArchived();

					oss<< "Number of scheduled tasks: " << dated.size() << "\n";
					oss << "Number of undated tasks:" << undated.size() << "\n";
					oss << "Details:" << "\n\n";

					String^ str2 = gcnew String(oss.str().c_str());
					richTextBox1->SelectionColor = Color :: Red;
					richTextBox1->SelectedText = str2 ;
					

					HEADERS << std :: left << std :: setw(18) << std :: setfill(' ') << "TASK DECSCRIPTION" << "\t" <<
					"START DATE" << "\t" << "START TIME" << "\t" << "END DATE" <<
					"\t" << "END TIME" << "\n" ;

					
					String^ str3 = gcnew String(HEADERS.str().c_str());
					richTextBox1->SelectionColor = Color :: Green;
					richTextBox1->SelectedText = str3 ;
					

					std::vector<TMTask>::iterator iter;
		
					for (iter = dated.begin(); iter != dated.end(); ++iter) {
						if ((*iter).getTaskType() == TaskType::WithEndDateTime) {
			
						DEADLINE << std :: left << std :: setw(18) << std :: setfill(' ')  << (*iter).getTaskDescription() << "\t\t\t" <<
						std :: setw (18) << "\t\t" << (*iter).getTaskTime().getEndDate() <<
						"\t" << (*iter).getTaskTime().getEndTime() <<std::endl;

						String^ str4 = gcnew String(DEADLINE.str().c_str());
						richTextBox1->SelectionColor = Color :: Navy;
						richTextBox1->SelectedText = str4 ;

						
			
						
						} else {
								if (iter->isClashed()) {
								} 
								TIMED << std :: left << std :: setw(18) << std :: setfill(' ') << (*iter).getTaskDescription() << "\t\t" << (*iter).getTaskTime().getStartDate() << 
								"\t" << (*iter).getTaskTime().getStartTime() << 
								"\t\t" << (*iter).getTaskTime().getEndDate() <<
								"\t" << (*iter).getTaskTime().getEndTime() <<std::endl;

								String^ str5 = gcnew String(TIMED.str().c_str());
								richTextBox1->SelectionColor = Color :: Purple;
								richTextBox1->SelectedText = str5 ;
		}
	}
	
	for (iter = undated.begin(); iter != undated.end(); ++iter) {
		
		FLOATING << (*iter).getTaskDescription() << std::endl;
		String^ str6 = gcnew String(FLOATING.str().c_str());
		richTextBox1->SelectionColor = Color :: YellowGreen;
		richTextBox1->SelectedText = str6 ;
		
	}

	
	
					
					
					
	
					
	userInput->Clear();
				 }
			 }
	private: System::Void displayScreen_TextChanged(System::Object^  sender, System::EventArgs^  e) {
			 }
	private: System::Void userInput_TextChanged(System::Object^  sender, System::EventArgs^  e) {
		 }
private: System::Void dataGridView1_CellContentClick(System::Object^  sender, System::Windows::Forms::DataGridViewCellEventArgs^  e) {
		 }
};
}
