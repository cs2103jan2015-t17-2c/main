#include "TMTaskTime.h"

TMTaskTime::TMTaskTime(){
}

TMTaskTime::TMTaskTime(std::string startDate, std::string startTime, std::string endDate,
                       std::string endTime){
    //need to try catch and throw
    TMDateTime startDateTime(startDate,startTime);
    _startDateTime = startDateTime;

    TMDateTime endDateTime(endDate,endTime);
    _endDateTime = endDateTime;
}

std::string TMTaskTime::getStartDate(){
    return _startDateTime.getDate();
}

std::string TMTaskTime::getStartTime(){
    return _startDateTime.getTime();
}

std::string TMTaskTime::getEndDate(){
    return _endDateTime.getDate();
}

std::string TMTaskTime::getEndTime(){
    return _endDateTime.getTime();
}

TMDateTime TMTaskTime::getStartDateTime(){
	return _startDateTime;
}

TMDateTime TMTaskTime::getEndDateTime(){
	return _endDateTime;
}

void TMTaskTime::setStartDate(std::string newStartDate){
    _startDateTime.setDate(newStartDate);
}

void TMTaskTime::setStartTime(std::string newStartTime){
    _startDateTime.setTime(newStartTime);
}

void TMTaskTime::setEndDate(std::string newEndDate){
    _endDateTime.setDate(newEndDate);
}

void TMTaskTime::setEndTime(std::string newEndTime){
    _endDateTime.setTime(newEndTime);
}