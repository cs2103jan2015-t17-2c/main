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
		EditableTaskComponent component = parser->parseTaskComponent();
		outcome = taskList.updateTask(std::stoi(tokens[0]), component, tokens[2]);
		taskListStates->addNewState(taskList);
	}

};
#endif