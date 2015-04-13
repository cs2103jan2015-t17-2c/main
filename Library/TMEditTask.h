//@author A0114130E
#ifndef TMEDITTASK_H
#define TMEDITTASK_H

#include "TMCommand.h"
const std::string UPDATE_SUCCESS = "Task successfully edited.";

class TMEditTask : public TMCommand {

public:
	
	void execute() {
		TMParser *parser = TMParser::getInstance(); 
		TMTaskListStates *taskListStates = TMTaskListStates::getInstance();
		TMTaskList taskList = taskListStates->getCurrentTaskList();
		
		std::string strPositionIndex = parser->extractTokenAfterCommand();
		
		if (strPositionIndex != "") {
			int positionIndex = std::stoi(strPositionIndex);
			if (taskList.isValidPositionIndex(positionIndex)) {
				TMTask alteredTask = parser->parseTaskInfo();
				taskList.updateTask(positionIndex, alteredTask);
				int index = taskList.getPositionIndexFromTask(alteredTask);
				positionIndexes.push_back(index);
				outcome = EDIT_SUCCESS;
				taskListStates->addNewState(taskList);
			} else {
				std::ostringstream oss;
				oss << positionIndex << STATUS_DISPLAY_INVALID_INDEXES;
				outcome = oss.str();
			}
		} else {
			outcome = NO_INDEX_OR_DETAILS_SPECIFIED;
		}

	}

};
#endif