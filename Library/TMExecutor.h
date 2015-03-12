#pragma once
#include <vector>
#include <iostream>
#include <string>
#include "TMTask.h"
#include "TMmemory.h"
#include <boost\date_time.hpp>


class TMExecutor{
	public:
	void addTimedAndDeadline(TMTask task, TMmemory tasklist);
	void addFloating(TMTask task, TMmemory tasklist);
	void blockMultiple(vector<TMTask> tasks, TMmemory tasklist);
	void freeMultiple(vector<TMTask> confirmedTasks, TMmemory tasklist);
	void updateTaskDetail(TMTask task, string component, string changeTo);
	void markAllAsDone(Time todaysDate, TMmemory tasklist);
	void deleteTask(TMTask task, TMmemory tasklist);
	void undoLast();
	void searchFreeTime();
	void searchEntry();
	void saveAt(string directory);
	void repeatTask(TMTask task, TMmemory tasklist);
};