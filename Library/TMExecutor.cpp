//CONTAINS MAIN FUNCTION NOW. UI TO HAVE A DIFFERENT CLASS

#include <iostream>
#include <string>
#include <cstdlib>
#include <vector>
#include <Windows.h>

#include "TMUserInterface.h"
#include "TMParser.h"
#include "TMTaskList.h"
#include "TMCommandCreator.h"
#include "TMCommand.h"

	
int main() {
	
	TMParser *parser = TMParser::getInstance(); 
	TMUserInterface *ui = TMUserInterface::getInstance();
	TMTaskList *taskList = TMTaskList::getInstance();
	TMCommandCreator cmdCreator;

    std::string userEntry;
	bool isSuccessfullyExecuted = false; 

	//ui->displayDefault();
    ui->promptForUserInput();
	userEntry = ui->returnUserInput();

	
	while(userEntry != "quit") {
        /*if(GetAsyncKeyState('exit')) {
			break;
		}*/
        parser->initialize(userEntry);
		std::string command = parser->extractCommand();
		TMCommand* commandObjPtr = cmdCreator.createNewCommandObj(command);
		commandObjPtr->execute();
		taskList->writeToFile();


		taskList->displayAllTasks();
		//ui->displayDefault();
		ui->promptForUserInput();
		userEntry = ui->returnUserInput();
    }
	
	taskList->leaveReferenceUponExit();

	return 0;
}
