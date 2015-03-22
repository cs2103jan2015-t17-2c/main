#ifndef TMVIEWFREETIME_H
#define TMVIEWFREETIME_H

#include "TMCommand.h"
#include "TMTaskList.h"

class TMViewFreeTime : public TMCommand {
public:
	void execute() {
		TMTaskList *taskList = TMTaskList::getInstance();
		std::cout << taskList->displayFreeTime() << std::endl;
	}
	void undo();
};

#endif
		