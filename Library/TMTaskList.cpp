#include <fstream>
#include <vector>
#include <algorithm>
#include <string>
#include <boost\date_time.hpp>

#include "TMTaskList.h"
#include "TMTask.h"
#include "TMParser.h"

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

	bool TMTaskList::hasClash(TMTask task) {
		std::vector<TMTask>::iterator iter;
		for (iter = timedAndDeadline.begin(); iter != timedAndDeadline.end(); ++iter) {
			if (startsEarlierThan(task, *iter)) {
				if (isTwoClash(task, *iter)) {
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

	bool TMTaskList::isTwoClash(TMTask task1, TMTask task2) {
		if (task1.getTaskTime().getEndDate() == task2.getTaskTime().getStartDate()) {
			return ( task1.getTaskTime().getEndTime() > task2.getTaskTime().getStartTime() );
		} else {
			( task1.getTaskTime().getEndDate() > task2.getTaskTime().getStartDate() );
		}
	}

	std::vector<TMTask> TMTaskList::findClashes(TMTask task) {
		std::vector<TMTask> clashes;
		std::vector<TMTask>::iterator iter;
		for (iter = timedAndDeadline.begin(); iter != timedAndDeadline.end(); ++iter) {
			if (startsEarlierThan(task, *iter)) {
				if (isTwoClash(task, *iter)) {
					clashes.push_back(*iter);
				}
			} else {
				if (isTwoClash(task, *iter)) {
					clashes.push_back(*iter);
				}
			}
		}
	return clashes;
	}

	
	int TMTaskList::getPositionIndexFromTask(TMTask task) {
		std::vector<TMTask>::iterator iter;
		int positionIndex = 1;
		
		for (iter = timedAndDeadline.begin(); iter != timedAndDeadline.end(); ++iter) {
			if (areEquivalent(task, *iter)) {
				return positionIndex;
			}
			positionIndex++;
		}
		
		for (iter = floating.begin(); iter != floating.end(); ++iter) {
			if (areEquivalent(task, *iter)) {
				return positionIndex;
			}
			positionIndex++;
		}

		return 0;
	}	

	TMTask TMTaskList::getTaskFromPositionIndex(int positionIndex) {
		if (positionIndex == timedAndDeadline.size()) {
			return timedAndDeadline.back();
		}

		if (positionIndex == timedAndDeadline.size() + floating.size()) {
			return floating.back();
		}

		if (positionIndex < timedAndDeadline.size()) {
			return timedAndDeadline[positionIndex - 1];
		}

		if (positionIndex < timedAndDeadline.size() + floating.size()) {
			return floating[positionIndex - timedAndDeadline.size() - 1];
		}
	}

	void TMTaskList::addTask (TMTask task) {
	
		if (task.getTaskType() == TaskType::Timed  || task.getTaskType() == TaskType::WithDeadline) {
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
						+ " " + (*iter).getTaskTime().getEndTime(); //TO IMPLEMENT NEW METHODS IN TMTASK
					std::cout << taskDetails << std::endl;
				}
				std::cout << "Do you want to add this task despite clashes? (Y/N)" << std::endl;
				std::string usersReply;
				std::cin >> usersReply;

				if (usersReply == "n" || usersReply == "N") {
					return;
				}
			}
			timedAndDeadline.push_back(task);
			std::cout << "TIMED TASK ADDED!" << std::endl;
			//chronoSort();
		} else {
			floating.push_back(task);
			std::cout << "FLOATING TASK ADDED!" << std::endl;
		}

	}

	void TMTaskList::updateTask(int positionIndex, std::string componentOfTask, std::string changeTo) {
		if (positionIndex <= timedAndDeadline.size()) {
			TMTask task = timedAndDeadline[positionIndex-1];

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
			
		} else if (positionIndex <= timedAndDeadline.size() + floating.size()) {
			TMTask task = floating[positionIndex-floating.size()-1];
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

	void TMTaskList::removeTask(int positionIndex) {	
		if (positionIndex <= timedAndDeadline.size()) {
			timedAndDeadline.erase(timedAndDeadline.begin() + positionIndex - 1);
		} else if (positionIndex <= timedAndDeadline.size() + floating.size()) {
			int floatingTaskNumber = positionIndex - timedAndDeadline.size();
			floating.erase(floating.begin() + floatingTaskNumber - 1);
		}
	}

	void TMTaskList::displayAllTasks() {
		std::cout << "Number of tasks with deadlines: " << timedAndDeadline.size() <<std::endl;
		std::cout << "Details:" << std::endl;
		std::vector<TMTask>::iterator iter;
		for(iter = timedAndDeadline.begin(); iter != timedAndDeadline.end(); ++iter) {
			std::cout << (*iter).getTaskDescription() << " " << (*iter).getTaskTime().getStartDate() << 
			" " << (*iter).getTaskTime().getStartTime() << 
			" " << (*iter).getTaskTime().getEndDate() <<
			" " << (*iter).getTaskTime().getEndTime() <<std::endl;
		}
	
		std::cout << "Number of floating tasks:" << floating.size() <<std::endl;
		std::cout << "Details:" << std::endl;
		for(iter = floating.begin(); iter != floating.end(); ++iter) {
			std::cout << (*iter).getTaskDescription() << std::endl;
		}
	}	
	
	
	void TMTaskList::chronoSort() {
		std::vector<TMTask>::iterator iter;
		for (iter = timedAndDeadline.begin(); iter != timedAndDeadline.end(); ++iter) {
			std::iter_swap(iter, findEarliestTaskIter(iter));
		}
		std::cout << "Chronological sort completed." <<std:: endl;
		
	}

	std::vector<TMTask>::iterator TMTaskList::findEarliestTaskIter(std::vector<TMTask>::iterator unsortedStart) {
									TMTask earliestTask = *unsortedStart;
									std::vector<TMTask>::iterator earliestTaskIter;
									earliestTaskIter = unsortedStart;
									std::vector<TMTask>::iterator iter;
		
									for (iter = unsortedStart; iter != timedAndDeadline.end(); ++iter) {
										if (startsEarlierThan(*iter, earliestTask)) {
											earliestTask = *iter;
											earliestTaskIter = iter;
										}
									}

									return earliestTaskIter;
	}

	bool TMTaskList::startsEarlierThan(TMTask task1, TMTask task2) {
		std::string task1StartDate = task1.getTaskTime().getStartDate();
		std::string task2StartDate = task2.getTaskTime().getStartDate();

		if (task1StartDate < task2StartDate) {
			return true;
		} else if (task1StartDate == task2StartDate) {
			return (task1.getTaskTime().getStartTime() <  task2.getTaskTime().getStartTime());
		}

		return false;
	}

	void TMTaskList::alphaSort() {}		//YET TO CODE

	void TMTaskList::blockMultiple(std::vector<TMTask> tasks, TMTaskList tasklist) {
		std::vector<TMTask>::iterator iter;
		for (iter = tasks.begin(); iter != tasks.end(); ++iter) {
			tasklist.addTask(*iter);
		}
		tasks.clear();
	}

	void TMTaskList::archiveTodaysTasks() {
		/*date dateToday(day_clock::local_day());
		std::vector<TMTask>::iterator iter;
		for (iter = timedAndDeadline.begin(); iter != timedAndDeadline.end(); ++iter) {
			if ((*iter).getTaskTime().getEndDate() == dateToday) { //THINK OF HOW TO CONVERT DATETODAY TO STRING
				
                archiveOneTask(getPositionIndexFromTask(*iter)); 
			}
		}*/
	}

	void TMTaskList::archiveOneTask(int positionIndex) {
		TMTask task = getTaskFromPositionIndex(positionIndex);
		task.setAsCompleted();
		archived.push_back(task);
		removeTask(positionIndex);
	}
	
	std::vector<int> TMTaskList::keywordSearch(std::string keyword) {
		std::string lowerKeyword = toLower(keyword);
		
		std::vector<std::string> taskDescInLower;
		std::vector<TMTask>::iterator iter;
		std::vector<int> searchResults;
		int posIndexCounter = 1;

		for (iter = timedAndDeadline.begin(); iter != timedAndDeadline.end(); ++iter) {
			taskDescInLower.push_back(toLower((*iter).getTaskDescription()));
		}
		for (iter = floating.begin(); iter != floating.end(); ++iter) {
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

	std::string TMTaskList::toLower(std::string toBeConverted) {
		std::string aCopy = toBeConverted;
		transform(aCopy.begin(), aCopy.end(), aCopy.begin(), ::tolower);
		return aCopy;
	}
	
	std::string TMTaskList::freeTimeSearch() {
		std::vector<TMTask>::iterator iter;
		std::string result;
		for (iter = timedAndDeadline.begin(); iter != timedAndDeadline.end(); ++iter) {
		result = (*iter).getTaskTime().getEndDate() + " " + (*iter).getTaskTime().getEndTime();
		++iter;
		result = result + " ~ " + (*iter).getTaskTime().getStartDate() + " " + (*iter).getTaskTime().getStartTime() + '\n';
		}
		return result;
	}

	void TMTaskList::writeToFile(){
		std::ofstream writeToFile;
		writeToFile.open("TMStorage.txt", std::ofstream::trunc);
		writeToFile << "Tasks With Deadline:"  <<std::endl;
		writeToFile << timedAndDeadline.size() <<std::endl;
		for(int i =0; i < timedAndDeadline.size(); ++i)
		{
			writeToFile << timedAndDeadline[i].getTaskDescription() << " " << timedAndDeadline[i].getTaskTime().getStartDate() << 
				" " << timedAndDeadline[i].getTaskTime().getStartTime() << 
				" " << timedAndDeadline[i].getTaskTime().getEndDate() <<
				" " << timedAndDeadline[i].getTaskTime().getEndTime() <<std::endl;
		}
	
		writeToFile << "Tasks Without Deadline:" <<std::endl;
		writeToFile <<floating.size()<<std::endl;
		for(int j=0; j < floating.size(); ++j)
		{
			writeToFile << floating[j].getTaskDescription() << std::endl;
		}
		writeToFile.close();
	}

	void TMTaskList::loadFromFile() {
		/*std::ifstream readFromFile;
		std::string unusedLines;
		std::string entryTimedAndDeadline;
		std::string entryFloating;
		TMParser parseTimedAndDeadline,parseFloating;
		readFromFile.open("TMStorage.txt");
		getline(readFromFile,unusedLines);
		int sizeTimed;
		std :: cin>>sizeTimed;
		for (int i=0; i < sizeTimed; ++i){
			getline(readFromFile,entryTimedAndDeadline);
			parseTimedAndDeadline.parseTaskInfo(entryTimedAndDeadline);
		}
		getline(readFromFile,unusedLines);
		int sizeFloating;
		std :: cin>>sizeFloating;
		for (int j=0;j < sizeFloating;++j){
			getline(readFromFile,entryFloating);
			parseFloating.parseTaskInfo(entryFloating);
		}
		readFromFile.close();*/
	} 