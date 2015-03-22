#include <iostream>
#include "TMUserInterface.h"

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

	TMTaskList *taskList = TMTaskList::getInstance();

	std::vector<TMTask> timedAndDeadline = taskList->getTimedAndDeadline();
	std::vector<TMTask> floating = taskList->getFloating();
	std::vector<TMTask> archived = taskList->getArchived();

	std::cout << "Number of scheduled tasks: " << timedAndDeadline.size() <<std::endl;
	std::cout << "Number of floating tasks:" << floating.size() <<std::endl;
	std::cout << "Details:" << "\n\n";
	std::vector<TMTask>::iterator iter;

	std::cout << std :: left << std :: setw(18) << std :: setfill(' ') << "TASK DECSCRIPTION" << "\t" <<
	"START DATE" << "\t" << "START TIME" << "\t" << "END DATE" <<
	"\t" << "END TIME" ;
		
	for (iter = timedAndDeadline.begin(); iter != timedAndDeadline.end(); ++iter) {
		if ((*iter).getTaskType() == TaskType::WithDeadline) {
			SetColor(Color::RED);
			std::cout << std :: left << std :: setw(18) << std :: setfill(' ') << (*iter).getTaskDescription() << "\t" <<
			std :: setw (18) << "\t\t" << (*iter).getTaskTime().getEndDate() <<
			"\t" << (*iter).getTaskTime().getEndTime() <<std::endl;
			SetColor(Color::GRAY);

		} else {
			SetColor(Color::PINK);
			std::cout << std :: left << std :: setw(18) << std :: setfill(' ') << (*iter).getTaskDescription() << "\t" << (*iter).getTaskTime().getStartDate() << 
			"\t" << (*iter).getTaskTime().getStartTime() << 
			"\t\t" << (*iter).getTaskTime().getEndDate() <<
			"\t" << (*iter).getTaskTime().getEndTime() <<std::endl;
			SetColor(Color::GRAY);
		}
	}
	
	for (iter = floating.begin(); iter != floating.end(); ++iter) {
		SetColor(Color::YELLOW);
		std::cout << (*iter).getTaskDescription() << std::endl;
		SetColor(Color::GRAY);
	}

	std::cout << std::endl;
}	

void TMUserInterface::displayFreeTime() {
	TMTaskList *taskList = TMTaskList::getInstance();
	std::vector<TMTask> timedAndDeadline = taskList->getTimedAndDeadline();
	std::vector<TMTask>::iterator iter;
	
	for (iter = timedAndDeadline.begin(); iter != timedAndDeadline.end(); ++iter) {
		std::cout << (*iter).getTaskTime().getEndDate() << " " << (*iter).getTaskTime().getEndTime();
		++iter;
		std::cout << " ~ " << (*iter).getTaskTime().getStartDate() << " " << (*iter).getTaskTime().getStartTime() << '\n\n';
	}
	
}
