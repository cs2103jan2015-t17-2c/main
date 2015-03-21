#ifndef TMDONEALLTODAY_H
#define TMDONEALLTODAY_H

#include "TMCommand.h"
#include "TMTaskList.h"

class TMDoneAllToday : public TMCommand {

public:

	void execute() {
		TMTaskList *taskList = TMTaskList::getInstance();
		taskList->archiveTodaysTasks();
	}

	void undo();
};

#endif