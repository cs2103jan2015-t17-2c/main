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
    bool isTime(std::string);
    bool isWordNext(std::string);

    std::string parseFirstToken(std::string);
    std::string parseSecondToken(std::string);
    std::string parseNthToken(std::string, int);
    int numberOfWords(std::string);
    //0 for Sunday and 6 for Saturday
    int dayOfWeek(std::string); 

    int parseTaskPositionNo(std::string);
    std::string parseSearchKey(std::string);
    std::string parseDirectory(std::string);

    std::string removeFirstToken(std::string,std::string);
    std::string removeFirstWord(std::string);
    std::string clearFirstWord(std::string);
    std::string clearFrontSpaces(std::string);

};
#endif