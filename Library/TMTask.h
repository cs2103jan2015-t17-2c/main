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
    TMTask(std::string, TMTaskTime, TaskType);

    std::string getTaskDescription();
    TMTaskTime getTaskTime();
    bool isCompleted();
	bool isConfirmed();
    TaskType getTaskType();

    void setTaskDescription(std::string);
    void setTaskTime(TMTaskTime);
    void setAsCompleted();
    void setAsIncompleted();
    void setAsConfirmed();
    void setAsUnconfirmed();
    void setTaskType(TaskType);
};
#endif
