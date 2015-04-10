#ifndef TMBLOCKTIME_H
#define TMBLOCKTIME_H

#include "TMCommand.h"
#include "TaskChecker.h"


class TMBlockTime : public TMCommand {
	
public:

	void execute() {
		TMTaskListStates *taskListStates = TMTaskListStates::getInstance();
		TMParser *parser = TMParser::getInstance();
        TaskChecker *taskChecker = TaskChecker::getInstance();
		TMTaskList taskList = taskListStates->getCurrentTaskList();
		std::ostringstream ossValid, ossInvalid;
		int i = taskList.generateUniqueBatchNum();
        
        std::vector<TMTask> tasks = parser->parseMultipleTimingTaskInfo();
		std::vector<TMTask>::iterator iter;
		int numInvalid = 0;

		for (iter = tasks.begin(); iter != tasks.end(); ++iter) {
			TMTask task = *iter;
			if (task.getTaskType() != TaskType::Invalid) {
				task.setAsUnconfirmed();
				task.setUnconfirmedBatchNumber(i);
				taskList.addTask(task);
				int positionIndex = taskList.getPositionIndexFromTask(task);
				positionIndexes.push_back(positionIndex);
			} else {
				numInvalid++;
			}
		}
		
		if (positionIndexes.size() != 0) {
			std::vector<int>::iterator intIter;
			ossValid << STATUS_DISPLAY_INDEXES;
			for (intIter = positionIndexes.begin(); intIter != positionIndexes.end(); ++intIter) {
				ossValid << *intIter << " ";
			}
			ossValid << BLOCK_SUCCESS << std::endl;
		}

		if (numInvalid != 0) {
			ossInvalid << BLOCK_FAILURE << numInvalid;
		}

		outcome = ossValid.str() + ossInvalid.str();
		taskListStates->addNewState(taskList);
	}

};
#endif