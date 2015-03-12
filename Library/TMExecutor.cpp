#include <vector>
#include <iostream>
#include "TMTask.h"
#include "TMmemory.h"
#include "TMExecutor.h"

void TMExecutor::addTask(TMTask task, TaskList tasklist) {
	tasklist.addTask(task);
}

void TMExecutor::blockMultiple(vector<TMTask> tasks, TaskList tasklist) {
	vector<TMTask> iterator::iter;
	for (iter = tasks.begin(); iter != tasks.end(); ++iter) {
		tasklist.addTask(*iter);
	}
}

void TMExecutor::freeMultiple(vector<TMTask> confirmedTasks, TaskList tasklist) {
	vector<TMTask> iterator::iter;

	for (iter = confirmedTasks.begin(); iter != confirmedTasks.end(); ++iter) {
		tasklist.updateTask(tasklist.searchOneTask(*iter), isPermanent, true);
		++iter;
	}

	iter = confirmedTasks.begin();
	string taskNameOfConfirmed = (*iter).getTaskName();

	for (iter = tasklist.begin(); iter != tasklist.end(); ++iter) {
		if ( (*iter).getTaskName() == taskNameOfConfirmed && (*iter).getIsPermanent() == false ) {
			tasklist.removeTask(tasklist.searchOneTask(*iter));
		} 
	}
}

void TMExecutor::updateTaskDetail(TMTask task, string component, string changeTo, TaskList tasklist) {
	tasklist.updateTask(tasklist.searchOneTask(task), component, changeTo);
}
	
void TMExecutor::markAllAsDone(Time todaysDate, TaskList tasklist) {
	}
	
void TMExecutor::deleteTask(TMTask task, TaskList tasklist) {
	tasklist.removeTask(tasklist.searchOneTask(task));
}
	void TMExecutor::undoLast;
	void TMExecutor::string searchFreeTime();
	void TMExecutor::searchEntry;
	void TMExecutor::saveAt(string directory);
	void TMExecutor::repeatTask(TMTask task, TaskList tasklist);
