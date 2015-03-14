#ifndef TMEXECUTOR_H
#define TMEXECUTOR_H

#include <vector>
#include <iostream>
#include <string>

#include "TMTask.h"
#include "TMTaskList.h"


class TMExecutor{
	public:
	
	void sortCommandToFunctions(std::string command, std::vector<TMTask> tasks, TMTaskList &tasklist);
	
	void addTasks(std::vector<TMTask> tasks, TMTaskList &tasklist);
	
	void updateOneTask(TMTask task, std::string component, std::string changeTo, TMTaskList &tasklist);
	
	void deleteOneTask(TMTask task, TMTaskList &tasklist);

    //incomplete
	//void markTodaysTasksAsDone(TMTaskList tasklist);

	void searchKeyword(std::string keyword, TMTaskList &tasklist);

	void searchFreeTime(TMTaskList &tasklist);

	void freeUnconfirmed(std::vector<TMTask> confirmedTasks, TMTaskList &tasklist);

	void saveAt(std::string directory);
	
	void undoLast();
};

#endif