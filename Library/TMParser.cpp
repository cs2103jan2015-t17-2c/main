#include "TMParser.h"

const std::string CMD_ADD = "add";
const std::string CMD_DELETE = "delete";
const std::string CMD_UNDO = "undo";
const std::string CMD_COMPLETE = "complete";
const std::string CMD_INCOMPLETE = "incomplete";
const std::string CMD_SEARCH = "search";
const std::string CMD_EDIT = "edit";
const std::string CMD_STORE = "store";

TMParser::TMParser() {
}

std::string TMParser::extractCommand(std::string userEntry) {
    std::string command;
    command = userEntry.substr(0,userEntry.find(' ');
    return command;
}

std::string TMParser::extractEntryAfterCommand(std::string userEntry) {
    std::string entryAfterCommand;
    entryAfterCommand = userEntry.substr(userEntry.find_first_of(" ") + 1);
    return entryAfterCommand;
}

TMParser::CommandTypes TMParser::determineCommandType(std::string command) {
    if(command == CMD_ADD) {
        return CommandTypes::Add;
    } else if (command == CMD_DELETE) {
        return CommandTypes::Delete;
    } else if (command == CMD_UNDO) {
        return CommandTypes::Undo;
    } else if (command == CMD_COMPLETE) {
        return CommandTypes::Complete;
    } else if (command == CMD_INCOMPLETE) {
        return CommandTypes::Incomplete;
    } else if (command == CMD_SEARCH) {
        return CommandTypes::Search;
    } else if (command == CMD_EDIT) {
        return CommandTypes::Edit;
    } else if (command == CMD_STORE) {
        return CommandTypes::Store;
    } else {
        return CommandTypes::Invalid;
    }
}

TMTask TMParser::parseTaskInfo(std::string taskInfo) {
    int startOfTokenAt;
    std::string taskDescription;
    std::string entryAfterTaskDescription;
    std::string remainingEntry;
    bool isTimedTask = false;
    bool isWithDeadlineTask = false;
    bool isFloatingTask = false;
    bool isRepeated = false;

    remainingEntry = taskInfo;
    //search for repeat tasks

    int startOfTokenEvery;
    int startOfTokenTimePeriodWithLy;

    startOfTokenAt = remainingEntry.find("at");
    //should check whether the syntax before and after at is right
    while(startOfTokenAt != std::string::npos) {
        if(remainingEntry[startOfTokenAt-1]==' ' && remainingEntry[startOfTokenAt+2]==' ') {
            taskDescription = remainingEntry.substr(0,startOfTokenAt-1);
            isTimedTask = true;
            break;
        } else {
            remainingEntry = remainingEntry.substr(startOfTokenAt+2);
            startOfTokenAt = remainingEntry.find("at");
        }
    }

    if(startOfTokenAt == std::string::npos) {
        taskDescription = taskInfo;
    }

    

    TMTimeParser timeParser;
    std::string startTime = timeParser.extractStartTime();
    std::string endTime = timeParser.extractEndTime();
    std::string startDate = timeParser.extractStartDate();
    std::string endDate = timeParser.extractEndDate();


