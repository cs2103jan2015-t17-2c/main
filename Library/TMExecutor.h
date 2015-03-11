#pragma once
#include <vector>
#include <iostream>
#include "TMTask.h"
#include "TMTaskList.h"

#include <boost\date_time.hpp>


class TMExecutor{
	public:
	void addTimedAndDeadline(TMTask task, TaskList tasklist);
	void addFloating(Task task, TaskList tasklist);
	void blockMultiple(vec<Task> tasks, TaskList tasklist);
	void freeMultiple(vec<Task> confirmedTasks, TaskList tasklist);
	void updateTaskDetail(Task task, string component, string changeTo);
	void markAllAsDone(Time todaysDate, TaskList tasklist);
	void deleteTask(Task task, TaskList tasklist);
	void undoLast
	void string searchFreeTime();
	void searchEntry
	void saveAt(string directory);
	void repeatTask(Task task, TaskList tasklist);
};