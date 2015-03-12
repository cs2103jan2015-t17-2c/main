#include <vector>
#include <iostream>
#include "TMTask.h"
#include "TMTaskList.h"

void TMExecutor::addTask(TMTask task, TMTaskList tasklist) {
	tasklist.addTask(task);
}

void TMExecutor::blockMultiple(vec<TMTask> tasks, TMTaskList tasklist) {
	vec<TMTask> iterator::iter;
	for (iter = tasks.begin(); iter != tasks.end(); ++iter) {
		tasklist.addTask(*iter);
	}
}

void TMExecutor::freeMultiple(vec<TMTask> confirmedTasks, TMTaskList tasklist) {
	vec<TMTask> iterator::iter;

	for (iter = confirmedTasks.begin(); iter != confirmedTasks.end(); ++iter) {
		tasklist.updateTask(tasklist.searchOneTask(*iter), isConfirmed, true);
		++iter;
	}

	iter = confirmedTasks.begin();
	string taskDescriptionOfConfirmed = (*iter).getTaskDescription();

	for (iter = tasklist.begin(); iter != tasklist.end(); ++iter) {
		if ( (*iter).getTaskDescription() == taskDescriptionOfConfirmed && (*iter).getIsConfirmed() == false ) {
			tasklist.removeTask(tasklist.searchOneTask(*iter));
		} 
	}
}

void TMExecutor::updateTaskDetail(TMTask task, string component, string changeTo, TMTaskList tasklist) {
	tasklist.updateTask(tasklist.searchOneTask(task), component, changeTo);
}
	
void TMExecutor::markAllAsDone(Time todaysDate, TMTaskList tasklist) {
	}
	
void TMExecutor::deleteTask(TMTask task, TMTaskList tasklist) { 
	tasklist.removeTask(tasklist.searchOneTask(task));
}
	void TMExecutor::undoLast;
	void TMExecutor::string searchFreeTime();
	void TMExecutor::searchEntry(string keyword, bool isCaseSensitive=false) {
		std::cout << tasklist.searchTasks(keyword, isCaseSensitive) << endl;
	}
	void TMExecutor::saveAt(string directory);
	void TMExecutor::createTasksFromRecurring(TMRecurringTask recurringTask, TMTaskList tasklist) {
		
		if (recurringTask.getRepetitionFrequency() = "daily") {
			//get the number of days left in the year//
			for (int i=1; i<=numberOfDaysLeftInYear; i++) {
				TMTaskTime newTaskTime = recurringTask.getTaskTime() + 1day;
				TMTask task(recurringTask.getTaskDescription(), newTaskTime, recurringTask.getTaskType());
				tasklist.addTask(task);
			}
		}
		if (task.getRepetitionFrequency() = "monthly") {
			//get the number of months left in the year//
			for (int i=1; i<=numberOfMonthsLeftInYear; i++) {
				TMTaskTime newTaskTime = recurringTask.getTaskTime() + 1month;
				TMTask task(recurringTask.getTaskDescription(), newTaskTime, recurringTask.getTaskType());
				tasklist.addTask(task);
			}
		}
		if (task.getRepetitionFrequency() = "yearly") {
			TMTaskTime newTaskTime = recurringTask.getTaskTime() + 1year;
			TMTask task(recurringTask.getTaskDescription(), newTaskTime, recurringTask.getTaskType());
			tasklist.addTask(task);
		}
}
