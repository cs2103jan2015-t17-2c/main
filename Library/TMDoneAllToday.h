#ifndef TMDONEALLTODAY_H
#define TMDONEALLTODAY_H

#include "TMCommand.h"
#include "TMTaskList.h"

class TMDoneAllToday : public TMCommand {

public:

	void execute() {
		TMTaskList *taskList = TMTaskList::getInstance();
		TMParser *parser = TMParser::getInstance(); 
		 boost::gregorian::date dateToday =  boost::gregorian::day_clock::local_day();
		 std::string strDateToday = parser->dateFromBoostToDelimitedDDMMYYYY(dateToday);

		int numTimedAndDeadline = taskList->getDatedSize();
		for (int i = 1; i <= numTimedAndDeadline; i++) {
			TMTask task = taskList->getTaskFromPositionIndex(i);
			std::cout << task.getTaskTime().getEndDate() << std::endl;
			if (task.getTaskTime().getEndDate() == strDateToday) {
				taskList->archiveOneTask(i); 
			}
		}
	}

	void undo();
};

#endif