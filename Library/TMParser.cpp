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

bool TMParser::isDeadlinedTask(std::string taskInfo) {
    std::string remainingEntry = taskInfo;
    int startOfTokenBefore = remainingEntry.find(TOKEN_BEFORE);

    while(startOfTokenBefore != std::string::npos) {
        std::string wordAfterTokenBefore;
        remainingEntry = remainingEntry.substr(startOfTokenBefore);
        remainingEntry = removeFirstToken(remainingEntry,TOKEN_BEFORE);
        wordAfterTokenBefore = parseFirstToken(remainingEntry);

        if(isDate(wordAfterTokenBefore)||isDay(wordAfterTokenBefore)) {
            return true;
        } else if(isTime(wordAfterTokenBefore)) {
            return true;
        } else {
            startOfTokenBefore = remainingEntry.find(TOKEN_BEFORE);
        }
    }

    return false;
}

bool TMParser::isTimedTask(std::string taskInfo) {
    std::string remainingEntry = taskInfo;
    int startOfTokenOn = remainingEntry.find(TOKEN_ON);

    while(startOfTokenOn != std::string::npos) {
        std::string wordAfterTokenOn;
        remainingEntry = remainingEntry.substr(startOfTokenOn);
        remainingEntry = removeFirstToken(remainingEntry,TOKEN_ON);
        wordAfterTokenOn = parseFirstToken(remainingEntry);

        if(isDay(wordAfterTokenOn)||isDate(wordAfterTokenOn)) {
            return true;
        } else {
            startOfTokenOn = remainingEntry.find(TOKEN_ON);
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

bool TMParser::isDate(std::string token) {
    //DDMMYYYY
    //need more rigorous testing of dates
    int lengthOfToken = token.size();
    if(lengthOfToken != 8) {
        return false;
    } else {
        for(std::string::iterator it = token.begin(); it < token.end(); it++) {
            if(!isdigit(*it)) {
                return false;
            }
        }
    }
    return true;
}

bool TMParser::isDay(std::string token) {
    if(token == DAY_MON||
       token == DAY_MONDAY||
       token == DAY_TUE||
       token == DAY_TUESDAY||
       token == DAY_WED||
       token == DAY_WEDNESDAY||
       token == DAY_THU||
       token == DAY_THURSDAY||
       token == DAY_FRI||
       token == DAY_FRIDAY||
       token == DAY_SAT||
       token == DAY_SATURDAY||
       token == DAY_SUN||
       token== DAY_SUNDAY) {
           return true;
    } else {
        return false;
    }
}

bool TMParser::isTime(std::string token) {
    //format: 10/ 1030/ 10:30
    int lengthOfToken = token.size();
    if(lengthOfToken == 2){
        int hour = std::stoi(token);
        if(hour >= 0 && hour <= 23) {
            return true;
        } else {
            return false;
        }
    } else if (lengthOfToken = 4) {
        int hour = std::stoi(token.substr(0,2));
        if(hour >= 0 && hour <= 23) {
            int minute = std::stoi(token.substr(2,2));
            if(minute >= 0 && minute <= 59) {
                return true;
            } else {
                return false;
            }
        } else {
            return false;
        }
    } else if (lengthOfToken = 5) {
        if(token[2] == ':') {
            int hour = std::stoi(token.substr(0,2));
            if(hour >= 0 && hour <= 23) {
                int minute = std::stoi(token.substr(3,2));
                if(minute >= 0 && minute <= 59) {
                    return true;
                } else {
                    return false;
                }
            } else {
                return false;
            }
        } else {
            return false;
        }
    }
}

std::string TMParser::parseFirstToken(std::string remainingEntry) {
    int posOfFirstSpace = remainingEntry.find_first_of(" ",0);
    if(posOfFirstSpace != std::string::npos) {
        return remainingEntry.substr(0,remainingEntry.find_first_of(" ",0));
    } else {
        return "";
    }
}

std::string TMParser::parseSecondToken(std::string remainingEntry) {
    int posOfFirstSpace = remainingEntry.find_first_of(" ",0);
    if(posOfFirstSpace != std::string::npos) {
        int posOfFirstCharOfSecondToken = remainingEntry.find_first_not_of(" ",posOfFirstSpace);
        if(posOfFirstCharOfSecondToken != std::string::npos) {
            int posOfSpaceAfterSecondToken = remainingEntry.find_first_of(" ",posOfFirstCharOfSecondToken);
            if (posOfSpaceAfterSecondToken != std::string::npos) {
                return remainingEntry.substr(posOfFirstCharOfSecondToken,posOfSpaceAfterSecondToken-posOfFirstCharOfSecondToken);
            } else {
                return "";
            }
        } else {
            return "";
        }
    } else {
        return "";
    }
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
    remainingEntry = remainingEntry.substr(tokenLength);
    if(remainingEntry != "") {
        int posOfFirstChar = remainingEntry.find_first_not_of(" ",0);
        return remainingEntry.substr(posOfFirstChar);
    } else {
        return "";
    }
}

std::string TMParser::removeFirstWord(std::string remainingEntry) {
    std::string entryAfterRemovalofFirstWord = clearFirstWord(remainingEntry);
    if(entryAfterRemovalofFirstWord != "") {
        std::string entryAfterRemovalOfFrontSpaces = clearFrontSpaces(entryAfterRemovalofFirstWord);
        if(entryAfterRemovalOfFrontSpaces != "") {
            return entryAfterRemovalOfFrontSpaces;
        } else {
            return "";
        }
    } else {
        return "";
    }
}

std::string TMParser::clearFirstWord(std::string remainingEntry) {
    if(remainingEntry != "") {
        int posOfFirstChar = remainingEntry.find_first_not_of(" ",0);
        if(posOfFirstChar != std::string::npos) {
            int posOfSpaceAfterFirstWord = remainingEntry.find_first_of(" ",posOfFirstChar);
            if(posOfSpaceAfterFirstWord != std::string::npos) {
                return remainingEntry.substr(posOfSpaceAfterFirstWord);
            } else {
                return "";
            }
        } else {
            return "";
        }
    } else {
        return "";
    }
}
 
std::string TMParser::clearFrontSpaces(std::string remainingEntry) {
    int posOfFirstChar = remainingEntry.find_first_not_of(" ",0);
    if(posOfFirstChar != std::string::npos) {
        return remainingEntry.substr(posOfFirstChar);
    } else {
        return "";
    }
}

