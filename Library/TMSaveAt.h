#ifndef TMSAVEAT_H
#define TMSAVEAT_H

#include "TMCommand.h"

class TMSaveAt : public TMCommand {
public:
	void execute() {
		TMParser *parser = TMParser::getInstance(); 
		TMTaskListStates *taskListStates = TMTaskListStates::getInstance();
		TMTaskList taskList = taskListStates->getCurrentTaskList();
		taskList.setFileDirectory(parser->parseDirectory());
		taskListStates->addNewState(taskList);
	}

};
#endif