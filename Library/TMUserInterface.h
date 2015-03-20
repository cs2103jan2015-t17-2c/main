#ifndef TMUSERINTERFACE_H
#define TMUSERINTERFACE_H

#include <string>

class TMUserInterface {
private:
	static TMUserInterface* theOne;
	TMUserInterface();

public:
	static TMUserInterface* getInstance();
	void promptForUserInput();
	std::string returnUserInput();
	void displayDefault();
};

#endif