#include "TMTask.h"
//constructor for deadlined tasks and timed tasks
TMTask::TMTask(std::string taskDescription, TMTaskTime taskTime, TaskType taskType){
    _taskDescription = taskDescription;
    _unconfirmedBatchNumber = 0;
    _taskTime = taskTime;
    //by default all tasks when created are presumed to be uncompleted
    _isCompleted = false;   
    _isConfirmed = true;
    _isClashed = false;
    _taskType = taskType;
}

std::string TMTask::getTaskDescription(){
    return _taskDescription;
}

int TMTask::getUnconfirmedBatchNumber(){
    return _unconfirmedBatchNumber;
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


bool TMTask::isClashed(){
    return _isClashed;
}

TaskType TMTask::getTaskType(){
    return _taskType;
}

std::string TMTask::getTaskTypeAsString(){
    std::map<TaskType, std::string> stringMap = boost::assign::map_list_of
        (TaskType::WithStartDateTime, "WithStartDateTime")
        (TaskType::WithEndDateTime, "WithEndDateTime")
        (TaskType::WithPeriod, "WithPeriod")
        (TaskType::Undated, "Undated")
        (TaskType::Invalid, "Invalid");

    return stringMap[_taskType];
}

void TMTask::setTaskDescription(std::string newTaskDescription){
    _taskDescription = newTaskDescription;
}

void TMTask::setUnconfirmedBatchNumber(int unconfirmedBatchNumber){
    _unconfirmedBatchNumber = unconfirmedBatchNumber;
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

void TMTask::setAsClashed(){
    _isClashed = true;
}

void TMTask::setAsUnclashed(){
    _isClashed = false;
}

void TMTask::setTaskType(TaskType newTaskType){
    _taskType = newTaskType;
}
