#ifndef TMEDITTASK_H
#define TMEDITTASK_H

#include "TMCommand.h"

class TMEditTask : public TMCommand {

public:
	
	void execute() {
		TMParser *parser = TMParser::getInstance(); 
		TMTaskListStates *taskListStates = TMTaskListStates::getInstance();
		TMTaskList taskList = taskListStates->getCurrentTaskList();

		int positionIndex = std::stoi(parser->extractTokenAfterCommand());
		TMTask alteredTask = parser->parseTaskInfo();
		outcome = taskList.updateTask(positionIndex, alteredTask);
		int index = taskList.getPositionIndexFromTask(alteredTask);
		positionIndexes.push_back(index);

		taskListStates->addNewState(taskList);
	}

};
#endif