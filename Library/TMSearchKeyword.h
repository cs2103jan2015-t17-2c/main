#ifndef TMSEARCHKEYWORD_H
#define TMSEARCHKEYWORD_H

#include "TMCommand.h";
#include "TMTasklist.h"

class TMSearchKeyword: public TMCommand {

public:
	void execute(std::string keyword, TMTaskList taskList) {
		std::vector<int> searchResults;
		searchResults = taskList.keywordSearch(keyword);

		std::vector<int>::iterator iter;
		for (iter = searchResults.begin(); iter != searchResults.end(); ++iter) {
			TMTask result = taskList.getTaskFromPositionIndex(*iter);
			std::cout << *iter << ". " << result.getTaskDescription() << std::endl;
		}
	}

	void undo();

};

#endif