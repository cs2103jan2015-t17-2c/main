#include "TMParser.h"
#include "TMTask.h"

const std::string CMD_ADD = "add";
const std::string CMD_DELETE = "delete";
const std::string CMD_UNDO = "undo";
const std::string CMD_REDO = "redo";
const std::string CMD_COMPLETE = "complete";
const std::string CMD_INCOMPLETE = "incomplete";
const std::string CMD_SEARCH = "search";
const std::string CMD_EDIT = "edit";
const std::string CMD_STORE = "store";
const std::string CMD_DONEALL = "doneall";
const std::string CMD_BLOCK = "block";
const std::string CMD_CONFIRM = "confirm";

const std::string CMD_SHORTCUT_ADD = "a";
const std::string CMD_SHORTCUT_DELETE = "d";
const std::string CMD_SHORTCUT_UNDO = "u";
const std::string CMD_SHORTCUT_REDO = "r";
const std::string CMD_SHORTCUT_COMPLETE = "c";
const std::string CMD_SHORTCUT_INCOMPLETE = "i";
const std::string CMD_SHORTCUT_SEARCH = "se";
const std::string CMD_SHORTCUT_EDIT= "e";
const std::string CMD_SHORTCUT_STORE = "st";
const std::string CMD_SHORTCUT_DONEALL = "da";

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
const std::string TOKEN_BY = "by";
const std::string TOKEN_ON = "on";
const std::string TOKEN_NEXT = "next";
const std::string TOKEN_AT = "at";
const std::string TOKEN_FROM = "from";
const std::string TOKEN_TO = "to";

/* in 3 hours, etc. for deadlined task
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
*/

TMParser* TMParser::theOne;

TMParser::TMParser() {
    _dateToday = boost::gregorian::day_clock::local_day();
}

TMParser* TMParser::getInstance() {
	if (theOne == NULL) {
		theOne = new TMParser();
	}
	return theOne;
}

void TMParser::initialize(std::string userEntry) {
	_originalUserInput = userEntry;
	_tokenizedUserEntry = getTokenizedUserEntry(_originalUserInput);
}

//Preconditions: tokens of string delimited by spaces
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
                        token = userEntry.substr(positionOfFrontChar,positionOfBackChar - positionOfFrontChar + 1);
                        tokenizedUserEntry.push_back(token);
                        positionOfBackChar++;
                    } else {
                        //close inverted commas missing treat as normal character
                        token = userEntry[positionOfFrontChar];
                        tokenizedUserEntry.push_back(token);
                        positionOfBackChar = positionOfFrontChar + 1;
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

//Preconditions: getTokenizedUserEntry(userEntry) first
//Postconditions: first token is extracted from vector of string and is no longer there
std::string TMParser::extractCommand() {
    if(_tokenizedUserEntry.size() == 0){
        return "";
    }
	std::string  command = _tokenizedUserEntry[0];
    _tokenizedUserEntry.erase(_tokenizedUserEntry.begin());

    return command;
}

std::vector<std::string> TMParser::returnTokens() {
	return _tokenizedUserEntry;
}

TMParser::CommandTypes TMParser::determineCommandType(std::string command) {
    
	command = returnLowerCase(command);
    if(command == CMD_ADD||command == CMD_SHORTCUT_ADD) {
        return CommandTypes::Add;
    } else if (command == CMD_DELETE||command == CMD_SHORTCUT_DELETE) {
        return CommandTypes::Delete;
    } else if (command == CMD_UNDO||command == CMD_SHORTCUT_UNDO) {
		return CommandTypes::Undo;
	}else if (command == CMD_REDO||command == CMD_SHORTCUT_REDO) {
        return CommandTypes::Redo;
    } else if (command == CMD_COMPLETE||command == CMD_SHORTCUT_COMPLETE) {
        return CommandTypes::Complete;
    } else if (command == CMD_INCOMPLETE||command == CMD_SHORTCUT_INCOMPLETE) {
        return CommandTypes::Incomplete;
    } else if (command == CMD_SEARCH||command == CMD_SHORTCUT_SEARCH) {
        return CommandTypes::SearchKeyword;
    } else if (command == CMD_EDIT||command == CMD_SHORTCUT_EDIT) {
        return CommandTypes::Edit;
    } else if (command == CMD_STORE||command == CMD_SHORTCUT_STORE) {
        return CommandTypes::SaveAt;
	} else if (command == CMD_DONEALL||command == CMD_SHORTCUT_DONEALL) {
        return CommandTypes::CompleteAllToday;
    } else if (command == CMD_BLOCK) {
        return CommandTypes::Block;
	} else if (command == CMD_CONFIRM) {
        return CommandTypes::Confirm; 
	} else {
        return CommandTypes::Invalid;
    }
}

//Preconditions: taskInfo contains only the entry after command. use extractEntryAfterCommand 1st
//             : use only when in adding or editing information
TMTask TMParser::parseTaskInfo() {
    if(isDeadlinedTask()){
        return parseDeadlinedTaskInfo();
    } else if(isTimedTask()) {
        return parseTimedTaskInfo();
    } else {
        return parseUndatedTaskInfo();
    }
}

//Preconditions:task is deadlined task use isDeadlinedTask to check
TMTask TMParser::parseDeadlinedTaskInfo() {
    std::queue<int> indexOfDatesAndTimes;
    TaskType taskType = TaskType::WithEndDateTime;
    std::string dateToMeet = "";
    std::string timeToMeet = "";
    std::string taskDescription = "";
    int lengthOfTokenizedUserEntry = _tokenizedUserEntry.size();

    std::string unitString;
    std::string nextWord;

    for(int index = 0; index < lengthOfTokenizedUserEntry; index++){

        if(index + 1 == lengthOfTokenizedUserEntry){
            break;
        }

        unitString = returnLowerCase(_tokenizedUserEntry[index]);
        nextWord = returnLowerCase(_tokenizedUserEntry[index + 1]);

        if(unitString == TOKEN_BEFORE||unitString == TOKEN_BY){
            if(isNumericDate(nextWord)||isDay(nextWord)||isDDMonDate(nextWord)) {
                //e.g. before 01012016 (DDMMYYYY)
                indexOfDatesAndTimes.push(index);
                dateToMeet = extractDayOrNumericDateOrDDMonDate(index + 1, indexOfDatesAndTimes);
                dateToMeet = dateFromNumericToBoostFormat(dateToMeet);
                dateToMeet = substractNDaysFromDate(dateToMeet,1);
                timeToMeet = "2359";
                index = indexOfDatesAndTimes.back();
            } else if (isNextDay(index + 1)) {
                //e.g. before next monday
                indexOfDatesAndTimes.push(index);
                dateToMeet = extractNextDay(index + 1, indexOfDatesAndTimes);
                dateToMeet = dateFromNumericToBoostFormat(dateToMeet);
                dateToMeet = substractNDaysFromDate(dateToMeet,1);
                timeToMeet = "2359";
                index = indexOfDatesAndTimes.back();
            } else if (is12HTime(nextWord)||is24HTime(nextWord)) {
                indexOfDatesAndTimes.push(index);
                timeToMeet = extractTime(index + 1, indexOfDatesAndTimes);
                index = indexOfDatesAndTimes.back();
            } else {
                if(nextWord[0] == '"' && nextWord[nextWord.length()-1] == '"' && nextWord.length() != 1){
                    if(numberOfWordsInQuote(nextWord) == 1){
                        nextWord = nextWord.substr(1,nextWord.length()-2);
                        if(is12HTime(nextWord)||is24HTime(nextWord)||
                           isNumericDate(nextWord)||isDay(nextWord)||
                           isDDMonDate(nextWord)){
                               //what about for next day?
                               std::string nextWordOriginal = _tokenizedUserEntry[index + 1];
                               _tokenizedUserEntry[index + 1] = nextWordOriginal.substr(1,nextWordOriginal.length()-2);
                        }
                    }
                }
            }
        } else if(unitString == TOKEN_ON) {
            if(isNumericDate(nextWord)||isDay(nextWord)||isDDMonDate(nextWord)) {
                indexOfDatesAndTimes.push(index);
                dateToMeet = extractDayOrNumericDateOrDDMonDate(index + 1,indexOfDatesAndTimes);
                index = indexOfDatesAndTimes.back();

                if(timeToMeet == "") {
                    timeToMeet = "2359";
                }
            } else {
                if(nextWord[0] == '"' && nextWord[nextWord.length()-1] == '"' && nextWord.length() != 1){
                    if(numberOfWordsInQuote(nextWord) == 1){
                        nextWord = nextWord.substr(1,nextWord.length()-2);
                        if(isNumericDate(nextWord)||isDay(nextWord)||isDDMonDate(nextWord)){
                               //what about for next day?
                            std::string nextWordOriginal = _tokenizedUserEntry[index + 1];
                            _tokenizedUserEntry[index + 1] = nextWordOriginal.substr(1,nextWordOriginal.length()-2);
                        }
                    }
                }
            }
        } else if(isNextDay(index)){
            dateToMeet = extractNextDay(index, indexOfDatesAndTimes);
            index = indexOfDatesAndTimes.back();
        }
    }

    for(int i = 0; i < lengthOfTokenizedUserEntry; i++) {
        int frontIndexOfQueue = -1; //will be -1 (invalid index) if index is empty 
        if(!indexOfDatesAndTimes.empty()){
            frontIndexOfQueue = indexOfDatesAndTimes.front();
        }

        if(i == frontIndexOfQueue){
            indexOfDatesAndTimes.pop();
        } else {
            std::string token = _tokenizedUserEntry[i];
            /*
            if(token[0] == '"' && token[token.length()-1] == '"' && token.length() != 1){
                if(numberOfWordsInQuote(token) == 1){
                    token = token.substr(1,token.length()-2);
                }
            }
            */
            taskDescription += token;
            if(i != lengthOfTokenizedUserEntry - 1) {
                taskDescription += " ";
            }
        }
    }

    if(dateToMeet == ""){
        std::string currentTime = getCurrentTime();
        if(timeToMeet >= currentTime){
            dateToMeet = dateFromBoostToDDMMYYYY(_dateToday);
        } else {
            dateToMeet = dateFromBoostToDelimitedDDMMYYYY(_dateToday);
            dateToMeet = addNDaysFromDate(dateToMeet,1);
        }
    }

    dateToMeet = dateFromNumericToBoostFormat(dateToMeet);

    if(isValidDate(dateToMeet)){
        TMTaskTime taskTime(dateToMeet, timeToMeet, dateToMeet, timeToMeet);
        TMTask task(taskDescription, taskTime, taskType);
        return task;
    } else {
        //when date is invalid
        TMTaskTime taskTime;
        TMTask task("",taskTime,TaskType::Invalid);
        return task;
    }
}

TMTask TMParser::parseTimedTaskInfo(){
    TaskType taskType;
    std::queue<int> indexOfDatesAndTimes;
    std::queue<int> mainIndexOfDatesAndTimes;
    std::vector<std::string>::iterator iter;
    std::string startTime = "";
    std::string startDate = "";
    std::string endTime = "";
    std::string endDate = "";
    std::string taskDescription = "";
    std::string unitString;
    int lengthOfTokenizedUserEntry = _tokenizedUserEntry.size();
    
    bool startTimeExtracted = false;
    bool startDateExtracted = false;
    bool endTimeExtracted = false;
    bool endDateExtracted = false;

    //will not check last string. last string treated as task desc
    
    for(int index = 0; index < lengthOfTokenizedUserEntry; index++){

        if(index + 1 == lengthOfTokenizedUserEntry){
            break;
        }

        unitString = returnLowerCase(_tokenizedUserEntry[index]);

        if(unitString == TOKEN_AT) {
            startTime = extractTime(index + 1, indexOfDatesAndTimes);
            if(startTime != ""){
                startTimeExtracted = true;
                mainIndexOfDatesAndTimes.push(index);
                while(!indexOfDatesAndTimes.empty()){
                    mainIndexOfDatesAndTimes.push(indexOfDatesAndTimes.front());
                    indexOfDatesAndTimes.pop();
                }
                index = mainIndexOfDatesAndTimes.back();
            } else {
                std::string nextWordOriginal = _tokenizedUserEntry[index + 1];
                std::string nextWord = returnLowerCase(nextWordOriginal);
                if(nextWord[0] == '"' && nextWord[nextWord.length()-1] == '"' && nextWord.length() != 1){
                    if(numberOfWordsInQuote(nextWord) == 1){
                        nextWord = nextWord.substr(1,nextWord.length()-2);
                        if(is12HTime(nextWord)||is24HTime(nextWord)){
                               //what about for next day?
                               _tokenizedUserEntry[index + 1] = nextWordOriginal.substr(1,nextWordOriginal.length()-2);
                        }
                    }
                }
            }
        } else if(unitString == TOKEN_ON) {
            startDate = extractDayOrNumericDateOrDDMonDate(index + 1, indexOfDatesAndTimes);
            if(startDate != ""){
                startDateExtracted = true;
                mainIndexOfDatesAndTimes.push(index);
                while(!indexOfDatesAndTimes.empty()){
                    mainIndexOfDatesAndTimes.push(indexOfDatesAndTimes.front());
                    indexOfDatesAndTimes.pop();
                }
                index = mainIndexOfDatesAndTimes.back();
            } else {
                std::string nextWordOriginal = _tokenizedUserEntry[index + 1];
                std::string nextWord = returnLowerCase(nextWordOriginal);
                if(nextWord[0] == '"' && nextWord[nextWord.length()-1] == '"' && nextWord.length() != 1){
                    if(numberOfWordsInQuote(nextWord) == 1){
                        nextWord = nextWord.substr(1,nextWord.length()-2);
                        if(isNumericDate(nextWord)||isDay(nextWord)||isDDMonDate(nextWord)){
                               //what about for next day?
                            _tokenizedUserEntry[index + 1] = nextWordOriginal.substr(1,nextWordOriginal.length()-2);
                        }
                    }
                }
            }
        } else if(unitString == TOKEN_FROM){
            std::string extractedDate;
            std::string extractedTime;
            extractDateAndOrTime(index + 1, indexOfDatesAndTimes, extractedDate, extractedTime);
            if((extractedDate != "")||(extractedTime != "")){
                if(extractedDate != ""){
                    startDate = extractedDate;
                    startDateExtracted = true;
                }
                if(extractedTime != ""){
                    startTime = extractedTime;
                    startTimeExtracted = true;
                }
                mainIndexOfDatesAndTimes.push(index);
                while(!indexOfDatesAndTimes.empty()){
                    mainIndexOfDatesAndTimes.push(indexOfDatesAndTimes.front());
                    indexOfDatesAndTimes.pop();
                }
                index = mainIndexOfDatesAndTimes.back();
            } else {
                std::string nextWordOriginal = _tokenizedUserEntry[index + 1];
                std::string nextWord = returnLowerCase(nextWordOriginal);
                if(nextWord[0] == '"' && nextWord[nextWord.length()-1] == '"' && nextWord.length() != 1){
                    if(numberOfWordsInQuote(nextWord) == 1){
                        nextWord = nextWord.substr(1,nextWord.length()-2);
                        if(is12HTime(nextWord)||is24HTime(nextWord)||
                           isNumericDate(nextWord)||isDay(nextWord)||
                           isDDMonDate(nextWord)){
                               //what about for next day?
                               _tokenizedUserEntry[index + 1] = nextWordOriginal.substr(1,nextWordOriginal.length()-2);
                               index = index + 1;
                        }
                    }
                }
            }
        } else if (unitString == TOKEN_TO){
            std::string extractedDate;
            std::string extractedTime;
            extractDateAndOrTime(index + 1, indexOfDatesAndTimes, extractedDate, extractedTime);
            if((extractedDate != "")||(extractedTime != "")){
                if(extractedDate != ""){
                    endDate = extractedDate;
                    endDateExtracted = true;
                }
                if(extractedTime != ""){
                    endTime = extractedTime;
                    endTimeExtracted = true;
                }
                    
                mainIndexOfDatesAndTimes.push(index);
                while(!indexOfDatesAndTimes.empty()){
                    mainIndexOfDatesAndTimes.push(indexOfDatesAndTimes.front());
                    indexOfDatesAndTimes.pop();
                }
                index = mainIndexOfDatesAndTimes.back();
            } else {
                std::string nextWordOriginal = _tokenizedUserEntry[index + 1];
                std::string nextWord = returnLowerCase(nextWordOriginal);
                if(nextWord[0] == '"' && nextWord[nextWord.length()-1] == '"' && nextWord.length() != 1){
                    if(numberOfWordsInQuote(nextWord) == 1){
                        nextWord = nextWord.substr(1,nextWord.length()-2);
                        if(is12HTime(nextWord)||is24HTime(nextWord)||
                           isNumericDate(nextWord)||isDay(nextWord)||
                           isDDMonDate(nextWord)){
                               //what about for next day?
                               _tokenizedUserEntry[index + 1] = nextWordOriginal.substr(1,nextWordOriginal.length()-2);
                               index = index + 1;
                        }
                    }
                }
            }
        } else if(isNextDay(index)) {
            startDate = extractNextDay(index, indexOfDatesAndTimes);
            startDateExtracted = true;

            while(!indexOfDatesAndTimes.empty()){
                mainIndexOfDatesAndTimes.push(indexOfDatesAndTimes.front());
                indexOfDatesAndTimes.pop();
            }
            index = mainIndexOfDatesAndTimes.back();
        } else {
            //check if next day is encompassed by open close inverted commas
            //cannot find any markers
        }
    }
    //if obtained only time then check if time passed...
    //if only date obtained then set as 0000

    if((startTimeExtracted||startDateExtracted) && !endTimeExtracted && !endDateExtracted){
        taskType = TaskType::WithStartDateTime;
        if(!startTimeExtracted){
            startTime = "0000";
        } else if(!startDateExtracted){
            std::string currentTime = getCurrentTime();
            if(startTime >= currentTime){
                boost::gregorian::date currentDate = _dateToday;
                startDate = dateFromBoostToDDMMYYYY(currentDate);
            } else {
                boost::gregorian::date currentDate = _dateToday;
                 startDate = dateFromBoostToDelimitedDDMMYYYY(currentDate);
                 startDate = addNDaysFromDate(startDate,1);
             }
        } else {
        //startTime and startDate are found
        }
        endDate = startDate;
        endTime = startTime;

    } else if((endTimeExtracted||endDateExtracted) && !startTimeExtracted && !startDateExtracted) {
        taskType = TaskType::WithEndDateTime;
        if(!endTimeExtracted){
            endTime = "0000";
        } else if(!endDateExtracted){
            std::string currentTime = getCurrentTime();
            if(endTime >= currentTime){
                boost::gregorian::date currentDate = _dateToday;
                endDate = dateFromBoostToDDMMYYYY(currentDate);
            } else {
                boost::gregorian::date currentDate = _dateToday;
                 endDate = dateFromBoostToDelimitedDDMMYYYY(currentDate);
                 endDate = addNDaysFromDate(startDate,1);
             }
        } else {
        //endTime and endDate are found
        }
        //no startDateTime, assumed to be current date and atime
        startDate = dateFromBoostToDDMMYYYY(_dateToday);
        startTime = getCurrentTime();

    } else if((startDateExtracted||startTimeExtracted) && (endDateExtracted||endTimeExtracted)){
        taskType = TaskType::WithPeriod;
        if(!startTimeExtracted){
            //there's startDate no startTime
            startTime = "0000";
            //check if there's no endDate but there's endTime then endDate will be startDate
            //else there is no endTime but there's endDate then endTime will be 2359
            if(!endDateExtracted){
                //if endTime = 0000 plus 1 more day?
                endDate = startDate;
            } else if(!endTimeExtracted){
                endTime = "2359";
            }
        } else if(!startDateExtracted){
            //no startDate but got startTime
            //no endDate got endTime then should be today till today/tomorrow
            //got endDate no endTime

            if(!endDateExtracted){
                startDate = dateFromBoostToDDMMYYYY(_dateToday);
                if(endTime >= startTime){
                    endDate = startDate;
                } else {
                    endDate = dateFromBoostToDelimitedDDMMYYYY(_dateToday);
                    endDate = addNDaysFromDate(endDate,1);
                }
            } else if (!endTimeExtracted){
                startDate = endDate;
                endTime = "2359";
            } else {
                if(startTime >= endTime){
                    startDate = endDate;
                    startDate = dateFromNumericToBoostFormat(startDate);
                    startDate = substractNDaysFromDate(startDate,1);
                } else {
                    startDate = endDate;
                }
            }
        } else {
            //startTime and startDate are found
            if(!endTimeExtracted){
                endTime = "2359";
            } else if (!endDateExtracted){
                endDate = startDate;
                if(endTime >= startTime){

                } else {
                    endDate = dateFromNumericToBoostFormat(endDate);
                    endDate = addNDaysFromDate(endDate,1);
                }
            } else {
                //all attributes found
            }
        }
    }

    for(int i = 0; i < lengthOfTokenizedUserEntry; i++) {
        //if indexOfDatesAndTimes is empty then always -1 (invalid index)
        int frontIndexOfQueue = -1;
        if(!mainIndexOfDatesAndTimes.empty()){
            frontIndexOfQueue = mainIndexOfDatesAndTimes.front();
        }

        if(i == frontIndexOfQueue){
            mainIndexOfDatesAndTimes.pop();
        } else {
            std::string token = _tokenizedUserEntry[i];

            taskDescription += token;
            if(i != lengthOfTokenizedUserEntry - 1) {
                taskDescription += " ";
            }
        }
    }
    
    startDate = dateFromNumericToBoostFormat(startDate);
    endDate = dateFromNumericToBoostFormat(endDate);
    
    if(isValidDate(startDate) && isValidDate(endDate) && isValidInfo(startDate,startTime,endDate,endTime)){
        TMTaskTime taskTime(startDate,startTime,endDate,endTime);
        TMTask task(taskDescription,taskTime,taskType);
        return task;
    } else {
        TMTaskTime taskTime;
        TMTask task("",taskTime,TaskType::Invalid);
        return task;
    }
}

TMTask TMParser::parseUndatedTaskInfo() {
    TaskType taskType = TaskType::Undated;
    TMTaskTime taskTime;
    std::string taskDescription;
    std::string unitString;
    std::string nextWord;

    int lengthOfVector = _tokenizedUserEntry.size();

    for(int index = 0; index < lengthOfVector; index++){
        if(index + 1 == lengthOfVector){
            break;
        }

        unitString = returnLowerCase(_tokenizedUserEntry[index]);
        nextWord = returnLowerCase(_tokenizedUserEntry[index + 1]);

        if(unitString == TOKEN_BEFORE||unitString == TOKEN_BY) {
            if(nextWord[0] == '"' && nextWord[nextWord.length()-1] == '"' && nextWord.length() != 1){
                if(numberOfWordsInQuote(nextWord) == 1){
                    nextWord = nextWord.substr(1,nextWord.length()-2);
                    if(is12HTime(nextWord)||is24HTime(nextWord)||
                        isNumericDate(nextWord)||isDay(nextWord)||
                        isDDMonDate(nextWord)){
                               //what about for next day?
                            std::string nextWordOriginal = _tokenizedUserEntry[index + 1];
                            _tokenizedUserEntry[index + 1] = nextWordOriginal.substr(1,nextWordOriginal.length()-2);
                    }
                }
            }
        } else if(unitString == TOKEN_ON) {
            if(nextWord[0] == '"' && nextWord[nextWord.length()-1] == '"' && nextWord.length() != 1){
                if(numberOfWordsInQuote(nextWord) == 1){
                    nextWord = nextWord.substr(1,nextWord.length()-2);
                    if(isNumericDate(nextWord)||isDay(nextWord)||isDDMonDate(nextWord)){
                        //what about for next day?
                        std::string nextWordOriginal = _tokenizedUserEntry[index + 1];
                        _tokenizedUserEntry[index + 1] = nextWordOriginal.substr(1,nextWordOriginal.length()-2);
                    }
                }
            }
        } else if(unitString == TOKEN_AT){
            std::string nextWordOriginal = _tokenizedUserEntry[index + 1];
            std::string nextWord = returnLowerCase(nextWordOriginal);
            if(nextWord[0] == '"' && nextWord[nextWord.length()-1] == '"' && nextWord.length() != 1){
                if(numberOfWordsInQuote(nextWord) == 1){
                    nextWord = nextWord.substr(1,nextWord.length()-2);
                    if(is12HTime(nextWord)||is24HTime(nextWord)){
                        //what about for next day?
                        _tokenizedUserEntry[index + 1] = nextWordOriginal.substr(1,nextWordOriginal.length()-2);
                    }
                }
            }
        } else if(unitString == TOKEN_FROM||unitString == TOKEN_TO){
            std::string nextWordOriginal = _tokenizedUserEntry[index + 1];
            std::string nextWord = returnLowerCase(nextWordOriginal);
            if(nextWord[0] == '"' && nextWord[nextWord.length()-1] == '"' && nextWord.length() != 1){
                if(numberOfWordsInQuote(nextWord) == 1){
                    nextWord = nextWord.substr(1,nextWord.length()-2);
                    if(is12HTime(nextWord)||is24HTime(nextWord)||
                       isNumericDate(nextWord)||isDay(nextWord)||
                       isDDMonDate(nextWord)){
                           //what about for next day?
                           _tokenizedUserEntry[index + 1] = nextWordOriginal.substr(1,nextWordOriginal.length()-2);
                           index = index + 1;
                      }
                }
            }
        } else {
            //test next day
        }
    }

    for(int i = 0; i < lengthOfVector; i++){
        std::string token = _tokenizedUserEntry[i];
        
        taskDescription += token;
        if(i != lengthOfVector - 1) {
            taskDescription += " ";
        }
    }

    TMTask task(taskDescription,taskTime,taskType);

    return task;
}

std::vector<TMTask> TMParser::parseMultipleTimingTaskInfo(){
    std::vector<TMTask> tasks;
    std::vector<TMTaskTime> taskTimings;
    std::queue<int> indexOfDatesAndTimes;
    std::queue<int> mainIndexOfDatesAndTimes;
    TaskType taskType;
    std::string startTime = "";
    std::string startDate = "";
    std::string stringAfterAnd;
    //std::string endTime = "";
    //std::string endDate = "";
    std::string taskDescription = "";
    std::string unitString;
    bool startTimeExtracted = false;
    bool startDateExtracted = false;
    //bool endTimeExtracted = false;
    //bool endDateExtracted = false;
    bool oneTimingFound = false;
    int lengthOfTokenizedUserEntry = _tokenizedUserEntry.size();

    for(int index = 0; index < lengthOfTokenizedUserEntry; index++){
        std::string extractedDate;
        std::string extractedTime;

        unitString = returnLowerCase(_tokenizedUserEntry[index]);

        if(oneTimingFound){
            if(unitString == "and"){
                if(index + 1 == lengthOfTokenizedUserEntry){
                    break;
                }

                extractDateAndOrTime(index + 1, indexOfDatesAndTimes,extractedDate,extractedTime);

                if((extractedDate != "")||(extractedTime != "")){
                    mainIndexOfDatesAndTimes.push(index);
                }
            } 
        } else {
            extractDateAndOrTime(index, indexOfDatesAndTimes, extractedDate, extractedTime);
        }
        
        if((extractedDate != "")||(extractedTime != "")){
            if(!oneTimingFound){
                //put in isValid loop?
                oneTimingFound = true;
            }

            if(extractedDate != ""){
                startDate = extractedDate;
                startDateExtracted = true;
            }

            if(extractedTime != ""){
                startTime = extractedTime;
                startTimeExtracted = true;
            }

            if(extractedDate == ""){
                std::string currentTime = getCurrentTime();
                if(startTime >= currentTime){
                    startDate = dateFromBoostToDDMMYYYY(_dateToday);
                } else {
                    startDate = dateFromBoostToDelimitedDDMMYYYY(_dateToday);
                    startDate = addNDaysFromDate(startDate,1);
                }
            }

            startDate = dateFromNumericToBoostFormat(startDate);

            if(isValidDate(startDate)){
                //IMPLEMENT FUNCTION TO CHECK DATE IMMEDIATELY BEFORE CONSIDERING AS DATE
                TMTaskTime taskTime(startDate, startTime, startDate, startTime);
                taskTimings.push_back(taskTime);
                while(!indexOfDatesAndTimes.empty()){
                    mainIndexOfDatesAndTimes.push(indexOfDatesAndTimes.front());
                    indexOfDatesAndTimes.pop();
                }
            } else {
                //invalid dates won't be added into the vector of taskTimings
            }
        }
    }
    
    for(int i = 0; i < lengthOfTokenizedUserEntry; i++) {
        if(i == mainIndexOfDatesAndTimes.front()){
            mainIndexOfDatesAndTimes.pop();
        } else {
            taskDescription += _tokenizedUserEntry[i];
            if(i != lengthOfTokenizedUserEntry - 1) {
                taskDescription += " ";
            }
        }
    }

    int lengthOfTaskTimings = taskTimings.size();

    for(int i = 0; i < lengthOfTaskTimings; i++){
        taskType = TaskType::WithStartDateTime;
        TMTask task(taskDescription,taskTimings[i],taskType);
        tasks.push_back(task);
    }

    return tasks;
}

EditableTaskComponent TMParser::parseTaskComponent() {
	return Description;
}

std::string TMParser::extractDayOrNumericDateOrDDMonDate(int index, std::queue<int>& indexOfDatesAndTimes){
    std::string stringAfterOn = returnLowerCase(_tokenizedUserEntry[index]);
    std::string startDate = "";

    if(isNumericDate(stringAfterOn)){
        startDate = extractNumericDate(index,indexOfDatesAndTimes);
    } else if(isDay(stringAfterOn)){
        startDate = extractDay(index,indexOfDatesAndTimes);
    } else if(isDDMonDate(stringAfterOn)){               
        startDate = extractDDMonDate(index,indexOfDatesAndTimes);
    } else {
        //found on but cannot find following date or day
    }

    return startDate;
}

std::string TMParser::extractNumericDate(int index, std::queue<int>& indexOfDatesAndTimes){
    std::string startDate = returnLowerCase(_tokenizedUserEntry[index]);
    indexOfDatesAndTimes.push(index);
    return startDate;
}

std::string TMParser::extractDDMonDate(int index, std::queue<int>& indexOfDatesAndTimes){
    std::string dd = "";
    std::string month = "";
    std::string yyyy = "";
    std::string stringDate = returnLowerCase(_tokenizedUserEntry[index]);

    int positionOfNextDash = stringDate.find_first_of("-");
    dd = stringDate.substr(0,positionOfNextDash);
    int lengthOfdd = dd.length();
    if(lengthOfdd == 1){
        dd = "0" + dd;
    }
    stringDate = stringDate.substr(positionOfNextDash + 1); //stringDate = mm-yyyy 
    positionOfNextDash = stringDate.find_first_of("-");
    
    if(positionOfNextDash != std::string::npos){
        month = stringDate.substr(0,positionOfNextDash);
        yyyy = stringDate.substr(positionOfNextDash + 1);
    } else {
        month = stringDate;
        std::string dateToday = dateFromBoostToDDMMYYYY(_dateToday);
        yyyy = dateToday.substr(4);
    }
    month = monthFromWrittenToNumeric(month);
    indexOfDatesAndTimes.push(index);
    return dd + month + yyyy;
}

//preconditions: isNextDay is true. used for deadline. comes after "before".
std::string TMParser::extractNextDay(int index, std::queue<int>& indexOfDatesAndTimes){
    //check for day after next, then check for time iter + 1 = next
    std::string date = getDateFromNextDay(index);
    indexOfDatesAndTimes.push(index);
    indexOfDatesAndTimes.push(index + 1);
    //
    return date;
}

std::string TMParser::extractDay(int index, std::queue<int>& indexOfDatesAndTimes){
    std::string day = returnLowerCase(_tokenizedUserEntry[index]); 
    int dayInInteger = dayOfWeek(day);
    boost::gregorian::first_day_of_the_week_after fdaf(dayInInteger);
    boost::gregorian::date dateInBoost = fdaf.get_date(_dateToday);
    std::string startDate = dateFromBoostToDDMMYYYY(dateInBoost);
    indexOfDatesAndTimes.push(index);

    return startDate;
}

std::string TMParser::extractTime(int index, std::queue<int>& indexOfDatesAndTimes){
    std::string stringAfterAt = returnLowerCase(_tokenizedUserEntry[index]);      
    std::string time = "";

    if(is12HTime(stringAfterAt)||is24HTime(stringAfterAt)){
        time = timeTo24HFormat(stringAfterAt);
        indexOfDatesAndTimes.push(index);
    }

    return time;
}

//NEED TO RETURN 2 VARIABLES
void TMParser::extractDateAndOrTime(int index, std::queue<int>& indexOfDatesAndTimes, std::string& extractedDate, std::string& extractedTime){
                //checks for startTime and startDate
    std::string stringAfterToken = returnLowerCase(_tokenizedUserEntry[index]);
    std::string date = "";
    std::string time = "";
    int lengthOfTokenizedUserEntry = _tokenizedUserEntry.size();

    if(isDay(stringAfterToken)||isNumericDate(stringAfterToken)||isDDMonDate(stringAfterToken)) {
        date = extractDayOrNumericDateOrDDMonDate(index, indexOfDatesAndTimes);
        
        if(index + 1 != lengthOfTokenizedUserEntry) {
            std::string stringAfterDate = returnLowerCase(_tokenizedUserEntry[index + 1]);
            
            if(is12HTime(stringAfterDate)||is24HTime(stringAfterDate)) {
                time = extractTime(index + 1, indexOfDatesAndTimes);
            }
        }
    } else if(isNextDay(index)) {
        date = extractNextDay(index, indexOfDatesAndTimes);

        if(index + 1 != lengthOfTokenizedUserEntry){
            std::string stringAfterNextDay = returnLowerCase(_tokenizedUserEntry[index + 1]);

            if(is12HTime(stringAfterNextDay)||is24HTime(stringAfterNextDay)) {
                time = extractTime(index + 1, indexOfDatesAndTimes);
            }

        }
    } else if(is12HTime(stringAfterToken)||is24HTime(stringAfterToken)){
        time = extractTime(index, indexOfDatesAndTimes);

        if(index + 1 != lengthOfTokenizedUserEntry){

            std::string stringAfterTime = returnLowerCase(_tokenizedUserEntry[index + 1]);
            if(isNumericDate(stringAfterTime)||isDay(stringAfterTime)){
                date = extractDayOrNumericDateOrDDMonDate(index + 1, indexOfDatesAndTimes);
            } else if(isNextDay(index + 1)){
                date = extractNextDay(index + 1, indexOfDatesAndTimes);
            }
        }
    } 

    extractedDate = date;
    extractedTime = time;

    return;
}

bool TMParser::isDeadlinedTask() {
    std::string unitString;
    std::string stringAfterBefore;
    int lengthOfTokenizedUserEntry = _tokenizedUserEntry.size();

    for(int index = 0; index < lengthOfTokenizedUserEntry; index++){
        if(index + 1 == lengthOfTokenizedUserEntry){
            break;
        }
        
        unitString = returnLowerCase(_tokenizedUserEntry[index]);
        
        if(unitString == TOKEN_BEFORE||unitString == TOKEN_BY){
            stringAfterBefore = returnLowerCase(_tokenizedUserEntry[index + 1]);

            if(isNumericDate(stringAfterBefore)||isDay(stringAfterBefore)||isDDMonDate(stringAfterBefore)) {
                return true;
            } else if (is12HTime(stringAfterBefore)||is24HTime(stringAfterBefore)) {
                return true;
            } else if (isNextDay(index + 1)){ 
                return true;
            }
        }
    }

    return false;
}

bool TMParser::isTimedTask() {
    bool isTokenOnFound = false;
    bool isTokenAtFound = false;
    bool isTokenFromFound = false;
    bool isTokenToFound = false;
    bool isNextDayFound =  false;
    std::string unitString;
    std::string stringAfterToken;
    int lengthOfTokenizedUserEntry = _tokenizedUserEntry.size();
    
    for(int index = 0; index < lengthOfTokenizedUserEntry; index++){
        //check if current iter is at the last string
        if(index + 1 == lengthOfTokenizedUserEntry){
            break;
        }
        
        unitString = returnLowerCase(_tokenizedUserEntry[index]);
        stringAfterToken = returnLowerCase(_tokenizedUserEntry[index + 1]);
        
        if(unitString == TOKEN_AT){
            if(is12HTime(stringAfterToken)||is24HTime(stringAfterToken)){
                isTokenAtFound = true;
            }
        }
        
        if(unitString == TOKEN_ON){
            if(isNumericDate(stringAfterToken)||isDay(stringAfterToken)||isDDMonDate(stringAfterToken)){
                isTokenOnFound = true;
            }
        }
        
        if(unitString == TOKEN_FROM){
            if(isNumericDate(stringAfterToken)||isDay(stringAfterToken)||isDDMonDate(stringAfterToken)){
                //check for time after date
                isTokenFromFound = true;
            } else if(is12HTime(stringAfterToken)||is24HTime(stringAfterToken)){
                isTokenFromFound = true;
            } else if(isNextDay(index + 1)){
                //check for day after next, then check for time
                isTokenFromFound = true;
            }
        }
        
        if(unitString == TOKEN_TO){

            if(isNumericDate(stringAfterToken)||isDay(stringAfterToken)||isDDMonDate(stringAfterToken)) {
                //check for time after date
                isTokenToFound = true;
            } else if(is12HTime(stringAfterToken)||is24HTime(stringAfterToken)){
                isTokenToFound = true;
            } else if(isNextDay(index + 1)) {
                //check for day after next, then check for time
                isTokenToFound = true;
            } 
        }
        
        if(isNextDay(index)){
            isNextDayFound = true;
        }
    }

    if(isTokenOnFound||isTokenAtFound||isTokenFromFound||isTokenToFound||isNextDayFound){
        return true;
    } else {
        return false;
    }
}

//user input must be date and/or time first followed by and date and/or time
//CHECK FOR PERIOD TOO 2-4PM AND MORE
bool TMParser::isMultipleTimingTask(){
    std::string unitString;
    std::string stringAfterAnd;
    int lengthOfTokenizedUserEntry = _tokenizedUserEntry.size();
    bool oneTimingIsFound = false;
    bool moreThanOneTimingIsFound = false;

    for(int index = 0; index < lengthOfTokenizedUserEntry; index++){

        unitString = returnLowerCase(_tokenizedUserEntry[index]);

        if(!oneTimingIsFound){
            if(isNumericDate(unitString)||isDay(unitString)||isDDMonDate(unitString)||
                is12HTime(unitString)||is24HTime(unitString)){
                    oneTimingIsFound = true;
            } else if (isNextDay(index)){
                oneTimingIsFound = true;   
            }
        } else {
            if(unitString == "and"){
                if(index + 1 == lengthOfTokenizedUserEntry){
                    break;
                }

                stringAfterAnd = returnLowerCase(_tokenizedUserEntry[index + 1]);
                if(isNumericDate(stringAfterAnd)||isDay(stringAfterAnd)||
                    isDDMonDate(stringAfterAnd)||is12HTime(stringAfterAnd)||
                    is24HTime(stringAfterAnd)){
                    moreThanOneTimingIsFound = true;

                } else if (isNextDay(index + 1)){
                    moreThanOneTimingIsFound = true;
                }
            }
        }
    }

    if(moreThanOneTimingIsFound){
        return true;
    } else if(!moreThanOneTimingIsFound && oneTimingIsFound){
        //print out message to use add instead since there's only one timing
        return false;
    } else {
        return false;
    }
}
//Preconditions: dd mm yyyy
bool TMParser::isValidDate(std::string date){
    boost::gregorian::date boostDate;
    try {
        boostDate = boost::gregorian::from_uk_string(date);
    }
    catch (const std::out_of_range& oor) {
        std::cerr << "Out of Range error: " << oor.what() << '\n';
        return false;
    }

    return true;
}

//Preconditions: dates must be in boost string format, times must be 24h format
//checks if startDate is less than or equal to endDate else invalid print error message
//if startDate = endDate check if startTime <= endTime
bool TMParser::isValidInfo(std::string startDate, std::string startTime, std::string endDate, std::string endTime) {
    boost::gregorian::date boostStartDate = boost::gregorian::from_uk_string(startDate);
    boost::gregorian::date boostEndDate = boost::gregorian::from_uk_string(endDate);

    if(boostStartDate < boostEndDate){
        return true;
    } else if(boostStartDate == boostEndDate) {
        if(startTime <= endTime){
            return true;
        } else {
            addErrorMessage("Error: start time is later than end time\n");
            return false;
        }
    } else {
        addErrorMessage("Error: end date is later than start date\n");
        return false;
    }
}

bool TMParser::isInteger(std::string token) {
    for(std::string::iterator it = token.begin(); it < token.end(); it++) {
        if(!isdigit(*it)) {
            return false;
        }
    }
    return true;
}
/*
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
*/
bool TMParser::isNumericDate(std::string token) {
    //DDMMYYYY DDMMYYYY
    int lengthOfToken = token.size();
    if(lengthOfToken == 8||lengthOfToken == 6) {
        if(!isInteger(token)){
            return false;
        }

        if(!isValidDate(dateFromNumericToBoostFormat(token))){
            return false;
        }

        return true;
    }

    return false;
}

bool TMParser::isDDMonDate(std::string token){
    std::string dd = "";
    std::string month = "";
    std::string yyyy = "";

    int positionOfNextDash = token.find_first_of("-");

    if(positionOfNextDash == std::string::npos){
        return false;
    }

    dd = token.substr(0,positionOfNextDash);//dd = dd
    int lengthOfdd = dd.length();
    //check whether length is 1 or 2 whether from 1-31?
    if(!isInteger(dd)||!(lengthOfdd == 1||lengthOfdd == 2)){
        return false;
    }

    token = token.substr(positionOfNextDash + 1); //stringDate = mm-yyyy 
    positionOfNextDash = token.find_first_of("-");
    
    if(positionOfNextDash != std::string::npos){
        month = token.substr(0,positionOfNextDash);
        if(!isMonth(month)){
            return false;
        }
        yyyy = token.substr(positionOfNextDash + 1);
        int lengthOfyyyy = yyyy.length();
        if(!isInteger(yyyy)||!(lengthOfyyyy == 2||lengthOfyyyy == 4)){
            return false;
        }
        if(lengthOfyyyy == 2){
            yyyy = "20" + yyyy;
        } else {
        }
    } else {
        month = token;
        std::string dateToday = dateFromBoostToDDMMYYYY(_dateToday);
        yyyy = dateToday.substr(4,4);
        if(!isMonth(month)){
            return false;
        }
    }

    std::string date = dd + "-" + month + "-" + yyyy;
    
    if(!isValidDate(date)){
        return false;
    }

    return true;
}

bool TMParser::isDay(std::string token) {
    token = returnLowerCase(token);
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
       token == DAY_SUNDAY) {
           return true;
    } else {
        return false;
    }
}

bool TMParser::is12HTime(std::string timeToken){
    //format 2am 10am 230pm 1230am
    //check for am or pm first

    if(isAM(timeToken)||isPM(timeToken)){

    } else {

        return false;
    }

    int lengthOfTimeToken = timeToken.size();
    if(lengthOfTimeToken == 3){
        int hour = std::stoi(timeToken.substr(0,1));
        if(hour >= 1 && hour <= 9){
            return true;
        } else {
            return false;
        }
    } else if(lengthOfTimeToken == 4){
        int hour = std::stoi(timeToken.substr(0,2));
        if(hour >= 10 && hour <= 12){
            return true;
        } else {
            return false;
        }
    } else if(lengthOfTimeToken == 5){
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
    } else if(lengthOfTimeToken == 6){
        int hour = std::stoi(timeToken.substr(0,2));
        if(hour >= 1 && hour <= 12) {
            int minute = std::stoi(timeToken.substr(1,2));
            if(minute >= 0 && minute <= 59){
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
    //format: 10/ 1030/ 10:30 consider single digit e.g 8!!!
    //to consider: 8 1-10(change to pm?) 
    unsigned int lengthOfTimeToken = timeToken.size();
    if(lengthOfTimeToken == 2){
        if(isInteger(timeToken)){
            int hour = std::stoi(timeToken);
            if(hour >= 0 && hour <= 23) {
                return true;
            } else {
                return false;
            }
        } else {
            return false;
        }
    } else if (lengthOfTimeToken == 4) {
        if(isInteger(timeToken)) {
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
        } else {
            return false;
        }
    } else if(lengthOfTimeToken == 5) {
        if(timeToken[2] == ':') {
            if(isInteger(timeToken.substr(0,2)) && isInteger(timeToken.substr(3,2))) {
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
    } else {
        return false;
    }
}

bool TMParser::isNextDay(int index){
    std::string firstWord = returnLowerCase(_tokenizedUserEntry[index]);

    if(firstWord == TOKEN_NEXT){
        int lengthOfTokenizedUserEntry = _tokenizedUserEntry.size();
        if(index + 1 != lengthOfTokenizedUserEntry) {
            std::string secondWord = returnLowerCase(_tokenizedUserEntry[index + 1]);
            if(isDay(secondWord)){
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

bool TMParser::isNext(std::string token){
    return token == TOKEN_NEXT;
}

bool TMParser::isAM(std::string token){
    std::string lastTwoCharacters = token.substr(token.length()-2,2);
    lastTwoCharacters = returnLowerCase(lastTwoCharacters);
    if(lastTwoCharacters == "am"){
        return true;
    } else {
        return false;
    }
}

bool TMParser::isPM(std::string token){
    std::string lastTwoCharacters = token.substr(token.length()-2,2);
    lastTwoCharacters = returnLowerCase(lastTwoCharacters);
    if(lastTwoCharacters == "pm"){
        return true;
    } else {
        return false;
    }
}

bool TMParser::isMonth(std::string token){
    token = returnLowerCase(token);
    if(token == "jan"||
       token == "january"||
       token == "feb"||
       token == "february"||
       token == "mar"||
       token == "march"||
       token == "apr"||
       token == "april"||
       token == "may"||
       token == "jun"||
       token == "jul"||
       token == "july"||
       token == "aug"||
       token == "august"||
       token == "sep"||
       token == "september"||
       token == "oct"||
       token == "october"||
       token == "nov"||
       token == "november"||
       token == "dec"||
       token == "december") {
           return true;
    } else {
        return false;
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
    } else {
        //invalid day!
        return -1;
    }
}

//precondition: command extracted
std::vector<int> TMParser::parseTaskPositionNo() {
    int intTaskPositionNo;
    std::vector<int> vectorTaskPositionNo;
    int vectorSize = _tokenizedUserEntry.size();
    for(int i = 0; i < vectorSize; i++) {
        std::string taskPositionNo = _tokenizedUserEntry[i];
        intTaskPositionNo = std::stoi(taskPositionNo);
        vectorTaskPositionNo.push_back(intTaskPositionNo);
    }
    return vectorTaskPositionNo;
}

//precondition: command extracted
std::string TMParser::parseSearchKey() {
    std::string searchKey;
    std::vector<std::string>::iterator iter;

    for(iter = _tokenizedUserEntry.begin(); iter < _tokenizedUserEntry.end(); iter++){
        searchKey += *iter;
        if(iter != _tokenizedUserEntry.end()-1){
            searchKey += " ";
        }
    }

    return searchKey;
}

//precondition: extract command
std::string TMParser::parseDirectory() {
    std::string directory = _tokenizedUserEntry[0];
    return directory;
}

std::string TMParser::returnLowerCase(std::string word) {
    int length = word.size();
    char character;

    for(int i = 0; i < length; i++) {
        character = tolower(word[i]);
        word[i] = character;
    }

    return word;
}

//check if it's ddmmyy or ddmmyyyy format first using isNumericDate
std::string TMParser::dateFromNumericToBoostFormat(std::string stringDate) {
    std::string dd = "";
    std::string mm = "";
    std::string yyyy = "";
    int lengthOfStringDate = stringDate.length();
    
    if(lengthOfStringDate == 6){
        //DDMMYY
        dd = stringDate.substr(0,2);
        mm = stringDate.substr(2,2);
        yyyy = "20" + stringDate.substr(4,2);
    } else if(lengthOfStringDate == 8) {
        //DDMMYYYY
        dd = stringDate.substr(0,2);
        mm = stringDate.substr(2,2);
        yyyy = stringDate.substr(4,4);
    } 

    return dd + "-" + mm + "-" + yyyy;

}

std::string TMParser::dateFromNumericToStandardFormat(std::string stringDate){
    std::string dateInBoostFormat = dateFromNumericToBoostFormat(stringDate);
    if(!isValidDate(dateInBoostFormat)){
        return "";
    }
    boost::gregorian::date boostDate = boost::gregorian::from_uk_string(dateInBoostFormat);
    return dateFromBoostToStandardFormat(boostDate);
}

std::string TMParser::dateFromBoostToStandardFormat(const boost::gregorian::date& date) {
    std::ostringstream os;
    boost::gregorian::date_facet* facet(new boost::gregorian::date_facet("%d %b %Y"));
    os.imbue(std::locale(std::cout.getloc(), facet));
    os << date;
    return os.str();
}

//incomplete
std::string TMParser::timeTo24HFormat(std::string time){
    std::string hhmm;
    int timeIn24HFormat;
    //
    //if this is true then there are only 4 possibilities check is24HTime function
    if(is12HTime(time)) {
        if(isAM(time)){
            if(time.length() == 3){
                hhmm = "0" + time.substr(0,1) + "00";
            } else if(time.length() == 4){
                timeIn24HFormat = (std::stoi(time.substr(0,2)))%12;
                if(timeIn24HFormat == 0){
                    hhmm = "0000";
                } else {
                    hhmm = time.substr(0,2) + "00";
                }
            } else if(time.length() == 5){
                hhmm = "0" + time.substr(0,3);
            } else {
                std::string hour = time.substr(0,2);
                if(hour == "12"){
                    hhmm = "00" + time.substr(2,2);
                } else {
                    hhmm = time.substr(0,4);
                }
            }
        } else {
            if (time.length() == 3){
                timeIn24HFormat = 12 + std::stoi(time.substr(0,1));
                hhmm = std::to_string(timeIn24HFormat) + "00";
            } else if(time.length() == 4){
                timeIn24HFormat = (std::stoi(time.substr(0,2)))%12 + 12;
                hhmm = std::to_string(timeIn24HFormat) + "00";
            } else if(time.length() == 5){
                timeIn24HFormat = 1200 + std::stoi(time.substr(0,3));
                hhmm = std::to_string(timeIn24HFormat);
            } else {
                int timeIn24HFormat = 1200 + (std::stoi(time.substr(0,4)))%1200;
                hhmm = std::to_string(timeIn24HFormat);
            }
        }
    } else if(is24HTime(time)){
        //if am or pm is not specified, will be treated as 24hour time
        if(time.length() == 2){
            hhmm = time + "00";
        } else if (time.length() == 4){
            hhmm = time;
        } else {
            hhmm = time.erase(2,1);
        }
    }

    return hhmm;
}

std::string TMParser::dateFromBoostToDelimitedDDMMYYYY(const boost::gregorian::date& date){
    std::ostringstream os;
    boost::gregorian::date_facet* facet(new boost::gregorian::date_facet("%d %m %Y"));
    os.imbue(std::locale(std::cout.getloc(), facet));
    os << date;
    return os.str();
}

std::string TMParser::dateFromBoostToDDMMYYYY(const boost::gregorian::date& date){
    std::ostringstream os;
    boost::gregorian::date_facet* facet(new boost::gregorian::date_facet("%d%m%Y"));
    os.imbue(std::locale(std::cout.getloc(),facet));
    os << date;
    return os.str();
}

std::string TMParser::monthFromWrittenToNumeric(std::string month){
    month = returnLowerCase(month);
    if(month == "jan"||month == "january"){
        return "01";
    } else if (month == "feb"||month == "february"){
        return "02";
    } else if (month == "mar"||month == "march"){
        return "03";
    } else if (month == "apr"||month == "april"){
        return "04";
    } else if (month == "may"){
        return "05";
    } else if (month == "jun"||month == "june"){
        return "06";
    } else if (month == "jul"||month == "july"){
        return "07";
    } else if (month == "aug"||month == "august"){
        return "08";
    } else if (month == "sep"||month == "september"){
        return "09";
    } else if (month == "oct"||month == "october"){
        return "10";
    } else if (month == "nov"||month == "november"){
        return "11";
    } else if (month == "dec"||month == "december") {
        return "12";
    } else {
        return "-1";
    }
}

//returns HHMM
std::string TMParser::getCurrentTime(){
    boost::posix_time::ptime currentDateAndTime(boost::posix_time::second_clock::local_time());
    std::ostringstream os;
    boost::posix_time::time_facet* facet(new boost::posix_time::time_facet("%H%M"));
    os.imbue(std::locale(std::cout.getloc(), facet));
    os << currentDateAndTime;
    return os.str();
}

//preconditions date dd mm yyyy use dateFromUserToBoost
std::string TMParser::substractNDaysFromDate(std::string date, int n){
    boost::gregorian::date initialBoostDate = boost::gregorian::from_uk_string(date);
    boost::gregorian::date_duration dateDuration(n);
    boost::gregorian::date finalBoostDate = initialBoostDate - dateDuration;
    return dateFromBoostToDDMMYYYY(finalBoostDate);
}

//preconditions date dd mm yyyy use dateFromUserToBoost
std::string TMParser::addNDaysFromDate(std::string date, int n){
    boost::gregorian::date initialBoostDate = boost::gregorian::from_uk_string(date);
    boost::gregorian::date_duration dateDuration(n);
    boost::gregorian::date finalBoostDate = initialBoostDate + dateDuration;
    return dateFromBoostToDDMMYYYY(finalBoostDate);
}

//preconditions only used after token before
std::string TMParser::getDateFromNextDay(int index){
    std::string stringDay = returnLowerCase(_tokenizedUserEntry[index + 1]);
    std::string date;

    boost::gregorian::greg_weekday day(dayOfWeek(stringDay));
    boost::gregorian::first_day_of_the_week_after fdaf(day);
    boost::gregorian::first_day_of_the_week_after firstSundayAfterToday(boost::gregorian::Sunday);
    boost::gregorian::date dateTM = firstSundayAfterToday.get_date(_dateToday);
    dateTM = fdaf.get_date(dateTM);
    std::string tempDate = boost::gregorian::to_iso_string(dateTM);

    date = tempDate.substr(6,2) + tempDate.substr(4,2) + tempDate.substr(0,4);

    return date;
}

TMTask TMParser::convertStringToTMTask(std::string listEntry){
    std::string taskDescription;

    int unconfirmedBatchNumber;

    std::string startDay;
    std::string startMonth;
    std::string startYear;
    std::string startTime;

    std::string endDay;
    std::string endMonth;
    std::string endYear;
    std::string endTime;

    bool isCompleted;
    bool isConfirmed;
    bool isClashed;

    std::string stringTaskType;
    TaskType taskType;

    std::istringstream iss(listEntry);

    iss >> taskDescription;

    iss >> unconfirmedBatchNumber;

    iss >> startDay;
    iss >> startMonth;
    iss >> startYear;
    iss >> startTime;

    iss >> endDay;
    iss >> endMonth;
    iss >> endYear;
    iss >> endTime;

    iss >> isCompleted;
    iss >> isConfirmed;
    iss >> isClashed;

    iss >> stringTaskType;
    taskType = convertStringToTaskType(stringTaskType);

    std::string startDate = startDay + "-" + startMonth + "-" + startYear;
    std::string endDate = endDay + "-" + endMonth + "-" + endYear;

    TMTaskTime taskTime(startDate, startTime, endDate, endTime);
    TMTask task(taskDescription, taskTime, taskType);

    if(isCompleted){
        task.setAsCompleted();
    }

    if(!isConfirmed){
        task.setAsUnconfirmed();
    }

    if(isClashed){
        task.setAsClashed();
    }

    return task;
}

//precondition: the string must be one of the 5 types in the exact same format
TaskType TMParser::convertStringToTaskType(std::string taskType){
    std::map<std::string, TaskType> taskTypeMap = boost::assign::map_list_of
        ("WithStartDateTime", TaskType::WithStartDateTime) 
        ("WithEndDateTime", TaskType::WithEndDateTime)
        ("WithPeriod", TaskType::WithPeriod)
        ("Undated", TaskType::Undated)
        ("Invalid", TaskType::Invalid);

    return taskTypeMap[taskType];
}

//precondition:: taskType is one of the 5 types
std::string TMParser::convertTaskTypeToString(TaskType taskType){
    std::map<TaskType, std::string> stringMap = boost::assign::map_list_of
        (TaskType::WithStartDateTime, "WithStartDateTime")
        (TaskType::WithEndDateTime, "WithEndDateTime")
        (TaskType::WithPeriod, "WithPeriod")
        (TaskType::Undated, "Undated")
        (TaskType::Invalid, "Invalid");

    return stringMap[taskType];
}

void TMParser::addErrorMessage(std::string errorMessage){
    _errorMessages.push_back(errorMessage);
    return;
}

std::string TMParser::getErrorMessage(){
    int numberOfErrorMessages = _errorMessages.size();
    std::string errorMessages;

    for(int i = 0; i < numberOfErrorMessages; i++){
        errorMessages += _errorMessages[i];
    }

    return errorMessages;
}

int TMParser::numberOfWordsInQuote(std::string quote){
    std::string quoteWithoutInvertedCommas = quote.substr(1,quote.length()-2);
    int numberOfWords = 0;
    std::istringstream iss(quoteWithoutInvertedCommas);
    std::string word;

    while(iss >> word){
        numberOfWords++;
    }

    return numberOfWords;
}