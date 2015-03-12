#include <vector>
#include <iostream>
#include <string>
#include "TMExecutor.h"
#include "TMTask.h"
#include "TMTaskList.h"
#include <boost\date_time.hpp>

void TMExecutor::sortCommandToFunctions(std::string command, TMTask task, TMTaskList tasklist) {
	if (command == 'add') {
		addTask(task, tasklist);
	}

	if (command == 'block') {
		blockMultiple(tasks, tasklist);
	}

	if (command == 'freeup') {
		freeMultiple(confirmedTasks, tasklist);
	}
}

void TMExecutor::addTask(TMTask task, TMTaskList tasklist) {
	tasklist.addTask(task);
}

void TMExecutor::blockMultiple(vector<TMTask> tasks, TMTaskList tasklist) {
	vector<TMTask> iterator::iter;
	for (iter = tasks.begin(); iter != tasks.end(); ++iter) {
		tasklist.addTask(*iter);
	}
}


void TMExecutor::freeMultiple(vector<TMTask> confirmedTasks, TMTaskList tasklist) {
	vector<TMTask> iterator::iter;

	//Mark confirmed tasks as confirmed
	for (iter = confirmedTasks.begin(); iter != confirmedTasks.end(); ++iter) {
		tasklist.updateTask(tasklist.searchOneTask(*iter), isConfirmed, true);
		++iter;
	}

	iter = confirmedTasks.begin();
	string taskDescriptionOfConfirmed = (*iter).getTaskDescription();

	//Delete unconfirmed tasks
	for (iter = tasklist.begin(); iter != tasklist.end(); ++iter) {
		if ( (*iter).getTaskDescription() == taskDescriptionOfConfirmed && (*iter).getIsConfirmed() == false ) {
			tasklist.removeTask(tasklist.searchOneTask(*iter));
		} 
	}
}


void TMExecutor::updateTaskDetail(TMTask task, string component, string changeTo, TMTaskList tasklist) {
	tasklist.updateTask(tasklist.searchOneTask(task), component, changeTo);
}
	
void TMExecutor::markAllAsDone(Time todaysDate, TMTaskList tasklist) {}
	

void TMExecutor::deleteTask(TMTask task, TMTaskList tasklist) {
	tasklist.removeTask(tasklist.searchOneTask(task));
}

void TMExecutor::undoLast() {}
	
void TMExecutor::string searchFreeTime() {}
	
void TMExecutor::searchEntry(string keyword, bool isCaseSensitive=false, TMTaskList tasklist) {
		std::cout << tasklist.searchTasks(keyword, isCaseSensitive) << endl;
}

void TMExecutor::saveAt(string directory) {}
