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

		int positionIndex = std::stoi(parser->extractTokenAfterCommand());
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

	}

};
#endif