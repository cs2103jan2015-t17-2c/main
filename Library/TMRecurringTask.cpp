#include "TMRecurringTask.h"

TMRecurringTask::TMRecurringTask(std::string taskDescription, TMTaskTime taskTime,
TaskType taskType, int taskID, std::string repetitionFrequency, int remainingRepetitions)
:TMTask(taskDescription, taskTime, taskType)
{
    _taskID = taskID;                   //we need a counter to store the numbers for taskID
    _repetitionFrequency = repetitionFrequency;
    _remainingRepetitions = remainingRepetitions;
}

int TMRecurringTask::getTaskID(){
    return _taskID;
}

std::string TMRecurringTask::getRepetitionFrequency(){
    return _repetitionFrequency;
}

int TMRecurringTask::getRemainingRepetitions(){
    return _remainingRepetitions;
}

void TMRecurringTask::setTaskID(int newTaskID){
    _taskID = newTaskID;
}

void TMRecurringTask::setRepetitionFrequency(std::string newRepetitionFrequency){
    _repetitionFrequency = newRepetitionFrequency;
}

void TMRecurringTask::setRemainingRepetitions(int newRemainingRepetitions){
    _remainingRepetitions = newRemainingRepetitions;
}