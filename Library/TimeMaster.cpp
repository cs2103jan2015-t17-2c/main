#include <iostream>
#include <string>
#include <cstdlib>
#include <vector>
#include <Windows.h>
#include "TMTaskList.h"
#include "TMParser.h"
#include "TMExecutor.h"

TMTaskList taskList;

void saveUponExit () {
	taskList.writeToFile();
}

int main() {
	
	TMParser parser;
	TMExecutor executor;
    std::string userInput;

    std::cout << "Welcome to TimeMaster. Please enter your command" << std::endl;
    std::getline(std :: cin, userInput);

	//taskList.loadFromFile();
	
	while(userInput != "quit") {
        /*if(GetAsyncKeyState('exit')) {
			break;
		}*/

		std::string command = parser.extractCommand(userInput);

        //use extractEntryAfterCommand first read preconditions
		std::vector<TMTask> taskVector = parser.parseTaskInfo(parser.extractEntryAfterCommand(userInput));
        std::cout << "TASK DESCRIPTION: " << taskVector[0].getTaskDescription() << std::endl;
		executor.sortCommandToFunctions(command, taskVector, taskList);


        //taskList.displayAllTasks();
        std::cout << std::endl;
        std::cout << "Welcome to TimeMaster. Please enter your command" << std::endl;
		getline(std :: cin, userInput);
    }
	
	std::atexit(saveUponExit);
	return 0;
}
