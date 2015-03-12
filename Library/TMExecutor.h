#pragma once
#include <vector>
#include <iostream>
#include <string>
#include "TMTask.h"
#include "TMTaskList.h"
#include <boost\date_time.hpp>


class TMExecutor{
	public:
	void addTimedAndDeadline(TMTask task, TMTaskList tasklist);
	void addFloating(TMTask task, TMTaskList tasklist);
	void blockMultiple(vector<TMTask> tasks, TMTaskList tasklist);
	void freeMultiple(vector<TMTask> confirmedTasks, TMTaskList tasklist);
	void updateTaskDetail(TMTask task, string component, string changeTo);
	void markAllAsDone(Time todaysDate, TMTaskList tasklist);
	void deleteTask(TMTask task, TMTaskList tasklist);
	void undoLast();
	void searchFreeTime();
	void searchEntry();
	void saveAt(string directory);
	void repeatTask(TMTask task, TMTaskList tasklist);
};