#ifndef TMEDITTASK_H
#define TMEDITTASK_H

#include "TMCommand.h"

class TMEditTask : public TMCommand {

public:
	
	void execute() {
		TMParser *parser = TMParser::getInstance(); 
		TMTaskListStates *taskListStates = TMTaskListStates::getInstance();
		TMTaskList taskList = taskListStates->getCurrentTaskList();

		std::vector<int> indexes = parser->parseTaskPositionNo();
		int positionIndex = indexes.front();
		EditableTaskComponent component = parser->parseEditableTaskComponent();
		std::string changeTo;

		switch (component) {
		case Description:
			changeTo = parser->parseDescription();
			//outcome = taskList.updateTask(positionIndex, component, changeTo);
			break;

		case StartDate:
			//changeTo = parser->parseDate();
			//outcome = taskList.updateTask(positionIndex, component, changeTo);
			break;

		case StartTime:
			//changeTo = parser->parseTime();
			//outcome = taskList.updateTask(positionIndex, component, changeTo);
			break;

		case EndDate:
			//changeTo = parser->parseDate();
			//outcome = taskList.updateTask(positionIndex, component, changeTo);
			break;

		case EndTime:
			//changeTo = parser->parseTime();
			//outcome = taskList.updateTask(positionIndex, component, changeTo);
			break;

		case InvalidComponent:
			outcome = taskList.updateTask(positionIndex, component, changeTo);
			break;
		}

		taskListStates->addNewState(taskList);
	}

};
#endif