#pragma once
#ifndef TMCOMMAND_H
#define TMCOMMAND_H

#include "TMParser.h"
#include "TMTaskListStates.h"
#include "TMTaskList.h"
#include "TMTask.h"
#include "TMExecutor.h"

class TMCommand{
	
	public: 
	
	virtual void execute() = 0;

	std::string outcome;
	
	std::vector<int> positionIndexes;
	
	//Returns true if the user specifies repeated indexes to be manipulated.
	bool hasRepeatedIndexes(std::vector<int> indexes) {
		std::vector<int>::iterator iter1, iter2;
		
		for (iter1 = indexes.begin(); iter1 != indexes.end(); ++iter1) {
			int checkIndex = *iter1;
			for (iter2 = iter1 + 1; iter2 != indexes.end(); ++iter2) {
				if (*iter2 == checkIndex) {
					return true;
				}
			}
		}

		return false;
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
