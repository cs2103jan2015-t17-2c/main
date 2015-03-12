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

const std::string DAY_YESTERDAY = "yesterday";
const std::string DAY_TODAY = "today";
const std::string DAY_TOMORROW = "tomorrow";
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
const std::string TOKEN_FOR = "for";
const std::string TOKEN_NEXT = "next";

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
    userEntry = clearFrontSpaces(userEntry);
    if(userEntry != "") {
        int posOfFirstSpaceAfterCommand = userEntry.find(' ');
        //case where there is more than one word
        if(posOfFirstSpaceAfterCommand != std::string::npos) {
            command = userEntry.substr(0,posOfFirstSpaceAfterCommand);
        } else { //case where there is only one word
            command = userEntry;
        }
    } else { //case where input is an empty string
        command = "";
    }

    return command;
}

//used on full entry in which command is within
std::string TMParser::extractEntryAfterCommand(std::string userEntry) {
    std::string entryAfterCommand;
    userEntry = clearFrontSpaces(userEntry);
    if(userEntry != "") {
        int posOfFirstSpace = userEntry.find_first_of(" ",0);
        if(posOfFirstSpace != std::string::npos) {
            int posOfFirstCharAfterFirstSpace = userEntry.find_first_not_of(" ",posOfFirstSpace);
            if(posOfFirstCharAfterFirstSpace != std::string::npos) {
                entryAfterCommand = userEntry.substr(posOfFirstCharAfterFirstSpace);
            } else {
                entryAfterCommand = "";
            }
        } else {
            entryAfterCommand = "";
        }
    } else {
        entryAfterCommand = "";
    }

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

//Preconditions: taskInfo contains only the entry after command. use extractEntryAfterCommand 1st
//             : use only when in adding or editing information
TMTask TMParser::parseTaskInfo(std::string taskInfo) {
    if(isDeadlinedTask(taskInfo)){
        return parseDeadlinedTaskInfo(taskInfo);
    } else if(isTimedTask(taskInfo)) {
        return parseTimedTaskInfo(taskInfo);
    } else {
        return parseFloatingTaskInfo(taskInfo);
    }
}

//Preconditions:task is deadlined task use isDeadlinedTask to check
TMTask TMParser::parseDeadlinedTaskInfo(std::string taskInfo) {
    TaskType taskType = TaskType::WithDeadline;
    std::string dateToMeet = "";
    std::string dayToMeet = "";
    std::string timeToMeet = "";
    std::string remainingEntry = taskInfo;
    bool deadlineFound = false;

    int startOfTokenBefore = remainingEntry.find(TOKEN_BEFORE);

    while(startOfTokenBefore != std::string::npos && !deadlineFound) {
        std::string wordAfterTokenBefore = parseSecondToken(remainingEntry.substr(startOfTokenBefore));
        
        if(isDate(wordAfterTokenBefore)) {
            dateToMeet = wordAfterTokenBefore;
            deadlineFound = true;
        } else if (isDay(wordAfterTokenBefore)) {
            //need to convert to date
            dayToMeet = wordAfterTokenBefore;
            deadlineFound = true;
        }  else if (isWordNext(wordAfterTokenBefore)) {
           if(numberOfWords(remainingEntry.substr(startOfTokenBefore)) >= 2) {
               if(isDay(parseNthToken(remainingEntry.substr(startOfTokenBefore),2))) {
                   std::string day = parseNthToken(remainingEntry.substr(startOfTokenBefore),2);
                   boost::gregorian::date today = boost::gregorian::day_clock::local_day();
                   boost::gregorian::greg_weekday day(dayOfWeek(day));
                   boost::gregorian::date dateTM = boost::gregorian::next_weekday(today, day);
                   date 
        }  else if (isTime(wordAfterTokenBefore)) {
            timeToMeet = wordAfterTokenBefore;
            deadlineFound = true;
        } else {
            startOfTokenBefore = remainingEntry.find(TOKEN_BEFORE,startOfTokenBefore+1);
        }
        }
    }
    }
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
        } else if (isTime(wordAfterTokenBefore)) {
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
    } else if(lengthOfToken = 5) {
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
    } else {
        return false;
    }
}

bool TMParser::isWordNext(std::string token) {
    return token == TOKEN_NEXT;
}

std::string TMParser::parseFirstToken(std::string remainingEntry) {
    if(remainingEntry != "") {
        int posOfFirstSpace = remainingEntry.find_first_of(" ",0);
        if(posOfFirstSpace != std::string::npos) {
            return remainingEntry.substr(0,posOfFirstSpace);
        } else {
            return remainingEntry;
        }
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

//Precondition: n <= numberOfWords
std::string TMParser::parseNthToken(std::string remainingEntry, int n) {
    if(remainingEntry != "") {
        std::string token;
        std::istringstream in(remainingEntry);
        for(int i = 0; i < n; i++) {
            in >> token;
        }
        return token;
    }
}

int TMParser::numberOfWords(std::string remainingEntry) {
    int count = 0;
    if(remainingEntry != ""){
        std::string token;
        std::istringstream in(remainingEntry);
        while(in >> token) {
            count++;
        }
    }
    return count;
}

int TMParser::dayOfWeek(std::string day) {
    if(day == DAY_SUN||day == DAY_SUNDAY) {
        return 0;
    } else if (day == DAY_MON||day == DAY_MONDAY) {
        return 1;
    } else if (day == DAY_TUE||day == DAY_TUESDAY) {
        return 2;
    } else if (day == DAY_WED||day == DAY_WEDNESDAY) {
        return 3;
    } else if (day == DAY_THU||day == DAY_THURSDAY) {
        return 4;
    } else if (day == DAY_FRI||day == DAY_FRIDAY) {
        return 5;
    } else if (day == DAY_SAT||day == DAY_SATURDAY) {
        return 6;
    }
}

int TMParser::parseTaskPositionNo(std::string userEntry) {
    std::string taskPositionNo = extractEntryAfterCommand(userEntry);
    return std::stoi(taskPositionNo);
}

std::string TMParser::parseSearchKey(std::string userEntry) {
    std::string searchKey = extractEntryAfterCommand(userEntry);
    return searchKey;
}

std::string TMParser::parseDirectory(std::string userEntry) {
    std::string directory = extractEntryAfterCommand(userEntry);
    return directory;
}

//preconditions: none
//for situation where there is everyday so we won't clear everyday but just every
std::string TMParser::removeFirstToken(std::string remainingEntry, std::string token) {
    int tokenLength = token.size();
    remainingEntry = remainingEntry.substr(tokenLength);
    if(remainingEntry != "") {
        //skip front spaces
        int posOfFirstChar = remainingEntry.find_first_not_of(" ",0);
        if(posOfFirstChar != std::string::npos) {
            return remainingEntry.substr(posOfFirstChar);
        } else {
            return "";
        }
    } else {
        return "";
    }
}

std::string TMParser::removeFirstWord(std::string remainingEntry) {
    std::string entryAfterRemovalOfFrontSpaces = clearFrontSpaces(remainingEntry);
    std::string entryAfterRemovalofFirstWord = clearFirstWord(entryAfterRemovalOfFrontSpaces);
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

//Preconditions: there are no spaces at the front
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
    if(posOfFirstChar != std::string::npos) { //case where there is a char 
        return remainingEntry.substr(posOfFirstChar);
    } else { //case where all spaces
        return "";
    }
}

