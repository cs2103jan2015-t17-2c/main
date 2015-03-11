#include"TMTimeParser.h"

TMTimeParser::TMTimeParser() {
    _startDate = "";
    _startTime = "";
    _endDate = "";
    _endTime = "";
}

std::string TMTimeParser::getStartDate() {
    return _startDate;
}

std::string TMTimeParser::getStartTime() {
    return _startTime;
}

std::string TMTimeParser::getEndDate() {
    return _endDate;
}

std::string TMTimeParser::getEndTime() {
    return _endTime;
}
    
//edit individual pre-existing information
void TMTimeParser::setStartDate(std::string newStartDate) {
    _startDate = newStartDate;
}

void TMTimeParser::setStartTime(std::string newStartTime) {
    _startTime = newStartTime;
}

void TMTimeParser::setEndDate(std::string newEndDate) {
    _endDate = newEndDate;
}

void TMTimeParser::setEndTime(std::string newEndTime) {
    _endTime = newEndTime;
}

//extract information from user's entry if not floating task
void TMTimeParser::extractStartTime(std::string remainingEntry) {
    //search for keyword at/from
    std::size_t atToken = remainingEntry.find("at");
    std::size_t fromToken = remainingEntry.find("from");

    //followed by numerical token from string
    if(atToken != std::string::npos || fromToken != std::string::npos) {
        std::size_t 
        std::string tokenAfterKey = 
    //check if there is '' for places with times in their names or if the user wants
    //a preset time time at an ungodly hour e.g. 4am
}

void TMTimeParser::extractEndTime(std::string remainingEntry) {
    //search for '-'/ 'to' check if there is a time before and after it
    //otherwise endTime = startTime
}
 
void TMTimeParser::extractStartDate(std::string remainingEntry) {
    //search for 'from'
    //if found check if trailing word is a day and word after the day is 'to'/'-'
    //search for 'on'
    //if found check if the trailing word is e.g. Friday/
    // 13022015/130215
    // 13 Feb 2015/ 13 February 2015
}

void TMTimeParser::extractEndDate(std::string remainingEntry) {
    //search for '-'/ 'to' check if there is a date before and after it
    //otherwise endDate = startDate
}