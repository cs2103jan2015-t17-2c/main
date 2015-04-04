#ifndef TMCOMPLETETASKS_H
#define TMCOMPLETETASKS_H

#include "TMCommand.h"

class TMCompleteTasks : public TMCommand {

public:
	TMCompleteTasks() {}
	void execute() {
		TMParser *parser = TMParser::getInstance(); 
		TMTaskListStates *taskListStates = TMTaskListStates::getInstance();
		TMTaskList taskList = taskListStates->getCurrentTaskList();

		std::vector<int> completeIndexes = parser->parseTaskPositionNo();
		std::vector<int>::iterator intIter;
		std::ostringstream oss;
		
		if (noRepeatedIndexes(completeIndexes)) {
			for (intIter = completeIndexes.begin(); intIter != completeIndexes.end(); ++intIter) {
				oss << taskList.archiveOneTask(*intIter) << std::endl;
				updatePositionIndexes(completeIndexes, *intIter);
			}
		} else {
			oss << WARNING_REPEATED_INDEXES_SPECIFIED << std::endl;
		}

		outcome = oss.str();
		taskListStates->addNewState(taskList);
	}
};
#endif