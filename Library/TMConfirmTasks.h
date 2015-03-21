#ifndef TMCONFIRMTASKS_H
#define TMCONFIRMTASKS_H

#include "TMCommand.h"
#include "TMTaskList.h"
#include "TMTask.h"

class TMConfirmTasks : public TMCommand {
public: 
	void execute(std::vector<int> confirmedTasksIndexes, TMTaskList &taskList) {
		std::vector<int>::iterator iter;
		for (iter = confirmedTasksIndexes.begin(); iter != confirmedTasksIndexes.end(); ++iter) {
				confirmTask(*iter);
		}

		TMTask oneConfirmedTask = taskList.getTaskFromPositionIndex(*(confirmedTasksIndexes.begin()));
		std::string confirmedTaskDesc = oneConfirmedTask.getTaskDescription();

		std::vector<int> searchResults;
		searchResults = taskList.keywordSearch(confirmedTaskDesc);

		for (iter = searchResults.begin(); iter != searchResults.end(); ++iter) {
			TMTask task = taskList.getTaskFromPositionIndex(*iter);
			if ( !(task.isConfirmed()) ) {
				taskList.removeTask(*iter);
			}
		}
	}

	void undo();
}
#endif