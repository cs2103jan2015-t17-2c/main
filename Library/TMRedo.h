//@author A0114130E
#ifndef TMREDO_H
#define TMREDO_H

#include "TMCommand.h"

class TMRedo: public TMCommand {

public:
	void execute() {
		TMTaskListStates *taskListStates = TMTaskListStates::getInstance();
		if (taskListStates->progressCurrentState()) {
			outcome = REDO_SUCCESS;
		} else {
			outcome = REDO_FAILURE;
		} 
	}
};

#endif