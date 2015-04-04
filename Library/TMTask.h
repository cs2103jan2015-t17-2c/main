#ifndef TMTASK_H
#define TMTASK_H
#include <string>
#include "TMTaskTime.h"
#include <boost/assign/list_of.hpp>

enum TaskType {
    WithStartDateTime, WithEndDateTime, WithPeriod, Undated, Invalid
};

enum EditableTaskComponent {
		Description, StartDate, StartTime, EndDate, EndTime, InvalidComponent
};

class TMTask{
private:
    std::string _taskDescription;
    //-1 if not unconfirmed else 1,2,...
    int _unconfirmedBatchNumber;
    TMTaskTime _taskTime;
    bool _isCompleted;
    bool _isConfirmed;
    bool _isClashed;
    TaskType _taskType;


public:
    //constructor for deadlined tasks and timed tasks
    TMTask(std::string, TMTaskTime, TaskType);
    //constructor for timedTask with multiple timings

    //constructor for floating tasks
    TMTask(std::string, TaskType);

    std::string getTaskDescription();
    int getUnconfirmedBatchNumber();
    TMTaskTime getTaskTime();
    bool isCompleted();
	bool isConfirmed();
	bool isClashed();
    TaskType getTaskType();
    std::string getTaskTypeAsString();

    void setTaskDescription(std::string);


    //Preconditions: TMTaskTime must be created prior to setting
    void setTaskTime(TMTaskTime);
    void setAsCompleted();
    void setAsIncompleted();
    void setAsConfirmed();
    void setAsUnconfirmed();
    void setAsClashed();
    void setAsUnclashed();
    void setTaskType(TaskType);
	void setUnconfirmedBatchNumber(int);
};
#endif

