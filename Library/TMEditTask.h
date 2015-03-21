#ifndef TMEDITTASK_H
#define TMEDITTASK_H

#include "TMCommand.h"
#include "TMParser.h"
#include "TMTaskList.h"


class TMEditTask : public TMCommand {

public:
	
	void execute() {
		TMParser *parser = TMParser::getInstance(); 
		TMTaskList *taskList = TMTaskList::getInstance();
		std::vector<std::string> tokens = parser->returnTokens();
		taskList->updateTask(std::stoi(tokens[0]), tokens[1], tokens[2]);
	}

	void undo();
};
#endif