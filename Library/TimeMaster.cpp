#include <iostream>
#include <string>
#include <cstdlib>
#include <vector>
#include <Windows.h>
#include "TMTaskList.h"
#include "TMParser.h"
#include "TMExecutor.h"

TMTaskList taskList;

void saveUponExit (){
	taskList.archiveAll();
}

int main() {
	
	TMParser parser;
	TMExecutor executor;
    std::string userInput;

    std::cout << "Welcome to TimeMaster. Please enter your command" << std::endl;
    std::getline(std :: cin, userInput);

	taskList.loadFromFile();
	
	while(userInput != "quit") {
        /*if(GetAsyncKeyState('exit')) {
			break;
		}*/

		std::string command = parser.extractCommand(userInput);
		std::vector<TMTask> taskVector = parser.parseTaskInfo(userInput);

		executor.sortCommandToFunctions(command, taskVector, taskList);
        taskList.displayAllTasks();
        std::cout << "Welcome to TimeMaster. Please enter your command" << std::endl;
		getline(std :: cin, userInput);
    }
	
	std::atexit(saveUponExit);
	return 0;
}
