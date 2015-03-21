#ifndef TMUPDATETASK_H
#define TMUPDATETASK_H

#include "TMCommand.h"
#include "TMTaskList.h"

class TMUpdateTask : public TMCommand {

public:
	
	void execute(int positionIndex, std::string component, std::string changeTo, TMTaskList &taskList) {
		taskList.updateTask(positionIndex, component, changeTo);
	}

	void undo();
};
#endif