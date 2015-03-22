#include <fstream>
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

	bool TMTaskList::hasClash(TMTask task) {
		/*std::vector<TMTask>::iterator iter;
		for (iter = timedAndDeadline.begin(); iter != timedAndDeadline.end(); ++iter) {
			if (startsEarlierThan(task, *iter)) {
				if (isTwoClash(task, *iter)) {
					return true;
				}
			} else {
				if (isTwoClash(*iter, task)) {
					return true;
				}
			}
		}	*/
	return false;
	}

	bool TMTaskList::isTwoClash(TMTask task1, TMTask task2) {
		if (task1.getTaskTime().getEndDate() == task2.getTaskTime().getStartDate()) {
			return ( task1.getTaskTime().getEndTime() > task2.getTaskTime().getStartTime() );
		} else {
			( task1.getTaskTime().getEndDate() > task2.getTaskTime().getStartDate() );
		}
	}

	bool TMTaskList::startsEarlierThan(TMTask task, TMTaskTime time) {
		/*std::string task1StartDate = task1.getTaskTime().getStartDate();
		std::string task2StartDate = task2.getTaskTime().getStartDate();

		if (task1StartDate < task2StartDate) {
			return true;
		} else if (task1StartDate == task2StartDate) {
			return (task1.getTaskTime().getStartTime() <  task2.getTaskTime().getStartTime());
		}*/

		return false;
	} 

	bool TMTaskList::endsEarlierThan(TMTask task, TMTaskTime time) {return true;} //YET TO CODE

	bool TMTaskList::isEarlierThan(TMTaskTime time1, TMTaskTime time2) {return true;} //YET TO CODE

	std::vector<TMTask> TMTaskList::findClashes(TMTask task) {
		std::vector<TMTask> clashes;
		/*std::vector<TMTask>::iterator iter;
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
		}*/
	return clashes;
	}

	std::vector<TMTask>::iterator TMTaskList::findEarliestTaskIter(std::vector<TMTask>::iterator unsortedStart) {
									TMTask earliestTask = *unsortedStart;
									std::vector<TMTask>::iterator earliestTaskIter;
									/*earliestTaskIter = unsortedStart;
									std::vector<TMTask>::iterator iter;
		
									for (iter = unsortedStart; iter != timedAndDeadline.end(); ++iter) {
										if (startsEarlierThan(*iter, earliestTask)) {
											earliestTask = *iter;
											earliestTaskIter = iter;
										}
									}*/

									return earliestTaskIter;
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

	int TMTaskList::getTimedAndDeadlineSize() {
		return timedAndDeadline.size();
	}

	int TMTaskList::getFloatingSize() {
		return floating.size();
	}

	int TMTaskList::getArchivedSize() {
		return archived.size();
	}

	std::vector<TMTask> TMTaskList::getTimedAndDeadline() {
		return timedAndDeadline;
	}
	
	std::vector<TMTask> TMTaskList::getFloating() {
		return floating;
	}
	
	std::vector<TMTask> TMTaskList::getArchived() {
		return archived;
	}




	//BASIC FUNCTIONS//
	void TMTaskList::addTask (TMTask task) {
	
		if (task.getTaskType() == TaskType::Timed  || task.getTaskType() == TaskType::WithDeadline) {
			/*if (hasClash(task)) {
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
			}*/
			timedAndDeadline.push_back(task);
			std::cout << "TIMED TASK ADDED!" << std::endl;
			//chronoSort();
		} else {
			floating.push_back(task);
			std::cout << "FLOATING TASK ADDED!" << std::endl;
		}

	}

	void TMTaskList::blockMultiple(std::vector<TMTask> tasks, TMTaskList tasklist) {
		std::vector<TMTask>::iterator iter;
		for (iter = tasks.begin(); iter != tasks.end(); ++iter) {
			tasklist.addTask(*iter);
		}
		tasks.clear();
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
			TMTask &task = floating[positionIndex-timedAndDeadline.size()-1];
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

	void TMTaskList::archiveOneTask(int positionIndex) {
		TMTask task = getTaskFromPositionIndex(positionIndex);
		task.setAsCompleted();
		archived.push_back(task);
		removeTask(positionIndex);
	}

	void TMTaskList::chronoSort() {
		std::vector<TMTask>::iterator iter;
		for (iter = timedAndDeadline.begin(); iter != timedAndDeadline.end(); ++iter) {
			std::iter_swap(iter, findEarliestTaskIter(iter));
		}
		std::cout << "Chronological sort completed." <<std:: endl;
		
	}

	void TMTaskList::alphaSort() {}		//YET TO CODE

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

	


	//EXPORT AND IMPORT FUNCTIONS//
	void TMTaskList::writeToFile(){
		std::ofstream outFile;
		int i;
		std::vector<TMTask>::iterator iter;

		outFile.open(_fileDirectory);
		outFile << "Number of timed and deadline: "  << timedAndDeadline.size() << "\n";
		for (i = 0; i < timedAndDeadline.size(); ++i) {
			outFile << timedAndDeadline[i].getTaskDescription() << 
				" " << timedAndDeadline[i].getTaskTime().getStartDate() << 
				" " << timedAndDeadline[i].getTaskTime().getStartTime() << 
				" " << timedAndDeadline[i].getTaskTime().getEndDate() <<
				" " << timedAndDeadline[i].getTaskTime().getEndTime() << "\n";
		}

		//outFile << '\n';
	
		outFile << "Number of undated tasks: " << floating.size() << "\n";
		for (i = 0; i < floating.size(); ++i) {
			outFile << floating[i].getTaskDescription() << "\n";
		}

		//outFile << "\n";

		outFile << "Number of completed tasks: " << archived.size() << "\n";
		for (iter = archived.begin(); iter != archived.end(); iter++) {
			
			if (iter->getTaskType() == TaskType::Timed || iter->getTaskType() == TaskType::WithDeadline) {
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
			timedAndDeadline.push_back(task);
		}

		//Floating tasks
		for (iter =linesFromFile.erase(linesFromFile.begin()); std::search(iter->begin(), iter->end(), M3.begin(), M3.end()) == iter->end(); iter = linesFromFile.erase(iter)) {
			parser->initialize(*iter);
			tasks = parser->parseTaskInfo();
			TMTask task = tasks[0];
			floating.push_back(task);
		}

		//Completed tasks
		for (iter = linesFromFile.erase(linesFromFile.begin()); iter != linesFromFile.end(); iter = linesFromFile.erase(iter)) {
			parser->initialize(*iter);
			tasks = parser->parseTaskInfo();
			TMTask task = tasks[0];
			task.setAsCompleted();
			archived.push_back(task);
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

	HANDLE hCon;

	enum Color { DARKBLUE = 1, DARKGREEN, DARKTEAL, DARKRED, DARKPINK, DARKYELLOW, GRAY, DARKGRAY, BLUE, GREEN, TEAL, RED, PINK, YELLOW, WHITE };

	void SetColor(Color c){
        if(hCon == NULL)
        hCon = GetStdHandle(STD_OUTPUT_HANDLE);
        SetConsoleTextAttribute(hCon, c);
}



	void TMTaskList::displayAllTasks() {
		std::cout << "Number of tasks with deadlines: " << timedAndDeadline.size() <<std::endl;
		std::cout << "Details:" << std::endl;
		std::vector<TMTask>::iterator iter;
		for(iter = timedAndDeadline.begin(); iter != timedAndDeadline.end(); ++iter) {
			SetColor(RED);
			std::cout << std :: left << std :: setw(25) << std :: setfill(' ') << (*iter).getTaskDescription() << "\t" << (*iter).getTaskTime().getStartDate() << 
			"\t" << (*iter).getTaskTime().getStartTime() << 
			"\t" << (*iter).getTaskTime().getEndDate() <<
			"\t" << (*iter).getTaskTime().getEndTime() <<std::endl;
			SetColor(GRAY);
		}
	
		std::cout << "Number of floating tasks:" << floating.size() <<std::endl;
		std::cout << "Details:" << std::endl;
		for(iter = floating.begin(); iter != floating.end(); ++iter) {
			SetColor(YELLOW);
			std::cout << (*iter).getTaskDescription() << std::endl;
			SetColor(GRAY);
		}
	}	



	std::string TMTaskList::displayFreeTime() {
		std::vector<TMTask>::iterator iter;
		std::string result;
		for (iter = timedAndDeadline.begin(); iter != timedAndDeadline.end(); ++iter) {
		result = (*iter).getTaskTime().getEndDate() + " " + (*iter).getTaskTime().getEndTime();
		++iter;
		result = result + " ~ " + (*iter).getTaskTime().getStartDate() + " " + (*iter).getTaskTime().getStartTime() + '\n';
		}
		return result;
	}