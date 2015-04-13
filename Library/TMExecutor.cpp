#include "TMExecutor.h"
#include <string>

TMExecutor* TMExecutor::theOne;

TMExecutor::TMExecutor() {
	_currentDisplay= TMDisplay::Default;
}

TMExecutor* TMExecutor::getInstance() {
	if (theOne == NULL) {
		theOne = new TMExecutor();
	}
	return theOne; 
}

std::vector<std::string> TMExecutor::getUserInput() {
	return _unmanagedUserInput;
}

TMDisplay TMExecutor::getCurrentDisplay() {
	return _currentDisplay;
}

std::string TMExecutor::getResultOfExecution() {
	return _resultOfExecution;
}

std::vector<int> TMExecutor::getPositionIndexes() {
	return _positionIndexes;
}


void TMExecutor::setCurrentDisplay(TMDisplay display) {
	_currentDisplay = display;
}


bool TMExecutor::isDisplayChange(std::string userInput) {
	return (userInput.find("v") != std::string::npos);
}

TMDisplay TMExecutor::determineDisplayType(std::string userInput) {
	if (userInput == CMD_VIEW_DEFAULT || userInput == CMD_SHORTCUT_VIEW_DEFAULT) {
		return Default;
	}else if (userInput == CMD_VIEW_DEADLINE || userInput == CMD_SHORTCUT_VIEW_DEADLINE) {
		return DeadlineTasks;
	}else if (userInput == CMD_VIEW_UNDATED || userInput == CMD_SHORTCUT_VIEW_UNDATED) {
		return UndatedTasks;
	}else if (userInput == CMD_VIEW_ARCHIVED || userInput == CMD_SHORTCUT_VIEW_ARCHIVED) {
		return ArchivedTasks;
	}else if (userInput == CMD_VIEW_SEARCH || userInput == CMD_SHORTCUT_VIEW_SEARCH) {
		return SearchResults;
	} else {
		return Default;
	}
}

void TMExecutor::executeMain(std::string userInput) {

	TMParser *parser = TMParser::getInstance(); 
	TMTaskListStates *taskListStates = TMTaskListStates::getInstance();
	TMTaskList taskList = taskListStates->getCurrentTaskList();
	TMCommandCreator cmdCreator;

	if (userInput != "") {
		_unmanagedUserInput.push_back(userInput);
		parser->initialize(userInput);
		std::string command = parser->extractCommand();
	
		if (isDisplayChange(command)) {
			_currentDisplay = determineDisplayType(command);
			_resultOfExecution = "";
		} else {
			TMParser::CommandTypes type = parser->determineCommandType(command);
			TMCommand* commandObjPtr = cmdCreator.createNewCommandObj(type);
			commandObjPtr->execute();
			_resultOfExecution = parser->getErrorMessage() + commandObjPtr->outcome;
			_positionIndexes = commandObjPtr->positionIndexes;
			if (type == TMParser::CommandTypes::SearchKeyword) {
				_positionIndexes = commandObjPtr->positionIndexes;
				_currentDisplay = SearchResults;
			} 
		}
	} else {
		TMCommand* commandObjPtr = cmdCreator.createNewCommandObj(TMParser::CommandTypes::Invalid);
		commandObjPtr->execute();
		_resultOfExecution = commandObjPtr->outcome;
	}

	taskList = taskListStates->getCurrentTaskList();
	taskList.writeToFile();

	return;
}
