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

	//Contains all the completed timed, deadline and floating tasks
	std::vector<TMTask> archived;


public:
	
	//Constructor creates a file storing all tasks of the year at a specified folder.
	
	//Getter functions//

	//Compares 2 tasks, returns true if both tasks have similar attributes.
	bool areEquivalent(TMTask task1, TMTask task2); 

	//Checks if the task to be added has clash with current tasks in tasklist.
	bool hasClash(TMTask task);

	//Precondition: task1 starts before task2
	bool isTwoClash(TMTask task1, TMTask task2);

	//Postcondition: Returns all tasks that clashes with the task to be added in the form of a vector
	std::vector<TMTask> findClashes(TMTask task);

	//Postcondition: Returns 0 if the position index is not found
	int getPositionIndexFromTask(TMTask task);
	TMTask getTaskFromPositionIndex(int positionIndex);

	//Basic functions//
	void addTask(TMTask task);
	void updateTask(int positionIndex, std::string componentOfTask, std::string changeTo);
	void removeTask(int positionIndex);
	void displayAllTasks();
	
	//Sorts tasks in tasklist in chronological order according to their start dates
	//Precondition: timedAndDeadline vector not empty
	void chronoSort();

	//Find the earliest task in the unsorted tasklist
	std::vector<TMTask>::iterator findEarliestTaskIter(std::vector<TMTask>::iterator unsortedStart);
	
	//Postcondition: Returns true if task1 starts first compared to task2
	bool startsEarlierThan(TMTask task1, TMTask task2);
	
	//Sort task in alphabetical order using their description
	void alphaSort();

	//Temporarily reserving timeslots for unconfirmed activities
	void blockMultiple(std::vector<TMTask> tasks, TMTaskList tasklist);
	
	//Archives//

	//Archive all tasks that are completed by the user today
	void archiveTodaysTasks();

	//Archive a selected task
	void archiveOneTask(int positionIndex);
	
	//Search Functions//
	std::vector<int> keywordSearch(std::string keyword);

	//Postcondition: Returns a string which consists of only lowercase letters
	std::string toLower(std::string toBeConverted);

	//Postcondition: Returns a string consisting all the timings in which the user is free
	std::string freeTimeSearch();

	//Save functions//
	void writeToFile();
	void loadFromFile();
};

#endif
