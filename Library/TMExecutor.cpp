#include <vector>
#include <iostream>
#include <string>
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
	std::string taskDescriptionOfConfirmed = (*iter).getTaskDescription();

	//Delete unconfirmed tasks
	for (iter = tasklist.begin(); iter != tasklist.end(); ++iter) {
		if ( (*iter).getTaskDescription() == taskDescriptionOfConfirmed && (*iter).getIsConfirmed() == false ) {
			tasklist.removeTask(tasklist.searchOneTask(*iter));
		} 
	}
}


void TMExecutor::updateTaskDetail(TMTask task, std::string component, std::string changeTo, TMTaskList tasklist) {
	tasklist.updateTask(tasklist.searchOneTask(task), component, changeTo);
}

//Mark all tasks that are due today as done
void TMExecutor::markAllAsDone(Time todaysDate, TMTaskList tasklist) {
	date dateToday(day_clock::local_day());
	vector<TMTask> iterator::iter;
	for (iter = tasklist.begin(); iter != tasklist.end(); ++iter) {
		if ((*iter).getTaskEndDate() == dateToday) {
			(*iter).setAsCompleted(); 
		}
	}
}
	

void TMExecutor::deleteTask(TMTask task, TMTaskList tasklist) {
	tasklist.removeTask(tasklist.searchOneTask(task));
}

//void TMExecutor::undoLast() {}
	
void TMExecutor::std::string searchFreeTime(TMTaskList tasklist) {
	vector<TMTask> iterator::iter;
	for (iter = tasklist.begin(); iter != tasklist.end(); ++iter) {
	cout << (*iter).getEndTime() << " " << (*iter).getEndDate();
	++iter;
	cout << " ~ " << (*iter).getStartTime() << " " << (*iter).getStartDate(); }
}
	
void TMExecutor::searchEntry(std::string keyword, bool isCaseSensitive=false, TMTaskList tasklist) {
		std::cout << tasklist.searchTasks(keyword, isCaseSensitive) << endl;
}

//void TMExecutor::saveAt(std::string directory) {}
