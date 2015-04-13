//author @A0114172U
#pragma once

namespace TMGUI {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	/// <summary>
	///  This class is created to produce splash screen at the startup of TimeMaster
	/// </summary>
	public ref class TMSplash : public System::Windows::Forms::Form
	{
	public:
		TMSplash(void)
		{
			InitializeComponent();
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~TMSplash()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::ProgressBar^  progressBar;
	protected: 

	protected: 
	private: System::Windows::Forms::Timer^  timer1;
	private: System::Windows::Forms::Label^  title;
	private: System::Windows::Forms::Label^  initialize;
	private: System::Windows::Forms::Label^  initials;
	private: System::Windows::Forms::Label^  slogan;
	private: System::ComponentModel::IContainer^  components;

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
			System::ComponentModel::ComponentResourceManager^  resources = (gcnew System::ComponentModel::ComponentResourceManager(TMSplash::typeid));
			this->progressBar = (gcnew System::Windows::Forms::ProgressBar());
			this->timer1 = (gcnew System::Windows::Forms::Timer(this->components));
			this->title = (gcnew System::Windows::Forms::Label());
			this->initialize = (gcnew System::Windows::Forms::Label());
			this->initials = (gcnew System::Windows::Forms::Label());
			this->slogan = (gcnew System::Windows::Forms::Label());
			this->SuspendLayout();
			// 
			// progressBar
			// 
			this->progressBar->Location = System::Drawing::Point(198, 474);
			this->progressBar->Name = L"progressBar";
			this->progressBar->Size = System::Drawing::Size(471, 39);
			this->progressBar->TabIndex = 0;
			// 
			// timer1
			// 
			this->timer1->Enabled = true;
			this->timer1->Interval = 32;
			this->timer1->Tick += gcnew System::EventHandler(this, &TMSplash::timer1_Tick);
			// 
			// title
			// 
			this->title->AutoSize = true;
			this->title->Font = (gcnew System::Drawing::Font(L"Corbel", 19.875F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->title->ForeColor = System::Drawing::SystemColors::ButtonFace;
			this->title->Location = System::Drawing::Point(288, 111);
			this->title->Name = L"title";
			this->title->Size = System::Drawing::Size(292, 64);
			this->title->TabIndex = 1;
			this->title->Text = L"TimeMaster";
			// 
			// initialize
			// 
			this->initialize->AutoSize = true;
			this->initialize->Font = (gcnew System::Drawing::Font(L"Corbel", 7.875F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->initialize->Location = System::Drawing::Point(368, 446);
			this->initialize->Name = L"initialize";
			this->initialize->Size = System::Drawing::Size(119, 26);
			this->initialize->TabIndex = 2;
			this->initialize->Text = L"Initializing...";
			// 
			// initials
			// 
			this->initials->AutoSize = true;
			this->initials->Font = (gcnew System::Drawing::Font(L"Corbel", 7.875F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->initials->Location = System::Drawing::Point(304, 607);
			this->initials->Name = L"initials";
			this->initials->Size = System::Drawing::Size(245, 26);
			this->initials->TabIndex = 3;
			this->initials->Text = L"© GA|NWE|NXL|TKM 2015";
			// 
			// slogan
			// 
			this->slogan->AutoSize = true;
			this->slogan->Font = (gcnew System::Drawing::Font(L"Corbel", 10.125F, System::Drawing::FontStyle::Italic, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->slogan->Location = System::Drawing::Point(235, 175);
			this->slogan->Name = L"slogan";
			this->slogan->Size = System::Drawing::Size(378, 33);
			this->slogan->TabIndex = 4;
			this->slogan->Text = L"Master Your Time, Master Your Life";
			// 
			// TMSplash
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(12, 25);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->BackColor = System::Drawing::Color::LightSlateGray;
			this->ClientSize = System::Drawing::Size(890, 651);
			this->Controls->Add(this->slogan);
			this->Controls->Add(this->initials);
			this->Controls->Add(this->initialize);
			this->Controls->Add(this->title);
			this->Controls->Add(this->progressBar);
			this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::None;
			this->Icon = (cli::safe_cast<System::Drawing::Icon^  >(resources->GetObject(L"$this.Icon")));
			this->Name = L"TMSplash";
			this->StartPosition = System::Windows::Forms::FormStartPosition::CenterScreen;
			this->Text = L"TMSplash";
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
	private: System::Void timer1_Tick(System::Object^  sender, System::EventArgs^  e) {
				 progressBar->Increment(4);
				 if(progressBar->Value == 100){
					 timer1->Stop();
					
				 }
			 }
	};
}
