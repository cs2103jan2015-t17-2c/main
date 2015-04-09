#ifndef TMADDTASKS_H
#define TMADDTASKS_H

const std::string ADD_SDT_SUCCESS = " (with start date and time) successfully added.";
const std::string ADD_EDT_SUCCESS = " (with end date and time/deadline) successfully added.";
const std::string ADD_PERIOD_SUCCESS = " (with period) successfully added.";
const std::string ADD_UNDATED_SUCCESS = " (undated) successfully added.";
const std::string ADD_INVALID = "Task you have specified has invalid component(s). Please specify a valid task.";
const std::string CLASH_WARNING = " has clash(es) with tasks on hand.\nTasks involved have been highlighted in blue.";

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
			oss << "New task " << positionIndex << ADD_SDT_SUCCESS;
			break;

		case WithEndDateTime:
			oss << "New task " << positionIndex << ADD_EDT_SUCCESS;
			break;

		case WithPeriod:
			if (task.isClashed()) {
				oss << "New task " << positionIndex << CLASH_WARNING;
			} else {
				oss << "New task " << positionIndex << ADD_PERIOD_SUCCESS;
			}
			break;

		case Undated:
			oss << "New task " << positionIndex << ADD_UNDATED_SUCCESS;
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

