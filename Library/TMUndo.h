#ifndef TMUNDO_H
#define TMUNDO_H

#include "TMCommand.h"


class TMUndo: public TMCommand {

public:
	void execute() {
		TMTaskListStates *taskListStates = TMTaskListStates::getInstance();
		outcome = taskListStates->reverseCurrentState();
	}
};

#endif