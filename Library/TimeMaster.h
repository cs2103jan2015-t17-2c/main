#pragma once
class TimeMaster{
private:

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

