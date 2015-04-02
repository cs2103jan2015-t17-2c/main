#ifndef TMUNCOMPLETETASKS_H
#define TMUNCOMPLETETASKS_H

#include "TMCommand.h"

class TMUncompleteTasks: public TMCommand { 
	
public:
	void execute() {
		TMParser *parser = TMParser::getInstance(); 
		TMTaskListStates *taskListStates = TMTaskListStates::getInstance();
		TMTaskList taskList = taskListStates->getCurrentTaskList();
		std::vector<int> uncompleteIndexes = parser->parseTaskPositionNo();
		std::vector<TMTask> reAddTasks, toBeRemoved;

		std::vector<int>::iterator iterInt;
		std::vector<TMTask>::iterator iterTask;
		std::ostringstream oss;

		for (iterInt = uncompleteIndexes.begin(); iterInt != uncompleteIndexes.end(); ++iterInt) {
			TMTask task = taskList.getTaskFromPositionIndex(*iterInt);
			toBeRemoved.push_back(task);
			task.setAsIncompleted();
			reAddTasks.push_back(task);
		}
		//Remove those tasks from archived
		for (iterTask = toBeRemoved.begin(); iterTask != toBeRemoved.end(); ++iterTask) {
			int positionIndex = taskList.getPositionIndexFromTask(*iterTask);
			oss << taskList.removeTask(positionIndex) << std::endl;
		}
		//Add new tasks to taskList
		for (iterTask = reAddTasks.begin(); iterTask != reAddTasks.end(); ++iterTask) {
			oss << taskList.addTask(*iterTask) << std::endl;
		}

		outcome = oss.str();
		taskListStates->addNewState(taskList);
	}
};
#endif