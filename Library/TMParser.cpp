#include "TMParser.h"
#include "TMTask.h"

TMParser* TMParser::theOne;

TMParser::TMParser() {
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
    _errorMessages.clear();
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

std::string TMParser::extractTokenAfterCommand() {
    ErrorMessageReport *errorMessageReport = ErrorMessageReport::getInstance(); 
    if(_tokenizedUserEntry.size() == 0){
        errorMessageReport->addErrorMessage("Index of task not specified\n");
        return "";
    } else if (_tokenizedUserEntry.size() == 1) {
        errorMessageReport->addErrorMessage("Missing new task information\n");
        return "";
    }

	std::string token = _tokenizedUserEntry[0];

    if(!isPositiveInteger(token)) {
        errorMessageReport->addErrorMessage("Index of task must be a positive integer\n");
        return "";
    }

    _tokenizedUserEntry.erase(_tokenizedUserEntry.begin());

    return token;
}

std::vector<std::string> TMParser::returnTokens() {
	return _tokenizedUserEntry;
}

TMParser::CommandTypes TMParser::determineCommandType(std::string command) {
    FormatConverter *formatConverter = FormatConverter::getInstance();
	command = formatConverter->returnLowerCase(command);
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
	} else if (command == CMD_QUIT||command == CMD_SHORTCUT_QUIT||command == CMD_EXIT||command == CMD_CLOSE) {
        return CommandTypes::Exit;
    } else {
        return CommandTypes::Invalid;
    }
}

//Preconditions: taskInfo contains only the entry after command. use extractEntryAfterCommand 1st
//             : use only when in adding or editing information
TMTask TMParser::parseTaskInfo() {
    TaskChecker *taskChecker = TaskChecker::getInstance();
    if(taskChecker->isDeadlinedTask(_tokenizedUserEntry)){
        return parseDeadlinedTaskInfo();
    } else if(taskChecker->isTimedTask(_tokenizedUserEntry)) {
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

    FormatConverter *formatConverter = FormatConverter::getInstance();
    TimeChecker *timeChecker = TimeChecker::getInstance();
    DateChecker *dateChecker = DateChecker::getInstance();
    Extractor *extractor = Extractor::getInstance();

    for(int index = 0; index < lengthOfTokenizedUserEntry; index++){

        unitString = formatConverter->returnLowerCase(_tokenizedUserEntry[index]);

        if(unitString == TOKEN_BEFORE||unitString == TOKEN_BY){
            if(index + 1 == lengthOfTokenizedUserEntry){
                break;
            }
            nextWord = formatConverter->returnLowerCase(_tokenizedUserEntry[index + 1]);
            if(dateChecker->isNumericDate(nextWord)||dateChecker->isDay(nextWord)||dateChecker->isOneDelimitedDate(nextWord)) {
                //e.g. before 01012016 (DDMMYYYY)
                indexOfDatesAndTimes.push(index);
                dateToMeet = extractor->extractDayOrNumericDateOrDelimitedDate(index + 1, indexOfDatesAndTimes, _tokenizedUserEntry);
                if(unitString == TOKEN_BEFORE) {
                    dateToMeet = substractNDaysFromDate(dateToMeet,1);
                }
                timeToMeet = "2359";
                index = indexOfDatesAndTimes.back();
            } else if (dateChecker->isNextDay(index + 1, _tokenizedUserEntry)) {
                //e.g. before next monday
                indexOfDatesAndTimes.push(index);
                dateToMeet = extractor->extractNextDay(index + 1, indexOfDatesAndTimes, _tokenizedUserEntry);
                if(unitString == TOKEN_BEFORE) {
                    dateToMeet = substractNDaysFromDate(dateToMeet,1);
                }
                timeToMeet = "2359";
                index = indexOfDatesAndTimes.back();
            } else if (timeChecker->is12HTime(nextWord)||timeChecker->is24HTime(nextWord)||
                timeChecker->isTimeWithoutPeriod(nextWord)) {
                    indexOfDatesAndTimes.push(index);
                    timeToMeet = extractor->extractTime(index + 1, indexOfDatesAndTimes, _tokenizedUserEntry);
                    index = indexOfDatesAndTimes.back();
            } else {
                editDateOrTimeInInvertedCommas(nextWord, index, true, true);
            }
        } else if(unitString == TOKEN_ON) {
            if(index + 1 == lengthOfTokenizedUserEntry){
                break;
            }
            nextWord = formatConverter->returnLowerCase(_tokenizedUserEntry[index + 1]);
            if(dateChecker->isNumericDate(nextWord)||dateChecker->isDay(nextWord)||dateChecker->isOneDelimitedDate(nextWord)) {
                indexOfDatesAndTimes.push(index);
                dateToMeet = extractor->extractDayOrNumericDateOrDelimitedDate(index + 1,indexOfDatesAndTimes, _tokenizedUserEntry);
                index = indexOfDatesAndTimes.back();

                if(timeToMeet == "") {
                    timeToMeet = "2359";
                }
            } else {
                editDateOrTimeInInvertedCommas(nextWord, index, true, false);
            }
        } else if(dateChecker->isNextDay(index, _tokenizedUserEntry)){
            dateToMeet = extractor->extractNextDay(index, indexOfDatesAndTimes, _tokenizedUserEntry);
            index = indexOfDatesAndTimes.back();
        } else {
            editDateOrTimeInInvertedCommas(unitString, index - 1, true, false);
        }
    }

    for(int i = 0; i < lengthOfTokenizedUserEntry; i++) {
        //will be -1 (invalid index) if index is empty 
        int frontIndexOfQueue = -1;

        if(!indexOfDatesAndTimes.empty()) {
            frontIndexOfQueue = indexOfDatesAndTimes.front();
        }

        if(i == frontIndexOfQueue) {
            indexOfDatesAndTimes.pop();
        } else {
            std::string token = _tokenizedUserEntry[i];
            taskDescription += token;
            if(i != lengthOfTokenizedUserEntry - 1) {
                taskDescription += " ";
            }
        }
    }

    configureEndTimeWithoutPeriods(timeToMeet);

    if(dateToMeet == "") {
        dateToMeet = formatConverter->dateFromBoostToDDMMYYYY(currentDate());
        if(timeToMeet < currentTime()){
            //OR EQUALS?
            dateToMeet = addNDaysFromDate(dateToMeet,1);
        }
    }

    dateToMeet = formatConverter->dateFromNumericToBoostFormat(dateToMeet);

    if(dateChecker->isValidDate(dateToMeet)){
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
    std::string nextWord;
    int lengthOfTokenizedUserEntry = _tokenizedUserEntry.size();

    FormatConverter *formatConverter = FormatConverter::getInstance();
    TimeChecker *timeChecker = TimeChecker::getInstance();
    DateChecker *dateChecker = DateChecker::getInstance();
    Extractor *extractor = Extractor::getInstance();

    //will not check last string. last string treated as task desc
    
    for(int index = 0; index < lengthOfTokenizedUserEntry; index++){

        unitString = formatConverter->returnLowerCase(_tokenizedUserEntry[index]);

        if(unitString == TOKEN_AT) {
            if(index + 1 == lengthOfTokenizedUserEntry){
                break;
            }
            nextWord = formatConverter->returnLowerCase(_tokenizedUserEntry[index + 1]);
            if(timeChecker->is12HTime(nextWord)||timeChecker->is24HTime(nextWord)||
                timeChecker->isTimeWithoutPeriod(nextWord)){
                    startTime = extractor->extractTime(index + 1, indexOfDatesAndTimes, _tokenizedUserEntry);
                    configureQueuesAndIndexAfterToken(mainIndexOfDatesAndTimes, indexOfDatesAndTimes, index);
            } else {
                editDateOrTimeInInvertedCommas(nextWord, index, false, true);
            }
        } else if(unitString == TOKEN_ON) {
            if(index + 1 == lengthOfTokenizedUserEntry){
                break;
            }
            nextWord = formatConverter->returnLowerCase(_tokenizedUserEntry[index + 1]);
            if(dateChecker->isNumericDate(nextWord)||dateChecker->isDay(nextWord)||dateChecker->isOneDelimitedDate(nextWord)){
                startDate = extractor->extractDayOrNumericDateOrDelimitedDate(index + 1, indexOfDatesAndTimes, _tokenizedUserEntry);
                configureQueuesAndIndexAfterToken(mainIndexOfDatesAndTimes, indexOfDatesAndTimes, index);
            } else {
                editDateOrTimeInInvertedCommas(nextWord, index, true, false);
            }
        } else if(unitString == TOKEN_FROM){
            if(index + 1 == lengthOfTokenizedUserEntry){
                break;
            }
            nextWord = formatConverter->returnLowerCase(_tokenizedUserEntry[index + 1]);
            if(dateChecker->isNumericDate(nextWord)||dateChecker->isDay(nextWord)||
                dateChecker->isOneDelimitedDate(nextWord)||
                dateChecker->isNextDay(index + 1, _tokenizedUserEntry)||
                timeChecker->is12HTime(nextWord)||timeChecker->is24HTime(nextWord)||
                timeChecker->isTimeWithoutPeriod(nextWord)) {
                    extractor->extractDateAndOrTime(index + 1, indexOfDatesAndTimes, startDate, startTime, _tokenizedUserEntry);
                    configureQueuesAndIndexAfterToken(mainIndexOfDatesAndTimes, indexOfDatesAndTimes, index);
            } else {
                editDateOrTimeInInvertedCommas(nextWord, index, true, true);
            }
        } else if (unitString == TOKEN_TO){
            if(index + 1 == lengthOfTokenizedUserEntry){
                break;
            }
            nextWord = formatConverter->returnLowerCase(_tokenizedUserEntry[index + 1]);
            if(dateChecker->isNumericDate(nextWord)||dateChecker->isDay(nextWord)||
                dateChecker->isOneDelimitedDate(nextWord)||
                dateChecker->isNextDay(index + 1, _tokenizedUserEntry)||
                timeChecker->is12HTime(nextWord)||timeChecker->is24HTime(nextWord)||
                timeChecker->isTimeWithoutPeriod(nextWord)) {
                    extractor->extractDateAndOrTime(index + 1, indexOfDatesAndTimes, endDate, endTime, _tokenizedUserEntry);
                    configureQueuesAndIndexAfterToken(mainIndexOfDatesAndTimes, indexOfDatesAndTimes, index);
            } else {
                editDateOrTimeInInvertedCommas(nextWord, index, true, true);
            }
        } else if(dateChecker->isNextDay(index, _tokenizedUserEntry)) {
            startDate = extractor->extractNextDay(index, indexOfDatesAndTimes, _tokenizedUserEntry);
            configureQueuesAndIndex(mainIndexOfDatesAndTimes, indexOfDatesAndTimes, index);
        } else {
            editDateOrTimeInInvertedCommas(unitString, index - 1, true, false);
        }
    }

    configureAllDatesAndTimes(startDate, startTime, endDate, endTime, taskType);

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
    
    startDate = formatConverter->dateFromNumericToBoostFormat(startDate);
    endDate = formatConverter->dateFromNumericToBoostFormat(endDate);
    
    if(dateChecker->isValidDate(startDate) && dateChecker->isValidDate(endDate) && isValidInfo(startDate,startTime,endDate,endTime)){
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

    FormatConverter *formatConverter = FormatConverter::getInstance();
    TimeChecker *timeChecker = TimeChecker::getInstance();
    DateChecker *dateChecker = DateChecker::getInstance();
    Extractor *extractor = Extractor::getInstance();

    for(int index = 0; index < lengthOfVector; index++){

        unitString = formatConverter->returnLowerCase(_tokenizedUserEntry[index]);

        if(unitString == TOKEN_BEFORE||unitString == TOKEN_BY) {
            if(index + 1 == lengthOfVector){
                break;
            }
            nextWord = formatConverter->returnLowerCase(_tokenizedUserEntry[index + 1]);
            editDateOrTimeInInvertedCommas(nextWord, index, true, true);
        } else if(unitString == TOKEN_ON) {
            if(index + 1 == lengthOfVector){
                break;
            }
            nextWord = formatConverter->returnLowerCase(_tokenizedUserEntry[index + 1]);
            editDateOrTimeInInvertedCommas(nextWord, index, true, false);
        } else if(unitString == TOKEN_AT){
            if(index + 1 == lengthOfVector){
                break;
            }
            nextWord = formatConverter->returnLowerCase(_tokenizedUserEntry[index + 1]);
            editDateOrTimeInInvertedCommas(nextWord, index, false, true);
        } else if(unitString == TOKEN_FROM||unitString == TOKEN_TO){
            if(index + 1 == lengthOfVector){
                break;
            }
            nextWord = formatConverter->returnLowerCase(_tokenizedUserEntry[index + 1]);
            editDateOrTimeInInvertedCommas(nextWord, index, true, true);
            //index = index + 1;?
        } else {
            editDateOrTimeInInvertedCommas(unitString, index - 1, true, false);
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
    std::vector<TaskType> taskTypes;
    std::string taskDescription;
    bool oneTimingFound = false;
    int lengthOfTokenizedUserEntry = _tokenizedUserEntry.size();

    FormatConverter *formatConverter = FormatConverter::getInstance();
    DateChecker *dateChecker = DateChecker::getInstance();
    Extractor *extractor = Extractor::getInstance();

    for(int index = 0; index < lengthOfTokenizedUserEntry; index++){
        std::string startTime;
        std::string startDate;
        std::string endTime;
        std::string endDate;
        TaskType taskType;
        int indexOfLastExtracted;
        std::string unitString;

        unitString = formatConverter->returnLowerCase(_tokenizedUserEntry[index]);

        if(oneTimingFound){
            if(unitString == "and"){
                if(index + 1 == lengthOfTokenizedUserEntry){
                    break;
                }

                extractor->extractDateAndOrTime(index + 1, indexOfDatesAndTimes, startDate, startTime, _tokenizedUserEntry);

                if((startDate != "")||(startTime != "")){
                    mainIndexOfDatesAndTimes.push(index);

                    indexOfLastExtracted = indexOfDatesAndTimes.back();

                    while(!indexOfDatesAndTimes.empty()){
                        mainIndexOfDatesAndTimes.push(indexOfDatesAndTimes.front());
                        indexOfDatesAndTimes.pop();
                    }

                    if(indexOfLastExtracted + 1 != lengthOfTokenizedUserEntry){
                        if(_tokenizedUserEntry[indexOfLastExtracted + 1] == "-"){
                            if(indexOfLastExtracted + 2 != lengthOfTokenizedUserEntry){
                                extractor->extractDateAndOrTime(indexOfLastExtracted + 2, indexOfDatesAndTimes, endDate, endTime, _tokenizedUserEntry);
                                if((endDate != "")||(endTime != "")){

                                    mainIndexOfDatesAndTimes.push(indexOfLastExtracted + 1);

                                    while(!indexOfDatesAndTimes.empty()){
                                        mainIndexOfDatesAndTimes.push(indexOfDatesAndTimes.front());
                                        indexOfDatesAndTimes.pop();
                                    }
                                }
                            }
                        }
                    }

                    index = mainIndexOfDatesAndTimes.back();
                } 
            } 
        } else {
            extractor->extractDateAndOrTime(index, indexOfDatesAndTimes, startDate, startTime, _tokenizedUserEntry);

            if((startDate != "")||(startTime != "")){

                if(!oneTimingFound){
                    //put in isValid loop?
                    oneTimingFound = true;
                }

                indexOfLastExtracted = indexOfDatesAndTimes.back();

                while(!indexOfDatesAndTimes.empty()){
                    mainIndexOfDatesAndTimes.push(indexOfDatesAndTimes.front());
                    indexOfDatesAndTimes.pop();
                }

                if(indexOfLastExtracted + 1 != lengthOfTokenizedUserEntry){
                    if(_tokenizedUserEntry[indexOfLastExtracted + 1] == "-"){
                        if(indexOfLastExtracted + 2 != lengthOfTokenizedUserEntry){
                            extractor->extractDateAndOrTime(indexOfLastExtracted + 2, indexOfDatesAndTimes, endDate, endTime, _tokenizedUserEntry);
                            if((endDate != "")||(endTime != "")){
                                mainIndexOfDatesAndTimes.push(indexOfLastExtracted + 1);

                                while(!indexOfDatesAndTimes.empty()){
                                    mainIndexOfDatesAndTimes.push(indexOfDatesAndTimes.front());
                                    indexOfDatesAndTimes.pop();
                                }
                            }
                        }
                    }
                }
                index = mainIndexOfDatesAndTimes.back();
            } 
        }
        
        configureAllDatesAndTimes(startDate,startTime,endDate,endTime,taskType);
        taskTypes.push_back(taskType);

        startDate = formatConverter->dateFromNumericToBoostFormat(startDate);
        endDate = formatConverter->dateFromNumericToBoostFormat(endDate);

        if(dateChecker->isValidDate(startDate) && dateChecker->isValidDate(endDate) && isValidInfo(startDate, startTime, endDate, endTime)){
            TMTaskTime taskTime(startDate, startTime, endDate, endTime);
            taskTimings.push_back(taskTime);
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
        TMTask task(taskDescription,taskTimings[i],taskTypes[i]);
        tasks.push_back(task);
    }

    return tasks;
}

void TMParser::configureQueuesAndIndex(std::queue<int>& mainIndexOfDatesAndTimes, std::queue<int>& indexOfDatesAndTimes, int& index) {
    while(!indexOfDatesAndTimes.empty()){
        mainIndexOfDatesAndTimes.push(indexOfDatesAndTimes.front());
        indexOfDatesAndTimes.pop();
    }
    index = mainIndexOfDatesAndTimes.back();
}

void TMParser::configureQueuesAndIndexAfterToken(std::queue<int>& mainIndexOfDatesAndTimes, std::queue<int>& indexOfDatesAndTimes, int& index) {
    mainIndexOfDatesAndTimes.push(index);
    while(!indexOfDatesAndTimes.empty()){
        mainIndexOfDatesAndTimes.push(indexOfDatesAndTimes.front());
        indexOfDatesAndTimes.pop();
    }
    index = mainIndexOfDatesAndTimes.back();
}

//nextWord must be lowercase
void TMParser::editDateOrTimeInInvertedCommas(std::string nextWord, int index, bool checkDate, bool checkTime) {
    TimeChecker *timeChecker = TimeChecker::getInstance();
    DateChecker *dateChecker = DateChecker::getInstance();

    if(nextWord[0] == '"' && nextWord[nextWord.length()-1] == '"' && nextWord.length() > 2){

        nextWord = nextWord.substr(1,nextWord.length()-2);
        std::string nextWordOriginal = _tokenizedUserEntry[index + 1];

        if(checkDate && checkTime) {
            if(timeChecker->is12HTime(nextWord)||timeChecker->is24HTime(nextWord)||
                timeChecker->isTimeWithoutPeriod(nextWord)||dateChecker->isNumericDate(nextWord)||
                dateChecker->isDay(nextWord)||dateChecker->isOneDelimitedDate(nextWord)||
                isNextDayInInvertedCommas(nextWord)){
                _tokenizedUserEntry[index + 1] = nextWordOriginal.substr(1,nextWordOriginal.length()-2);
            }
        } else if(checkDate) {
            if(dateChecker->isNumericDate(nextWord)||dateChecker->isDay(nextWord)||
               dateChecker->isOneDelimitedDate(nextWord)||isNextDayInInvertedCommas(nextWord)){
                _tokenizedUserEntry[index + 1] = nextWordOriginal.substr(1,nextWordOriginal.length()-2);
            }
        
        } else if(checkTime) {
            if(timeChecker->is12HTime(nextWord)||timeChecker->is24HTime(nextWord)||
                timeChecker->isTimeWithoutPeriod(nextWord)){
                _tokenizedUserEntry[index + 1] = nextWordOriginal.substr(1,nextWordOriginal.length()-2);
            }
        }
    }

    return;
}

bool TMParser::isNextDayInInvertedCommas(std::string nextWord) {
    DateChecker *dateChecker = DateChecker::getInstance();

    if(nextWord.length() < 8) {
        return false;
    }

    std::string firstPart = nextWord.substr(0,4);

    if(firstPart != TOKEN_NEXT) {
        return false;
    }

    std::string secondPart = nextWord.substr(5);

    if(dateChecker->isDay(secondPart)) {
        return true;
    } else {
        return false;
    }
}

void TMParser::configureAllDatesAndTimes(std::string& startDate, std::string& startTime, std::string& endDate, std::string& endTime, TaskType& taskType){
    FormatConverter *formatConverter = FormatConverter::getInstance();
    TimeChecker *timeChecker = TimeChecker::getInstance();

    if((startTime != ""||startDate != "") && endTime == "" && endDate == ""){
        taskType = TaskType::WithStartDateTime;
        if(startTime == ""){
            startTime = "0000";
        } else if(startDate == ""){
            if(timeChecker->isTimeWithoutPeriod(startTime)) {
                configureStartTimeWithoutPeriods(startTime);
            }
            startDate = formatConverter->dateFromBoostToDDMMYYYY(currentDate());
            if(startTime <= currentTime()){
                startDate = addNDaysFromDate(startDate,1);
             }
        } else {
        //startTime and startDate are found
        }
        endDate = startDate;
        endTime = startTime;

    } else if((endTime != ""||endDate != "") && startTime == "" && startDate == "") {
        taskType = TaskType::WithPeriod;
        if(endTime == ""){
            endTime = "0000";
        } else if(endDate == ""){
            if(timeChecker->isTimeWithoutPeriod(endTime)) {
                configureEndTimeWithoutPeriods(endTime);
            }
            endDate = formatConverter->dateFromBoostToDDMMYYYY(currentDate());
            if(endTime <= currentTime()){
                endDate = addNDaysFromDate(endDate,1);
             }
        } else {
        //endTime and endDate are found
        }
        //no startDateTime, assumed to be current date and atime
        startDate = formatConverter->dateFromBoostToDDMMYYYY(currentDate());
        startTime = currentTime();

    } else if((startDate != ""||startTime != "") && (endDate != ""||endTime != "")){
        taskType = TaskType::WithPeriod;
        if(startTime == ""){
            //startDate only
            startTime = "0000";
            //check if there's no endDate but there's endTime then endDate will be startDate
            //else there is no endTime but there's endDate then endTime will be 2359
            if(endDate == ""){
                //if endTime = 0000 plus 1 more day?
                endDate = startDate;
            } else if(endTime == ""){
                endTime = "2359";
            }
        } else if(startDate == ""){
            //startTime only
            //no endDate got endTime then should be today till today/tomorrow
            //got endDate no endTime

            if(endDate == "") {
                if(timeChecker->isTimeWithoutPeriod(startTime) && timeChecker->isTimeWithoutPeriod(endTime)) {
                    configureStartTimeEndTimeWithoutPeriods(startTime, endTime);
                }
                startDate = formatConverter->dateFromBoostToDDMMYYYY(currentDate());
                if(endTime >= startTime){
                    endDate = startDate;
                } else { 
                    endDate = addNDaysFromDate(startDate,1);
                }
            } else if (endTime == ""){
                startDate = endDate;
                endTime = "2359";
            } else {
                startDate = endDate;
                if(startTime >= endTime){
                    
                    startDate = substractNDaysFromDate(startDate,1);
                }
            }
        } else {
            //startTime and startDate are found
            if(endTime == ""){
                endTime = "2359";
            } else if (endDate == ""){
                if(timeChecker->isTimeWithoutPeriod(endTime)) {
                    configureEndTimeWithoutPeriods(endTime);
                }
                endDate = startDate;
                if(endTime < startTime){
                    endDate = addNDaysFromDate(endDate,1);
                }
            } else {
                //all attributes found
                if(timeChecker->isTimeWithoutPeriod(startTime) && timeChecker->isTimeWithoutPeriod(endTime)) {
                    configureStartTimeEndTimeWithoutPeriods(startTime, endTime);
                }
            }
        }
    }
}

void TMParser::configureStartTimeEndTimeWithoutPeriods(std::string& stringStartTime, std::string& stringEndTime) {
    int startTime = std::stoi(stringStartTime);
    int endTime = std::stoi(stringEndTime);
    std::string startTimeWithPeriod;
    std::string endTimeWithPeriod;
    FormatConverter *formatConverter = FormatConverter::getInstance();
    TimeChecker *timeChecker = TimeChecker::getInstance();

    if(startTime >= 8 && startTime <= 11) {
        startTimeWithPeriod = stringStartTime + "am";
        if(endTime == 12 || (endTime >= 1 && endTime <=8)) {
            endTimeWithPeriod = stringEndTime + "pm";
        } else {
            if(endTime > startTime) {
                endTimeWithPeriod = stringEndTime + "am";
            } else {
                endTimeWithPeriod = stringEndTime + "pm";
            }
        }
    } else {
        startTimeWithPeriod = stringStartTime + "pm";
        if(endTime >= 9 && endTime <= 11) {
            endTimeWithPeriod = stringEndTime + "pm";
        } else {
            if(startTime == 12) {
                if(startTime > endTime) {
                    endTimeWithPeriod = stringEndTime + "pm";
                } else {
                    endTimeWithPeriod = stringEndTime + "am";
                }
            } else {
                if(startTime < endTime && endTime != 12) {
                    endTimeWithPeriod = stringEndTime + "pm";
                } else {
                    endTimeWithPeriod = stringEndTime + "am";
                }
            }
        }
    }
    
    if(timeChecker->isAM(startTimeWithPeriod)) {
        stringStartTime = formatConverter->timeFrom12HourAMToHHMM(startTimeWithPeriod);
    } else {
        stringStartTime = formatConverter->timeFrom12HourPMToHHMM(startTimeWithPeriod);
    }

    if(timeChecker->isAM(endTimeWithPeriod)) {
        stringEndTime = formatConverter->timeFrom12HourAMToHHMM(endTimeWithPeriod);
    } else {
        stringEndTime = formatConverter->timeFrom12HourPMToHHMM(endTimeWithPeriod);
    }

    return;
}

void TMParser::configureStartTimeWithoutPeriods(std::string& stringStartTime) {
    int startTime = std::stoi(stringStartTime);
    std::string startTimeWithPeriod;
    FormatConverter *formatConverter = FormatConverter::getInstance();

    if(startTime >= 8 && startTime <= 11) {
        startTimeWithPeriod = stringStartTime + "am";
        stringStartTime = formatConverter->timeFrom12HourAMToHHMM(startTimeWithPeriod);
    } else {
        startTimeWithPeriod = stringStartTime + "pm";
        stringStartTime = formatConverter->timeFrom12HourPMToHHMM(startTimeWithPeriod);
    }
    
    return;
}

void TMParser::configureEndTimeWithoutPeriods(std::string& stringEndTime) {
    int endTime = std::stoi(stringEndTime);
    std::string endTimeWithPeriod;
    FormatConverter *formatConverter = FormatConverter::getInstance();

    if(endTime == 12 || (endTime >= 1 && endTime <=8)) {
        endTimeWithPeriod = stringEndTime + "pm";
        stringEndTime = formatConverter->timeFrom12HourPMToHHMM(endTimeWithPeriod);
    } else {
        endTimeWithPeriod = stringEndTime + "am";
        stringEndTime = formatConverter->timeFrom12HourAMToHHMM(endTimeWithPeriod);
    }

    return;
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

bool TMParser::isPositiveInteger(std::string token) {
    for(std::string::iterator it = token.begin(); it < token.end(); it++) {
        if(!isdigit(*it)) {
            return false;
        }
    }

    int integer = std::stoi(token);
    if(integer > 0) {
        return true;
    } else {
        return false;
    }
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

//precondition: command extracted
//postcondition: returns unique and valid indices
std::vector<int> TMParser::parseTaskPositionNo() {
    int intTaskPositionNo;
    std::vector<int> vectorTaskPositionNo;
    std::string token;
    int vectorSize = _tokenizedUserEntry.size();

    for(int i = 0; i < vectorSize; i++) {
        token = _tokenizedUserEntry[i];
        int positionOfFirstDash = token.find_first_of("-");

        if(isPositiveInteger(token)) {//will skip the first "-"
            intTaskPositionNo = std::stoi(token);
            
            if(isUniqueIndex(intTaskPositionNo, vectorTaskPositionNo)) {
                vectorTaskPositionNo.push_back(intTaskPositionNo);
            }
        } else if (positionOfFirstDash != std::string::npos && token[0] != '-'
            && token[token.length()-1] != '-'){
                std::string firstHalfOfToken = token.substr(0, positionOfFirstDash);
                std::string secondHalfOfToken = token.substr(positionOfFirstDash + 1);

                if(isPositiveInteger(firstHalfOfToken) && isPositiveInteger(secondHalfOfToken)){
                    int firstInteger = std::stoi(firstHalfOfToken);
                    int secondInteger = std::stoi(secondHalfOfToken);

                    while(firstInteger <= secondInteger) {
                        if(isUniqueIndex(firstInteger, vectorTaskPositionNo)) {
                            vectorTaskPositionNo.push_back(firstInteger);
                        }
                        firstInteger++;
                    }
                }
        }
    }
    return vectorTaskPositionNo;
}

bool TMParser::isUniqueIndex(int index, std::vector<int> vectorTaskPositionNumber) {
    std::vector<int>::iterator iter;
    iter = std::find(vectorTaskPositionNumber.begin(), vectorTaskPositionNumber.end(), index);
    return (iter == vectorTaskPositionNumber.end());
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

//preconditions ddmmyyyy
std::string TMParser::substractNDaysFromDate(std::string date, int n){
    FormatConverter *formatConverter = FormatConverter::getInstance();
    date = formatConverter->dateFromNumericToBoostFormat(date);
    boost::gregorian::date initialBoostDate = boost::gregorian::from_uk_string(date);
    boost::gregorian::date_duration dateDuration(n);
    boost::gregorian::date finalBoostDate = initialBoostDate - dateDuration;
    return formatConverter->dateFromBoostToDDMMYYYY(finalBoostDate);
}

//preconditions ddmmyyyy
std::string TMParser::addNDaysFromDate(std::string date, int n){
    FormatConverter *formatConverter = FormatConverter::getInstance();
    date = formatConverter->dateFromNumericToBoostFormat(date);
    boost::gregorian::date initialBoostDate = boost::gregorian::from_uk_string(date);
    boost::gregorian::date_duration dateDuration(n);
    boost::gregorian::date finalBoostDate = initialBoostDate + dateDuration;
    return formatConverter->dateFromBoostToDDMMYYYY(finalBoostDate);
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
