#include "TMTaskListStates.h"

const std::string UNDO_SUCCESS = "Undo last action successfully.";
const std::string UNDO_FAILURE = "Initial state of the system is reached. Unable to undo.";
const std::string REDO_SUCCESS = "Redo successfully.";
const std::string REDO_FAILURE = "Latest state of the system is reached. Unable to redo.";


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

std::string TMTaskListStates::reverseCurrentState() {
	if (currentState != states.begin()) {
		--currentState;
		currentTaskList = *currentState;
		return UNDO_SUCCESS;
	} else {
		return UNDO_FAILURE;
	}
}

std::string TMTaskListStates::progressCurrentState() {
	if (currentState != states.end()-1) {
		++currentState;
		currentTaskList = *currentState;
		return REDO_SUCCESS;
	} else {
		return REDO_FAILURE;
	}
}

TMTaskList  TMTaskListStates::getCurrentTaskList() {
	return currentTaskList;}