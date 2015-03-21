//CREATES A COMMAND OBJECT BASED ON THE COMMAND RETURNED BY PARSER
//INCLUDES ALL THE COMMAND CLASSES

#ifndef TMCOMMANDCREATOR_H
#define TMCOMMANDCREATOR_H

#include "TMCommand.h"
#include "TMAddTasks.h"
#include "TMDeleteTasks.h"
#include "TMEditTask.h"
#include "TMSearchKeyword.h"
#include "TMSaveAt.h"
#include "TMConfirmTasks.h"
#include "TMDoneAllToday.h"
#include "TMViewFreeTime.h"
#include "TMCompleteTasks.h"

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

		if (command == "edit") {
			TMEditTask* newCmdPtr = new TMEditTask();
			return newCmdPtr;
		}	

		if (command == "search") {
			TMSearchKeyword* newCmdPtr = new TMSearchKeyword();
			return newCmdPtr;
		}	

		if (command == "saveat") {
			TMSaveAt* newCmdPtr = new TMSaveAt();
			return newCmdPtr;
		}	

		if (command == "confirm") {
			TMConfirmTasks* newCmdPtr = new TMConfirmTasks();
			return newCmdPtr;
		}	

		if (command == "doneall") {
			TMDoneAllToday* newCmdPtr = new TMDoneAllToday();
			return newCmdPtr;
		}	

		if (command == "viewfreetime") {
			TMViewFreeTime* newCmdPtr = new TMViewFreeTime();
			return newCmdPtr;
		}	

		if (command == "complete") {
			TMCompleteTasks* newCmdPtr = new TMCompleteTasks();
			return newCmdPtr;
		}	
	}
};

#endif