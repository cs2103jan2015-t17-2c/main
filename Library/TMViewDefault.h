#pragma once
#ifndef TMVIEWDEFAULT_H
#define TMVIEWDEFAULT_H

#include "TMCommand.h"
#include <string>
#include <sstream>


class TMViewDefault : public TMCommand {

public:
	TMViewDefault() {
	
	}
	

	void execute() {
		std::ostringstream oss;
		TMExecutor *exe = TMExecutor::getInstance();
		TMTaskListStates *taskListStates = TMTaskListStates::getInstance();
		TMTaskList taskList = taskListStates->getCurrentTaskList();
		
		std::vector<std::string> displayInfo;
		std::vector<std::string> displayColours;
		
		displayInfo.push_back(getHeaderInfo());
		getDatedInfo(taskList, displayInfo);
		getUndatedInfo(taskList, displayInfo);
		getArchivedInfo(taskList, displayInfo);
		getDisplayColours(taskList, displayColours);
		
		
		exe->setDisplay(displayInfo);
		exe->setDisplayColours(displayColours);
		
	}

	std::string getHeaderInfo() {
		std::ostringstream oss; 
		oss << std :: left << std :: setw(18) << std :: setfill(' ') << "TASK DECSCRIPTION" << "\t" <<
		"START DATE" << "\t" << "START TIME" << "\t" << "END DATE" << "\t" << "END TIME" << "\t" << "CONFIRMATION" << "\t" << "CLASH STATUS" << "\n";
		return oss.str();
	}

	void getDatedInfo(TMTaskList taskList, std::vector<std::string>& displayInfo) {
		
		std::vector<TMTask> dated = taskList.getDated();
		std::vector<TMTask>::iterator iter;
		
		for (iter = dated.begin(); iter != dated.end(); ++iter) {
			TaskType type = (*iter).getTaskType();
			std::ostringstream oss;
			
			switch (type) {
			case WithStartDateTime:
				oss << std :: left << std :: setw(18) << std :: setfill(' ')  << (*iter).getTaskDescription() << "\t\t\t" <<
				std :: setw (18) << "\t\t" << (*iter).getTaskTime().getStartDate() << "\t" << (*iter).getTaskTime().getStartTime() << "\t" << 
				std::boolalpha << (*iter).isConfirmed() << std::boolalpha << (*iter).isClashed() << std::endl;
				displayInfo.push_back(oss.str());

			case WithEndDateTime:
				oss << std :: left << std :: setw(18) << std :: setfill(' ')  << (*iter).getTaskDescription() << "\t\t\t" <<
				std :: setw (18) << "\t\t" << (*iter).getTaskTime().getEndDate() << "\t" << (*iter).getTaskTime().getEndTime() << 
				std::boolalpha << (*iter).isConfirmed() << std::boolalpha << (*iter).isClashed() << std::endl;
				displayInfo.push_back(oss.str());

			case WithPeriod:
				oss << std :: left << std :: setw(18) << std :: setfill(' ') << (*iter).getTaskDescription() << "\t\t" << (*iter).getTaskTime().getStartDate() <<
				"\t" << (*iter).getTaskTime().getStartTime() << "\t\t" << (*iter).getTaskTime().getEndDate() << "\t" << (*iter).getTaskTime().getEndTime() << 
				std::boolalpha << (*iter).isConfirmed() << std::boolalpha << (*iter).isClashed() << std::endl;
				displayInfo.push_back(oss.str());
			}
		
		}
	}

	void getUndatedInfo(TMTaskList taskList, std::vector<std::string>& displayInfo) {
		std::vector<TMTask> undated = taskList.getUndated();
		std::ostringstream oss;
		std::vector<TMTask>::iterator iter;
		for (iter = undated.begin(); iter != undated.end(); ++iter) {
			oss << (*iter).getTaskDescription() << std::boolalpha << (*iter).isConfirmed() << std::endl;
			displayInfo.push_back(oss.str());
		}
	}

	void getArchivedInfo(TMTaskList taskList, std::vector<std::string>& displayInfo) {	
		
		std::vector<TMTask> archived = taskList.getArchived();
		std::vector<TMTask>::iterator iter;
		for (iter = archived.begin(); iter != archived.end(); ++iter) {
			TaskType type = (*iter).getTaskType();
			std::ostringstream oss;
			
			switch (type) {
			case WithStartDateTime:
				oss << std :: left << std :: setw(18) << std :: setfill(' ')  << (*iter).getTaskDescription() << "\t\t\t" <<
				std :: setw (18) << "\t\t" << (*iter).getTaskTime().getStartDate() << "\t" << (*iter).getTaskTime().getStartTime() << "\t" << 
				std::boolalpha << (*iter).isConfirmed() << std::boolalpha << (*iter).isClashed() << std::endl;
				displayInfo.push_back(oss.str());

			case WithEndDateTime:
				oss << std :: left << std :: setw(18) << std :: setfill(' ')  << (*iter).getTaskDescription() << "\t\t\t" <<
				std :: setw (18) << "\t\t" << (*iter).getTaskTime().getEndDate() << "\t" << (*iter).getTaskTime().getEndTime() << 
				std::boolalpha << (*iter).isConfirmed() << std::boolalpha << (*iter).isClashed() << std::endl;
				displayInfo.push_back(oss.str());

			case WithPeriod:
				oss << std :: left << std :: setw(18) << std :: setfill(' ') << (*iter).getTaskDescription() << "\t\t" << (*iter).getTaskTime().getStartDate() <<
				"\t" << (*iter).getTaskTime().getStartTime() << "\t\t" << (*iter).getTaskTime().getEndDate() << "\t" << (*iter).getTaskTime().getEndTime() << 
				std::boolalpha << (*iter).isConfirmed() << std::boolalpha << (*iter).isClashed() << std::endl;
				displayInfo.push_back(oss.str());

			case Undated:
				oss << (*iter).getTaskDescription() << std::boolalpha << (*iter).isConfirmed() << std::endl;
				displayInfo.push_back(oss.str());
			}
		
		}
	}

	void getDisplayColours(TMTaskList taskList, std::vector<std::string>& displayColours) {
		std::vector<TMTask> dated = taskList.getDated();
		int undatedSize = taskList.getUndatedSize();
		int archivedSize = taskList.getArchivedSize();
		std::vector<TMTask>::iterator iter;
		
		for (iter = dated.begin(); iter != dated.end(); ++iter) {
			TaskType type = (*iter).getTaskType();
			switch (type) {
			case WithStartDateTime:
				displayColours.push_back("White");

			case WithEndDateTime:
				displayColours.push_back("Red");

			case WithPeriod:
				displayColours.push_back("White");
			}
		}

		for (int i = 1; i <= undatedSize; ++i) {
			displayColours.push_back("Green");
		}

		for (int i = 1; i <= archivedSize; ++i) {
			displayColours.push_back("Grey");
		}
	}
};
#endif
