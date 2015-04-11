#ifndef TMEXECUTOR_H
#define TMEXECUTOR_H

#include <sstream>
#include <cstdlib>
#include <vector>

#include "TMParser.h"
#include "TMTaskListStates.h"
#include "TMTaskList.h"
#include "TMCommandCreator.h"

enum TMDisplay {
    Default, DeadlineTasks, UndatedTasks, ArchivedTasks, SearchResults
	};


class TMExecutor {
	
private:
	
	std::vector<std::string> _unmanagedUserInput;
	TMExecutor::TMDisplay _currentDisplay;
	std::string _resultOfExecution;
	std::vector<int> _positionIndexes;
	
	static TMExecutor* theOne;
	TMExecutor();
	
public:
	
	static TMExecutor* getInstance();
	
	std::vector<std::string> getUserInput();
	TMDisplay getCurrentDisplay();
	std::string getResultOfExecution();
	std::vector<int> getPositionIndexes();
	
	void setCurrentDisplay(TMDisplay display);

	bool isDisplayChange(std::string userInput);
	TMDisplay determineDisplayType(std::string userInput);
	void executeMain(std::string userInput);
	

	
	
};
#endif