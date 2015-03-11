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

};
#endif