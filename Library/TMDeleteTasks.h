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
		std::vector<int> deleteIndexes = parser->parseTaskPositionNo();
		std::vector<int>::iterator iter;
		for (iter = deleteIndexes.begin(); iter != deleteIndexes.end(); ++iter) {
			taskList->removeTask(*iter);
		}
		std::cout << "DELETE TASKS CALLED." << std::endl;
	}

	void undo() {}

};
#endif