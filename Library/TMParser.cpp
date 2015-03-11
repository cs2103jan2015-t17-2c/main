#include "TMParser.h"
#include "TMTask.h"

const std::string CMD_ADD = "add";
const std::string CMD_DELETE = "delete";
const std::string CMD_UNDO = "undo";
const std::string CMD_COMPLETE = "complete";
const std::string CMD_INCOMPLETE = "incomplete";
const std::string CMD_SEARCH = "search";
const std::string CMD_EDIT = "edit";
const std::string CMD_STORE = "store";

const std::string DAY_MONDAY = "monday";
const std::string DAY_TUESDAY = "tuesday";
const std::string DAY_WEDNESDAY = "wednesday";
const std::string DAY_THURSDAY = "thursday";
const std::string DAY_FRIDAY = "friday";
const std::string DAY_SATURDAY = "saturday";
const std::string DAY_SUNDAY = "sunday";

const std::string DAY_MON = "mon";
const std::string DAY_TUE = "tue";
const std::string DAY_WED = "wed";
const std::string DAY_THU = "thu";
const std::string DAY_FRI = "fri";
const std::string DAY_SAT = "sat";
const std::string DAY_SUN = "sun";

const std::string TOKEN_EVERY = "every";
const std::string TOKEN_BEFORE = "before";
const std::string TOKEN_ON = "on";

const std::string PERIOD_HOUR = "hour";
const std::string PERIOD_HOURS = "hours";
const std::string PERIOD_DAY = "day";
const std::string PERIOD_DAYS = "days";
const std::string PERIOD_WEEK = "week";
const std::string PERIOD_WEEKS = "weeks";
const std::string PERIOD_MONTH = "month";
const std::string PERIOD_MONTHS = "months";
const std::string PERIOD_YEAR = "year";
const std::string PERIOD_YEARS = "years";

TMParser::TMParser() {
}

std::string TMParser::extractCommand(std::string userEntry) {
    std::string command;
    command = userEntry.substr(0,userEntry.find(' '));
    return command;
}

std::string TMParser::extractEntryAfterCommand(std::string userEntry) {
    std::string entryAfterCommand;
    //assume that the user would separate each word with only a space
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

}

bool TMParser::isRepeatedTask(std::string taskInfo) {
    std::string remainingEntry = taskInfo;
    int startOfTokenEvery = remainingEntry.find(TOKEN_EVERY);

    while(startOfTokenEvery != std::string::npos) {
        std::string wordAfterTokenEvery;
        remainingEntry = remainingEntry.substr(startOfTokenEvery);
        remainingEntry = removeFirstToken(remainingEntry,TOKEN_EVERY);
        wordAfterTokenEvery = parseFirstToken(remainingEntry);

        if(isInteger(wordAfterTokenEvery)) {
            std::string wordAfterTokenInteger = parseSecondToken(remainingEntry);
            if(isPeriod(wordAfterTokenInteger)) {
                return true;
            }
        } else if(isPeriod(wordAfterTokenEvery)) {
            return true;
        } else {
            startOfTokenEvery = remainingEntry.find(TOKEN_EVERY);
        }
    }

    return false;
}

bool TMParser::isInteger(std::string token) {
    for(std::string::iterator it = token.begin(); it < token.end(); it++) {
        if(!isdigit(*it)) {
            return false;
        }
    }
    return true;
}

bool TMParser::isPeriod(std::string token) {
    if(token == PERIOD_HOUR||
       token == PERIOD_HOURS||
       token == PERIOD_DAY||
       token == PERIOD_DAYS||
       token == PERIOD_WEEK||
       token == PERIOD_WEEKS||
       token == PERIOD_MONTH||
       token == PERIOD_MONTHS||
       token == PERIOD_YEAR||
       token == PERIOD_YEARS) {
           return true; } else {
               return false;}
}

std::string TMParser::parseFirstToken(std::string remainingEntry) {
    return remainingEntry.substr(0,remainingEntry.find_first_of(" ",0));
}

std::string TMParser::parseSecondToken(std::string remainingEntry) {
    int posOfFirstSpace = remainingEntry.find_first_of(" ",0);
    int posOfFirstCharOfSecondToken = remainingEntry.find_first_not_of(" ",posOfFirstSpace);
    int posOfSpaceAfterSecondToken = remainingEntry.find_first_of(" ",posOfFirstCharOfSecondToken);
    return remainingEntry.substr(posOfFirstCharOfSecondToken,posOfSpaceAfterSecondToken-posOfFirstCharOfSecondToken);
}

int TMParser::parseTaskPositionNo(std::string entryAfterCommand) {
    return std::stoi(entryAfterCommand);
}

std::string TMParser::parseSearchKey(std::string entryAfterCommand) {
    return entryAfterCommand;
}

std::string TMParser::parseDirectory(std::string entryAfterCommand) {
    return entryAfterCommand;
}

std::string TMParser::removeFirstToken(std::string remainingEntry, std::string token) {
    int tokenLength = token.size();
    return remainingEntry.substr(0,tokenLength);
}

std::string TMParser::removeFirstWord(std::string remainingEntry) {
    std::string entryAfterRemovalofFirstWord = clearFirstWord(remainingEntry);
    std::string entryAfterRemovalOfFrontSpaces = clearFrontSpaces(entryAfterRemovalofFirstWord);
    return entryAfterRemovalOfFrontSpaces;
    }

std::string TMParser::clearFirstWord(std::string remainingEntry) {
    int posOfFirstChar = remainingEntry.find_first_not_of(" ",0);
    int posOfSpaceAfterFirstChar = remainingEntry.find_first_of(" ",posOfFirstChar);
    return remainingEntry.substr(posOfFirstChar,posOfSpaceAfterFirstChar-posOfFirstChar);
}
 
std::string TMParser::clearFrontSpaces(std::string remainingEntry) {
    int posOfFirstChar = remainingEntry.find_first_not_of(" ",0);
    return remainingEntry.substr(0,posOfFirstChar);
}

