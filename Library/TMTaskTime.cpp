#include "TMTaskTime.h"

const std::locale facet(std::locale::classic(), new boost::gregorian::date_facet("%d-%m-%Y"));

TMTaskTime::TMTaskTime(){
    boost::gregorian::date startDate(boost::gregorian::not_a_date_time);
    _startDate = startDate;
    _startTime = boost::posix_time::not_a_date_time;
    boost::gregorian::date endDate(boost::gregorian::not_a_date_time);
    _endDate = endDate;
    _endTime = boost::posix_time::not_a_date_time;
}

TMTaskTime::TMTaskTime(std::string startDate, std::string startTime, std::string endDate,
                       std::string endTime){
    boost::gregorian::date_facet *facet = new boost::gregorian::date_facet("%d-%m-%Y");
    std::cout.imbue(std::locale(std::cout.getloc(), facet));

    _startDate = boost::gregorian::from_undelimited_string(startDate);

    if(startTime.size() == 4){
        int hours = std::stoi(startTime.substr(0,2));
        int minutes = std::stoi(startTime.substr(2,2));
        _startTime = boost::posix_time::ptime(_startDate, boost::posix_time::time_duration(hours, minutes, 0));
    } else if (startTime.size() == 0){
        _startTime = boost::posix_time::ptime(_startDate, boost::posix_time::time_duration(0, 0, 0));
    } else {
        _startTime = boost::posix_time::ptime(_startDate, boost::posix_time::time_duration(std::stoi(startTime.substr(0,2)), std::stoi(startTime.substr(3,2)),0));
    }

    _endDate = boost::gregorian::from_undelimited_string(endDate);

    if(endTime.size() == 4){
        int hours = std::stoi(endTime.substr(0,2));
        int minutes = std::stoi(endTime.substr(2,2));
        _endTime = boost::posix_time::ptime(_endDate, boost::posix_time::time_duration(hours, minutes, 0));
    } else if (endTime.size() == 0){
        _endTime = boost::posix_time::ptime(_endDate, boost::posix_time::time_duration(0, 0, 0));
    } else {
        _endTime = boost::posix_time::ptime(_endDate, boost::posix_time::time_duration(std::stoi(endTime.substr(0,2)), std::stoi(endTime.substr(3,2)),0));
    }
}

std::string TMTaskTime::getStartDate(){
    std::stringstream stream;
    stream.imbue(facet);
    stream << _startDate;

    return stream.str();
}

std::string TMTaskTime::getStartTime(){
    std::stringstream stream;
    // Use a facet to display time in a custom format (only hour and minutes).
    boost::posix_time::time_facet* facet = new boost::posix_time::time_facet();
    facet->format("%H:%M");
    stream.imbue(std::locale(std::locale::classic(), facet));
    stream << _startTime;
    return stream.str();
}

std::string TMTaskTime::getEndDate(){
    std::stringstream stream;
    stream.imbue(facet);
    stream << _endDate;

    return stream.str();
    //return boost::gregorian::to_iso_extended_string(_endDate);
}

std::string TMTaskTime::getEndTime(){
    std::stringstream stream;
    // Use a facet to display time in a custom format (only hour and minutes).
    boost::posix_time::time_facet* facet = new boost::posix_time::time_facet();
    facet->format("%H:%M");
    stream.imbue(std::locale(std::locale::classic(), facet));
    stream << _endTime;
    return stream.str();
}

void TMTaskTime::setStartDate(std::string newStartDate){
    _startDate = boost::gregorian::from_undelimited_string(newStartDate);
}

void TMTaskTime::setStartTime(std::string newStartTime){
    if(newStartTime.size() == 4){    //user has to edit by keying in the new date then new time
        int hours = std::stoi(newStartTime.substr(0,2));
        int minutes = std::stoi(newStartTime.substr(2,2));
        _startTime = boost::posix_time::ptime(_startDate, boost::posix_time::time_duration(hours, minutes, 0));
    } else if (newStartTime.size() == 0){
        _startTime = boost::posix_time::ptime(_startDate, boost::posix_time::time_duration(0, 0, 0));
    } else { //for situation e.g. 10:30
        _startTime = boost::posix_time::ptime(_startDate, boost::posix_time::time_duration(std::stoi(newStartTime.substr(0,2)), std::stoi(newStartTime.substr(3,2)),0));
    }
}

void TMTaskTime::setEndDate(std::string newEndDate){
    _endDate = boost::gregorian::from_undelimited_string(newEndDate);
}

void TMTaskTime::setEndTime(std::string newEndTime){
    if(newEndTime.size() == 4){    //user has to edit by keying in the new date then new time
        int hours = std::stoi(newEndTime.substr(0,2));
        int minutes = std::stoi(newEndTime.substr(2,2));
        _endTime = boost::posix_time::ptime(_endDate, boost::posix_time::time_duration(hours, minutes, 0));
    } else if (newEndTime.size() == 0){
        _endTime = boost::posix_time::ptime(_endDate, boost::posix_time::time_duration(0, 0, 0));
    } else {
        _endTime = boost::posix_time::ptime(_endDate, boost::posix_time::time_duration(std::stoi(newEndTime.substr(0,2)), std::stoi(newEndTime.substr(3,2)),0));
    }
}