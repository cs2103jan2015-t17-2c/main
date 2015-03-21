#ifndef TMCOMPLETETASKS_H
#define TMCOMPLETETASKS_H

#include "TMCommand.h"
#include "TMTaskList.h"
#include "TMParser.h"

class TMCompleteTasks : public TMCommand {

public:
	TMCompleteTasks() {}
	void execute() {
		TMParser *parser = TMParser::getInstance(); 
		TMTaskList *taskList = TMTaskList::getInstance();
		std::vector<int> completeIndexes = parser->parseTaskPositionNo();
		std::vector<int>::iterator iter;

		for (iter = completeIndexes.begin(); iter != completeIndexes.end(); ++iter) {
			taskList->archiveOneTask(*iter);
		}
	}

	void undo() {}
};
#endif