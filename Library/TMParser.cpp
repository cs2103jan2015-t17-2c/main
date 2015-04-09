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

        if(unitString == TOKEN_BEFORE||unitString == TOKEN_BY||unitString == TOKEN_SHORTCUT_BEFORE) {
            if(index + 1 == lengthOfTokenizedUserEntry){
                break;
            }
            nextWord = formatConverter->returnLowerCase(_tokenizedUserEntry[index + 1]);
            if(dateChecker->isNumericDate(nextWord)||dateChecker->isDay(nextWord)||
                dateChecker->isOneDelimitedDate(nextWord)||
                dateChecker->isSpacedDate(index + 1, _tokenizedUserEntry)) {
                //e.g. before 01012016 (DDMMYYYY)
                indexOfDatesAndTimes.push(index);
                dateToMeet = extractor->extractDayOrNumericDateOrDelimitedDate(index + 1, indexOfDatesAndTimes, _tokenizedUserEntry);
                if(unitString == TOKEN_BEFORE||unitString == TOKEN_SHORTCUT_BEFORE) {
                    dateToMeet = substractNDaysFromDate(dateToMeet,1);
                }
                timeToMeet = "2359";
                index = indexOfDatesAndTimes.back();
            } else if (dateChecker->isNextDay(index + 1, _tokenizedUserEntry)) {
                //e.g. before next monday
                indexOfDatesAndTimes.push(index);
                dateToMeet = extractor->extractNextDay(index + 1, indexOfDatesAndTimes, _tokenizedUserEntry);
                if(unitString == TOKEN_BEFORE||unitString == TOKEN_SHORTCUT_BEFORE) {
                    dateToMeet = substractNDaysFromDate(dateToMeet,1);
                }
                timeToMeet = "2359";
                index = indexOfDatesAndTimes.back();
            } else if (timeChecker->is12HTime(nextWord)||timeChecker->is24HTime(nextWord)||
                timeChecker->isTimeWithoutPeriod(nextWord)) {
                    indexOfDatesAndTimes.push(index);
                    timeToMeet = extractor->extractTime(index + 1, indexOfDatesAndTimes, _tokenizedUserEntry);
                    index = indexOfDatesAndTimes.back();
            } else if (dateChecker->isToday(nextWord)) {
                //intepreted as before end of today or by today
                indexOfDatesAndTimes.push(index);
                dateToMeet = extractor->extractToday(index + 1, indexOfDatesAndTimes, _tokenizedUserEntry);
                timeToMeet = "2359";
                index = indexOfDatesAndTimes.back();
            } else if (dateChecker->isTomorrow(nextWord)) {
                indexOfDatesAndTimes.push(index);
                dateToMeet = extractor->extractTomorrow(index + 1, indexOfDatesAndTimes, _tokenizedUserEntry);
                if(unitString == TOKEN_BEFORE||unitString == TOKEN_SHORTCUT_BEFORE) {
                    dateToMeet = substractNDaysFromDate(dateToMeet,1);
                }
                timeToMeet = "2359";
                index = indexOfDatesAndTimes.back();
            } else {
                editDateOrTimeInInvertedCommas(nextWord, index + 1, true, true);
            }
        } else if(unitString == TOKEN_ON) {
            if(index + 1 == lengthOfTokenizedUserEntry){
                break;
            }
            nextWord = formatConverter->returnLowerCase(_tokenizedUserEntry[index + 1]);
            if(dateChecker->isNumericDate(nextWord)||dateChecker->isDay(nextWord)||
                dateChecker->isOneDelimitedDate(nextWord)||
                dateChecker->isSpacedDate(index + 1, _tokenizedUserEntry)) {
                indexOfDatesAndTimes.push(index);
                dateToMeet = extractor->extractDayOrNumericDateOrDelimitedDate(index + 1,indexOfDatesAndTimes, _tokenizedUserEntry);
                index = indexOfDatesAndTimes.back();

                if(timeToMeet == "") {
                    timeToMeet = "2359";
                }
            } else {
                editDateOrTimeInInvertedCommas(nextWord, index + 1, true, false);
            }
        } else if(dateChecker->isNextDay(index, _tokenizedUserEntry)){
            dateToMeet = extractor->extractNextDay(index, indexOfDatesAndTimes, _tokenizedUserEntry);
            index = indexOfDatesAndTimes.back();
        } else if(dateChecker->isToday(unitString)){
            dateToMeet = extractor->extractToday(index, indexOfDatesAndTimes, _tokenizedUserEntry);
            index = indexOfDatesAndTimes.back();
        } else if(dateChecker->isTomorrow(unitString)){
            dateToMeet = extractor->extractTomorrow(index, indexOfDatesAndTimes, _tokenizedUserEntry);
            index = indexOfDatesAndTimes.back();
        } else {
            editDateOrTimeInInvertedCommas(unitString, index, true, false);
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

    if(dateToMeet.length() == 4) {
        configureDayMonth(dateToMeet);
    }

    if(timeChecker->isTimeWithoutPeriod(timeToMeet)) {
        configureEndTimeWithoutPeriods(timeToMeet);
    }

    if(dateToMeet == "") {
        dateToMeet = formatConverter->dateFromBoostToDDMMYYYY(currentDate());
        if(timeToMeet <= currentTime()){
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

        if(unitString == TOKEN_AT||unitString == TOKEN_SHORTCUT_AT) {
            if(index + 1 == lengthOfTokenizedUserEntry){
                break;
            }
            nextWord = formatConverter->returnLowerCase(_tokenizedUserEntry[index + 1]);
            if(timeChecker->is12HTime(nextWord)||timeChecker->is24HTime(nextWord)||
                timeChecker->isTimeWithoutPeriod(nextWord)){
                    startTime = extractor->extractTime(index + 1, indexOfDatesAndTimes, _tokenizedUserEntry);
                    configureQueuesAndIndexAfterToken(mainIndexOfDatesAndTimes, indexOfDatesAndTimes, index);
            } else {
                editDateOrTimeInInvertedCommas(nextWord, index + 1, false, true);
            }
        } else if(unitString == TOKEN_ON) {
            if(index + 1 == lengthOfTokenizedUserEntry){
                break;
            }
            nextWord = formatConverter->returnLowerCase(_tokenizedUserEntry[index + 1]);
            if(dateChecker->isNumericDate(nextWord)||dateChecker->isDay(nextWord)||
                dateChecker->isOneDelimitedDate(nextWord)||
                dateChecker->isSpacedDate(index + 1, _tokenizedUserEntry)){
                    startDate = extractor->extractDayOrNumericDateOrDelimitedDate(index + 1, indexOfDatesAndTimes, _tokenizedUserEntry);
                    configureQueuesAndIndexAfterToken(mainIndexOfDatesAndTimes, indexOfDatesAndTimes, index);
                    index = mainIndexOfDatesAndTimes.back();
            } else {
                editDateOrTimeInInvertedCommas(nextWord, index + 1, true, false);
            }
        } else if(unitString == TOKEN_FROM||unitString == TOKEN_SHORTCUT_FROM){
            if(index + 1 == lengthOfTokenizedUserEntry){
                break;
            }
            nextWord = formatConverter->returnLowerCase(_tokenizedUserEntry[index + 1]);
            if(dateChecker->isNumericDate(nextWord)||dateChecker->isDay(nextWord)||
                dateChecker->isOneDelimitedDate(nextWord)||
                dateChecker->isNextDay(index + 1, _tokenizedUserEntry)||
                timeChecker->is12HTime(nextWord)||timeChecker->is24HTime(nextWord)||
                timeChecker->isTimeWithoutPeriod(nextWord)||
                dateChecker->isToday(nextWord)||dateChecker->isTomorrow(nextWord)||
                dateChecker->isSpacedDate(index + 1, _tokenizedUserEntry)) {
                    extractor->extractDateAndOrTime(index + 1, indexOfDatesAndTimes, startDate, startTime, _tokenizedUserEntry);
                    configureQueuesAndIndexAfterToken(mainIndexOfDatesAndTimes, indexOfDatesAndTimes, index);
                    index = mainIndexOfDatesAndTimes.back();
            } else {
                editDateOrTimeInInvertedCommas(nextWord, index + 1, true, true);
            }
        } else if (unitString == TOKEN_TO||unitString == TOKEN_SHORTCUT_TO||unitString == TOKEN_DASH){
            if(index + 1 == lengthOfTokenizedUserEntry){
                break;
            }
            nextWord = formatConverter->returnLowerCase(_tokenizedUserEntry[index + 1]);
            if(dateChecker->isNumericDate(nextWord)||dateChecker->isDay(nextWord)||
                dateChecker->isOneDelimitedDate(nextWord)||
                dateChecker->isNextDay(index + 1, _tokenizedUserEntry)||
                timeChecker->is12HTime(nextWord)||timeChecker->is24HTime(nextWord)||
                timeChecker->isTimeWithoutPeriod(nextWord)||
                dateChecker->isToday(nextWord)||dateChecker->isTomorrow(nextWord)||
                dateChecker->isSpacedDate(index + 1, _tokenizedUserEntry)) {
                    extractor->extractDateAndOrTime(index + 1, indexOfDatesAndTimes, endDate, endTime, _tokenizedUserEntry);
                    configureQueuesAndIndexAfterToken(mainIndexOfDatesAndTimes, indexOfDatesAndTimes, index);
                    index = mainIndexOfDatesAndTimes.back();
            } else {
                editDateOrTimeInInvertedCommas(nextWord, index + 1, true, true);
            }
        } else if(dateChecker->isNextDay(index, _tokenizedUserEntry)) {
            startDate = extractor->extractNextDay(index, indexOfDatesAndTimes, _tokenizedUserEntry);
            configureQueuesAndIndex(mainIndexOfDatesAndTimes, indexOfDatesAndTimes, index);
        } else if (dateChecker->isToday(unitString)) {
            startDate = extractor->extractToday(index, indexOfDatesAndTimes, _tokenizedUserEntry);
            configureQueuesAndIndex(mainIndexOfDatesAndTimes, indexOfDatesAndTimes, index);
        } else if (dateChecker->isTomorrow(unitString)) {
            startDate = extractor->extractTomorrow(index, indexOfDatesAndTimes, _tokenizedUserEntry);
            configureQueuesAndIndex(mainIndexOfDatesAndTimes, indexOfDatesAndTimes, index);
        } else {
            editDateOrTimeInInvertedCommas(unitString, index, true, false);
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

        if(unitString == TOKEN_BEFORE||unitString == TOKEN_BY||unitString == TOKEN_SHORTCUT_BEFORE) {
            if(index + 1 == lengthOfVector){
                break;
            }
            nextWord = formatConverter->returnLowerCase(_tokenizedUserEntry[index + 1]);
            editDateOrTimeInInvertedCommas(nextWord, index + 1, true, true);
        } else if(unitString == TOKEN_ON) {
            if(index + 1 == lengthOfVector){
                break;
            }
            nextWord = formatConverter->returnLowerCase(_tokenizedUserEntry[index + 1]);
            editDateOrTimeInInvertedCommas(nextWord, index + 1, true, false);
        } else if(unitString == TOKEN_AT||unitString == TOKEN_SHORTCUT_AT){
            if(index + 1 == lengthOfVector){
                break;
            }
            nextWord = formatConverter->returnLowerCase(_tokenizedUserEntry[index + 1]);
            editDateOrTimeInInvertedCommas(nextWord, index, false, true);
        } else if(unitString == TOKEN_FROM||unitString == TOKEN_SHORTCUT_FROM||
            unitString == TOKEN_TO||unitString == TOKEN_SHORTCUT_TO){
                if(index + 1 == lengthOfVector){
                break;
                }
                nextWord = formatConverter->returnLowerCase(_tokenizedUserEntry[index + 1]);
                editDateOrTimeInInvertedCommas(nextWord, index + 1, true, true);
            //index = index + 1;?
        } else {
            editDateOrTimeInInvertedCommas(unitString, index, true, false);
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
                        if(_tokenizedUserEntry[indexOfLastExtracted + 1] == TOKEN_DASH||
                            _tokenizedUserEntry[indexOfLastExtracted + 1] == TOKEN_SHORTCUT_TO||
                            _tokenizedUserEntry[indexOfLastExtracted + 1] == TOKEN_TO){
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
                    if(_tokenizedUserEntry[indexOfLastExtracted + 1] == TOKEN_DASH||
                       _tokenizedUserEntry[indexOfLastExtracted + 1] == TOKEN_SHORTCUT_TO||
                       _tokenizedUserEntry[indexOfLastExtracted + 1] == TOKEN_TO) {
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
        //will be -1 (invalid index) if index is empty 
        int frontIndexOfQueue = -1;

        if(!mainIndexOfDatesAndTimes.empty()) {
            frontIndexOfQueue = mainIndexOfDatesAndTimes.front();
        }

        if(i == frontIndexOfQueue) {
            mainIndexOfDatesAndTimes.pop();
        } else {
            std::string token = _tokenizedUserEntry[i];
            taskDescription += token;
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
        std::string nextWordOriginal = _tokenizedUserEntry[index];

        if(checkDate && checkTime) {
            if(timeChecker->is12HTime(nextWord)||timeChecker->is24HTime(nextWord)||
                timeChecker->isTimeWithoutPeriod(nextWord)||dateChecker->isNumericDate(nextWord)||
                dateChecker->isDay(nextWord)||dateChecker->isOneDelimitedDate(nextWord)||
                isNextDayInInvertedCommas(nextWord)||
                dateChecker->isToday(nextWord)||dateChecker->isTomorrow(nextWord)||
                isSpacedDateInInvertedCommas(nextWord)){
                    _tokenizedUserEntry[index] = nextWordOriginal.substr(1,nextWordOriginal.length()-2);
            }
        } else if(checkDate) {
            if(dateChecker->isNumericDate(nextWord)||dateChecker->isDay(nextWord)||
               dateChecker->isOneDelimitedDate(nextWord)||isNextDayInInvertedCommas(nextWord)||
               dateChecker->isToday(nextWord)||dateChecker->isTomorrow(nextWord)||
               isSpacedDateInInvertedCommas(nextWord)){
                    _tokenizedUserEntry[index] = nextWordOriginal.substr(1,nextWordOriginal.length()-2);
            }
        
        } else if(checkTime) {
            if(timeChecker->is12HTime(nextWord)||timeChecker->is24HTime(nextWord)||
                timeChecker->isTimeWithoutPeriod(nextWord)){
                _tokenizedUserEntry[index] = nextWordOriginal.substr(1,nextWordOriginal.length()-2);
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

    if(!(firstPart == TOKEN_NEXT||firstPart == TOKEN_SHORTCUT_NEXT)) {
        return false;
    }

    std::string secondPart = nextWord.substr(5);

    if(dateChecker->isDay(secondPart)) {
        return true;
    } else {
        return false;
    }
}

bool TMParser::isSpacedDateInInvertedCommas(std::string nextWord) {
    DateChecker *dateChecker = DateChecker::getInstance();
    std::vector<std::string> tokenizedNextWord;
    std::istringstream iss(nextWord);
    std::string token;

    while(iss >> token){
        tokenizedNextWord.push_back(token);
    }

    return dateChecker->isSpacedDate(0, tokenizedNextWord);
}

void TMParser::configureAllDatesAndTimes(std::string& startDate, std::string& startTime, std::string& endDate, std::string& endTime, TaskType& taskType){
    FormatConverter *formatConverter = FormatConverter::getInstance();
    TimeChecker *timeChecker = TimeChecker::getInstance();

    if((startTime != ""||startDate != "") && endTime == "" && endDate == ""){
        taskType = TaskType::WithStartDateTime;
        configureDayMonth(startDate);
        if(timeChecker->isTimeWithoutPeriod(startTime)) {
            configureStartTimeWithoutPeriods(startTime);
        }

        if(startTime == ""){
            startTime = "0000";
            
        } else if(startDate == ""){
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
        configureDayMonth(endDate);
        if(timeChecker->isTimeWithoutPeriod(endTime)) {
            configureEndTimeWithoutPeriods(endTime);
        }

        if(endTime == ""){
            endTime = "0000";
        } else if(endDate == ""){
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

    } else if((startDate != ""||startTime != "") && (endDate != ""||endTime != "")) {
        taskType = TaskType::WithPeriod;
        
        if(startTime == "" && endDate == "") {
            //startDate endTime
            //startTime = 0000 endDate = startDate

            startTime = "0000";
            
            configureDayMonth(startDate);

            //if today's date add 1 more year
            if(startDate == formatConverter->dateFromBoostToDDMMYYYY(currentDate())) {
                startDate = addNYearsFromDate(startDate,1);
             }

             if(timeChecker->isTimeWithoutPeriod(endTime)) {
                configureEndTimeWithoutPeriods(endTime);
             }

             endDate = startDate;

             //if endTime = 0000 plus 1 more day
             if(endTime == "0000") {
                endDate = addNDaysFromDate(endDate, 1);
             }
        
        } else if(startTime == "" && endTime == "") {
            //startDate endDate
            
            startTime = "0000";
            endTime = "2359";

            if(startDate.length() == 4 && endDate.length() == 4) {
                configureStartDayMonthEndDayMonth(startDate, endDate);
            } else if(startDate.length() == 4 && endDate.length() == 8) {
                //IF 4 8 LET STARTDATE TAKE THE YEAR OF 8 AND IF >= MINUS 1 YEAR
                startDate = startDate + endDate.substr(4);
            } else if(startDate.length() == 8 && endDate.length() == 4) {
                    //IF 8 4 LET ENDDATE TAKE THE YEAR OF 8 AND IF <= PLUS 1 YEAR
                    endDate = endDate + startDate.substr(4);
                    //INTRODUCE CHECKER!
            }

        } else if (startTime == "" && !(endTime == ""||endDate == "")) {
            //startDate endDate endTime
            if(startDate.length() == 4 && endDate.length() == 4) {
                //startDate not occured i.e. from tomorrow onwards
                //endDate more than or equal to startDate
                //if startDate == endDate and endTime == 0000 add 1 more YEAR
                configureStartDayMonthEndDayMonth(startDate, endDate);
            } else if (startDate.length() == 4 && endDate.length() == 8) {
                startDate = startDate + endDate.substr(4);
                //if startDate not less than endDate, startDate - 1 year
                //OR if startDate == endDate && endTime == 0000 startDate - 1 year
            } else if (startDate.length() == 8 && endDate.length() == 4) {
                endDate = endDate + startDate.substr(4);
                //if endDate not more than startDate endDate + 1 year
                //OR if endDate == startDate endDate == 0000, endDate + 1 year
            }
                
            //MAY NEED TO REMOVE
            configureEndTimeWithoutPeriods(endTime);
               
        } else if(startDate == "" && endDate == "") {
            //startTime only
            //no endDate got endTime then should be today till today/tomorrow
            //got endDate no endTime
            if(timeChecker->isTimeWithoutPeriod(startTime) && timeChecker->isTimeWithoutPeriod(endTime)) {
                configureStartTimeEndTimeWithoutPeriods(startTime, endTime);
                //IF STARTTIME HAS PASSED <= CURRENTTIME ADD ONE DAY TO STARTDATE AND ENDDATE
            } else if (timeChecker->isTimeWithoutPeriod(startTime) && endTime.length() == 4) {
                std::string timeInAM = formatConverter->timeFrom12HourAMToHHMM(startTime + "am");
                std::string timeInPM = formatConverter->timeFrom12HourPMToHHMM(startTime + "pm");
                
                if(endTime == "0000") {
                    startTime = timeInPM;
                } else if (timeInPM < endTime) {
                    startTime = timeInPM;
                } else if (timeInAM < endTime) {
                    startTime = timeInAM;
                } else {
                    startTime = timeInPM;
                }
            } else if (startTime.length() == 4 && timeChecker->isTimeWithoutPeriod(endTime)) {
                std::string timeInAM = formatConverter->timeFrom12HourAMToHHMM(endTime + "am");
                std::string timeInPM = formatConverter->timeFrom12HourPMToHHMM(endTime + "pm");

                if(startTime == "0000") {
                    if(timeInAM != "0000") {
                        endTime = timeInAM; 
                    } else {
                        endTime = timeInPM;
                    }
                } else if (timeInAM > startTime) {
                    endTime = timeInAM;
                } else if (timeInPM > startTime) {
                    endTime = timeInPM;
                } else {
                    endTime = timeInAM;
                }
            }
            
            startDate = formatConverter->dateFromBoostToDDMMYYYY(currentDate());

            //if time passed then start date is tomorrow's date
            if(startTime <= currentTime()) {
                startDate = addNDaysFromDate(startDate, 1);
            }

            if(endTime > startTime){
                endDate = startDate;
            } else { 
                endDate = addNDaysFromDate(startDate,1);
            }
        
        } else if (startDate == "" && endTime == "") {
            //startTime endDate
            //today or tomorrow if time has passed
            //endDate may be 4 or 8
            if(timeChecker->isTimeWithoutPeriod(startTime)) {
                configureStartTimeWithoutPeriods(startTime);
            }
            
            startDate = formatConverter->dateFromBoostToDDMMYYYY(currentDate());
            //if start time has passed
            
            if(startTime <= currentTime()) {
                startDate = addNDaysFromDate(startDate, 1);
            }

            if(endDate.length() == 4) {
                configureStartDayMonthEndDayMonth(startDate.substr(0,4),endDate);
            }
                endTime = "2359";
        } else if (startDate == "" && !(endDate == ""||endTime == "")){
            //startTime endDate endTime
            //startDate = endDate or one day earlier
            if(timeChecker->isTimeWithoutPeriod(startTime) && timeChecker->isTimeWithoutPeriod(endTime)) {
                configureStartTimeEndTimeWithoutPeriods(startTime, endTime);
            } else if (timeChecker->isTimeWithoutPeriod(startTime) && endTime.length() == 4) {

                std::string timeInAM = formatConverter->timeFrom12HourAMToHHMM(startTime + "am");
                std::string timeInPM = formatConverter->timeFrom12HourPMToHHMM(startTime + "pm");

                if(endTime == "0000") {
                    startTime = timeInPM;
                } else if (timeInPM < endTime) {
                    startTime = timeInPM;
                } else if (timeInAM < endTime) {
                    startTime = timeInAM;
                } else {
                    startTime = timeInPM;
                }
            } else if (startTime.length() == 4 && timeChecker->isTimeWithoutPeriod(endTime)) {

                std::string timeInAM = formatConverter->timeFrom12HourAMToHHMM(endTime + "am");
                std::string timeInPM = formatConverter->timeFrom12HourPMToHHMM(endTime + "pm");

                if(startTime == "0000") {
                    if(timeInAM != "0000") {
                        endTime = timeInAM; 
                    } else {
                        endTime = timeInPM;
                    }
                } else if (timeInAM > startTime) {
                    endTime = timeInAM;
                } else if (timeInPM > startTime) {
                    endTime = timeInPM;
                } else if (timeInPM <= startTime && timeInAM <= startTime && endTime != "12") {
                    endTime = timeInAM;
                } else if (timeInPM <= startTime && timeInAM <= startTime && endTime == "12") {
                    endTime = timeInPM;
                }
            }
            
            startDate = endDate;

            if(startTime >= endTime){
                startDate = substractNDaysFromDate(startDate,1);
            }

        } else if (!(startDate == ""||startTime == "") && endTime == "") {
            //startTime and startDate are found
            //startDate startTime endDate 2359
            //4 8
            //8 4
            if(timeChecker->isTimeWithoutPeriod(startTime)) {
                configureStartTimeWithoutPeriods(startTime);
            }

            if(startDate.length() == 4 && endDate.length() == 4) {
                configureStartDayMonthEndDayMonth(startDate, endDate);
            } else if (startDate.length() == 4 && endDate.length() == 8) {
                startDate = startDate + endDate.substr(4);
                //if startDate not less than endDate, startDate - 1 year
                //OR if startDate == endDate && endTime == 0000 startDate - 1 year
                std::string delimitedStartDate = formatConverter->dateFromNumericToBoostFormat(startDate);
                std::string delimitedEndDate = formatConverter->dateFromNumericToBoostFormat(endDate);
                boost::gregorian::date boostStartDate = boost::gregorian::from_uk_string(delimitedStartDate);
                boost::gregorian::date boostEndDate = boost::gregorian::from_uk_string(delimitedEndDate);

                if(boostEndDate <= boostStartDate) {
                    startDate = subtractNYearsFromDate(startDate, 1);
                } else if(boostStartDate == boostEndDate && endTime == "0000") {
                    startDate = subtractNYearsFromDate(startDate, 1);
                }
            
            } else if (startDate.length() == 8 && endDate.length() == 4) {
                endDate = endDate + startDate.substr(4);
                //if endDate not more than startDate endDate + 1 year
                //OR if endDate == startDate endDate == 0000, endDate + 1 year
                std::string delimitedStartDate = formatConverter->dateFromNumericToBoostFormat(startDate);
                std::string delimitedEndDate = formatConverter->dateFromNumericToBoostFormat(endDate);
                boost::gregorian::date boostStartDate = boost::gregorian::from_uk_string(delimitedStartDate);
                boost::gregorian::date boostEndDate = boost::gregorian::from_uk_string(delimitedEndDate);

                if(boostEndDate <= boostStartDate) {
                    endDate = addNYearsFromDate(endDate, 1);
                } else if(boostStartDate == boostEndDate && endTime == "0000") {
                    endDate = addNYearsFromDate(endDate, 1);
                
                }
            }

            endTime = "2359";
        
        } else if (!(startDate == ""||startTime == "") && endDate == ""){
            //startDate startTime endTime
            //if today's date find next year's date > currentDate
            //configure TIME!
            configureDayMonth(startDate);

            if(timeChecker->isTimeWithoutPeriod(endTime) && timeChecker->isTimeWithoutPeriod(startTime)) {
                configureStartTimeEndTimeWithoutPeriods(startTime, endTime);
            } else if (timeChecker->isTimeWithoutPeriod(startTime) && endTime.length() == 4) {

                std::string timeInAM = formatConverter->timeFrom12HourAMToHHMM(startTime + "am");
                std::string timeInPM = formatConverter->timeFrom12HourPMToHHMM(startTime + "pm");

                if(endTime == "0000") {
                    startTime = timeInPM;
                } else if (timeInPM < endTime) {
                    startTime = timeInPM;
                } else if (timeInAM < endTime) {
                    startTime = timeInAM;
                } else {
                    startTime = timeInPM;
                }
            } else if (startTime.length() == 4 && timeChecker->isTimeWithoutPeriod(endTime)) {

                std::string timeInAM = formatConverter->timeFrom12HourAMToHHMM(endTime + "am");
                std::string timeInPM = formatConverter->timeFrom12HourPMToHHMM(endTime + "pm");

                if(startTime == "0000") {
                    if(timeInAM != "0000") {
                        endTime = timeInAM; 
                    } else {
                        endTime = timeInPM;
                    }
                } else if (timeInAM > startTime) {
                    endTime = timeInAM;
                } else if (timeInPM > startTime) {
                    endTime = timeInPM;
                } else {
                    endTime = timeInAM;
                }
            }

            endDate = startDate;
            if(endTime <= startTime){
                endDate = addNDaysFromDate(endDate,1);
            }
        } else {
            //all attributes found
            configureStartDayMonthEndDayMonth(startDate, endDate);
            if(timeChecker->isTimeWithoutPeriod(startTime) && timeChecker->isTimeWithoutPeriod(endTime)) {
                configureStartTimeEndTimeWithoutPeriods(startTime, endTime);
            }
        }
    }
}

//preconditions date is only 4 digits in length
//postconditions 
void TMParser::configureDayMonth(std::string& stringDate) {
    if(stringDate.length() != 4) {
        return;
    }

    FormatConverter *formatConverter = FormatConverter::getInstance();
    DateChecker *dateChecker = DateChecker::getInstance();

    std::string stringCurrentDate = formatConverter->dateFromBoostToDDMMYYYY(currentDate());
    std::string stringCurrentYear = stringCurrentDate.substr(4);
    stringDate = stringDate + stringCurrentYear;
    std::string ddmm = stringDate.substr(0,4);

    if(!dateChecker->isUnoccurredDate(stringDate)) {
        stringDate = addNYearsFromDate(stringDate, 1);
    }

    return;
}

//preconditions date is only 4 digits in length
void TMParser::configureStartDayMonthEndDayMonth(std::string& startDate, std::string& endDate) {
    FormatConverter *formatConverter = FormatConverter::getInstance();
    DateChecker *dateChecker = DateChecker::getInstance();

    std::string stringCurrentDate = formatConverter->dateFromBoostToDDMMYYYY(currentDate());
    std::string stringCurrentYear = stringCurrentDate.substr(4);

    std::string stringStartDate = startDate + stringCurrentYear;
    std::string stringEndDate = endDate + stringCurrentYear;

    if(dateChecker->isUnoccurredDate(stringStartDate)) {

    } else {
        stringStartDate = addNYearsFromDate(stringStartDate, 1);
    }

    startDate = stringStartDate;

    std::string delimitedStartDate = formatConverter->dateFromNumericToBoostFormat(stringStartDate);
    std::string delimitedEndDate = formatConverter->dateFromNumericToBoostFormat(stringEndDate);
    boost::gregorian::date boostStartDate = boost::gregorian::from_uk_string(delimitedStartDate);
    boost::gregorian::date boostEndDate = boost::gregorian::from_uk_string(delimitedEndDate);

    while(boostEndDate < boostStartDate) {
        stringEndDate = addNYearsFromDate(stringEndDate, 1);
        delimitedEndDate = formatConverter->dateFromNumericToBoostFormat(stringEndDate);
        boostEndDate = boost::gregorian::from_uk_string(delimitedEndDate);
    }

    endDate = stringEndDate;

    return;
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

//preconditions ddmmyyyy
//postconditions ddmmyyyy
//if 29 feb it will find the first leap year if not this year
std::string TMParser::addNYearsFromDate(std::string date, int n) {
    FormatConverter *formatConverter = FormatConverter::getInstance();
    date = formatConverter->dateFromNumericToBoostFormat(date);
    boost::gregorian::date initialBoostDate = boost::gregorian::from_uk_string(date);
    boost::gregorian::date finalBoostDate;

    std::string ddmm = date.substr(0,4);
    boost::gregorian::year_iterator year(initialBoostDate);

    for(int i = 0; i < n; i++) {
            ++year;
        }

    finalBoostDate = *year;

    if(ddmm == "2802") {
        if(boost::gregorian::gregorian_calendar::is_leap_year(finalBoostDate.year())) {
            boost::gregorian::date_duration oneDay(1);
            finalBoostDate = finalBoostDate - oneDay;
        }
    } else if(ddmm == "2902") {
        while(!boost::gregorian::gregorian_calendar::is_leap_year(finalBoostDate.year())) {
            ++year;
            finalBoostDate = *year;
        }

    }

    return formatConverter->dateFromBoostToDDMMYYYY(finalBoostDate);
}

std::string TMParser::subtractNYearsFromDate(std::string date, int n) {
    FormatConverter *formatConverter = FormatConverter::getInstance();
    date = formatConverter->dateFromNumericToBoostFormat(date);
    boost::gregorian::date initialBoostDate = boost::gregorian::from_uk_string(date);
    boost::gregorian::date finalBoostDate;

    std::string ddmm = date.substr(0,4);
    boost::gregorian::year_iterator year(initialBoostDate);

    for(int i = 0; i < n; i++) {
            --year;
        }

    if(ddmm == "2802") {
        if(boost::gregorian::gregorian_calendar::is_leap_year(finalBoostDate.year())) {
            boost::gregorian::date_duration oneDay(1);
            finalBoostDate = *year - oneDay;
        }
    } else if(ddmm == "2902") {
        while(!boost::gregorian::gregorian_calendar::is_leap_year(finalBoostDate.year())) {
            --year;
        }

        finalBoostDate = *year;
    }

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
