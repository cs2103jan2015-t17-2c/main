#ifndef TMBLOCKTIME_H
#define TMBLOCKTIME_H

#include "TMCommand.h"

class TMBlockTime : public TMCommand {
	
public:

	void execute() {
		TMTaskListStates *taskListStates = TMTaskListStates::getInstance();
		TMParser *parser = TMParser::getInstance(); 
		TMTaskList taskList = taskListStates->getCurrentTaskList();
		std::vector<TMTask> unmanaged = parser->parseTaskInfo();
		std::vector<TMTask>::iterator iter;
		
		int i = taskList.getUniqueBatchNum();
		std::cout << "BATCH NUM IS: " << i <<std::endl; 

		for (iter = unmanaged.begin(); iter != unmanaged.end(); ++iter) {
			iter->setAsUnconfirmed();
			iter->setUnconfirmedBatchNum(i);
			taskList.addTask(*iter);
			
		}
		
		taskListStates->addNewState(taskList);
	}

};
#endif