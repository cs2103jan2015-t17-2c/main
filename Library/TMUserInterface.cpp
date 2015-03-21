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