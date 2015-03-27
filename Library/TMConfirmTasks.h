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


		for (iter1 = confirmedTasksIndexes.begin(); iter1 != confirmedTasksIndexes.end(); ++iter1) {
			TMTask task = taskList.getTaskFromPositionIndex(*iter1);
			int batchNum = task.getUnconfirmedBatchNum();
			batchNums.push_back(batchNum);
			taskList.updateTask(*iter1, "confirmation", "1");
			taskList.updateTask(*iter1, "unconfirmedBatchNum", "0");
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
					taskList.removeTask(positionIndex);
				}
			}
		}

		taskListStates->addNewState(taskList);
	}

};
#endif