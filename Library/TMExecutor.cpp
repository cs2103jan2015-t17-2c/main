#include <vector>
#include <iostream>
#include <string>
#include "TMExecutor.h"
#include <boost\date_time.hpp>

void TMExecutor::sortCommandToFunctions(std::string command, std::vector<TMTask> tasks, TMTaskList &tasklist) {
	if (command == "add" || command == "blockmultiple") {
		addTasks(tasks, tasklist);
	}

	if (command == "delete") {
		TMTask task = tasks.front();
		deleteOneTask(task, tasklist);
	}

	if (command == "confirmed") {
		freeUnconfirmed(tasks, tasklist);
	}

}

void TMExecutor::addTasks(std::vector<TMTask> tasks, TMTaskList &tasklist) {
	std::vector<TMTask>::iterator iter;
	for (iter = tasks.begin(); iter != tasks.end(); ++iter) {
		tasklist.addTask(*iter);
	}
	
	tasks.clear();
	std::cout << "Current schedule: " << std::endl;
	tasklist.displayAllTasks();
	 
}

void TMExecutor::updateOneTask(TMTask task, std::string component, std::string changeTo, TMTaskList &tasklist) {
	tasklist.updateTask(tasklist.getPositionIndexFromTask(task), component, changeTo);
}

void TMExecutor::deleteOneTask(TMTask task, TMTaskList &tasklist) {
	tasklist.removeTask(tasklist.getPositionIndexFromTask(task));
}

void TMExecutor::markTodaysTasksAsDone(TMTaskList tasklist) {
	/*tasklist.archiveTodaysTasks();*/
}

void TMExecutor::searchKeyword(std::string keyword, TMTaskList &tasklist) {
	/*std::vector<int> searchResults;
	searchResults = tasklist.keywordSearch(keyword);

	std::vector<int>::iterator resultsIter;
	for (resultsIter = searchResults.begin(); resultsIter != searchResults.end(); ++resultsIter) {
		TMTask result = tasklist.getTaskFromPositionIndex(*resultsIter);
		std::cout << *resultsIter << ". " << result.getTaskDescription() << std::endl;
	}*/
}
	
void TMExecutor::searchFreeTime(TMTaskList tasklist) {
	/*std::cout << tasklist.freeTimeSearch() << std::endl;*/
}
	
void TMExecutor::freeUnconfirmed(std::vector<TMTask> confirmedTasks, TMTaskList &tasklist) {
	/*std::vector<TMTask>::iterator tasksIter;
	for (tasksIter = confirmedTasks.begin(); tasksIter != confirmedTasks.end(); ++tasksIter) {
			updateOneTask(*tasksIter, "isConfirmed", "true", tasklist);
		}
	std::vector<int> searchResults;
	searchResults = tasklist.keywordSearch(confirmedTasks.front().getTaskDescription());

	std::vector<int>::iterator resultsIter;
	for (resultsIter = searchResults.begin(); resultsIter != searchResults.end(); ++resultsIter) {
		TMTask task = tasklist.getTaskFromPositionIndex(*resultsIter);
		if ( !(task.isConfirmed()) ) {
			tasklist.removeTask(*resultsIter);
		}
	}*/
}

void TMExecutor::saveAt(std::string directory) {} //YET TO CODE

void TMExecutor::undoLast() {} //YET TO CODE