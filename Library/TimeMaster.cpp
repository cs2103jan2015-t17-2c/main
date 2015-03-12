#include <iostream>
#include <string>


int main() {
	TMTaskList tasklist;
	TMParser parser;
	TMExecutor executor;

	std::string userInput;

	std::cout << "Welcome to TimeMaster. Please enter your command" << std::endl;
	while(true) {
        if(GetAsyncKeyState('exit')) {
			break;
		}
		getline(cin, userInput);

		std::string command = parser.extractCommand(userInput);
		TMTask task = parser.parseTaskInfo(userInput);

		executor.sortCommandToFunctions(command, task, tasklist);
    }
	
	return 0;
}
