#include <iostream>
#include "TMUserInterface.h"
#include "TMTaskListStates.h"

TMUserInterface* TMUserInterface::theOne;


TMUserInterface* TMUserInterface::getInstance() {
	if (theOne == NULL) {
		theOne = new TMUserInterface;
	}
	return theOne;
}

TMUserInterface::TMUserInterface() {}

void TMUserInterface::promptForUserInput() {
	std::cout << "Welcome to TimeMaster. Please enter your command" << std::endl;
}

std::string TMUserInterface::returnUserInput() {
	std::string userInput;
	getline(std::cin, userInput);
	return userInput;
}

void TMUserInterface:: SetColor(Color c){
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, c);

}


void TMUserInterface::displayDefault() {

	TMTaskListStates *taskListStates = TMTaskListStates::getInstance();
	TMTaskList taskList = taskListStates->getCurrentTaskList();

	std::vector<TMTask> dated = taskList.getDated();
	std::vector<TMTask> undated = taskList.getUndated();
	std::vector<TMTask> archived = taskList.getArchived();

	std::cout << "Number of scheduled tasks: " << dated.size() <<std::endl;
	std::cout << "Number of undated tasks:" << undated.size() <<std::endl;
	std::cout << "Details:" << "\n\n";
	std::vector<TMTask>::iterator iter;

	std::cout << std :: left << std :: setw(18) << std :: setfill(' ') << "TASK DECSCRIPTION" << "\t" <<
	"START DATE" << "\t" << "START TIME" << "\t" << "END DATE" <<
	"\t" << "END TIME" ;
		
	for (iter = dated.begin(); iter != dated.end(); ++iter) {
		if ((*iter).getTaskType() == TaskType::WithDeadline) {
			SetColor(Color::RED);
			std::cout << std :: left << std :: setw(18) << std :: setfill(' ') << (*iter).getTaskDescription() << "\t" <<
			std :: setw (18) << "\t\t" << (*iter).getTaskTime().getEndDate() <<
			"\t" << (*iter).getTaskTime().getEndTime() <<std::endl;
			SetColor(Color::GRAY);

		} else {
			if (taskList.isInClashes(*iter)) {
					SetColor(Color::BLUE);
			} else {
				SetColor(Color::WHITE);
			}
			printPeriodTask(*iter);
		}
	}
	
	for (iter = undated.begin(); iter != undated.end(); ++iter) {
		SetColor(Color::YELLOW);
		std::cout << (*iter).getTaskDescription() << std::endl;
		SetColor(Color::GRAY);
	}

	std::cout << std::endl;
	std::cout << "TOTAL NUM CLASHES: " << taskList.getClashesSize() << std::endl;
}	

void TMUserInterface::displayFreeTime() {
	TMTaskListStates *taskListStates = TMTaskListStates::getInstance();
	TMTaskList taskList = taskListStates->getCurrentTaskList();
	std::vector<TMTask> dated = taskList.getDated();
	std::vector<TMTask>::iterator iter;
	
	for (iter = dated.begin(); iter != dated.end(); ++iter) {
		std::cout << (*iter).getTaskTime().getEndDate() << " " << (*iter).getTaskTime().getEndTime();
		++iter;
		std::cout << " ~ " << (*iter).getTaskTime().getStartDate() << " " << (*iter).getTaskTime().getStartTime() << '\n\n';
	}
	
}

void TMUserInterface::printPeriodTask(TMTask task) {
	std::cout << std :: left << std :: setw(18) << std :: setfill(' ') << task.getTaskDescription() << "\t" << task.getTaskTime().getStartDate() << 
			"\t" << task.getTaskTime().getStartTime() << 
			"\t\t" << task.getTaskTime().getEndDate() <<
			"\t" << task.getTaskTime().getEndTime() <<std::endl;
			SetColor(Color::GRAY);
}
