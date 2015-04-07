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
		std::vector<int>::iterator intIter;
		std::vector<TMTask> reAddTasks;
		std::vector<TMTask>::iterator taskIter;
		
		std::ostringstream oss;

		if (noRepeatedIndexes(uncompleteIndexes)) {
			for (intIter = uncompleteIndexes.begin(); intIter != uncompleteIndexes.end(); ++intIter) {
				TMTask task = taskList.getTaskFromPositionIndex(*intIter);
			
				oss << taskList.removeTask(*intIter) << std::endl;
				updatePositionIndexes(uncompleteIndexes, *intIter);
			
				task.setAsIncompleted();
				reAddTasks.push_back(task);
			}
		
			//Re-add tasks to taskList
			for (taskIter = reAddTasks.begin(); taskIter != reAddTasks.end(); ++taskIter) {
				TMTask task = *taskIter;
				oss << taskList.addTask(task) << std::endl;
				int positionIndex = taskList.getPositionIndexFromTask(task);
				positionIndexes.push_back(positionIndex);
			}
		} else {
			oss << WARNING_REPEATED_INDEXES_SPECIFIED << std::endl;
		}

		outcome = oss.str();
		taskListStates->addNewState(taskList);
	}
};
#endif