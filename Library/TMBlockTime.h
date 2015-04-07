#ifndef TMBLOCKTIME_H
#define TMBLOCKTIME_H

#include "TMCommand.h"
#include "TaskChecker.h"

const std::string BATCH_NUMBER_INFO = "The batch number for this batch of unconfirmed task(s) is: ";

class TMBlockTime : public TMCommand {
	
public:

	void execute() {
		TMTaskListStates *taskListStates = TMTaskListStates::getInstance();
		TMParser *parser = TMParser::getInstance();
        TaskChecker *taskChecker = TaskChecker::getInstance();
		TMTaskList taskList = taskListStates->getCurrentTaskList();
		std::ostringstream oss;
		int i = taskList.generateUniqueBatchNum();

		if (taskChecker->isMultipleTimingTask(parser->returnTokens())) {
			std::vector<TMTask> tasks = parser->parseMultipleTimingTaskInfo();
			std::vector<TMTask>::iterator iter;
		
			for (iter = tasks.begin(); iter != tasks.end(); ++iter) {
				TMTask task = *iter;
				task.setAsUnconfirmed();
				task.setUnconfirmedBatchNumber(i);
				oss << taskList.addTask(task) << std::endl;
				int positionIndex = taskList.getPositionIndexFromTask(task);
				positionIndexes.push_back(positionIndex);
			}
		} else {
			TMTask task = parser->parseTaskInfo();
			task.setAsUnconfirmed();
			task.setUnconfirmedBatchNumber(i);
			oss << taskList.addTask(task) << std::endl;
			int positionIndex = taskList.getPositionIndexFromTask(task);
			positionIndexes.push_back(positionIndex);
		}
		
		outcome = oss.str();
		taskListStates->addNewState(taskList);
	}

};
#endif