#include "TMTask.h"

TMTask::TMTask(std::string taskDescription,TMTaskTime taskTime,bool isCompleted){
    _taskDescription = taskDescription;
    _taskTime = taskTime;
    _isCompleted = isCompleted;
}

std::string TMTask::getTaskDescription(){
    return _taskDescription;
}

TMTaskTime TMTask::getTaskTime(){
    return _taskTime;
}

bool TMTask::isCompleted(){
    return _isCompleted;
}

void TMTask::setTaskDescription(std::string newTaskDescription){
    _taskDescription = newTaskDescription;
}

void TMTask::setTaskTime(TMTaskTime newTaskTime) {
    _taskTime = newTaskTime;
}

void TMTask::setAsCompleted(){
    _isCompleted = true;
}

void TMTask::setAsIncompleted(){
    _isCompleted = false;
}


    