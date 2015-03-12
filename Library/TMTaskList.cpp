#include <fstream>
#include <vector>
#include <algorithm>
#include <string>

#include "TMTaskList.h"
#include "TMTask.h"
#include "TMParser.h"

	bool TMTaskList::areEquivalent(TMTask task1, TMTask task2) {
		if (task1.getTaskDescription() != task2.getTaskDescription()) {
			return false;
		}

		if (task1.getTaskTime() != task2.getTaskTime()) {
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

	//Returns 0 if the position index is not found
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
		if (positionIndex == sizeOfTimedAndDeadline) {
			return timedAndDeadline.back();
		}

		if (positionIndex == sizeOfTimedAndDeadline + sizeOfFloating) {
			return floating.back();
		}

		if (positionIndex < sizeOfTimedAndDeadline) {
			return timedAndDeadline[positionIndex];
		}

		if (positionIndex < sizeOfTimedAndDeadline + sizeOfFloating) {
			return floating[positionIndex - sizeOfTimedAndDeadline];
		}
	}

	void TMTaskList::addTask (TMTask task) {
	
		if (task.getTaskType() == TaskType::Timed  || task.getTaskType() == TaskType::WithDeadline) {
			timedAndDeadline.push_back(task);
		}
		if (task.getTaskType() == TaskType::Floating) {
			floating.push_back(task);
		}
	
	}

	void TMTaskList::updateTask(int positionIndex, std::string componentOfTask, std::string changeTo) {}

	void TMTaskList::removeTask(int positionIndex) {	
		if (positionIndex <= timedAndDeadline.size()) {
			timedAndDeadline.erase(timedAndDeadline.begin() + positionIndex);
		} else {
			int floatingTaskNumber = positionIndex - timedAndDeadline.size();
			floating.erase(floating.begin() + floatingTaskNumber);
		}
	}

	void TMTaskList::displayAllTasks() {}
	void TMTaskList::sortTasksAccDate() {}

	void TMTaskList::blockMultiple(std::vector<TMTask> tasks, TMTaskList tasklist) {
		std::vector<TMTask>::iterator iter;
		for (iter = tasks.begin(); iter != tasks.end(); ++iter) {
			tasklist.addTask(*iter);
		}
	}

	void TMTaskList::freeMultiple(std::vector<TMTask> confirmedTasks, TMTaskList tasklist) {
		std::vector<TMTask>::iterator iter;

		//Mark confirmed tasks as confirmed
		for (iter = confirmedTasks.begin(); iter != confirmedTasks.end(); ++iter) {
			tasklist.updateTask(tasklist.searchOneTask(*iter), "isConfirmed", true);
			++iter;
		}

		iter = confirmedTasks.begin();
		std::string taskDescriptionOfConfirmed = (*iter).getTaskDescription();

		//Delete unconfirmed tasks
		for (iter = tasklist.begin(); iter != tasklist.end(); ++iter) {
			if ( (*iter).getTaskDescription() == taskDescriptionOfConfirmed && (*iter).getIsConfirmed() == false ) {
				tasklist.removeTask(tasklist.searchOneTask(*iter));
			} 
		}
	}

	void TMTaskList::archiveTodaysTasks() {
		ofstream writeToFile;
		writeToFile.open("TMStorage.txt");
		writeToFile << "Tasks With Deadline:" << endl;
		writeToFile << timedAndDeadline.size() <<endl;
		for(int i =0; i < timedAndDeadline.size(); ++i)
		{
			writeToFile << timedAndDeadline[i].getTaskDescription() << " " << timedAndDeadline[i].getTaskTime().getStartDate() << 
				" " << timedAndDeadline[i].getTaskTime().getStartTime() << 
				" " << timedAndDeadline[i].getTaskTime().getEndDate() <<
				" " << timedAndDeadline[i].getTaskTime().getEndTime() <<endl;
		}
	
		writeToFile << "Tasks Without Deadline:" <<endl;
		writeToFile <<floating.size()<<endl;
		for(int j=0; j < floating.size(); ++j)
		{
			writeToFile << floating[j].getTaskDescription() << endl;
		}
		writeToFile.close();
	
	/*date dateToday(day_clock::local_day());
	std::vector<TMTask> iterator::iter;
	for (iter = tasklist.begin(); iter != tasklist.end(); ++iter) {
		if ((*iter).getTaskEndDate() == dateToday) {
			(*iter).setAsCompleted(); 
		}
	}*/
	}

	void TMTaskList::archiveOneTask() {}
	
	std::vector<int> TMTaskList::keywordSearch(std::string keyword) {
		std::vector<TMTask> searchResults;
		int positionTimed, positionFloating;
		//Search for keyword in timedAndDeadline std::vector
		for (int i = 0; i<timedAndDeadline.size(); ++i) {
			positionTimed = timedAndDeadline[i].getTaskDescription().find(keyword);
			if (positionTimed != string::npos) {
				searchResults.push_back(timedAndDeadline[i]);
			}
		}

		//Search for keyword in floating std::vector
		for (int j = 0; j<floating.size(); ++j) {
			positionFloating = floating[j].getTaskDescription().find(keyword);
			if (positionFloating != string::npos) {
				searchResults.push_back(floating[j]);
			}
		}
	
		for (int k=0; k<searchResults.size(); ++k) {
			cout << searchResults[k].getTaskDescription() << " " 
			<< searchResults[k].getTaskTime().getStartDate() << " "
			<< searchResults[k].getTaskTime().getStartTime() << " " 
			<< searchResults[k].getTaskTime().getEndDate() << " " 
			<< searchResults[k].getTaskTime().getEndTime() <<endl;
		}
	}
	
	std::string TMTaskList::freeTimeSearch() {
		std::vector<TMTask>::iterator iter;
		for (iter = tasklist.begin(); iter != tasklist.end(); ++iter) {
		cout << (*iter).getEndTime() << " " << (*iter).getEndDate();
		++iter;
		cout << " ~ " << (*iter).getStartTime() << " " << (*iter).getStartDate(); 
	}

	void TMTaskList::loadFromFile() {
		ifstream readFromFile;
		string unusedLines;
		string entryTimedAndDeadline;
		string entryFloating;
		TMParser parseTimedAndDeadline,parseFloating;
		readFromFile.open("TMStorage.txt");
		getline(readFromFile,unusedLines);
		int sizeTimed;
		cin>>sizeTimed;
		for (int i=0; i < sizeTimed; ++i)
		{
			getline(readFromFile,entryTimedAndDeadline);
			parseTimedAndDeadline.parseTaskInfo(entryTimedAndDeadline);
		}
		getline(readFromFile,unusedLines);
		int sizeFloating;
		cin>>sizeFloating;
		for (int j=0;j < sizeFloating;++j)
		{
			getline(readFromFile,entryFloating);
			parseFloating.parseTaskInfo(entryFloating);
		}
	}


	