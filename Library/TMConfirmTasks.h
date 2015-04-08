#ifndef TMCONFIRMTASKS_H
#define TMCONFIRMTASKS_H


#include "TMCommand.h"

class TMConfirmTasks : public TMCommand {
public: 
	void execute() {
		TMParser *parser = TMParser::getInstance(); 
		TMTaskListStates *taskListStates = TMTaskListStates::getInstance();
		TMTaskList taskList = taskListStates->getCurrentTaskList();
		
		std::vector<int> confirmedTasksIndexes = parser->parseTaskPositionNo();
		std::vector<int>::iterator intIter;
		std::vector<int>::iterator resultsIter;
		std::vector<int> batchNumbers;
		std::vector<TMTask> toBeAdded;
		std::vector<TMTask>::iterator taskIter;
		std::ostringstream ossValid, ossInvalid;

		if (!noRepeatedIndexes(confirmedTasksIndexes)) {
			outcome = WARNING_REPEATED_INDEXES_SPECIFIED;
			return;
		}

		for (intIter = confirmedTasksIndexes.begin(); intIter != confirmedTasksIndexes.end(); ) {
			TMTask task = taskList.getTaskFromPositionIndex(*intIter);
			if (!taskList.isValidPositionIndex(*intIter) || task.isConfirmed()) {
				ossInvalid << *intIter << " ";
				intIter = confirmedTasksIndexes.erase(intIter);
			} else {
				++intIter;
			}
		}


		//Creating new confirmed tasks and storing the batch numbers of confirmed tasks into a vector
		for (intIter = confirmedTasksIndexes.begin(); intIter != confirmedTasksIndexes.end(); ++intIter) {
			TMTask task = taskList.getTaskFromPositionIndex(*intIter);
			int batchNum = task.getUnconfirmedBatchNumber();
			batchNumbers.push_back(batchNum);
			
			task.setAsConfirmed();
			task.setUnconfirmedBatchNumber(0);
			toBeAdded.push_back(task);
		}

		//Removing tasks sharing similar batch numbers
		for (intIter = batchNumbers.begin(); intIter != batchNumbers.end(); ++intIter) {
			std::vector<int> results;
			results = taskList.searchUnconfirmedBatchNum(*intIter);
			for (resultsIter = results.begin(); resultsIter != results.end(); ++resultsIter) {
				taskList.removeTask(*resultsIter);
				updatePositionIndexes(results, *resultsIter);
			}
		}

		//Adding confirmed tasks
		for (taskIter = toBeAdded.begin(); taskIter != toBeAdded.end(); ++taskIter) {
			TMTask task = *taskIter;
			taskList.addTask(task);
			int positionIndex = taskList.getPositionIndexFromTask(task);
			positionIndexes.push_back(positionIndex);
		}

		ossValid << "Tasks ";
		for (intIter = positionIndexes.begin(); intIter != positionIndexes.end(); ++intIter) {
			ossValid << *intIter << " ";
		}
		ossValid << " is/are confirmed." << std::endl;
		if (ossInvalid.str().size() != 0) {
			ossInvalid << " is/are invalid position indexe(s)."; 
		}

		outcome = ossValid.str() + ossInvalid.str();
		taskListStates->addNewState(taskList);
	}

};
#endif