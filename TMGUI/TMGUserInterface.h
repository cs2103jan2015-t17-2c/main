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
	private: System::Windows::Forms::TextBox^  displayScreen;
	protected: 

	private: System::Windows::Forms::TextBox^  userInput;
	private: System::Windows::Forms::Label^  welcomeMessage;
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
			this->displayScreen = (gcnew System::Windows::Forms::TextBox());
			this->userInput = (gcnew System::Windows::Forms::TextBox());
			this->welcomeMessage = (gcnew System::Windows::Forms::Label());
			this->SuspendLayout();
			// 
			// displayScreen
			// 
			this->displayScreen->BackColor = System::Drawing::SystemColors::ActiveCaptionText;
			this->displayScreen->ForeColor = System::Drawing::SystemColors::Info;
			this->displayScreen->Location = System::Drawing::Point(140, 106);
			this->displayScreen->Margin = System::Windows::Forms::Padding(4, 4, 4, 4);
			this->displayScreen->Multiline = true;
			this->displayScreen->Name = L"displayScreen";
			this->displayScreen->ReadOnly = true;
			this->displayScreen->Size = System::Drawing::Size(932, 369);
			this->displayScreen->TabIndex = 0;
			this->displayScreen->TextChanged += gcnew System::EventHandler(this, &TMGUserInterface::displayScreen_TextChanged);
			this->displayScreen->KeyPress += gcnew System::Windows::Forms::KeyPressEventHandler(this, &TMGUserInterface::userInput_KeyPress);
			// 
			// userInput
			// 
			this->userInput->Location = System::Drawing::Point(140, 594);
			this->userInput->Margin = System::Windows::Forms::Padding(4, 4, 4, 4);
			this->userInput->Name = L"userInput";
			this->userInput->Size = System::Drawing::Size(932, 31);
			this->userInput->TabIndex = 1;
			this->userInput->TextChanged += gcnew System::EventHandler(this, &TMGUserInterface::userInput_TextChanged);
			this->userInput->KeyPress += gcnew System::Windows::Forms::KeyPressEventHandler(this, &TMGUserInterface::userInput_KeyPress);
			// 
			// welcomeMessage
			// 
			this->welcomeMessage->AutoSize = true;
			this->welcomeMessage->Location = System::Drawing::Point(135, 530);
			this->welcomeMessage->Margin = System::Windows::Forms::Padding(4, 0, 4, 0);
			this->welcomeMessage->Name = L"welcomeMessage";
			this->welcomeMessage->Size = System::Drawing::Size(530, 25);
			this->welcomeMessage->TabIndex = 2;
			this->welcomeMessage->Text = L"Welcome to TimeMaster. Please enter your command:";
			// 
			// TMGUserInterface
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(12, 25);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(1231, 814);
			this->Controls->Add(this->welcomeMessage);
			this->Controls->Add(this->userInput);
			this->Controls->Add(this->displayScreen);
			this->Margin = System::Windows::Forms::Padding(4, 4, 4, 4);
			this->MaximizeBox = false;
			this->Name = L"TMGUserInterface";
			this->Text = L"TMGUserInterface";
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
	
	private: System::Void userInput_KeyPress(System::Object^  sender, System::Windows::Forms::KeyPressEventArgs^  e) {
				if(e->KeyChar == (char)13){
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
					std :: string display = ui->displayDefault();
					std :: cout << display;
					String^ str2 = gcnew String(display.c_str());

					displayScreen->Text = str2;
				 }
			 }
	private: System::Void displayScreen_TextChanged(System::Object^  sender, System::EventArgs^  e) {
			 }
	private: System::Void userInput_TextChanged(System::Object^  sender, System::EventArgs^  e) {
			 if (userInput->Text == "add") {
				displayScreen->Text = "add detected";}
		 }
};
}
