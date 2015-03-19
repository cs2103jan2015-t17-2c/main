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
    std::string userEntry;
    std::vector<std::string> tokenizedUserEntry;

    std::cout << "Welcome to TimeMaster. Please enter your command" << std::endl;
    std::getline(std :: cin, userEntry);

	//taskList.loadFromFile();
	
	while(userEntry != "quit") {
        /*if(GetAsyncKeyState('exit')) {
			break;
		}*/
        tokenizedUserEntry = parser.getTokenizedUserEntry(userEntry);
		std::string command = parser.extractCommand(tokenizedUserEntry);

        parser.parseTaskInfo(tokenizedUserEntry);
        //use extractEntryAfterCommand first read preconditions
		std::vector<TMTask> taskVector = parser.parseTaskInfo(tokenizedUserEntry);
        std::cout << "TASK DESCRIPTION: " << taskVector[0].getTaskDescription() << std::endl;
		executor.sortCommandToFunctions(command, taskVector, taskList);


        //taskList.displayAllTasks();
        std::cout << std::endl;
        std::cout << "Welcome to TimeMaster. Please enter your command" << std::endl;
		getline(std :: cin, userEntry);
    }
	
	std::atexit(saveUponExit);
	return 0;
}
