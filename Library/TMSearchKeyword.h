#ifndef TMSEARCHKEYWORD_H
#define TMSEARCHKEYWORD_H

#include "TMCommand.h"

const std::string SEARCH_CONCLUSION = "Number of tasks containing the keyword <";

class TMSearchKeyword: public TMCommand {

public:
	void execute() {
		TMParser *parser = TMParser::getInstance(); 
		TMTaskListStates *taskListStates = TMTaskListStates::getInstance();
		TMTaskList taskList = taskListStates->getCurrentTaskList();

		positionIndexes = taskList.keywordSearch(parser->parseSearchKey());
		std::ostringstream oss;
		oss << SEARCH_CONCLUSION << parser->parseSearchKey() << "> is: " positionIndexes.size();
		outcome = oss.str();
	}

};

#endif