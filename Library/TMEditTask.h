#ifndef TMEDITTASK_H
#define TMEDITTASK_H

#include "TMCommand.h"

class TMEditTask : public TMCommand {

public:
	
	void execute() {
		TMParser *parser = TMParser::getInstance(); 
		TMTaskListStates *taskListStates = TMTaskListStates::getInstance();
		TMTaskList taskList = taskListStates->getCurrentTaskList();

		std::vector<std::string> tokens = parser->returnTokens();
		taskList.updateTask(std::stoi(tokens[0]), tokens[1], tokens[2]);
		taskListStates->addNewState(taskList);
	}

};
#endif