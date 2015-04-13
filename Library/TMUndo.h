//@author A0114130E
#ifndef TMUNDO_H
#define TMUNDO_H

#include "TMCommand.h"


class TMUndo: public TMCommand {

public:
	void execute() {
		TMTaskListStates *taskListStates = TMTaskListStates::getInstance();
		if (taskListStates->regressCurrentState()) {
			outcome = UNDO_SUCCESS;
		} else {
			outcome = UNDO_FAILURE;
		} 
	}
};

#endif