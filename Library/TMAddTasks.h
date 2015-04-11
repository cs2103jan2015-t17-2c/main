#ifndef TMADDTASKS_H
#define TMADDTASKS_H

#include "TMCommand.h"

#include <iostream>

class TMAddTasks : public TMCommand {


public:
	TMAddTasks() {}

	void execute() {
		TMParser *parser = TMParser::getInstance(); 
		TMTaskListStates *taskListStates = TMTaskListStates::getInstance();

		TMTask task = parser -> parseTaskInfo();
		TMTaskList taskList = taskListStates->getCurrentTaskList();
		std::ostringstream oss;
		int positionIndex;

		taskList.addTask(task);
		TaskType type = task.getTaskType();
		
		if (type != Invalid) {
			positionIndex = taskList.getPositionIndexFromTask(task);
			positionIndexes.push_back(positionIndex);
		}

		switch (type) {
		case WithStartDateTime:
			oss << STATUS_DISPLAY_NEW_INDEX << positionIndex << ADD_SDT_SUCCESS;
			break;

		case WithEndDateTime:
			oss << STATUS_DISPLAY_NEW_INDEX << positionIndex << ADD_EDT_SUCCESS;
			break;

		case WithPeriod:
			if (task.isClashed()) {
				oss << STATUS_DISPLAY_NEW_INDEX << positionIndex << CLASH_WARNING;
			} else {
				oss << STATUS_DISPLAY_NEW_INDEX << positionIndex << ADD_PERIOD_SUCCESS;
			}
			break;

		case Undated:
			oss << STATUS_DISPLAY_NEW_INDEX << positionIndex << ADD_UNDATED_SUCCESS;
			break;

		case Invalid:
			oss << ADD_INVALID;
			break;
		}

		outcome = oss.str();
		taskListStates->addNewState(taskList);
	
	}


};

#endif

