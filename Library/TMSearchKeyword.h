#ifndef TMSEARCHKEYWORD_H
#define TMSEARCHKEYWORD_H

#include "TMCommand.h"
#include "TMTaskList.h"
#include "TMParser.h"

class TMSearchKeyword: public TMCommand {

public:
	void execute() {
		TMParser *parser = TMParser::getInstance(); 
		TMTaskListStates *taskListStates = TMTaskListStates::getInstance();
		TMTaskList taskList = taskListStates->getCurrentTaskList();
		std::vector<int> searchResults;
		searchResults = taskList.keywordSearch(parser->parseSearchKey());

		std::vector<int>::iterator iter;
		std::ostringstream oss;

		for (iter = searchResults.begin(); iter != searchResults.end(); ++iter) {
			TMTask result = taskList.getTaskFromPositionIndex(*iter);
			oss << *iter << ". " << result.getTaskDescription() << std::endl;
		}

		outcome = oss.str();
	}

};

#endif