#ifndef TMTASK_H
#define TMTASK_H
#include <string>
#include "TMTaskTime.h"

class TMTask{
private:
    std::string _taskDescription;
    TMTaskTime _taskTime;
    bool _isCompleted;

public:
    TMTask(std::string, TMTaskTime, bool);

    std::string getTaskDescription();
    TMTaskTime getTaskTime();
    bool isCompleted();

    void setTaskDescription(std::string);
    void setTaskTime(TMTaskTime);
    void setAsCompleted();
    void setAsIncompleted();
};
#endif
