#include <fstream>
#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include <iomanip>
#include <Windows.h>
#include <boost\date_time.hpp>

#include "TMTaskList.h"
#include "TMTask.h"
#include "TMParser.h"

	TMTaskList* TMTaskList::theOne;
	
	TMTaskList::TMTaskList() {
		_fileDirectory = "DEFAULT.txt"; //could consider using the name of the year instead
		//loadFromFile();
		}
	
	TMTaskList* TMTaskList::getInstance() {
		if (theOne == NULL) {
			theOne = new TMTaskList();
		}
		return theOne;
	}



	//LOGIC FUNCTIONS//
	bool TMTaskList::areEquivalent(TMTask task1, TMTask task2) {
		if (task1.getTaskDescription() != task2.getTaskDescription()) {
			return false;
		}

		if (task1.getTaskTime().getStartDate() != task2.getTaskTime().getStartDate()) { //TO IMPLEMENT FUNCTION TO COMPARE TASKTIME
			return false;
		}
		if (task1.getTaskTime().getStartTime() != task2.getTaskTime().getStartTime()) { //TO IMPLEMENT FUNCTION TO COMPARE TASKTIME
			return false;
		}
		if (task1.getTaskTime().getEndDate() != task2.getTaskTime().getEndDate()) { //TO IMPLEMENT FUNCTION TO COMPARE TASKTIME
			return false;
		}
		if (task1.getTaskTime().getEndTime() != task2.getTaskTime().getEndTime()) { //TO IMPLEMENT FUNCTION TO COMPARE TASKTIME
			return false;
		}
		if (task1.isCompleted() != task2.isCompleted()) {
			return false;
		}

		if (task1.isConfirmed() != task2.isConfirmed()) {
			return false;
		}

		if (task1.getTaskType() != task2.getTaskType()) {
			return false;
		}
	
		return true;
	}

	bool TMTaskList::hasClash(TMTask task) { //REVISIT CODE AND CHECK FOR BOUNDARY VALUES
		std::vector<TMTask>::iterator iter;
		TMDateTime start = task.getTaskTime().getStartDateTime();
		
		for (iter = _dated.begin(); iter != _dated.end(); ++iter) {
			if (startsBeforeTime(*iter, start)) {
				if (isTwoClash(*iter, task)) {
					return true;
				}
			} else {
				if (isTwoClash(task, *iter)) {
					return true;
				}
			}
		}	
	return false;
	}

	bool TMTaskList::isTwoClash(TMTask task1, TMTask task2) { //REVISIT CODE
		return startsBeforeTime(task2, task1.getTaskTime().getEndDateTime());
		return true;
	}

	bool TMTaskList::startsBeforeTime(TMTask task, TMDateTime time) { //REVISIT CODE
		TMDateTime dateTime = task.getTaskTime().getStartDateTime();
		return isBefore(dateTime, time);
	} 

	bool TMTaskList::endsBeforeTime(TMTask task, TMDateTime time) {
		TMDateTime dateTime = task.getTaskTime().getEndDateTime();
		return isBefore(dateTime, time);
	} //REVISIT CODE

	bool TMTaskList::isBefore(TMDateTime time1, TMDateTime time2) { //REVISIT CODE
		if (time1.getBoostDate() > time2.getBoostDate()) {
			return false;
		} else if (time1.getBoostDate() == time2.getBoostDate()) {
			if (time1.getTime() >= time2.getTime()) {
				return false;
			}
		}

		return true;
	}

	bool TMTaskList::isValidPositionIndex(int positionIndex) {
		return (positionIndex > 0 && positionIndex <= int(_dated.size() + _undated.size()));
	}
	
	bool TMTaskList::isInClashes(TMTask task) {
		std::vector<TMTask>::iterator iter;
		for (iter = _clashes.begin(); iter != _clashes.end(); ++iter) {
			if (areEquivalent(*iter, task)) {
				return true;
			}
		}
		return false;
	}

	std::vector<TMTask> TMTaskList::findClashes(TMTask task) { //REVISIT CODE AND CHECK FOR BOUNDARY VALUES
		std::vector<TMTask> clashes;
		std::vector<TMTask>::iterator iter;
		TMDateTime start = task.getTaskTime().getStartDateTime();
		
		for (iter = _dated.begin(); iter != _dated.end(); ++iter) {
			if (startsBeforeTime(*iter, start)) {
				if (isTwoClash(*iter, task)) {
					clashes.push_back(*iter);
					if (!isInClashes(*iter)) {
						_clashes.push_back(*iter);
					}
				}
			} else {
				if (isTwoClash(task, *iter)) {
					clashes.push_back(*iter);
					if (!isInClashes(*iter)) {
						_clashes.push_back(*iter);
					}
				}
			}
		}
	return clashes;
	}

	std::vector<TMTask>::iterator TMTaskList::findEarliestTaskIter(std::vector<TMTask>::iterator unsortedStart) {
									TMTask earliestTask = *unsortedStart;
									std::vector<TMTask>::iterator earliestTaskIter;
									earliestTaskIter = unsortedStart;
									std::vector<TMTask>::iterator iter;
		
									for (iter = unsortedStart; iter != _dated.end(); ++iter) {
										if (startsBeforeTime(*iter, earliestTask.getTaskTime().getStartDateTime())) {
											earliestTask = *iter;
											earliestTaskIter = iter;
										}
									}

									return earliestTaskIter;
	}

	
	std::vector<TMTask>::iterator TMTaskList::findSmallestAlphaTaskIter(std::vector<TMTask>::iterator unsortedStart) {
									TMTask smallestAlphaTask = *unsortedStart;
									std::vector<TMTask>::iterator smallestAlphaTaskIter;
									smallestAlphaTaskIter = unsortedStart;
									std::vector<TMTask>::iterator iter;
		
									for (iter = unsortedStart; iter != _undated.end(); ++iter) {
										if (iter->getTaskDescription() < smallestAlphaTaskIter->getTaskDescription()) {
											smallestAlphaTask = *iter;
											smallestAlphaTaskIter = iter;
										}
									}

									return smallestAlphaTaskIter;
	}

	std::string TMTaskList::toLower(std::string toBeConverted) {
		std::string aCopy = toBeConverted;
		transform(aCopy.begin(), aCopy.end(), aCopy.begin(), ::tolower);
		return aCopy;
	}



	//GETTER FUNCTIONS//
	int TMTaskList::getPositionIndexFromTask(TMTask task) {
		std::vector<TMTask>::iterator iter;
		int positionIndex = 1;
		
		for (iter = _dated.begin(); iter != _dated.end(); ++iter) {
			if (areEquivalent(task, *iter)) {
				return positionIndex;
			}
			positionIndex++;
		}
		
		for (iter = _undated.begin(); iter != _undated.end(); ++iter) {
			if (areEquivalent(task, *iter)) {
				return positionIndex;
			}
			positionIndex++;
		}

		return 0;
	}	

	TMTask TMTaskList::getTaskFromPositionIndex(int positionIndex) {
		assert(isValidPositionIndex(positionIndex));

		if (positionIndex <= int(_dated.size())) {
			return _dated[positionIndex - 1];
		}

		if (positionIndex <= int(_dated.size() + _undated.size())) {
			return _undated[positionIndex - int(_dated.size()) - 1];
		}
	}

	int TMTaskList::getDatedSize() {
		return _dated.size();
	}

	int TMTaskList::getUndatedSize() {
		return _undated.size();
	}

	int TMTaskList::getArchivedSize() {
		return _archived.size();
	}

	int TMTaskList::getClashesSize() {
		return _clashes.size();
	}

	std::vector<TMTask> TMTaskList::getDated() {
		return _dated;
	}
	
	std::vector<TMTask> TMTaskList::getUndated() {
		return _undated;
	}
	
	std::vector<TMTask> TMTaskList::getArchived() {
		return _archived;
	}




	//BASIC FUNCTIONS//
	void TMTaskList::addTask (TMTask task) {
		TaskType type = task.getTaskType();
		switch (type) {
			
		case WithStartDateTime:
			if (hasClash(task)) {
				std::vector<TMTask> clashesWith;
				clashesWith = findClashes(task);
				std::cout << "Clashes in timing of the following tasks:" << std::endl;
				std::vector<TMTask>::iterator iter;
				for (iter = clashesWith.begin(); iter != clashesWith.end(); ++iter) {
					std::string taskDetails;
					taskDetails = (*iter).getTaskDescription() + " " + (*iter).getTaskTime().getStartDate()
						+ " " + (*iter).getTaskTime().getStartTime()
						+ " " + (*iter).getTaskTime().getEndDate()
						+ " " + (*iter).getTaskTime().getEndTime(); 
					std::cout << taskDetails << std::endl;
				}
				std::cout << "Do you want to add this task despite clashes? (Y/N)" << std::endl;
				std::string usersReply;
				std::getline(std::cin, usersReply);

				if (usersReply == "n" || usersReply == "N") {
					return;
				} else if (usersReply == "y") {
					_clashes.push_back(task);
				}
			}
			_dated.push_back(task);
			std::cout << "WITH SDT TASK ADDED!" << std::endl;
			chronoSort();
			break;

		case WithEndDateTime:
			_dated.push_back(task);
			std::cout << "WITH EDT TASK ADDED!" << std::endl;
			chronoSort();
			break;
		
		case WithPeriod:
				if (hasClash(task)) {
				std::vector<TMTask> clashesWith;
				clashesWith = findClashes(task);
				std::cout << "Clashes in timing of the following tasks:" << std::endl;
				std::vector<TMTask>::iterator iter;
				for (iter = clashesWith.begin(); iter != clashesWith.end(); ++iter) {
					std::string taskDetails;
					taskDetails = (*iter).getTaskDescription() + " " + (*iter).getTaskTime().getStartDate()
						+ " " + (*iter).getTaskTime().getStartTime()
						+ " " + (*iter).getTaskTime().getEndDate()
						+ " " + (*iter).getTaskTime().getEndTime(); 
					std::cout << taskDetails << std::endl;
				}
				std::cout << "Do you want to add this task despite clashes? (Y/N)" << std::endl;
				std::string usersReply;
				std::getline(std::cin, usersReply);

				if (usersReply == "n" || usersReply == "N") {
					return;
				} else if (usersReply == "y") {
					_clashes.push_back(task);
				}
			}
			_dated.push_back(task);
			std::cout << "WITH PERIOD TASK ADDED!" << std::endl;
			chronoSort();
			break;

		case WithDeadline:
			if (hasClash(task)) {
				std::vector<TMTask> clashesWith;
				clashesWith = findClashes(task);
				std::cout << "Clashes in timing of the following tasks:" << std::endl;
				std::vector<TMTask>::iterator iter;
				for (iter = clashesWith.begin(); iter != clashesWith.end(); ++iter) {
					std::string taskDetails;
					taskDetails = (*iter).getTaskDescription() + " " + (*iter).getTaskTime().getStartDate()
						+ " " + (*iter).getTaskTime().getStartTime()
						+ " " + (*iter).getTaskTime().getEndDate()
						+ " " + (*iter).getTaskTime().getEndTime(); 
					std::cout << taskDetails << std::endl;
				}
				std::cout << "Do you want to add this task despite clashes? (Y/N)" << std::endl;
				std::string usersReply;
				std::getline(std::cin, usersReply);

				if (usersReply == "n" || usersReply == "N") {
					return;
				}
			}
			_dated.push_back(task);
			std::cout << "WITHDEADLINE TASK ADDED!" << std::endl;
			chronoSort();
			break;

		case Undated: 
			_undated.push_back(task);
			std::cout << "UNDATED TASK ADDED!" << std::endl;
			alphaSort();
			break;

		case Invalid:
			std::cout << "INVALID TASK DETECTED. NO ADD WAS CARRIED OUT" << std::endl;
		}
	}

	void TMTaskList::blockMultiple(std::vector<TMTask> tasks, TMTaskList tasklist) {
		std::vector<TMTask>::iterator iter;
		for (iter = tasks.begin(); iter != tasks.end(); ++iter) {
			tasklist.addTask(*iter);
		}
		tasks.clear();
	}

	//NEED TO USE ASSERT TO DETERMINE VALID POSITION INDEX
	void TMTaskList::updateTask(int positionIndex, std::string componentOfTask, std::string changeTo) {
		assert(isValidPositionIndex(positionIndex));
		if (positionIndex <= int(_dated.size())) {
			TMTask task = _dated[positionIndex-1];

			if (componentOfTask == "desc") {
				task.setTaskDescription(changeTo);
			}
			if (componentOfTask == "time") {
				//YET TO CODE
			}
			if (componentOfTask == "completion") {
				if (changeTo == "1") {
					task.setAsCompleted();
				}
				if (changeTo == "0") {
					task.setAsIncompleted();
				}
			}
			if (componentOfTask == "confirmation") {
				if (changeTo == "1") {
					task.setAsConfirmed();
				}
				if (changeTo == "0") {
					task.setAsUnconfirmed();
				}
			}
			
		} else if (positionIndex <= int(_dated.size() + _undated.size())) {
			TMTask &task = _undated[positionIndex-_dated.size()-1];
			if (componentOfTask == "desc") {
				task.setTaskDescription(changeTo);
			}
			if (componentOfTask == "completion") {
				if (changeTo == "1") {
					task.setAsCompleted();
				}
				if (changeTo == "0") {
					task.setAsIncompleted();
				}
			}
		}
	}

	//NEED TO USE ASSERT TO DETERMINE VALID POSITION INDEX
	void TMTaskList::removeTask(int positionIndex) {	
		assert(isValidPositionIndex(positionIndex));
		if (positionIndex <= int(_dated.size())) {
			_dated.erase(_dated.begin() + positionIndex - 1);
		} else if (positionIndex <= int(_dated.size() + _undated.size())) {
			int floatingTaskNumber = positionIndex - _dated.size();
			_undated.erase(_undated.begin() + floatingTaskNumber - 1);
		}
	}

	//NEED TO USE ASSERT TO DETERMINE VALID POSITION INDEX
	void TMTaskList::archiveOneTask(int positionIndex) {
		assert(isValidPositionIndex(positionIndex));
		TMTask task = getTaskFromPositionIndex(positionIndex);
		task.setAsCompleted();
		_archived.push_back(task);
		removeTask(positionIndex);
	}

	void TMTaskList::chronoSort() {
		std::vector<TMTask>::iterator iter;
		for (iter = _dated.begin(); iter != _dated.end(); ++iter) {
			std::iter_swap(iter, findEarliestTaskIter(iter));
		}
		std::cout << "Chronological sort completed." <<std:: endl;
		
	}


	void TMTaskList::alphaSort() {
		std::vector<TMTask>::iterator iter;
		for (iter = _undated.begin(); iter != _undated.end(); ++iter) {
			std::iter_swap(iter, findSmallestAlphaTaskIter(iter));
		}
		std::cout << "Alphabetical sort completed." <<std:: endl;
	}	

	std::vector<int> TMTaskList::keywordSearch(std::string keyword) {
		std::string lowerKeyword = toLower(keyword);
		
		std::vector<std::string> taskDescInLower;
		std::vector<TMTask>::iterator iter;
		std::vector<int> searchResults;
		int posIndexCounter = 1;

		for (iter = _dated.begin(); iter != _dated.end(); ++iter) {
			taskDescInLower.push_back(toLower((*iter).getTaskDescription()));
		}
		for (iter = _undated.begin(); iter != _undated.end(); ++iter) {
			taskDescInLower.push_back(toLower((*iter).getTaskDescription()));
		}

		std::vector<std::string>::iterator iterForLower;
		for (iterForLower = taskDescInLower.begin(); iterForLower != taskDescInLower.end(); ++iterForLower) {
			std::string searchLine = *iterForLower;
			if ( std::search(searchLine.begin(), searchLine.end(), lowerKeyword.begin(), lowerKeyword.end()) != searchLine.end() ) {
				searchResults.push_back(posIndexCounter);
			}
			posIndexCounter++;
		}
	
		return searchResults;
	}

	


	//EXPORT AND IMPORT FUNCTIONS//
	void TMTaskList::writeToFile(){
		std::ofstream outFile;
		int i;
		std::vector<TMTask>::iterator iter;

		outFile.open(_fileDirectory);
		outFile << "Number of timed and deadline: "  << _dated.size() << "\n";
		for (i = 0; i < int(_dated.size()); ++i) {
			outFile << _dated[i].getTaskDescription() << 
				" " << _dated[i].getTaskTime().getStartDate() << 
				" " << _dated[i].getTaskTime().getStartTime() << 
				" " << _dated[i].getTaskTime().getEndDate() <<
				" " << _dated[i].getTaskTime().getEndTime() << "\n";
		}

		//outFile << '\n';
	
		outFile << "Number of undated tasks: " << _undated.size() << "\n";
		for (i = 0; i < int(_undated.size()); ++i) {
			outFile << _undated[i].getTaskDescription() << "\n";
		}

		//outFile << "\n";

		outFile << "Number of completed tasks: " << _archived.size() << "\n";
		for (iter = _archived.begin(); iter != _archived.end(); iter++) {
			
			if (iter->getTaskType() == TaskType::WithPeriod || iter->getTaskType() == TaskType::WithDeadline) {
				outFile << iter->getTaskDescription() <<
				" " << iter->getTaskTime().getStartDate() << 
				" " << iter->getTaskTime().getStartTime() << 
				" " << iter->getTaskTime().getEndDate() <<
				" " << iter->getTaskTime().getEndTime() << '\n';
			}

			else {
				outFile << iter->getTaskDescription() << '\n';
			}
		}

		outFile.close();
	}

	void TMTaskList::loadFromFile() {

		std::ifstream directoryReference("REFERENCE.txt");
		getline(directoryReference, _fileDirectory);

		std::ifstream contentReference(_fileDirectory);
		std::vector<std::string> linesFromFile;
		std::string line;
		std::string M1 = "Number of timed and deadline:";
		std::string M2 = "Number of undated tasks:";
		std::string M3 = "Number of completed tasks:";

		while (getline(contentReference, line)) {
			if (line != "\n") {
				linesFromFile.push_back(line);
			}
		}

		directoryReference.close();
		contentReference.close();

		std::vector<std::string>::iterator iter;

		//std::search(searchLine.begin(), searchLine.end(), keyword.begin(), keyword.end()) != searchLine.end()

		std::vector<TMTask> tasks;
		TMParser *parser = TMParser::getInstance(); 

		//Assumes first line in saved text file is as unmodified by user.
		//Loop stops when M2 is detected
		//Timed and deadline tasks
		for (iter = linesFromFile.erase(linesFromFile.begin()); std::search(iter->begin(), iter->end(), M2.begin(), M2.end()) == iter->end();iter = linesFromFile.erase(iter)) {
			parser->initialize(*iter);
			tasks = parser->parseTaskInfo();
			TMTask task = tasks[0];
			_dated.push_back(task);
		}

		//Floating tasks
		for (iter =linesFromFile.erase(linesFromFile.begin()); std::search(iter->begin(), iter->end(), M3.begin(), M3.end()) == iter->end(); iter = linesFromFile.erase(iter)) {
			parser->initialize(*iter);
			tasks = parser->parseTaskInfo();
			TMTask task = tasks[0];
			_undated.push_back(task);
		}

		//Completed tasks
		for (iter = linesFromFile.erase(linesFromFile.begin()); iter != linesFromFile.end(); iter = linesFromFile.erase(iter)) {
			parser->initialize(*iter);
			tasks = parser->parseTaskInfo();
			TMTask task = tasks[0];
			task.setAsCompleted();
			_archived.push_back(task);
		}

		std::cout << "Successfully loaded from file" << std::endl;
	} 

	void TMTaskList::setFileDirectory(std::string directory) {
		_fileDirectory = directory;
	}

	void TMTaskList::leaveReferenceUponExit() {
		std::ofstream outFile;
		outFile.open("REFERENCE.txt");
		outFile << _fileDirectory << '\n';
		outFile << "THIS FILE IS FOR REFERENCE TO LOAD FROM FILE. DO NOT DELETE" << '\n';
		outFile.close();
		}


