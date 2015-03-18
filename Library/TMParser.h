#ifndef TMPARSER_H
#define TMPARSER_H

#include "TMTask.h"
#include "TMTaskTime.h"
#include "TMTimeParser.h"
#include <iostream>
#include <sstream>
#include <vector>
#include <ctype.h>

class TMParser {
private:
    boost::gregorian::date _dateToday;
public:
    TMParser();
    
    enum CommandTypes {
        Add, Delete, Undo, Complete, Incomplete, Search, Edit, Store, Invalid
    };

    std::vector<std::string> getTokenizedUserEntry(std::string);
    std::string extractCommand(std::vector<std::string>&);

    CommandTypes determineCommandType(std::string);
    
    //parse relevant info into the respective tasks
    std::vector<TMTask> parseTaskInfo(std::vector<std::string>);
    TMTask parseDeadlinedTaskInfo(std::vector<std::string>);
    TMTask parseTimedTaskInfo(std::vector<std::string>);
    TMTask parseFloatingTaskInfo(std::vector<std::string>);

    bool isDeadlinedTask(std::vector<std::string>);
    bool isTimedTask(std::vector<std::string>);
    //do we need a isFloatingTask?
    //
    bool isValidDate(std::string);
    //
    bool isInteger(std::string);
    bool isPeriod(std::string);
    //to test for date in other forms. define another function to convert 
    bool isDate(std::string);
    bool isDay(std::string);
    bool is12HTime(std::string);
    bool is24HTime(std::string);
    bool isWordNext(std::string);
    bool isAM(std::string);
    bool isPM(std::string);

    int numberOfWords(std::string);
    //0 for Sunday and 6 for Saturday
    int dayOfWeek(std::string); 

    std::vector<int> parseTaskPositionNo(std::string);
    std::string parseSearchKey(std::vector<std::string>);
    std::string parseDirectory(std::vector<std::string>);


  //! From delimited date string where with order day-month-year eg: 25-1-2002 or 25-Jan-2003 (full month name is also accepted)
  //inline date from_uk_string(std::string s) {
  //  return date_time::parse_date<date>(s, date_time::ymd_order_dmy);
  //}
  //include "boost/date_time/gregorian/parsers.hpp"
  //date test = boost::gregorian::from_us_string("07-Sep-2010")
    std::string returnLowerCase(std::string);
    std::string dateFromUserToBoostFormat(std::string);
    std::string dateFromBoostToStandardFormat(const boost::gregorian::date&);
    std::string dateFromBoostToDelimitedDDMMYYYY(const boost::gregorian::date&);

    std::string timeTo24HFormat(std::string);
    std::string getCurrentTime();

};
#endif