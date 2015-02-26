#pragma once
#include <list>
#include <iostream>
#include <Windows.h>
#include <boost\date_time.hpp>

class Task{
	public:
	std::string startDate;
	int startTime;
	std::string endDate;
	int endTime;
	bool hasDeadline;
	std::string taskDescription;
};

class TimeMaster{
private:
	std::list<Task> timedTasks;
	std::list<Task> floatingTasks;

public:
	TimeMaster(void);
	~TimeMaster(void);

	void parseEntry(std::string);
	void addTask();
	void addFloating();
	void addBeforeSpecific();
	void addTimePeriod();
	void addRepeatTask();
	void searchFreeDates();
	void deleteTask();
	void undoEntry();
	void searchEntry();

	bool listIsNotEmpty(std::list<Task> &);
	void clearList(std::list<Task> &);

};
