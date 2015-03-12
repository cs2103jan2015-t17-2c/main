#ifndef TMTASKLIST_H
#define TMTASKLIST_H

#include <fstream>
#include <vector>
#include <string>
#include "TMTask.h"


class TMTaskList{
private:
	
	//Contains all the incompleted timed and deadline tasks for the current year
	std::vector<TMTask> timedAndDeadline; 
	
	//Contains all the incompleted floating tasks
	std::vector<TMTask> floating; 
	
	int sizeOfTimedAndDeadline;
	int sizeOfFloating;

public:
	
	//Constructor creates a file storing all tasks of the year at a specified folder.
	
	//Getter functions
	bool areEquivalent(TMTask task1, TMTask task2); //Compares 2 tasks, returns true if both tasks have similar attributes.
	int getPositionIndexFromTask(TMTask task);
	TMTask getTaskFromPositionIndex(int positionIndex);

	//Basic functions
	void addTask(TMTask task);
	void updateTask(int positionIndex, std::string componentOfTask, std::string changeTo);
	void removeTask(int positionIndex);
	void displayAllTasks();
	void sortTasksAccDate();
	
	//Temporarily reserving timeslots for unconfirmed activities
	void blockMultiple(std::vector<TMTask> tasks, TMTaskList tasklist);
	void freeMultiple(std::vector<TMTask> confirmedTasks, TMTaskList tasklist);
	
	//Archives
	void archiveTodaysTasks();
	void archiveOneTask();
	
	//Search Functions
	std::vector<int> keywordSearch(std::string keyword);
	std::string freeTimeSearch();

	//Save functions
	void loadFromFile();
};

#endif
