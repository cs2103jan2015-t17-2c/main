#ifndef TMTASKLISTSTATES_H
#define TMTASKLISTSTATES_H

#include "TMTaskList.h"

class TMTaskListStates {

private: 
	std::vector<TMTaskList> _states;
	std::vector<TMTaskList>::iterator _currentState;
	static TMTaskListStates *theOne;
	TMTaskListStates();

public:
	static TMTaskListStates* getInstance();
	void addNewState(TMTaskList taskList);
	
	//Returns true if current state is able to regress and false otherwise
	bool regressCurrentState();

	//Returns true if current state is able to progress and false otherwise
	bool progressCurrentState();

	TMTaskList getCurrentTaskList();
	

};
#endif