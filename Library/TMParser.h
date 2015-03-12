#ifndef TMPARSER_H
#define TMPARSER_H

#include "TMTask.h"
#include "TMRecurringTask.h"
#include "TMTaskTime.h"
#include "TMTimeParser.h"

class TMParser {
public:
    TMParser();
    
    enum CommandTypes {
        Add, Delete, Undo, Complete, Incomplete, Search, Edit, Store, Invalid
    };


    std::string extractCommand(std::string);
    std::string extractEntryAfterCommand(std::string);

    CommandTypes determineCommandType(std::string);

    TMTask parseTaskInfo(std::string);
    TMTask parseRepeatTaskInfo(std::string);
    TMTask parseDeadlineTaskInfo(std::string);
    TMTask parseTimedTaskInfo(std::string);
    TMTask parseFloatingTaskInfo(std::string);

    bool isRepeatedTask(std::string);
    bool isInteger(std::string);
    bool isPeriod(std::string);

    bool isDeadlinedTask(std::string);
    bool isDate(std::string);
    bool isDay(std::string);
    bool isTime(std::string);

    bool isTimedTask(std::string);

    std::string parseFirstToken(std::string);
    std::string parseSecondToken(std::string);

    int parseTaskPositionNo(std::string);
    std::string parseSearchKey(std::string);
    std::string parseDirectory(std::string);

    std::string removeFirstToken(std::string,std::string);
    std::string removeFirstWord(std::string);
    std::string clearFirstWord(std::string);
    std::string clearFrontSpaces(std::string);

};
#endif