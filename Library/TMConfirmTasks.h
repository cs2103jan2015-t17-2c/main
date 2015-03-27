#ifndef TMCONFIRMTASKS_H
#define TMCONFIRMTASKS_H


#include "TMCommand.h"

class TMConfirmTasks : public TMCommand {
public: 
	void execute() {
		TMParser *parser = TMParser::getInstance(); 
		TMTaskListStates *taskListStates = TMTaskListStates::getInstance();
		TMTaskList taskList = taskListStates->getCurrentTaskList();
		std::vector<int> confirmedTasksIndexes = parser->parseTaskPositionNo();
		std::vector<int>::iterator iter;

		for (iter = confirmedTasksIndexes.begin(); iter != confirmedTasksIndexes.end(); ++iter) {
				taskList.updateTask(*iter, "completion", "1");
		}

		TMTask oneConfirmedTask = taskList.getTaskFromPositionIndex(confirmedTasksIndexes[0]);
		std::string confirmedTaskDesc = oneConfirmedTask.getTaskDescription();

		std::vector<int> searchResults;
		searchResults = taskList.keywordSearch(confirmedTaskDesc);

		for (iter = searchResults.begin(); iter != searchResults.end(); ++iter) {
			TMTask task = taskList.getTaskFromPositionIndex(*iter);
			if ( !(task.isConfirmed()) ) {
				taskList.removeTask(*iter);
			}
		}

		taskListStates->addNewState(taskList);
	}

};
#endif