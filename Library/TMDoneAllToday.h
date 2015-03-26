#ifndef TMDONEALLTODAY_H
#define TMDONEALLTODAY_H

#include "TMCommand.h"

class TMDoneAllToday : public TMCommand {

public:

	void execute() {
		TMTaskListStates *taskListStates = TMTaskListStates::getInstance();
		TMParser *parser = TMParser::getInstance(); 
		TMTaskList taskList = taskListStates->getCurrentTaskList();

		 boost::gregorian::date dateToday =  boost::gregorian::day_clock::local_day();
		 std::string strDateToday = parser->dateFromBoostToDelimitedDDMMYYYY(dateToday);

		int numTimedAndDeadline = taskList.getDatedSize();
		for (int i = 1; i <= numTimedAndDeadline; i++) {
			TMTask task = taskList.getTaskFromPositionIndex(i);
			std::cout << task.getTaskTime().getEndDate() << std::endl;
			if (task.getTaskTime().getEndDate() == strDateToday) {
				taskList.archiveOneTask(i); 
			}
		}
		taskListStates->addNewState(taskList);
	}

};

#endif