#ifndef TMADDTASKS_H
#define TMADDTASKS_H

#include "TMTask.h";
#include "TMCommand.h";
#include "TMTaskList.h"
#include "TMParser.h"

#include <iostream>

class TMAddTasks : public TMCommand {

public:
	TMAddTasks() {}

	void execute() {
		//std::cout << "ADDTASKS CALLED" << std::endl << std::endl;
		TMParser *parser = TMParser::getInstance(); 
		TMTaskList *taskList = TMTaskList::getInstance();

		std::vector<TMTask> tasks = parser -> parseTaskInfo();
		std::vector<TMTask>::iterator iter;
		for (iter = tasks.begin(); iter != tasks.end(); ++iter) {
			taskList -> addTask(*iter);
		}
	
		tasks.clear();
		std::cout << "ADD OPERATION COMPLETED." << std::endl;
		//taskList.displayAllTasks();
	}

	void undo();

};

#endif
