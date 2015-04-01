#ifndef TMSAVEAT_H
#define TMSAVEAT_H

#include "TMCommand.h"
#include <Windows.h>

const std::string CURRENT_DIRECTORY = "Current file directory is now at: ";
const std::string INVAILD_DIRECTORY = "The directory you have specified is invalid";

class TMSaveAt : public TMCommand {
public:
	void execute() {
		TMParser *parser = TMParser::getInstance(); 
		TMTaskListStates *taskListStates = TMTaskListStates::getInstance();
		TMTaskList taskList = taskListStates->getCurrentTaskList();
		std::string directory = parser->parseDirectory();
		if (isValidDirectory(directory.c_str())) {
			taskList.setFileDirectory(directory);
			outcome = CURRENT_DIRECTORY + taskList.getFileDirectory();
		} else {
			outcome = INVAILD_DIRECTORY;
		}
		
		taskListStates->addNewState(taskList);
	}

	bool isValidDirectory(const char* directory) {
		DWORD attributes = GetFileAttributesA(directory);
		if (attributes == INVALID_FILE_ATTRIBUTES) {
			return false;
		}
		
		return (attributes & FILE_ATTRIBUTE_DIRECTORY);
	}
};
#endif