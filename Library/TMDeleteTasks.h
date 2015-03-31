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
		std::ostringstream oss;
		
		for (intIter = deleteIndexes.begin(); intIter !=deleteIndexes.end(); ++intIter) {
			TMTask task = taskList.getTaskFromPositionIndex(*intIter);
			deleteTasks.push_back(task);
		}

		for (taskIter = deleteTasks.begin(); taskIter != deleteTasks.end(); ++taskIter) {
			int positionIndex = taskList.getPositionIndexFromTask(*taskIter);
			oss << taskList.removeTask(positionIndex) << std::endl;
		}

		outcome = oss.str();
		taskListStates->addNewState(taskList);
		
	}


};
#endif