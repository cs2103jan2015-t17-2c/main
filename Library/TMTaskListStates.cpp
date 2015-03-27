#include "TMTaskListStates.h"

TMTaskListStates* TMTaskListStates::theOne;

TMTaskListStates* TMTaskListStates::getInstance() {
	if (theOne == NULL) {
		theOne = new TMTaskListStates();
	}
	return theOne;
}

TMTaskListStates::TMTaskListStates() {
	TMTaskList taskList;
	//taskList.loadFromFile();
	addNewState(taskList);
}

void TMTaskListStates::addNewState(TMTaskList taskList) {
	if (!states.empty() && currentState != states.end()-1) {
		std::vector<TMTaskList>::iterator iter;
		for (iter = currentState+1; iter != states.end(); iter = states.erase(iter)) {
		}
	} 

	states.push_back(taskList);
	currentState = states.end()-1;
	currentTaskList = *currentState;
}

void TMTaskListStates::reverseCurrentState() {
	if (currentState != states.begin()) {
		--currentState;
		currentTaskList = *currentState;
	} else {
		std::cout << "INITIAL STATE REACHED. CANNOT UNDO ANYMORE" << std::endl;
	}
}

void TMTaskListStates::progressCurrentState() {
	if (currentState != states.end()-1) {
		++currentState;
		currentTaskList = *currentState;
	} else {
		std::cout << "LAST SAVED STATE REACHED. CANNOT REDO ANYMORE" << std::endl;
	}
}

TMTaskList  TMTaskListStates::getCurrentTaskList() {
	return currentTaskList;}