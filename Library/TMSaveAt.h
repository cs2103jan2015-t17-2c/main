#ifndef TMSAVEAT_H
#define TMSAVEAT_H

#include "TMCommand.h"
#include "TMTaskList.h"
#include "TMParser.h"

class TMSaveAt : public TMCommand {
public:
	void execute() {
		TMParser *parser = TMParser::getInstance(); 
		TMTaskList *taskList = TMTaskList::getInstance();
		taskList->saveFileAt(parser->parseDirectory());
	}
	void undo();
};
#endif