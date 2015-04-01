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
	
	TMExecutor::TMDisplay _currentDisplay;
	//std::vector<std::string> _displayInfo;
	//std::vector<std::string> _displayColours;
	std::string _resultOfExecution;
	std::vector<int> _positionIndexes;
	static TMExecutor* theOne;
	TMExecutor();
	
public:
	
	static TMExecutor* getInstance();
	void executeMain(std::string userInput);
	std::string returnResultOfExecution();
	void setCurrentDisplay(TMDisplay display);
	TMDisplay getCurrentDisplay();
	std::vector<int> getPositionIndexes();

	bool isDisplayChange(std::string userInput);
	TMDisplay determineDisplayType(std::string userInput);
	
	//void setDisplayInfo();
	//void setDisplayColours();
	//std::vector<std::string> getDisplayInfo();
	//std::vector<std::string> getDisplayColours();

	//GORDY LOOK AT THESE FUNCTIONS
	/*std::vector<std::string> getDefaultInfo();
	std::vector<std::string> getSpecificDateInfo();
	std::vector<std::string> getAllDeadlineInfo();
	std::vector<std::string> getAllUndatedInfo();
	std::vector<std::string> getAllArchivedInfo();

	std::string getHeaderInfo();
	std::string getOneWithStartInfo(TMTask task);
	std::string getOneWithEndInfo(TMTask task);
	std::string getOnePeriodInfo(TMTask task);
	std::string getOneUndatedInfo(TMTask task);*/

	
	
};
#endif