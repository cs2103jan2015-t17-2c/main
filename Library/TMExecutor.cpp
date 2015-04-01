#include "TMExecutor.h"
#include <string>

TMExecutor* TMExecutor::theOne;

TMExecutor* TMExecutor::getInstance() {
	if (theOne == NULL) {
		theOne = new TMExecutor();
	}
	return theOne; 
}

TMExecutor::TMExecutor() {
	_currentDisplay= TMDisplay::Default;
}

void TMExecutor::executeMain(std::string userInput) {
	//_displayInfo.clear();
	//_displayColours.clear();

	TMParser *parser = TMParser::getInstance(); 
	TMTaskListStates *taskListStates = TMTaskListStates::getInstance();
	TMTaskList taskList = taskListStates->getCurrentTaskList();
	TMCommandCreator cmdCreator;

	parser->initialize(userInput);
	std::string command = parser->extractCommand();
	
	if (isDisplayChange(command)) {
		_currentDisplay = determineDisplayType(command);
	} else {
	TMParser::CommandTypes type = parser->determineCommandType(command);
	TMCommand* commandObjPtr = cmdCreator.createNewCommandObj(type);
	commandObjPtr->execute();
	_resultOfExecution = commandObjPtr->outcome;
	}
	//taskList.writeToFile();
	//taskList.leaveReferenceUponExit();

	//setDisplayInfo();
	//setDisplayColours();

	return;
}

std::string TMExecutor::returnResultOfExecution() {
	return _resultOfExecution;
}

bool TMExecutor::isDisplayChange(std::string userInput) {
	return (userInput.find("display") != std::string::npos);
}

TMDisplay TMExecutor::determineDisplayType(std::string userInput) {
	if (userInput == "viewdefault") {
		return Default;
	}else if (userInput == "viewdate") {
		return SpecificDate;
	}else if (userInput == "viewdeadline") {
		return DeadlineTasks;
	}else if (userInput == "viewundated") {
		return UndatedTasks;
	}else if (userInput == "viewarchived") {
		return ArchivedTasks;
	}
}

/*
void TMExecutor::setDisplayInfo() {
	
	std::vector<std::string> displayInfo;

	switch (_currentDisplay) {
	
	case Default:
		displayInfo = getDefaultInfo();
		std::cout << "GET DEFAULT CALLED....." << std::endl;
		break;

	case SpecificDate:
		displayInfo = getSpecificDateInfo();
		break;

	case DeadlineTasks:
		displayInfo = getAllDeadlineInfo();
		break;

	case UndatedTasks:
		displayInfo = getAllUndatedInfo();
		break;

	case ArchivedTasks:
		displayInfo = getAllArchivedInfo();
		break;

	}

	std::cout << "DISPLAY INFO SIZE: " << displayInfo.size() << std::endl;
	_displayInfo = displayInfo; 
}

void TMExecutor::setDisplayColours() {}

std::vector<std::string> TMExecutor::getDisplayInfo() {
	return _displayInfo;
}

std::vector<std::string> TMExecutor::getDefaultInfo() {
	TMTaskListStates *taskListStates = TMTaskListStates::getInstance();
	TMTaskList taskList = taskListStates->getCurrentTaskList();
	std::vector<TMTask> dated = taskList.getDated();
	std::vector<TMTask> undated = taskList.getUndated();
	std::vector<TMTask> archived = taskList.getArchived();
	std::vector<TMTask>::iterator iter;
	std::vector<std::string> results;

	results.push_back(getHeaderInfo());

	//GETTING ALL DATED TASKS
	for (iter = dated.begin(); iter != dated.end(); ++iter) {
		TaskType type = iter->getTaskType();
		switch (type) {
		case WithStartDateTime:
			results.push_back(getOneWithStartInfo(*iter));
			break;

		case WithEndDateTime:
			results.push_back(getOneWithEndInfo(*iter));
			break;

		case WithPeriod:
			results.push_back(getOnePeriodInfo(*iter));
			break;
		}
	}

	//GETTING ALL UNDATED TASKS
	for (iter = undated.begin(); iter != undated.end(); ++iter) {
		results.push_back(getOneUndatedInfo(*iter));
	}

	//GETTING ALL ARCHIVED TASKS
	for (iter = archived.begin(); iter != archived.end(); ++iter) {
		TaskType type = iter->getTaskType();
		switch (type) {
		case WithStartDateTime:
			results.push_back(getOneWithStartInfo(*iter));
			break;

		case WithEndDateTime:
			results.push_back(getOneWithEndInfo(*iter));
			break;

		case WithPeriod:
			results.push_back(getOnePeriodInfo(*iter));
			break;

		case Undated:
			results.push_back(getOneUndatedInfo(*iter));
			break;
		}
	}
	std::cout << "RESULTS SIZE: " << results.size() << std::endl;
	return results;
}

std::vector<std::string> TMExecutor::getSpecificDateInfo() {
	std::vector<std::string> results;
	return results;
}
std::vector<std::string> TMExecutor::getAllDeadlineInfo() {
	std::vector<std::string> results;
	return results;
}
std::vector<std::string> TMExecutor::getAllUndatedInfo() {
	std::vector<std::string> results;
	return results;
}
std::vector<std::string> TMExecutor::getAllArchivedInfo() {
	std::vector<std::string> results;
	return results;
}

std::string TMExecutor::getHeaderInfo() {
	std::ostringstream oss; 
	oss << std :: left << std :: setw(18) << std :: setfill(' ') << "TASK DECSCRIPTION" << "\t" <<
	"START DATE" << "\t" << "START TIME" << "\t" << "END DATE" << "\t" << "END TIME" << "\t" << "CONFIRMATION" << "\t" << "CLASH STATUS" << "\n";
	return oss.str();
}

std::string TMExecutor::getOneWithStartInfo(TMTask task) {
	std::ostringstream oss; 
	oss << std :: left << std :: setw(18) << std :: setfill(' ')  << task.getTaskDescription() << "\t\t\t" <<
	std :: setw (18) << "\t\t" << task.getTaskTime().getStartDate() << "\t" << task.getTaskTime().getStartTime() << "\t" << 
	std::boolalpha << task.isConfirmed() << std::boolalpha << std::endl;
	return oss.str();
}

std::string TMExecutor::getOneWithEndInfo(TMTask task) {
	std::ostringstream oss; 
	oss << std :: left << std :: setw(18) << std :: setfill(' ')  << task.getTaskDescription() << "\t\t\t" <<
	std :: setw (18) << "\t\t" << task.getTaskTime().getEndDate() << "\t" << task.getTaskTime().getEndTime() << "\t" << 
	std::boolalpha << task.isConfirmed() << std::boolalpha << std::endl;
	return oss.str();
}

std::string TMExecutor::getOnePeriodInfo(TMTask task) {
	std::ostringstream oss;
	oss << std :: left << std :: setw(18) << std :: setfill(' ') << task.getTaskDescription() << "\t\t" << task.getTaskTime().getStartDate() <<
	"\t" << task.getTaskTime().getStartTime() << "\t\t" << task.getTaskTime().getEndDate() << "\t" << task.getTaskTime().getEndTime() << 
	std::boolalpha << task.isConfirmed() << std::boolalpha << task.isClashed() << std::endl;
	return oss.str();
}

std::string TMExecutor::getOneUndatedInfo(TMTask task) {
	std::ostringstream oss;
	oss << task.getTaskDescription() << "\t\t" << std::boolalpha << task.isConfirmed() << std::endl;
	return oss.str();
}

*/