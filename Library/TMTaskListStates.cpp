//@author A0114130E
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
	taskList.loadOrCreateFile();
	addNewState(taskList);
}

void TMTaskListStates::addNewState(TMTaskList taskList) {
	if (!_states.empty() && _currentState != _states.end()-1) {
		_states.erase(_currentState + 1, _states.end());
	} 

	_states.push_back(taskList);
	_currentState = _states.end()-1;
}

bool TMTaskListStates::regressCurrentState() {
	if (_currentState != _states.begin()) {
		--_currentState;
		return true;
	} else {
		return false;
	}
}

bool TMTaskListStates::progressCurrentState() {
	if (_currentState != _states.end()-1) {
		++_currentState;
		return true;
	} else {
		return false;
	}
}

TMTaskList  TMTaskListStates::getCurrentTaskList() {
	return *_currentState;
}