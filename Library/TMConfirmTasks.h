#ifndef TMCONFIRMTASKS_H
#define TMCONFIRMTASKS_H


#include "TMCommand.h"

class TMConfirmTasks : public TMCommand {
public: 
	void execute() {
		TMParser *parser = TMParser::getInstance(); 
		TMTaskListStates *taskListStates = TMTaskListStates::getInstance();
		TMTaskList taskList = taskListStates->getCurrentTaskList();
		std::vector<int> confirmedTasksIndexes = parser->parseTaskPositionNo();
		std::vector<int>::iterator iter1;
		std::vector<int>::iterator iter2;
		std::vector<int> batchNums;
		std::vector<TMTask> deleteTasks;
		std::vector<TMTask>::iterator taskIter;
		std::ostringstream oss;

		for (iter1 = confirmedTasksIndexes.begin(); iter1 != confirmedTasksIndexes.end(); ++iter1) {
			TMTask &task = taskList.getTaskFromPositionIndex(*iter1);
			int batchNum = task.getUnconfirmedBatchNumber();
			batchNums.push_back(batchNum);
			task.setAsConfirmed();
			task.setUnconfirmedBatchNumber(0);
		}

		for (iter1 = batchNums.begin(); iter1 != batchNums.end(); ++iter1) {
			std::vector<int> results;
			results = taskList.searchUnconfirmedBatchNum(*iter1);
			if (!results.empty()) {
				for (iter2 = results.begin(); iter2 != results.end(); ++iter2) {
					TMTask task = taskList.getTaskFromPositionIndex(*iter2);
					deleteTasks.push_back(task);
				}

				for (taskIter = deleteTasks.begin(); taskIter != deleteTasks.end(); ++taskIter) {
					int positionIndex = taskList.getPositionIndexFromTask(*taskIter);
					oss << taskList.removeTask(positionIndex) << std::endl;
				}
			}
		}

		outcome = oss.str();
		taskListStates->addNewState(taskList);
	}

};
#endif