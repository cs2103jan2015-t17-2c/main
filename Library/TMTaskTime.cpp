#include "TMTaskTime.h"

TMTaskTime::TMTaskTime(){
}

TMTaskTime::TMTaskTime(std::string startDate, int startTime, std::string endDate, int endTime){
    _startDate = startDate;
    _startTime = startTime;
    _endDate = endDate;
    _endTime = endTime;
}

std::string TMTaskTime::getStartDate(){
    return _startDate;
}

int TMTaskTime::getStartTime(){
    return _startTime;
}

std::string TMTaskTime::getEndDate(){
    return _endDate;
}

int TMTaskTime::getEndTime(){
    return _endTime;
}

void TMTaskTime::setStartDate(std::string newStartDate){
    _startDate = newStartDate;
}

void TMTaskTime::setStartTime(int newStartTime){
    _startTime = newStartTime;
}

void TMTaskTime::setEndDate(std::string newEndDate){
    _endDate = newEndDate;
}

void TMTaskTime::setEndTime(int newEndTime){
    _endTime = newEndTime;
}