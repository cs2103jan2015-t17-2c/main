#ifndef TMSEARCHDATE_H
#define TMSEARCHDATE_H

#include "TMCommand.h"

const std::string SEARCH_CONCLUSION_DATE = "Number of tasks with the date <";

class TMSearchDate: public TMCommand { 
	
public:
	void execute() {
		/*TMParser *parser = TMParser::getInstance(); 
		TMTaskListStates *taskListStates = TMTaskListStates::getInstance();
		TMTaskList taskList = taskListStates->getCurrentTaskList();

		positionIndexes = taskList.dateSearch(parser->parseSearchDate());
		std::ostringstream oss;
		oss << SEARCH_CONCLUSION_DATE << parser->parseSearchDate() << "> is: " << positionIndexes.size();
		outcome = oss.str();*/
		}
};
#endif