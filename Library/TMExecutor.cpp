//CONTAINS MAIN FUNCTION NOW. UI TO HAVE A DIFFERENT CLASS

#include <iostream>
#include <string>
#include <cstdlib>
#include <vector>
#include <Windows.h>

#include "TMUserInterface.h"
#include "TMParser.h"
#include "TMTaskListStates.h"
#include "TMTaskList.h"
#include "TMCommandCreator.h"
#include "TMCommand.h"

	
int main() {
	
	TMParser *parser = TMParser::getInstance(); 
	TMUserInterface *ui = TMUserInterface::getInstance();
	TMTaskListStates *taskListStates = TMTaskListStates::getInstance();
	TMTaskList taskList = taskListStates->getCurrentTaskList();
	TMCommandCreator cmdCreator;

    std::string userEntry;
	bool isSuccessfullyExecuted = false; 

	ui->displayDefault();
    ui->promptForUserInput();
	userEntry = ui->returnUserInput();
	
	std::cout << std::endl << "RESULTS" << std::endl;

	
	while(userEntry != "quit") {
        /*if(GetAsyncKeyState('exit')) {
			break;
		}*/
        parser->initialize(userEntry);
		std::string command = parser->extractCommand();
		TMCommand* commandObjPtr = cmdCreator.createNewCommandObj(parser->determineCommandType(command));
		commandObjPtr->execute();
		taskList.writeToFile();


		std:: cout << ui->displayDefault();
		ui->promptForUserInput();
		userEntry = ui->returnUserInput();
    }
	
	taskList.leaveReferenceUponExit();

	return 0;
}
