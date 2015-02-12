#pragma once
#include <list>
struct Task{

	int startTime;
	int endTime;
	std::string taskDescription;

};
class TimeMaster{
private:
	std::list<Task> tasksForTheYear;

public:
	TimeMaster(void);
	~TimeMaster(void);

	void addTask();
	void addFloating();
	void addBeforeSpecific();
	void addTimePeriod();
	void addRepeatTask();
	void searchFreeDates();
	void deleteTask();
	void undoEntry();
	void searchEntry();

};

