#ifndef TMTASK_H
#define TMTASK_H
#include <string>
#include "TMTaskTime.h"

enum TaskType {
    WithStartDateTime, WithEndDateTime, WithPeriod, Undated, Invalid
};

class TMTask{
private:
    std::string _taskDescription;
    TMTaskTime _taskTime;
    bool _isCompleted;
    bool _isConfirmed;
	bool _isClash;
    TaskType _taskType;
	int _unconfirmedBatchNum;


public:
    //constructor for deadlined tasks and timed tasks
    TMTask(std::string, TMTaskTime, TaskType);
    //constructor for floating tasks
    TMTask(std::string, TaskType);

    std::string getTaskDescription();
    TMTaskTime getTaskTime();
    bool isCompleted();
	bool isConfirmed();
	bool isClash();
    TaskType getTaskType();

    void setTaskDescription(std::string);
	int getUnconfirmedBatchNum();
    //Preconditions: TMTaskTime must be created prior to setting
    void setTaskTime(TMTaskTime);
    void setAsCompleted();
    void setAsIncompleted();
    void setAsConfirmed();
    void setAsUnconfirmed();
	void setAsClash();
	void setAsNotClash();
    void setTaskType(TaskType);
	void setUnconfirmedBatchNum(int);
};
#endif

