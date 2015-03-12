#ifndef _TMTaskList_H_
#define _TMTaskList_H_

#include <fstream>
#include <vector>
#include <string>
#include "TMTask.h"

using namespace std;

class TMTaskList{
private:
	//Contains all the task for the current year
	vector<TMTask> timedAndDeadline; 
	//Contains all floating tasks from the beginning of use unless task is completed or deleted
	vector<TMTask> floating; 
	vector<TMTask> repeatingTasks;
	int sizeOfTimedAndDeadline;
	int sizeOfFloating;

public:
	//Constructor creates a file storing all tasks of the year at a specified folder.
	void setTaskID(TMTask task);
	void addTask(TMTask task);
	void updateTask(string componentOfTask, string changeTo);
	void removeTask();
	void archiveAllTasks();
	void archiveOneTask();
	//Done once a year on the first day of every year 
	void addRepeatingTasksToList();
	string searchTasks(string keyword);
	void loadFromFile();
}

#endif _TMTaskList_H_