#ifndef TMTASKLIST_H
#define TMTASKLIST_H

#include <fstream>
#include <vector>
#include <string>
#include "TMDateTime.h"
#include "TMTask.h"
#include <assert.h>
#include <Windows.h>


class TMTaskList{
private:
	
	//Contains all the incompleted timed and deadline tasks for the current year
	std::vector<TMTask> _dated; 
	
	//Contains all the incompleted floating tasks
	std::vector<TMTask> _undated; 

	//Contains all the completed timed, deadline and floating tasks
	std::vector<TMTask> _archived;

	std::vector<TMTask> _clashes;

	//File directory name
	std::string _fileDirectory;

public:
	TMTaskList();

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

	bool isValidPositionIndex(int positionIndex);

	//Postcondition: Returns all tasks that clashes with the task to be added in the form of a vector
	//Assupmtion: task is not present in taskList
	void setClashes(TMTask task, std::vector<TMTask>::iterator beginFrom);

	//Find the earliest task in the unsorted tasklist
	std::vector<TMTask>::iterator findEarliestTaskIter(std::vector<TMTask>::iterator unsortedStart);

	std::vector<TMTask>::iterator findSmallestAlphaTaskIter(std::vector<TMTask>::iterator unsortedStart);
	
	//Postcondition: Returns a string which consists of only lowercase letters
	std::string toLower(std::string toBeConverted);

	int getUniqueBatchNum();

	bool isUniqueBatchNum(int i);

	std::vector<int> searchUnconfirmedBatchNum(int i);

	void updateClashes(TMTask deleteTask);
	

	//GETTER FUNCTIONS//
	//Postcondition: Returns 0 if the position index is not found
	int getPositionIndexFromTask(TMTask task);
	TMTask getTaskFromPositionIndex(int positionIndex);
	int getDatedSize();
	int getUndatedSize();
	int getArchivedSize();
	int getClashesSize();
	std::vector<TMTask> getDated();
	std::vector<TMTask> getUndated();
	std::vector<TMTask> getArchived();


	//BASIC FUNCTIONS//
	std::string addTask(TMTask task);

	std::string addClashedTask(TMTask task);
	
	std::string updateTask(int positionIndex, EditableTaskComponent component, std::string changeTo);
	
	std::string removeTask(int positionIndex);
	
	//Archive a selected task
	std::string archiveOneTask(int positionIndex);
	
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
	std::string getFileDirectory();
	void leaveReferenceUponExit();

};

#endif
