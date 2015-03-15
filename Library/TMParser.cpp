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

const std::string TOKEN_BEFORE = "before";
const std::string TOKEN_ON = "on";
const std::string TOKEN_NEXT = "next";
const std::string TOKEN_AT = "at";

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

//Preconditions: string parameter string
//Postconditions: returns vector of strings
std::vector<std::string> TMParser::getTokenizedUserEntry(std::string userEntry){
std::vector<std::string> tokenizedUserEntry;
    //to keep track of current position
    int positionOfFrontChar = 0;
    int positionOfBackChar = 0;
    std::string token;
    if(userEntry == "") {
        return tokenizedUserEntry;
    } else {

        positionOfFrontChar = userEntry.find_first_not_of(" ",positionOfFrontChar);

        if(positionOfFrontChar == std::string::npos){
            return tokenizedUserEntry;
        } else {

            while(positionOfFrontChar != std::string::npos) {
                if(userEntry[positionOfFrontChar] == '"'){
                    positionOfBackChar = userEntry.find_first_of("\"",positionOfFrontChar+1);
                    if(positionOfBackChar != std::string::npos) {
                        token = userEntry.substr(positionOfFrontChar+1,positionOfBackChar - positionOfFrontChar - 1);
                        tokenizedUserEntry.push_back(token);
                        positionOfBackChar++;
                    } else {
                        //print invalid;
                        //return empty vector;
                        cout << "close inverted commas missing\n";
                        return std::vector<std::string>();
                    }
                } else {
                    positionOfBackChar = userEntry.find_first_of(" ",positionOfFrontChar);
                    if(positionOfBackChar != std::string::npos) {
                        token = userEntry.substr(positionOfFrontChar,positionOfBackChar - positionOfFrontChar);
                        tokenizedUserEntry.push_back(token);
                    } else {
                        token = userEntry.substr(positionOfFrontChar);
                        tokenizedUserEntry.push_back(token);
                        break;
                    }
                }

                positionOfFrontChar = userEntry.find_first_not_of(" ",positionOfBackChar);
            }
        }
    }

    return tokenizedUserEntry;
}




//Preconditions: none (works for both with spaces and without in front)
std::string TMParser::extractCommand(std::string userEntry) {
    std::string command = "";
    userEntry = clearFrontSpaces(userEntry);
    if(userEntry != "") {
        int posOfFirstSpaceAfterCommand = userEntry.find(" ");
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
std::vector<TMTask> TMParser::parseTaskInfo(std::string taskInfo) {
    std::vector<TMTask> task;
    if(isDeadlinedTask(taskInfo)){
        std::cout << "IS DEADLINE\n";
        task.push_back(parseDeadlinedTaskInfo(taskInfo));
    } else if(isTimedTask(taskInfo)) {
        task.push_back(parseTimedTaskInfo(taskInfo));
    } else {
        task.push_back(parseFloatingTaskInfo(taskInfo));
    }

    return task;
}

//Preconditions:task is deadlined task use isDeadlinedTask to check
TMTask TMParser::parseDeadlinedTaskInfo(std::string taskInfo) {
    TaskType taskType = TaskType::WithDeadline;
    std::string dateToMeet = "";
    //std::string dayToMeet = "";
    std::string timeToMeet = "";
    std::string remainingEntry = taskInfo;
    std::string taskDescription = "";
    bool deadlineFound = false;

    int startOfTokenBefore = remainingEntry.find(TOKEN_BEFORE);
    std::cout << "position of start of word before: " << startOfTokenBefore << std::endl;
    while(startOfTokenBefore != std::string::npos && !deadlineFound) {
        std::string wordAfterTokenBefore = parseSecondToken(remainingEntry.substr(startOfTokenBefore));
        //
        std::cout << "word after word before: " << wordAfterTokenBefore << std::endl;
        int positionOfWordAfterBefore = remainingEntry.find(wordAfterTokenBefore,startOfTokenBefore);
        //
        std::cout << "position of start of word after before: " << positionOfWordAfterBefore << std::endl;
        if(positionOfWordAfterBefore != std::string::npos) {
            if(isDate(wordAfterTokenBefore)) {
                dateToMeet = wordAfterTokenBefore;
                deadlineFound = true;
            } else if (isDay(wordAfterTokenBefore)) {
                //need to convert to date
                //dayToMeet = wordAfterTokenBefore;
                //deadlineFound = true;
            } else if (isWordNext(wordAfterTokenBefore)) {
                if(numberOfWords(remainingEntry.substr(positionOfWordAfterBefore)) >= 2) {
                    if(isDay(parseNthToken(remainingEntry.substr(positionOfWordAfterBefore),2))) {
                        std::string stringDay = parseNthToken(remainingEntry.substr(positionOfWordAfterBefore),2);
                        boost::gregorian::date today = boost::gregorian::day_clock::local_day();
                        boost::gregorian::greg_weekday day(dayOfWeek(stringDay));
                        boost::gregorian::date dateTM = boost::gregorian::next_weekday(today, day);
                        std::string tempDate = boost::gregorian::to_iso_string(dateTM);
                        dateToMeet = tempDate.substr(6,2)
                                    + "-"
                                    + tempDate.substr(4,2)
                                    + "-"
                                    + tempDate.substr(0,4);
                        deadlineFound = true;
                    } else {
                        std::cout << "Invalid" << std::endl;
                    }
                }
            } else if (isTime(wordAfterTokenBefore)) {
                //
                std::cout << "IS TIME\n";
                timeToMeet = wordAfterTokenBefore;
                //check for word on
                deadlineFound = true;
            } else {
                startOfTokenBefore = remainingEntry.find(TOKEN_BEFORE, startOfTokenBefore + 1);
            }
        } else {
            break;
        }
    }

    int count = 0;
    if(dateToMeet != "") {
        count++;
    }
    if(timeToMeet != "") {
        count++;
    }
    std::string entryToRemove = dateToMeet + timeToMeet + TOKEN_BEFORE;
    int lengthOfBeforeAndInfo = entryToRemove.length() + count;

    remainingEntry.erase(startOfTokenBefore,lengthOfBeforeAndInfo);
    //
    std::cout << remainingEntry << std::endl;
    taskDescription = remainingEntry;
    //case 1: time but no date assume it is today if time hasn't passed or else tomorrow
    
    TMTaskTime taskTime(dateToMeet,timeToMeet,dateToMeet,timeToMeet);
    TMTask task(taskDescription,taskTime,taskType);

    return task;
}

TMTask TMParser::parseTimedTaskInfo(std::string taskInfo){
    TaskType taskType = TaskType::Timed;
    std::string startTime = "";
    //std::string dayToMeet = "";
    //missing endTime and endDate need to use function to determine period of time and dates
    std::string startDate = "";
    std::string remainingEntry = taskInfo;
    std::string taskDescription = "";
    bool timeExtracted = false;
    bool dateExtracted = false;

    int startOfTokenAt = remainingEntry.find(TOKEN_AT);
    int posOfWordAfterAt;
    while(startOfTokenAt != std::string::npos) {
        std::string wordAfterTokenAt = parseSecondToken(remainingEntry.substr(startOfTokenAt));
        posOfWordAfterAt = remainingEntry.find(wordAfterTokenAt,startOfTokenAt);

        if(posOfWordAfterAt != std::string::npos) {
            if(isTime(wordAfterTokenAt)){
                startTime = wordAfterTokenAt;
                timeExtracted = true;
            } else {
                startOfTokenAt = remainingEntry.find(TOKEN_AT,startOfTokenAt + 1);
            }
        }
    }

    if(timeExtracted){
        int noOfSpaces = 0;
        int lengthOfSubString;
        
        if(remainingEntry.find_first_of(" ",posOfWordAfterAt) != std::string::npos) {
            noOfSpaces = 2;
        } else {
            noOfSpaces = 1;
        }
        
        lengthOfSubString = TOKEN_AT.length() + startTime.length() + noOfSpaces; //length of 2 spaces
        remainingEntry.erase(startOfTokenAt,lengthOfSubString);
    }

    //after "at (time)" is removed from string
    int startOfTokenOn = remainingEntry.find(TOKEN_ON);
    int posOfWordAfterOn;
    while(startOfTokenOn != std::string::npos) {
        std::string wordAfterTokenOn = parseSecondToken(remainingEntry.substr(startOfTokenOn));
        posOfWordAfterOn = remainingEntry.find(wordAfterTokenOn,startOfTokenOn);

        if(posOfWordAfterOn != std::string::npos) {
            if(isDate(wordAfterTokenOn)){
                startDate = wordAfterTokenOn;
                dateExtracted = true;
            } else {
                startOfTokenOn = remainingEntry.find(TOKEN_ON,startOfTokenOn + 1);
            }
        }
    }

    if(dateExtracted){
        int noOfSpaces = 0;
        int lengthOfSubString;
        
        if(remainingEntry.find_first_of(" ",posOfWordAfterOn) != std::string::npos) {
            noOfSpaces = 2;
        } else {
            noOfSpaces = 1;
        }
        
        lengthOfSubString = TOKEN_ON.length() + startDate.length() + noOfSpaces; //length of 2 spaces
        remainingEntry.erase(startOfTokenOn,lengthOfSubString);
    }

    taskDescription = remainingEntry;
    TMTaskTime taskTime(startDate,startTime,startDate,startTime);
    TMTask task(taskDescription,taskTime,taskType);

    return task;
}

TMTask TMParser::parseFloatingTaskInfo(std::string taskInfo) {
    TaskType taskType = TaskType::Floating;
    TMTaskTime taskTime;
    TMTask task(taskInfo,taskTime,taskType);

    return task;
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
        } else if (is24HTime(wordAfterTokenBefore)) {//check 12HTime
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

bool TMParser::is12HTime(std::string timeToken){
    //format 2am 230pm 1230am
    //implement check for am or pm!
    int lengthOfTimeToken = timeToken.size();
    if(lengthOfTimeToken == 3) {
        int hour = std::stoi(timeToken.substr(0,1));
        if(hour >= 1 && hour <= 12) {
            return true;
        } else {
            return false;
        }
    } else if (lengthOfTimeToken == 5) {
        int hour = std::stoi(timeToken.substr(0,1));
        if(hour >= 1 && hour <= 12) {
            int minute = std::stoi(timeToken.substr(1,2));
            if(minute >= 0 && minute <= 59) {
                return true;
            } else {
                return false;
            }
        } else {
            return false;
        }
    } else if (lengthOfTimeToken == 6) {
        int hour = std::stoi(timeToken.substr(0,2));
        if(hour >= 1 && hour <= 12) {
            int minute = std::stoi(timeToken.substr(1,2));
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

bool TMParser::is24HTime(std::string timeToken) {
    //format: 10/ 1030/ 10:30
    int lengthOfTimeToken = timeToken.size();
    if(lengthOfTimeToken == 2){
        int hour = std::stoi(timeToken);
        if(hour >= 0 && hour <= 23) {
            return true;
        } else {
            return false;
        }
    } else if (lengthOfTimeToken = 4) {
        int hour = std::stoi(timeToken.substr(0,2));
        if(hour >= 0 && hour <= 23) {
            int minute = std::stoi(timeToken.substr(2,2));
            if(minute >= 0 && minute <= 59) {
                return true;
            } else {
                return false;
            }
        } else {
            return false;
        }
    } else if(lengthOfTimeToken = 5) {
        if(timeToken[2] == ':') {
            int hour = std::stoi(timeToken.substr(0,2));
            if(hour >= 0 && hour <= 23) {
                int minute = std::stoi(timeToken.substr(3,2));
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
    //
    std::cout << "remaining entry: " << remainingEntry << std::endl;
    int posOfFirstSpace = remainingEntry.find_first_of(" ",0);
    if(posOfFirstSpace != std::string::npos) {
        int posOfFirstCharOfSecondToken = remainingEntry.find_first_not_of(" ",posOfFirstSpace);
        if(posOfFirstCharOfSecondToken != std::string::npos) {
            int posOfSpaceAfterSecondToken = remainingEntry.find_first_of(" ",posOfFirstCharOfSecondToken);
            if (posOfSpaceAfterSecondToken != std::string::npos) {
                return remainingEntry.substr(posOfFirstCharOfSecondToken,posOfSpaceAfterSecondToken-posOfFirstCharOfSecondToken);
            } else {
                return remainingEntry.substr(posOfFirstCharOfSecondToken);
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

std::vector<int> TMParser::parseTaskPositionNo(std::string userEntry) {
    int intTaskPositionNo;
    std::vector<int> vectorTaskPositionNo;
    std::string taskPositionNo = extractEntryAfterCommand(userEntry);
    intTaskPositionNo = std::stoi(taskPositionNo);
    vectorTaskPositionNo.push_back(intTaskPositionNo);
    return vectorTaskPositionNo;
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

std::string TMParser::dateFromUserToBoostFormat(std::string stringDate) {
    std::string dd = "";
    std::string mm = "";
    std::string yyyy = "";
    std::string month = "";
    bool isNumeric = true;
    
    if(stringDate.length() == 5){
        // 1 mar
        //need to check more
        dd = stringDate.substr(0,1);
        month = stringDate.substr(2,3);
    } else if(stringDate.length() == 6){
        //DDMMYY
        for(std::string::iterator it = stringDate.begin(); it < stringDate.end() && isNumeric; it++) {
            if(!isdigit(*it)) {
                isNumeric = false;
            }
        }
        
        if(isNumeric) {
            //DDMMYY
            dd = stringDate.substr(0,2);
            mm = stringDate.substr(2,2);
            yyyy = "20" + stringDate.substr(4,2);
        } else {
            //assume dd month e.g. 16 mar
            dd = stringDate.substr(0,2);
            month = stringDate.substr(3,3);
        }

    } else if(stringDate.length() == 8) {
        //DDMMYYYY
        for(std::string::iterator it = stringDate.begin(); it < stringDate.end() && isNumeric; it++) {
            if(!isdigit(*it)) {
                isNumeric = false;
            }
        }

        if(isNumeric) {
            dd = stringDate.substr(0,2);
            mm = stringDate.substr(2,2);
            yyyy = stringDate.substr(4,4);
        }
    } else if(stringDate.length() == 10){
        //d month yyyy
        //need to check more
        dd = stringDate.substr(0,1);
        month = stringDate.substr(2,3);
        yyyy = stringDate.substr(8,4);
    } else if(stringDate.length() == 11){
        //dd month yyyy
        dd = stringDate.substr(0,2);
        month = stringDate.substr(3,3);
        yyyy = stringDate.substr(9,4);
    } else {
        //IMPORTANT CALLER MUST TEST DATE
        return "";
    }

    if(month == ""){
        return dd + "-" + mm + "-" + yyyy;
    } else {
        return dd + "-" + month + "-" + yyyy;
    }
}

std::string dateFromBoostToStandardFormat(const boost::gregorian::date& date) {
    std::ostringstream os;
    boost::gregorian::date_facet* facet(new boost::gregorian::date_facet("%A, %d %B %Y"));
    os.imbue(std::locale(std::cout.getloc(), facet));
    os << date;
    return os.str();
}

std::string TMParser::timeFrom12To24HFormat(std::string time){
    std::string hhmm;

    if(is24HTime(time)){
        if(time.length() == 2){
            hhmm = time + "00";
        } else if (time.length() == 4){
            hhmm = time;
        } else {
            hhmm = time.erase(2,1);
        }
    } else if(is12HTime(time)) {
        
