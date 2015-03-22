#ifndef TMDELETETASKS_H
#define TMDELETETASKS_H

#include "TMCommand.h"
#include "TMTaskList.h"
#include "TMParser.h"

class TMDeleteTasks : public TMCommand {

public:
	TMDeleteTasks() {}

	void execute() {
		TMParser *parser = TMParser::getInstance(); 
		TMTaskList *taskList = TMTaskList::getInstance();
		std::vector<int> deleteIndex = parser->parseTaskPositionNo();
		taskList->removeTask(deleteIndex[0]);

		std::cout << "DELETE TASKS CALLED." << std::endl;
	}

	void undo() {}

};
#endif