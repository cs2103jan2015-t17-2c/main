#ifndef TMREDO_H
#define TMREDO_H

#include "TMCommand.h"

class TMRedo: public TMCommand {

public:
	void execute() {
		TMTaskListStates *taskListStates = TMTaskListStates::getInstance();
		outcome = taskListStates->progressCurrentState();
	}
};

#endif