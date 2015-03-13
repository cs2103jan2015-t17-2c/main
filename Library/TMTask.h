#ifndef TMTASK_H
#define TMTASK_H
#include <string>
#include "TMTaskTime.h"

enum TaskType {
    Timed, WithDeadline, Floating
};

class TMTask{
private:
    std::string _taskDescription;
    TMTaskTime _taskTime;
    bool _isCompleted;
    bool _isConfirmed;
    TaskType _taskType;

public:
    //constructor for deadlined tasks and timed tasks
    TMTask(std::string, TMTaskTime, TaskType);
    //constructor for floating tasks
    TMTask(std::string, TaskType);

    std::string getTaskDescription();
    TMTaskTime getTaskTime();
    bool isCompleted();
	bool isConfirmed();
    TaskType getTaskType();

    void setTaskDescription(std::string);
    //Preconditions: TMTaskTime must be created prior to setting
    void setTaskTime(TMTaskTime);
    void setAsCompleted();
    void setAsIncompleted();
    void setAsConfirmed();
    void setAsUnconfirmed();
    void setTaskType(TaskType);
};
#endif
