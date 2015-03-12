#ifndef _TMEXECUTOR_H_
#define _TMEXECUTOR_H_

#include <vector>
#include <iostream>
#include <string>

#include "TMExecutor.h"
#include "TMTaskList.h"
#include "TMTask.h"


class TMExecutor{
	public:
	void sortCommandToFunctions(std::string command, TMTask task, TMTaskList tasklist);
	void addTask(TMTask task, TMTaskList tasklist);
	void blockMultiple(vector<TMTask> tasks, TMTaskList tasklist) ;
	void freeMultiple(vector<TMTask> confirmedTasks, TMTaskList tasklist);
	void updateTaskDetail(TMTask task, std::string component, std::string changeTo, TMTaskList tasklist);
	void markAllAsDone(Time todaysDate, TMTaskList tasklist);
	void deleteTask(TMTask task, TMTaskList tasklist);
	//void undoLast();
	void string searchFreeTime(TMTaskList tasklist);
	void searchEntry(std::string keyword, bool isCaseSensitive=false, TMTaskList tasklist);
	//void saveAt(std::string directory);
};

#endif _TMEXECUTOR_H_