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
		std::vector<TMTask> completedTasks;
		std::vector<TMTask>::iterator taskIter;
		
		for (intIter = completeIndexes.begin(); intIter != completeIndexes.end(); ++intIter) {
			TMTask task = taskList.getTaskFromPositionIndex(*intIter);
			completedTasks.push_back(task);
		}

		for (taskIter = completedTasks.begin(); taskIter != completedTasks.end(); ++taskIter) {
			int positionIndex = taskList.getPositionIndexFromTask(*taskIter);
			taskList.archiveOneTask(positionIndex);
		}

		taskListStates->addNewState(taskList);
	}
};
#endif