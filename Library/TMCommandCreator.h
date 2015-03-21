//CREATES A COMMAND OBJECT BASED ON THE COMMAND RETURNED BY PARSER
//INCLUDES ALL THE COMMAND CLASSES

#ifndef TMCOMMANDCREATOR_H
#define TMCOMMANDCREATOR_H

#include "TMCommand.h"
#include "TMAddTasks.h"
#include "TMDeleteTasks.h"

class TMCommandCreator {
public:

	TMCommand* createNewCommandObj(std::string command) {
		if (command == "add") {
			TMAddTasks* newCmdPtr = new TMAddTasks();
			return newCmdPtr;
		}

		if (command == "delete") {
			TMDeleteTasks* newCmdPtr = new TMDeleteTasks();
			return newCmdPtr;
		}	
	}
};

#endif