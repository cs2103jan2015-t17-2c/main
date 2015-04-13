//@author A0114130E
#ifndef TMSEARCHKEYWORD_H
#define TMSEARCHKEYWORD_H

#include "TMCommand.h"


class TMSearchKeyword: public TMCommand {

public:
	void execute() {
		TMParser *parser = TMParser::getInstance(); 
		TMTaskListStates *taskListStates = TMTaskListStates::getInstance();
		TMTaskList taskList = taskListStates->getCurrentTaskList();

		positionIndexes = taskList.keywordSearch(parser->parseSearchKey());
		std::ostringstream oss;
		oss << SEARCH_CONCLUSION_KEYWORD_PART1 << parser->parseSearchKey() << SEARCH_CONCLUSION_KEYWORD_PART2 << positionIndexes.size();
		outcome = oss.str();
	}

};

#endif