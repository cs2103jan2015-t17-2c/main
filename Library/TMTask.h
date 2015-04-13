//@author A0111712Y

#ifndef TMTASK_H
#define TMTASK_H
#include <string>
#include "TMTaskTime.h"
#include <boost/assign/list_of.hpp>

enum TaskType {
    WithStartDateTime, WithEndDateTime, WithPeriod, Undated, Invalid
};

class TMTask{
private:
    std::string _taskDescription;
    
	//0 for confirmed tasks, else batch number would be a positive integer
    int _unconfirmedBatchNumber;
    TMTaskTime _taskTime;
    bool _isCompleted;
    bool _isConfirmed;
    bool _isClashed;
    TaskType _taskType;


public:
    TMTask(std::string, TMTaskTime, TaskType);

    std::string getTaskDescription();
    int getUnconfirmedBatchNumber();
    TMTaskTime getTaskTime();
    bool isCompleted();
	bool isConfirmed();
	bool isClashed();
    TaskType getTaskType();
    std::string getTaskTypeAsString();

    void setTaskDescription(std::string);
    void setUnconfirmedBatchNumber(int);

    //Preconditions: TMTaskTime must be created prior to setting
    void setTaskTime(TMTaskTime);
    void setAsCompleted();
    void setAsIncompleted();
    void setAsConfirmed();
    void setAsUnconfirmed();
    void setAsClashed();
    void setAsUnclashed();
    void setTaskType(TaskType);
};
#endif

