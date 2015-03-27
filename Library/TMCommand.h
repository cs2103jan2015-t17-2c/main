#ifndef TMCOMMAND_H
#define TMCOMMAND_H

#include "TMParser.h"
#include "TMTaskListStates.h"
#include "TMTaskList.h"
#include "TMTask.h"
#include "TMUserInterface.h"

class TMCommand{

	public: 
	virtual void execute() = 0;
	//void undo();

};

#endif
