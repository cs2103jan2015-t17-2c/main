#ifndef _TMMEMORY_H_
#define _TMMEMORY_H_

#include <fstream>
#include <vector>
#include <string>

class TaskList{
private:
	//Contains all the task for the current year
	vector<Task> timedAndDeadline; 
	//Contains all floating tasks from the beginning of use unless task is completed or deleted
	vector<Task> floating; 
	vector<Task> repeatingTasks;
	int sizeOfTimedAndDeadline;
	int sizeOfFloating;
	int nextTaskID = 1;

public:
	//Constructor creates a file storing all tasks of the year at a specified folder.
	void setTaskID(Task task);
	void addTask(Task task);
	void updateTask(int taskID, string componentOfTask, string changeTo);
	void removeTask(int taskID);
	void archiveAllTasks(vec<int> archiveExceptionsID);
	void archiveOneTask();
	//Done once a year on the first day of every year 
	void addRepeatingTasksToList();
	string searchTasks(string keyword, bool isCaseSensitive);
	void saveToFile (vec<Task> timedAndDeadline, vec<Task> floating, vec<Task> repeatingTasks);
	void loadFromFile();
}



#endif _TMMEMORY_H_