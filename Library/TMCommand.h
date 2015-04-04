#pragma once
#ifndef TMCOMMAND_H
#define TMCOMMAND_H

#include "TMParser.h"
#include "TMTaskListStates.h"
#include "TMTaskList.h"
#include "TMTask.h"
#include "TMExecutor.h"

const std::string WARNING_REPEATED_INDEXES_SPECIFIED = "Please do not specify an index more than once.";

class TMCommand{
	
	public: 
	
	virtual void execute() = 0;

	std::string outcome;
	
	std::vector<int> positionIndexes;
	
	bool noRepeatedIndexes(std::vector<int> indexes) {
		std::vector<int>::iterator iter1, iter2;
		
		for (iter1 = indexes.begin(); iter1 != indexes.end(); ++iter1) {
			int checkIndex = *iter1;
			for (iter2 = iter1 + 1; iter2 != indexes.end(); ++iter2) {
				if (*iter2 == checkIndex) {
					return false;
				}
			}
		}

		return true;
	}

	void updatePositionIndexes(std::vector<int>& indexes, int deletedIndex) {
		std::vector<int>::iterator iter;
		for (iter = indexes.begin(); iter != indexes.end(); ++iter) {
			if (*iter > deletedIndex) {
				(*iter)--;
			}
		}
	}
};

#endif
