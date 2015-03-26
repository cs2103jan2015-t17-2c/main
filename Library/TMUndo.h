#ifndef TMUNDO_H
#define TMUNDO_H

#include "TMCommand.h"


class TMUndo: public TMCommand {

public:
	void execute() {
		TMTaskListStates *taskListStates = TMTaskListStates::getInstance();
		taskListStates->reverseCurrentState();
		std::cout << "UNDO CALLED" << std::endl;
	}
};

#endif