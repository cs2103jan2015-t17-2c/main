#ifndef TMUSERINTERFACE_H
#define TMUSERINTERFACE_H

#include <string>
#include "TMTaskList.h"
#include "TMTask.h"
#include <Windows.h>
#include <sstream>

class TMUserInterface {
private:
	static TMUserInterface* theOne;
	TMUserInterface();
	enum Color {
		DARKBLUE = 1, DARKGREEN, DARKTEAL, DARKRED, DARKPINK, DARKYELLOW, GRAY, DARKGRAY, BLUE, GREEN, TEAL, RED, PINK, YELLOW, WHITE 
	};


public:
	static TMUserInterface* getInstance();
	void promptForUserInput();
	std::string returnUserInput();
	void SetColor(Color c);
	std::string displayDefault();
	void printPeriodTask(std::ostringstream& oss, TMTask task);

	//Postcondition: Returns a string consisting all the timings in which the user is free
	void TMUserInterface::displayFreeTime();
};

#endif