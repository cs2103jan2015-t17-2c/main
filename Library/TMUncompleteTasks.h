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
		
		std::ostringstream ossValid, ossInvalid;

		if (!noRepeatedIndexes(uncompleteIndexes)) {
			outcome = WARNING_REPEATED_INDEXES_SPECIFIED;
			return;
		}
			
		for (intIter = uncompleteIndexes.begin(); intIter != uncompleteIndexes.end(); ) {
			
			if (!taskList.isValidPositionIndex(*intIter)) {
				ossInvalid << *intIter << " ";
				intIter = uncompleteIndexes.erase(intIter);
			} else {
				TMTask task = taskList.getTaskFromPositionIndex(*intIter);
				if (!task.isCompleted()) {
					ossInvalid << *intIter << " ";
					intIter = uncompleteIndexes.erase(intIter);
				} else {
					++intIter;
				}
			}
		}

		for (intIter = uncompleteIndexes.begin(); intIter != uncompleteIndexes.end(); ++intIter) {
			TMTask task = taskList.getTaskFromPositionIndex(*intIter);
			taskList.removeTask(*intIter);
			updatePositionIndexes(uncompleteIndexes, *intIter);			
			task.setAsIncompleted();
			reAddTasks.push_back(task);
		}
		
			//Re-add tasks to taskList
		for (taskIter = reAddTasks.begin(); taskIter != reAddTasks.end(); ++taskIter) {
			TMTask task = *taskIter;
			taskList.addTask(task);
			int positionIndex = taskList.getPositionIndexFromTask(task);
			positionIndexes.push_back(positionIndex);
		}

		ossValid << reAddTasks.size() << " tasks successfully marked as uncomplete." << std::endl;
		if (ossInvalid.str().size() != 0) {
			ossInvalid << " is/are invalid position indexe(s).";
		}
		outcome = ossValid.str() + ossInvalid.str();
		taskListStates->addNewState(taskList);
	}
};
#endif