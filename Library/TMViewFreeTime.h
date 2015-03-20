#ifndef TMVIEWFREETIME_H
#define TMVIEWFREETIME_H

#include "TMCommand.h";
#include "TMTaskList.h"

class TMViewFreeTime : public TMCommand {
public:
	void execute(TMTaskList taskList) {
		std::cout << taskList.freeTimeSearch() << std::endl;
	}
	void undo();
};

#endif
		