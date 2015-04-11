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
		std::ostringstream ossValid, ossInvalid;
		int numArchived = 0;

		if (hasRepeatedIndexes(completeIndexes)) {
			outcome = WARNING_REPEATED_INDEXES_SPECIFIED;
			return;
		}

		for (intIter = completeIndexes.begin(); intIter != completeIndexes.end(); ) {
			if (!taskList.isValidPositionIndex(*intIter) || taskList.isInArchived(*intIter)) {
				ossInvalid << *intIter << " ";
				intIter = completeIndexes.erase(intIter);
			} else {
				++intIter;
			}
		}

		for (intIter = completeIndexes.begin(); intIter != completeIndexes.end(); ++intIter) {
			taskList.archiveOneTask(*intIter);
			updatePositionIndexes(completeIndexes, *intIter);
			numArchived++;

		}
		
		ossValid << numArchived << ARCHIVE_SUCCESS << std::endl;
		if (ossInvalid.str().size() != 0) {
			ossInvalid << STATUS_DISPLAY_INVALID_INDEXES;
		}
		outcome = ossValid.str() + '\n' + ossInvalid.str();
		taskListStates->addNewState(taskList);
	}
};
#endif