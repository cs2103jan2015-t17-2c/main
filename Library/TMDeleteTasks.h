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
		std::vector<TMTask> deleteTasks;
		std::vector<TMTask>::iterator taskIter;
		
		for (intIter = deleteIndexes.begin(); intIter !=deleteIndexes.end(); ++intIter) {
			TMTask task = taskList.getTaskFromPositionIndex(*intIter);
			deleteTasks.push_back(task);
		}

		for (taskIter = deleteTasks.begin(); taskIter != deleteTasks.end(); ++taskIter) {
			int positionIndex = taskList.getPositionIndexFromTask(*taskIter);
			taskList.removeTask(positionIndex);
		}


		taskListStates->addNewState(taskList);
		std::cout << "DELETE TASKS CALLED." << std::endl;
	}


};
#endif