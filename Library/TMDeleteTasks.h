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
		std::ostringstream ossValid, ossInvalid;
		int numDeleted = 0;
		
		if (hasRepeatedIndexes(deleteIndexes)) {
			handleRepeatedIndexes();
			return;
		}

		for (intIter = deleteIndexes.begin(); intIter != deleteIndexes.end(); ) {
			if (!taskList.isValidPositionIndex(*intIter)) {
				ossInvalid << *intIter << " ";
				intIter = deleteIndexes.erase(intIter);
			} else {
				++intIter;
			}
		}
		

		for (intIter = deleteIndexes.begin(); intIter != deleteIndexes.end(); ++intIter) {
			taskList.removeTask(*intIter);
			numDeleted++;
			updatePositionIndexes(deleteIndexes, *intIter);
		}
		
		if (hasValidIndexes(numDeleted)) {
			ossValid << numDeleted << DELETE_SUCCESS << '\n';
		}

		if (hasInvalidIndexes(ossInvalid.str())) {
			ossInvalid << STATUS_DISPLAY_INVALID_INDEXES;
		}

		outcome = ossValid.str() + ossInvalid.str();
		taskListStates->addNewState(taskList);
		
	}


};
#endif