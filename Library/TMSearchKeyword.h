#ifndef TMSEARCHKEYWORD_H
#define TMSEARCHKEYWORD_H

#include "TMCommand.h";
#include "TMTaskList.h"
#include "TMParser.h"

class TMSearchKeyword: public TMCommand {

public:
	void execute() {
		TMParser *parser = TMParser::getInstance(); 
		TMTaskList *taskList = TMTaskList::getInstance();
		std::vector<int> searchResults;
		searchResults = taskList->keywordSearch(parser->parseSearchKey());

		std::vector<int>::iterator iter;
		for (iter = searchResults.begin(); iter != searchResults.end(); ++iter) {
			TMTask result = taskList->getTaskFromPositionIndex(*iter);
			std::cout << *iter << ". " << result.getTaskDescription() << std::endl;
		}
	}

	void undo();

};

#endif