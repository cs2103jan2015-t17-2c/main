//CREATES A COMMAND OBJECT BASED ON THE COMMAND RETURNED BY PARSER
//INCLUDES ALL THE COMMAND CLASSES

#ifndef TMCOMMANDCREATOR_H
#define TMCOMMANDCREATOR_H

#include "TMCommand.h"
#include "TMParser.h"
#include "TMAddTasks.h"
#include "TMDeleteTasks.h"
#include "TMEditTask.h"
#include "TMSearchKeyword.h"
#include "TMSearchDate.h"
#include "TMSaveAt.h"
#include "TMBlockTime.h"
#include "TMConfirmTasks.h"
#include "TMDoneAllToday.h"
#include "TMCompleteTasks.h"
#include "TMUndo.h"
#include "TMRedo.h"
#include "TMInvalidCommand.h"

class TMCommandCreator {
public:

	TMCommand* createNewCommandObj(TMParser::CommandTypes command) {
		TMCommand *newCmdPtr;
		switch (command) {
		case TMParser::CommandTypes::Add:
			newCmdPtr = new TMAddTasks();
			return newCmdPtr;
			break;

		case TMParser::CommandTypes::Delete:
			newCmdPtr = new TMDeleteTasks();
			return newCmdPtr;
			break;

		case TMParser::CommandTypes::Edit:
			newCmdPtr = new TMEditTask();
			return newCmdPtr;
			break;

		case TMParser::CommandTypes::SearchKeyword:
			newCmdPtr = new TMSearchKeyword();
			return newCmdPtr;
			break;

		case TMParser::CommandTypes::SearchDate:
			newCmdPtr = new TMSearchDate();
			return newCmdPtr;
			break;

		case TMParser::CommandTypes::SaveAt:
			newCmdPtr = new TMSaveAt();
			return newCmdPtr;
			break;

		case TMParser::CommandTypes::Block:
			newCmdPtr = new TMBlockTime();
			return newCmdPtr;
			break;

		case TMParser::CommandTypes::Confirm:
			newCmdPtr = new TMConfirmTasks();
			return newCmdPtr;
			break;

		case TMParser::CommandTypes::Complete: 
			newCmdPtr = new TMCompleteTasks();
			return newCmdPtr;
			break;

		case TMParser::CommandTypes::CompleteAllToday:
			newCmdPtr = new TMDoneAllToday();
			return newCmdPtr;
			break;

		case TMParser::CommandTypes::Incomplete:;
		
		case TMParser::CommandTypes::Undo: 
			newCmdPtr = new TMUndo();
			return newCmdPtr;
			break;

		case TMParser::CommandTypes::Redo:
			newCmdPtr = new TMRedo();
			return newCmdPtr;
			break;

		case TMParser::CommandTypes::Invalid:
			newCmdPtr = new TMInvalidCommand();
			return newCmdPtr;
			break;
		
		}
	}
};

#endif