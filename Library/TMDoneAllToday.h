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
		std::vector<TMTask> completedTasks;

		for (int i = 1; i <= numDated; i++) {
			TMTask task = taskList.getTaskFromPositionIndex(i);
			std::cout << task.getTaskTime().getEndDate() << " " << strDateToday << std::endl;
			if (task.getTaskTime().getEndDate() == strDateToday) {
				completedTasks.push_back(taskList.getTaskFromPositionIndex(i));
			}
		}

		std::vector<TMTask>::iterator iter;
		for (iter = completedTasks.begin(); iter != completedTasks.end(); ++iter) {
			taskList = taskListStates->getCurrentTaskList();
			int positionIndex = taskList.getPositionIndexFromTask(*iter);
			taskList.archiveOneTask(positionIndex); 
			taskListStates->addNewState(taskList);
		}
				
	}

};

#endif