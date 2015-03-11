#ifndef TMTASK_H
#define TMTASK_H
#include <string>
#include "TMTaskTime.h"

enum TaskType {
    Timed, WithDeadline, Floating, Repeated
};

class TMTask{
protected:
    std::string _taskDescription;
    TMTaskTime _taskTime;
    bool _isCompleted;
    bool _isConfirmed;

    TaskType _taskType;

public:
    TMTask(std::string, TMTaskTime, TaskType);

    std::string getTaskDescription();
    TMTaskTime getTaskTime();
    bool isCompleted();
    TaskType getTaskType();

    void setTaskDescription(std::string);
    void setTaskTime(TMTaskTime);
    void setAsCompleted();
    void setAsIncompleted();
    void setTaskType(TaskType);
};
#endif
