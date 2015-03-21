#ifndef TMCONFIRMTASKS_H
#define TMCONFIRMTASKS_H

#include "TMTask.h"
#include "TMCommand.h"
#include "TMTaskList.h"
#include "TMParser.h"

class TMConfirmTasks : public TMCommand {
public: 
	void execute() {
		TMParser *parser = TMParser::getInstance(); 
		TMTaskList *taskList = TMTaskList::getInstance();
		std::vector<int> confirmedTasksIndexes = parser->parseTaskPositionNo();
		std::vector<int>::iterator iter;

		for (iter = confirmedTasksIndexes.begin(); iter != confirmedTasksIndexes.end(); ++iter) {
				taskList->updateTask(*iter, "completion", "1");
		}

		TMTask oneConfirmedTask = taskList->getTaskFromPositionIndex(confirmedTasksIndexes[0]);
		std::string confirmedTaskDesc = oneConfirmedTask.getTaskDescription();

		std::vector<int> searchResults;
		searchResults = taskList->keywordSearch(confirmedTaskDesc);

		for (iter = searchResults.begin(); iter != searchResults.end(); ++iter) {
			TMTask task = taskList->getTaskFromPositionIndex(*iter);
			if ( !(task.isConfirmed()) ) {
				taskList->removeTask(*iter);
			}
		}
	}

	void undo() {}
};
#endif