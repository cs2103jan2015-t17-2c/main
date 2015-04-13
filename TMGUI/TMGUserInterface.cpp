//author @A0114172U

#include "TMGUserInterface.h"
using namespace System;
using namespace System::Windows::Forms;
	[STAThread]
void main(array<String^>^ args) {
	Application::EnableVisualStyles();
	Application::SetCompatibleTextRenderingDefault(false);
	TMGUI::TMGUserInterface mainWindow;
	Application::Run(%mainWindow);
}

