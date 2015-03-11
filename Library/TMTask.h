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
<<<<<<< HEAD

public:
     enum TaskType {
        Timed, WithDeadline, Floating, Repeated
    };
=======
    bool _isConfirmed;
>>>>>>> c9abfcfc5d5a81c5f1ca65ec7476325ce86c1365

protected:
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
