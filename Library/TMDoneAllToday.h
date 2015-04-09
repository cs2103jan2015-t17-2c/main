#ifndef TMDONEALLTODAY_H
#define TMDONEALLTODAY_H

#include "TMCommand.h"

class TMDoneAllToday : public TMCommand {

public:

	void execute() {
		TMTaskListStates *taskListStates = TMTaskListStates::getInstance();
		TMTaskList taskList = taskListStates->getCurrentTaskList();

		boost::gregorian::date dateToday =  boost::gregorian::day_clock::local_day();
		std::locale facet(std::locale::classic(), new boost::gregorian::date_facet("%d %b %Y"));
		std::ostringstream stream;
		stream.imbue(facet);
		std::string  strDateToday;
		stream << dateToday;
		strDateToday = stream.str();
		 

		int numDated = taskList.getDatedSize();
		std::vector<int> completeIndexes;
		std::ostringstream oss;

		for (int i = 1; i <= numDated; i++) {
			TMTask task = taskList.getTaskFromPositionIndex(i);
			if (task.getTaskTime().getEndDate() == strDateToday) {
				completeIndexes.push_back(i);
			}
		}

		std::vector<int>::iterator iter;
		int numArchived = 0;
		for (iter = completeIndexes.begin(); iter != completeIndexes.end(); ++iter) {
			taskList.archiveOneTask(*iter);
			numArchived++;
			updatePositionIndexes(completeIndexes, *iter);
		}

		oss << numArchived << " tasks successfully marked as completed and archived.";
		outcome = oss.str();
		taskListStates->addNewState(taskList);		
	}

};

#endif