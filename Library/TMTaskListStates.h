#ifndef TMTASKLISTSTATES_H
#define TMTASKLISTSTATES_H

#include "TMTaskList.h"

class TMTaskListStates {

private: 
	std::vector<TMTaskList> states;
	std::vector<TMTaskList>::iterator currentState;
	TMTaskList currentTaskList;
	static TMTaskListStates *theOne;
	TMTaskListStates();

public:
	static TMTaskListStates* getInstance();
	void addNewState(TMTaskList taskList);
	void reverseCurrentState();
	void progressCurrentState();
	TMTaskList getCurrentTaskList();
	

};
#endif