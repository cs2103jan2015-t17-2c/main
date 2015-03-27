#include "TMTask.h"
//constructor for deadlined tasks and timed tasks
TMTask::TMTask(std::string taskDescription, TMTaskTime taskTime, TaskType taskType){
    _taskDescription = taskDescription;
    _taskTime = taskTime;
    //by default all tasks when created are presumed to be uncompleted
    _isCompleted = false;   
    _isConfirmed = true;
	_isClash = false;
    _taskType = taskType;
	_unconfirmedBatchNum = 0;
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

bool TMTask::isConfirmed(){
    return _isConfirmed;
}

bool TMTask::isClash() {
	return _isClash;
}

TaskType TMTask::getTaskType(){
    return _taskType;
}

int TMTask::getUnconfirmedBatchNum() {
	return _unconfirmedBatchNum;
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

void TMTask::setAsClash() {
	_isClash = true;
}

void TMTask::setAsNotClash() {
	_isClash = false;
}

void TMTask::setTaskType(TaskType newTaskType){
    _taskType = newTaskType;
}

void TMTask::setUnconfirmedBatchNum(int i) {
	_unconfirmedBatchNum = i;
}