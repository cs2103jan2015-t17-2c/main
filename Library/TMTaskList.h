#ifndef TMTASKLIST_H
#define TMTASKLIST_H

#include <fstream>
#include <vector>
#include <algorithm>
#include <string>
#include <iomanip>
#include <Windows.h>
#include <boost\date_time.hpp>

#include "TMDateTime.h"
#include "TMTask.h"
#include "FormatConverter.h"


class TMTaskList{
private:
	
	//Contains all the incompleted dated tasks for the current year
	std::vector<TMTask> _dated; 
	
	//Contains all the incompleted undated tasks for the current year
	std::vector<TMTask> _undated; 

	//Contains all the completed dated and undated tasks for the current year
	std::vector<TMTask> _archived;

	//A temporary container which stores tasks that clashes with a task of interest
	std::vector<TMTask> _clashes;
	
	//File name
	std::string _fileName;

	//Directory name
	std::string _directoryName;

public:
	TMTaskList();

	//LOGIC FUNCTIONS//
	//Compares 2 tasks, returns true if both tasks have similar attributes.
	bool areEquivalent(TMTask task1, TMTask task2); 

	//Checks if the task to be added has clash with current tasks in tasklist.
	bool hasClash(TMTask task);

	//Precondition: task1 starts before task2
	bool isTwoClash(TMTask task1, TMTask task2);

	//Postcondition: Returns true if task starts before time
	bool startsBeforeTime(TMTask task, TMDateTime time);

	//Postcondition: Returns true if task ends before time
	bool endsBeforeTime(TMTask task, TMDateTime time);

	//Postcondition: Returns true if time1 is before time2, returning false if time1 is after or equals to time2
	bool isBefore(TMDateTime time1, TMDateTime time2);

	bool areEquivalentDateTime(TMDateTime time1, TMDateTime time2);
 
	bool isValidPositionIndex(int positionIndex);

	bool isInDated(int positionIndex);

	bool isInUndated(int positionIndex);

	bool isInArchived(int positionIndex);

	bool isUniqueBatchNum(int i);

	
	//Assupmtion: task to be added to taskList is yet to be added
	//Set the clash status of tasks which clashes with the task to be added 
	//Adds tasks which clashes with task into _clashes vector, with the targeted search region specified by iterator beginFrom,
	//which is an iterator of the vector _dated
	void setClashes(TMTask task, std::vector<TMTask>::iterator beginFrom);

	//Updates the clash status of current tasks that clashes with the task to be removed or archived.
	//Remaining tasks which clashes with each other after removal of involved task will have their clash status set as clashed.
	void updateClashes(TMTask deleteTask);

	//Returns the iterator of the earliest task in the unsorted tasklist
	std::vector<TMTask>::iterator findEarliestTaskIter(std::vector<TMTask>::iterator unsortedStart);

	//Returns the iterator of the task with description containing the earliest alphabet in the unsorted tasklist
	std::vector<TMTask>::iterator findSmallestAlphaTaskIter(std::vector<TMTask>::iterator unsortedStart);
	
	//Postcondition: Returns a string which consists of only lowercase letters
	std::string toLower(std::string toBeConverted);

	//Returns the positionIndexes of tasks which unconfirmed batch number is i in a vector
	std::vector<int> searchUnconfirmedBatchNum(int i);

	
	

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
	
	//Returns a unique unconfirmed batch number such that tasks that have been blocked at the same instance share the same unconfirmed batch number
	int generateUniqueBatchNum();


	//BASIC FUNCTIONS//
	std::string addTask(TMTask &task);
	
	//Removes the outdated task from taskList and adds alteredTask
	std::string updateTask(int positionIndex, TMTask &alteredTask);
	
	std::string removeTask(int positionIndex);
	
	//Carried out when the task is marked as completed by the user
	std::string archiveOneTask(int positionIndex);
	
	//Sorts tasks in tasklist in chronological order according to their start dates
	//Precondition: _dated vector not empty
	void chronoSort();
	
	//Sorts tasks in alphabetical order of their description
	void alphaSort();
	
	//Return the position indexes of tasks which match the keyword
	//Note that the search is not case sensitive.
	std::vector<int> keywordSearch(std::string keyword);

	//EXPORT AND IMPORT FUNCTIONS//
	void writeToFile();
	std::string loadFromFile(std::string pathName);
	void setFileDirectory(std::string directory);
	std::string getFileDirectory();
	void leaveReferenceUponExit();
	bool isFoundInLine(std::string text, std::string line);
	void determineLoadOrCreateFile();
};

#endif
