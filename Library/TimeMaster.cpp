#include <iostream>
#include <string>
#include <cstdlib>
#include <vector>
#include <Windows.h>
#include "TMTaskList.h"
#include "TMParser.h"
#include "TMExecutor.h"

TMTaskList tasklist;

void saveUponExit (){
	tasklist.archiveAll();
}

int main() {
	
	TMParser parser;
	TMExecutor executor;

	std::string userInput;
	tasklist.loadFromFile();
	std::cout << "Welcome to TimeMaster. Please enter your command" << std::endl;
	while(true) {
        if(GetAsyncKeyState('exit')) {
			break;
		}
		getline(std :: cin, userInput);

		std::string command = parser.extractCommand(userInput);
		std::vector<TMTask> taskVector = parser.parseTaskInfo(userInput);

		executor.sortCommandToFunctions(command, taskVector, tasklist);
    }
	
	std::atexit(saveUponExit);
	return 0;
}
