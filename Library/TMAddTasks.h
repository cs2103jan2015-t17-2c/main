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

		std::vector<TMTask> tasks = parser -> parseTaskInfo();
		TMTaskList taskList = taskListStates->getCurrentTaskList();
		std::vector<TMTask>::iterator iter;
		for (iter = tasks.begin(); iter != tasks.end(); ++iter) {
			taskList.addTask(*iter);
		}
	
		tasks.clear();
		taskListStates->addNewState(taskList);
		std::cout << "ADD OPERATION COMPLETED." << std::endl;
		//taskList.displayAllTasks();
	}

};

#endif

