#ifndef TMPARSER_H
#define TMPARSER_H

#include "TMTask.h"
#include "TMTaskTime.h"
#include "TMTimeParser.h"
#include <iostream>
#include <sstream>
#include <vector>

class TMParser {
public:
    TMParser();
    
    enum CommandTypes {
        Add, Delete, Undo, Complete, Incomplete, Search, Edit, Store, Invalid
    };

    std::vector<std::string> getTokenizedUserEntry(std::string);
    std::string extractCommand(std::string);
    std::string extractEntryAfterCommand(std::string);

    CommandTypes determineCommandType(std::string);
    
    //parse relevant info into the respective tasks
    std::vector<TMTask> parseTaskInfo(std::string);
    TMTask parseDeadlinedTaskInfo(std::string);
    TMTask parseTimedTaskInfo(std::string);
    TMTask parseFloatingTaskInfo(std::string);

    bool isDeadlinedTask(std::string);
    bool isTimedTask(std::string);
    //do we need a isFloatingTask?

    bool isInteger(std::string);
    bool isPeriod(std::string);
    bool isDate(std::string);
    bool isDay(std::string);
    bool is12HTime(std::string);
    bool is24HTime(std::string);
    bool isWordNext(std::string);

    std::string parseFirstToken(std::string);
    std::string parseSecondToken(std::string);
    std::string parseNthToken(std::string, int);
    int numberOfWords(std::string);
    //0 for Sunday and 6 for Saturday
    int dayOfWeek(std::string); 

    std::vector<int> parseTaskPositionNo(std::string);
    std::string parseSearchKey(std::string);
    std::string parseDirectory(std::string);

    std::string removeFirstToken(std::string,std::string);
    std::string removeFirstWord(std::string);
    std::string clearFirstWord(std::string);
    std::string clearFrontSpaces(std::string);

  //! From delimited date string where with order day-month-year eg: 25-1-2002 or 25-Jan-2003 (full month name is also accepted)
  //inline date from_uk_string(std::string s) {
  //  return date_time::parse_date<date>(s, date_time::ymd_order_dmy);
  //}
  //include "boost/date_time/gregorian/parsers.hpp"
  //date test = boost::gregorian::from_us_string("07-Sep-2010")

    std::string dateFromUserToBoostFormat(std::string);
    std::string dateFromBoostToStandardFormat(const boost::gregorian::date&);

    std::string timeFrom12To24HFormat(std::string);

};
#endif