#ifndef TMRECURRINGTASK_H
#define TMRECURRINGTASK_H
#include "TMTask.h"

class TMRecurringTask : public TMTask{
protected:
    int _taskID;
    std::string _repetitionFrequency;
    int _remainingRepetitions;

public:
    TMRecurringTask(std::string, TMTaskTime, TaskType, int, std::string, int);

    int getTaskID();
    std::string getRepetitionFrequency();
    int getRemainingRepetitions();

    void setTaskID(int);
    void setRepetitionFrequency(std::string);
    void setRemainingRepetitions(int);
};
#endif