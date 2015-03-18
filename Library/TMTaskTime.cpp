#include "TMTaskTime.h"

const std::locale facet(std::locale::classic(), new boost::gregorian::date_facet("%d-%m-%Y"));

TMTaskTime::TMTaskTime(){
    boost::gregorian::date startDate(boost::gregorian::not_a_date_time);
    _startDate = startDate;
    _startTime = "";
    boost::gregorian::date endDate(boost::gregorian::not_a_date_time);
    _endDate = endDate;
    _endTime = "";
}

TMTaskTime::TMTaskTime(std::string startDate, std::string startTime, std::string endDate,
                       std::string endTime){
    //need to try catch and throw
    _startDate = boost::gregorian::from_uk_string(startDate);

    _startTime = startTime;

    _endDate = boost::gregorian::from_uk_string(endDate);

    _endTime = endTime;
}

std::string TMTaskTime::getStartDate(){
    std::stringstream stream;
    stream.imbue(facet);
    stream << _startDate;

    return stream.str();
}

std::string TMTaskTime::getStartTime(){
    return _startTime;
}

std::string TMTaskTime::getEndDate(){
    std::stringstream stream;
    stream.imbue(facet);
    stream << _endDate;

    return stream.str();
    //return boost::gregorian::to_iso_extended_string(_endDate);
}

std::string TMTaskTime::getEndTime(){
    return _endTime;
}

void TMTaskTime::setStartDate(std::string newStartDate){
    _startDate = boost::gregorian::from_undelimited_string(newStartDate);
}

void TMTaskTime::setStartTime(std::string newStartTime){
    _startTime = newStartTime;
}

void TMTaskTime::setEndDate(std::string newEndDate){
    _endDate = boost::gregorian::from_undelimited_string(newEndDate);
}

void TMTaskTime::setEndTime(std::string newEndTime){
    _endTime = newEndTime;
}