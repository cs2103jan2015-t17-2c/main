#ifndef TMSAVEAT_H
#define TMSAVEAT_H

#include "TMCommand.h"
#include <Windows.h>



class TMSaveAt : public TMCommand {
public:
	void execute() {
		TMParser *parser = TMParser::getInstance(); 
		TMTaskListStates *taskListStates = TMTaskListStates::getInstance();
		TMTaskList taskList = taskListStates->getCurrentTaskList();
		std::string directory = parser->parseDirectory();
		if (taskList.isValidDirectory(directory.c_str())) {
			taskList.setDirectoryName(directory);
			outcome = CURRENT_DIRECTORY_NAME + taskList.getDirectoryName() + '\n' + CURRENT_FILE_NAME + taskList.getFileName();
		} else {
			outcome = INVAILD_DIRECTORY_SPECIFIED;
		}
		
		taskListStates->addNewState(taskList);
	}

};
#endif