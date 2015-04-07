#ifndef TMADDTASKS_H
#define TMADDTASKS_H

#include "TMCommand.h"

#include <iostream>

class TMAddTasks : public TMCommand {


public:
	TMAddTasks() {}

	void execute() {
		//std::cout << "ADDTASKS CALLED" << std::endl << std::endl;
		TMParser *parser = TMParser::getInstance(); 
		TMTaskListStates *taskListStates = TMTaskListStates::getInstance();

		TMTask task = parser -> parseTaskInfo();
		TMTaskList taskList = taskListStates->getCurrentTaskList();
		outcome = taskList.addTask(task);
		int positionIndex = taskList.getPositionIndexFromTask(task);
		std::cout << "PI IS : " << positionIndex;
		positionIndexes.push_back(positionIndex);
		taskListStates->addNewState(taskList);
	
	}


};

#endif

