#include "TMTask.h"
//constructor for deadlined tasks and timed tasks
TMTask::TMTask(std::string taskDescription, TMTaskTime taskTime, TaskType taskType){
    _taskDescription = taskDescription;
    _taskTime = taskTime;
    //by default all tasks when created are presumed to be uncompleted
    _isCompleted = false;   
    _isConfirmed = true;
    _taskType = taskType;
}

/*
//constructor for floating tasks
TMTask::TMTask(std::string taskDescription, TaskType taskType) {
    _taskDescription = taskDescription;
    TMTaskTime taskTime("","","","");
    _taskTime = taskTime;
    _isCompleted = false;
    _isConfirmed = true;
    _taskType = taskType;
}*/

std::string TMTask::getTaskDescription(){
    return _taskDescription;
}

TMTaskTime TMTask::getTaskTime(){
    return _taskTime;
}

bool TMTask::isCompleted(){
    return _isCompleted;
}

bool TMTask::isConfirmed(){
    return _isConfirmed;
}

TaskType TMTask::getTaskType(){
    return _taskType;
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

void TMTask::setAsConfirmed() {
    _isConfirmed = true;
}

void TMTask::setAsUnconfirmed() {
    _isConfirmed = false;
}

void TMTask::setTaskType(TaskType newTaskType){
    _taskType = newTaskType;
}
    