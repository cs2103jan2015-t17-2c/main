#ifndef TMTASKLIST_H
#define TMTASKLIST_H

#include <fstream>
#include <vector>
#include <string>
#include "TMDateTime.h"
#include "TMTask.h"


class TMTaskList{
private:
	
	//Contains all the incompleted timed and deadline tasks for the current year
	std::vector<TMTask> timedAndDeadline; 
	
	//Contains all the incompleted floating tasks
	std::vector<TMTask> floating; 

	//Contains all the completed timed, deadline and floating tasks
	std::vector<TMTask> archived;

	static TMTaskList *theOne;
	
	//Private constructor
	TMTaskList();

	//File directory name
	std::string _fileDirectory;

public:
	
	static TMTaskList* getInstance();


	//LOGIC FUNCTIONS//
	//Compares 2 tasks, returns true if both tasks have similar attributes.
	bool areEquivalent(TMTask task1, TMTask task2); 

	//Checks if the task to be added has clash with current tasks in tasklist.
	bool hasClash(TMTask task);

	//Precondition: task1 starts before task2
	bool isTwoClash(TMTask task1, TMTask task2);

	//Postcondition: Returns true if task starts before tmTime
	bool startsBeforeTime(TMTask task, TMDateTime time);

	//Postcondition: Returns true if task ends before tmTime
	bool endsBeforeTime(TMTask task, TMDateTime time);

	//Postcondition: Returns true if time1 is before time2, returning false if time1 is after or equals to time2
	bool TMTaskList::isBefore(TMDateTime time1, TMDateTime time2);

	//Postcondition: Returns all tasks that clashes with the task to be added in the form of a vector
	std::vector<TMTask> findClashes(TMTask task);

	//Find the earliest task in the unsorted tasklist
	std::vector<TMTask>::iterator findEarliestTaskIter(std::vector<TMTask>::iterator unsortedStart);

	//Postcondition: Returns a string which consists of only lowercase letters
	std::string toLower(std::string toBeConverted);



	//GETTER FUNCTIONS//
	//Postcondition: Returns 0 if the position index is not found
	int getPositionIndexFromTask(TMTask task);
	TMTask getTaskFromPositionIndex(int positionIndex);
	int getTimedAndDeadlineSize();
	int getFloatingSize();
	int getArchivedSize();
	std::vector<TMTask> getTimedAndDeadline();
	std::vector<TMTask> getFloating();
	std::vector<TMTask> getArchived();


	//BASIC FUNCTIONS//
	void addTask(TMTask task);
	
	//Temporarily reserving timeslots for unconfirmed activities
	void blockMultiple(std::vector<TMTask> tasks, TMTaskList tasklist); //CONSIDER REMOVING IT
	
	void updateTask(int positionIndex, std::string componentOfTask, std::string changeTo);
	
	void removeTask(int positionIndex);
	
	//Archive a selected task
	void archiveOneTask(int positionIndex);
	
	//Sorts tasks in tasklist in chronological order according to their start dates
	//Precondition: timedAndDeadline vector not empty
	void chronoSort();
	
	//Sort task in alphabetical order using their description
	void alphaSort();
	
	//Return the position indexes of tasks which match the keyword. Note that the search is not case sensitive.
	std::vector<int> keywordSearch(std::string keyword);

	//EXPORT AND IMPORT FUNCTIONS//
	void writeToFile();
	void loadFromFile();
	void setFileDirectory(std::string directory);
	void leaveReferenceUponExit();

};

#endif
