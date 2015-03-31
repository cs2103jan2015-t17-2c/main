#ifndef TMSAVEAT_H
#define TMSAVEAT_H

#include "TMCommand.h"

const std::string CURRENT_DIRECTORY = "Current file directory is now at: ";

class TMSaveAt : public TMCommand {
public:
	void execute() {
		TMParser *parser = TMParser::getInstance(); 
		TMTaskListStates *taskListStates = TMTaskListStates::getInstance();
		TMTaskList taskList = taskListStates->getCurrentTaskList();
		taskList.setFileDirectory(parser->parseDirectory());
		outcome = CURRENT_DIRECTORY + taskList.getFileDirectory();
		taskListStates->addNewState(taskList);
	}

};
#endif