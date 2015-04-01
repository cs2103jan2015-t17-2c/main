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

const std::string ADD_SDT_SUCCESS = "Successfully added a task with start date and time!";
const std::string ADD_EDT_SUCCESS = "Successfully added a task with end date and time!";
const std::string ADD_PERIOD_SUCCESS = "Successfully added a period task!";
const std::string ADD_UNDATED_SUCCESS = "Successfully added an undated task!";
const std::string ADD_INVALID = "Task you have specified has invalid component(s). Please specify a valid task.";
const std::string CLASH_WARNING = "Task added has clashes with tasks on hand.\nInvolved tasks have been highlighted in blue.";
const std::string UPDATE_DESCRIPTION_SUCCESS = "Task description successfully changed.";
const std::string UPDATE_DATE_SUCCESS = "Task date successfully changed.";
const std::string UPDATE_TIME_SUCCESS = "Task time successfully changed.";
const std::string UPDATE_FAILURE = "The component of that task you specified is invalid, please enter a valid component.";
const std::string DELETE_SUCCESS = "Task successfully removed from database!";
const std::string ARCHIVED_SUCCESS = "Task is successfully completed and archived.";

	TMTaskList::TMTaskList() {
		_fileDirectory = "DEFAULT.txt"; //could consider using the name of the year instead
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

	std::vector<TMTask> TMTaskList::findAndSetClashes(TMTask task, std::vector<TMTask>::iterator beginFrom) { //REVISIT CODE AND CHECK FOR BOUNDARY VALUES
		std::vector<TMTask> clashes;
		std::vector<TMTask>::iterator iter;
		TMDateTime start = task.getTaskTime().getStartDateTime();
		
		for (iter = beginFrom; iter != _dated.end(); ++iter) {
			TMTask &registeredTask = *iter;
			if (startsBeforeTime(registeredTask, start)) {
				if (isTwoClash(registeredTask, task)) {
					registeredTask.setAsClashed();
					clashes.push_back(registeredTask);
				}
			} else if (isTwoClash(registeredTask, *iter)) {
					registeredTask.setAsClashed();
					clashes.push_back(registeredTask);
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

	int TMTaskList::getUniqueBatchNum() {
		std::vector<TMTask>::iterator iter;
		int i;
		for (i = 1; !isUniqueBatchNum(i); ++i) {
			}

		return i;
	}

	bool TMTaskList::isUniqueBatchNum(int i) {
		std::vector<TMTask>::iterator iter;
		for (iter = _dated.begin(); iter != _dated.end(); ++iter) {
			if (iter->getUnconfirmedBatchNumber() == i) {
				return false;
			}
		}
		return true;
	}

	std::vector<int> TMTaskList::searchUnconfirmedBatchNum(int i) {
		std::vector<TMTask>::iterator iter;
		std::vector<int> results;

		for (iter = _dated.begin(); iter != _dated.end(); ++iter) {
			if (iter->getUnconfirmedBatchNumber() == i && !iter->isConfirmed()) {
				results.push_back(getPositionIndexFromTask(*iter));
			}
		}
		std::cout << "SIZE OF SEARCH: " << results.size() << std::endl;
		return results;
	}

	void  TMTaskList::updateClashes(TMTask deleteTask) {
		std::vector<TMTask> &clashedWithToBeDeleted = findAndSetClashes(deleteTask, _dated.begin());
		std::vector<TMTask>::iterator iter;
		for (iter = clashedWithToBeDeleted.begin(); iter != clashedWithToBeDeleted.end(); ++iter) {
			iter->setAsUnclashed();
		}

		/*for (iter = _dated.begin(); iter != _dated.end(); ++iter) {
			TMTask &focusTask = *iter;
			findAndSetClashes(focusTask, iter+1);	
		}*/

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
	std::string TMTaskList::addTask (TMTask task) {
		TaskType type = task.getTaskType();
		std::string outcome;
		switch (type) {
			
		case WithStartDateTime:
			_dated.push_back(task);
			chronoSort();
			return ADD_SDT_SUCCESS;
			break;

		case WithEndDateTime:
			_dated.push_back(task);
			chronoSort();
			return ADD_EDT_SUCCESS;
			break;
		
		case WithPeriod:
			if (int(_dated.size()) != 0 && hasClash(task)) {
				findAndSetClashes(task, _dated.begin());
				task.setAsClashed();
				outcome = CLASH_WARNING;
			} else {
				outcome = ADD_PERIOD_SUCCESS;
			}

			_dated.push_back(task);
			chronoSort();
			return outcome;
			break;

		case Undated: 
			_undated.push_back(task);
			alphaSort();
			return ADD_UNDATED_SUCCESS;
			break;

		case Invalid:
			return ADD_INVALID;
			break;
		}
	}

	std::string TMTaskList::addClashedTask(TMTask task) {
		_dated.push_back(task);
		chronoSort();
		return ADD_PERIOD_SUCCESS;
	}

	//NEED TO USE ASSERT TO DETERMINE VALID POSITION INDEX
	std::string TMTaskList::updateTask(int positionIndex, EditableTaskComponent component, std::string changeTo) {
		assert(isValidPositionIndex(positionIndex));
		if (positionIndex <= int(_dated.size())) {
			TMTask &task = _dated[positionIndex-1];
			TaskType type = task.getTaskType();
			switch (type) {
			
			case WithStartDateTime:
				switch (component) {
				case EditableTaskComponent::Description:
					task.setTaskDescription(changeTo);
					return UPDATE_DESCRIPTION_SUCCESS;
					break;

				case EditableTaskComponent::StartDate:
					task.getTaskTime().setStartDate(changeTo);
					return UPDATE_DATE_SUCCESS;
					break;

				case EditableTaskComponent::StartTime:
					task.getTaskTime().setStartTime(changeTo);
					return UPDATE_TIME_SUCCESS;
					break;

				case EditableTaskComponent::EndDate:
					return UPDATE_FAILURE;
					break;

				case EditableTaskComponent::EndTime:
					return UPDATE_FAILURE;
					break;

				case EditableTaskComponent::InvalidComponent:
					return UPDATE_FAILURE;
				}
				break;
				
			case WithEndDateTime:
				switch (component) {
				case EditableTaskComponent::Description:
					task.setTaskDescription(changeTo);
					return UPDATE_DESCRIPTION_SUCCESS;
					break;

				case EditableTaskComponent::StartDate:
					return UPDATE_FAILURE;
					break;

				case EditableTaskComponent::StartTime:
					return UPDATE_FAILURE;
					break;

				case EditableTaskComponent::EndDate:
					task.getTaskTime().setEndDate(changeTo);
					return UPDATE_DATE_SUCCESS;
					break;

				case EditableTaskComponent::EndTime:
					task.getTaskTime().setEndTime(changeTo);
					return UPDATE_TIME_SUCCESS;
					break;

				case EditableTaskComponent::InvalidComponent:
					return UPDATE_FAILURE;
				}
				break;

			case WithPeriod:
				switch (component) {
				case EditableTaskComponent::Description:
					task.setTaskDescription(changeTo);
					return UPDATE_DESCRIPTION_SUCCESS;
					break;

				case EditableTaskComponent::StartDate:
					task.getTaskTime().setStartDate(changeTo);
					return UPDATE_DATE_SUCCESS;
					break;

				case EditableTaskComponent::StartTime:
					task.getTaskTime().setStartTime(changeTo);
					return UPDATE_TIME_SUCCESS;
					break;

				case EditableTaskComponent::EndDate:
					task.getTaskTime().setEndDate(changeTo);
					return UPDATE_DATE_SUCCESS;
					break;

				case EditableTaskComponent::EndTime:
					task.getTaskTime().setEndTime(changeTo);
					return UPDATE_TIME_SUCCESS;
					break;
			
				case EditableTaskComponent::InvalidComponent:
					return UPDATE_FAILURE;
				}
				break;
			}
			

		} else if (positionIndex <= int(_dated.size() + _undated.size())) {
			TMTask &task = _undated[positionIndex-_dated.size()-1];
			switch (component) {
				case EditableTaskComponent::Description:
					task.setTaskDescription(changeTo);
					return UPDATE_DESCRIPTION_SUCCESS;
					break;

				case EditableTaskComponent::StartDate:
					return UPDATE_FAILURE;
					break;

				case EditableTaskComponent::StartTime:
					return UPDATE_FAILURE;
					break;

				case EditableTaskComponent::EndDate:
					return UPDATE_FAILURE;
					break;

				case EditableTaskComponent::EndTime:
					return UPDATE_FAILURE;
					break;
			
				case EditableTaskComponent::InvalidComponent:
					return UPDATE_FAILURE;
			}
		}
	}

	//NEED TO USE ASSERT TO DETERMINE VALID POSITION INDEX
	std::string TMTaskList::removeTask(int positionIndex) {	
		assert(isValidPositionIndex(positionIndex));
		if (positionIndex <= int(_dated.size())) {
			TMTask deleteTask = getTaskFromPositionIndex(positionIndex);
			_dated.erase(_dated.begin() + positionIndex - 1);
			updateClashes(deleteTask);
			return DELETE_SUCCESS;
		} else if (positionIndex <= int(_dated.size() + _undated.size())) {
			int floatingTaskNumber = positionIndex - _dated.size();
			_undated.erase(_undated.begin() + floatingTaskNumber - 1);
			return DELETE_SUCCESS;
		}
	}

	//NEED TO USE ASSERT TO DETERMINE VALID POSITION INDEX
	std::string TMTaskList::archiveOneTask(int positionIndex) {
		assert(isValidPositionIndex(positionIndex));
		TMTask &task = getTaskFromPositionIndex(positionIndex);
		task.setAsCompleted();
		_archived.push_back(task);
		
		if (positionIndex <= int(_dated.size())) {
			_dated.erase(_dated.begin() + positionIndex - 1);
			return ARCHIVED_SUCCESS;
		} else {
			int floatingTaskNumber = positionIndex - _dated.size();
			_undated.erase(_undated.begin() + floatingTaskNumber - 1);
			return ARCHIVED_SUCCESS;
		}
	}

	void TMTaskList::chronoSort() {
		std::vector<TMTask>::iterator iter;
		for (iter = _dated.begin(); iter != _dated.end(); ++iter) {
			std::iter_swap(iter, findEarliestTaskIter(iter));
		}
	}


	void TMTaskList::alphaSort() {
		std::vector<TMTask>::iterator iter;
		for (iter = _undated.begin(); iter != _undated.end(); ++iter) {
			std::iter_swap(iter, findSmallestAlphaTaskIter(iter));
		}
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
			
			if (iter->getTaskType() == TaskType::WithPeriod || iter->getTaskType() == TaskType::WithEndDateTime) {
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

	
		TMParser *parser = TMParser::getInstance(); 

		//Assumes first line in saved text file is as unmodified by user.
		//Loop stops when M2 is detected
		//Timed and deadline tasks
		for (iter = linesFromFile.erase(linesFromFile.begin()); std::search(iter->begin(), iter->end(), M2.begin(), M2.end()) == iter->end();iter = linesFromFile.erase(iter)) {
			parser->initialize(*iter);
			TMTask task = parser->parseTaskInfo();
			_dated.push_back(task);
		}

		//Floating tasks
		for (iter =linesFromFile.erase(linesFromFile.begin()); std::search(iter->begin(), iter->end(), M3.begin(), M3.end()) == iter->end(); iter = linesFromFile.erase(iter)) {
			parser->initialize(*iter);
			TMTask task = parser->parseTaskInfo();
			_undated.push_back(task);
		}

		//Completed tasks
		for (iter = linesFromFile.erase(linesFromFile.begin()); iter != linesFromFile.end(); iter = linesFromFile.erase(iter)) {
			parser->initialize(*iter);
			TMTask task = parser->parseTaskInfo();
			task.setAsCompleted();
			_archived.push_back(task);
		}

		std::cout << "Successfully loaded from file" << std::endl;
	} 

	void TMTaskList::setFileDirectory(std::string directory) {
		_fileDirectory = directory;
	}

	std::string TMTaskList::getFileDirectory() {
		return _fileDirectory;
	}

	void TMTaskList::leaveReferenceUponExit() {
		std::ofstream outFile;
		outFile.open("REFERENCE.txt");
		outFile << _fileDirectory << '\n';
		outFile << "THIS FILE IS FOR REFERENCE TO LOAD FROM FILE. DO NOT DELETE" << '\n';
		outFile.close();
		}


