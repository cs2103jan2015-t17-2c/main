#ifndef TMBLOCKTIME_H
#define TMBLOCKTIME_H

#include "TMCommand.h"
const std::string BATCH_NUMBER_INFO = "The batch number for this batch of unconfirmed task(s) is: ";

class TMBlockTime : public TMCommand {
	
public:

	void execute() {
		TMTaskListStates *taskListStates = TMTaskListStates::getInstance();
		TMParser *parser = TMParser::getInstance(); 
		TMTaskList taskList = taskListStates->getCurrentTaskList();
		std::ostringstream oss;
		int i = taskList.getUniqueBatchNum();
		oss << BATCH_NUMBER_INFO << i << std::endl;

		if (parser->isMultipleTimingTask()) {
			std::vector<TMTask> tasks = parser->parseMultipleTimingTaskInfo();
			std::vector<TMTask>::iterator iter;
		
			for (iter = tasks.begin(); iter != tasks.end(); ++iter) {
				iter->setAsUnconfirmed();
				iter->setUnconfirmedBatchNumber(i);
				oss << taskList.addTask(*iter) << std::endl;
			}
		} else {
			TMTask task = parser->parseTaskInfo();
			task.setAsUnconfirmed();
			task.setUnconfirmedBatchNumber(i);
			oss << taskList.addTask(task) << std::endl;
		}
		
		outcome = oss.str();
		taskListStates->addNewState(taskList);
	}

};
#endif