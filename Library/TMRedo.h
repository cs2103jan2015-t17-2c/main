#ifndef TMREDO_H
#define TMREDO_H

#include "TMCommand.h"


class TMRedo: public TMCommand {

public:
	void execute() {
		TMTaskListStates *taskListStates = TMTaskListStates::getInstance();
		taskListStates->progressCurrentState();
		std::cout << "REDO CALLED" << std::endl;
	}
};

#endif