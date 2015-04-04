#ifndef TMDELETETASKS_H
#define TMDELETETASKS_H

#include "TMCommand.h"

class TMDeleteTasks : public TMCommand {

public:
	TMDeleteTasks() {}

	void execute() {
		TMParser *parser = TMParser::getInstance(); 
		TMTaskListStates *taskListStates = TMTaskListStates::getInstance();
		TMTaskList taskList = taskListStates->getCurrentTaskList();
		
		std::vector<int> deleteIndexes = parser->parseTaskPositionNo();
		std::vector <int>::iterator intIter;
		std::ostringstream oss;
		
		if (noRepeatedIndexes(deleteIndexes)) {
			for (intIter = deleteIndexes.begin(); intIter != deleteIndexes.end(); ++intIter) {
				oss << taskList.removeTask(*intIter) << std::endl;
				updatePositionIndexes(deleteIndexes, *intIter);
			}
		} else {
			oss << WARNING_REPEATED_INDEXES_SPECIFIED << std::endl;
		}

		outcome = oss.str();
		taskListStates->addNewState(taskList);
		
	}


};
#endif