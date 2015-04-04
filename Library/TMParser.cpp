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

        if(index + 1 == lengthOfTokenizedUserEntry){
            break;
        }

        unitString = formatConverter->returnLowerCase(_tokenizedUserEntry[index]);
        nextWord = formatConverter->returnLowerCase(_tokenizedUserEntry[index + 1]);

        if(unitString == TOKEN_BEFORE||unitString == TOKEN_BY){
            if(dateChecker->isNumericDate(nextWord)||dateChecker->isDay(nextWord)||dateChecker->isDDMonDate(nextWord)) {
                //e.g. before 01012016 (DDMMYYYY)
                indexOfDatesAndTimes.push(index);
                dateToMeet = extractor->extractDayOrNumericDateOrDDMonDate(index + 1, indexOfDatesAndTimes, _tokenizedUserEntry);
                dateToMeet = substractNDaysFromDate(dateToMeet,1);
                timeToMeet = "2359";
                index = indexOfDatesAndTimes.back();
            } else if (dateChecker->isNextDay(index + 1, _tokenizedUserEntry)) {
                //e.g. before next monday
                indexOfDatesAndTimes.push(index);
                dateToMeet = extractor->extractNextDay(index + 1, indexOfDatesAndTimes, _tokenizedUserEntry);
                dateToMeet = substractNDaysFromDate(dateToMeet,1);
                timeToMeet = "2359";
                index = indexOfDatesAndTimes.back();
            } else if (timeChecker->is12HTime(nextWord)||timeChecker->is24HTime(nextWord)) {
                indexOfDatesAndTimes.push(index);
                timeToMeet = extractor->extractTime(index + 1, indexOfDatesAndTimes, _tokenizedUserEntry);
                index = indexOfDatesAndTimes.back();
            } else {
                if(nextWord[0] == '"' && nextWord[nextWord.length()-1] == '"' && nextWord.length() != 1){
                    if(numberOfWordsInQuote(nextWord) == 1){
                        nextWord = nextWord.substr(1,nextWord.length()-2);
                        if(timeChecker->is12HTime(nextWord)||timeChecker->is24HTime(nextWord)||
                           dateChecker->isNumericDate(nextWord)||dateChecker->isDay(nextWord)||
                           dateChecker->isDDMonDate(nextWord)){
                               //what about for next day?
                               std::string nextWordOriginal = _tokenizedUserEntry[index + 1];
                               _tokenizedUserEntry[index + 1] = nextWordOriginal.substr(1,nextWordOriginal.length()-2);
                        }
                    }
                }
            }
        } else if(unitString == TOKEN_ON) {
            if(dateChecker->isNumericDate(nextWord)||dateChecker->isDay(nextWord)||dateChecker->isDDMonDate(nextWord)) {
                indexOfDatesAndTimes.push(index);
                dateToMeet = extractor->extractDayOrNumericDateOrDDMonDate(index + 1,indexOfDatesAndTimes, _tokenizedUserEntry);
                index = indexOfDatesAndTimes.back();

                if(timeToMeet == "") {
                    timeToMeet = "2359";
                }
            } else {
                if(nextWord[0] == '"' && nextWord[nextWord.length()-1] == '"' && nextWord.length() != 1){
                    if(numberOfWordsInQuote(nextWord) == 1){
                        nextWord = nextWord.substr(1,nextWord.length()-2);
                        if(dateChecker->isNumericDate(nextWord)||dateChecker->isDay(nextWord)||dateChecker->isDDMonDate(nextWord)){
                               //what about for next day?
                            std::string nextWordOriginal = _tokenizedUserEntry[index + 1];
                            _tokenizedUserEntry[index + 1] = nextWordOriginal.substr(1,nextWordOriginal.length()-2);
                        }
                    }
                }
            }
        } else if(dateChecker->isNextDay(index, _tokenizedUserEntry)){
            dateToMeet = extractor->extractNextDay(index, indexOfDatesAndTimes, _tokenizedUserEntry);
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
        dateToMeet = formatConverter->dateFromBoostToDDMMYYYY(currentDate);
        if(timeToMeet < currentTime){
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
    int lengthOfTokenizedUserEntry = _tokenizedUserEntry.size();

    FormatConverter *formatConverter = FormatConverter::getInstance();
    TimeChecker *timeChecker = TimeChecker::getInstance();
    DateChecker *dateChecker = DateChecker::getInstance();
    Extractor *extractor = Extractor::getInstance();

    //will not check last string. last string treated as task desc
    
    for(int index = 0; index < lengthOfTokenizedUserEntry; index++){

        if(index + 1 == lengthOfTokenizedUserEntry){
            break;
        }

        unitString = formatConverter->returnLowerCase(_tokenizedUserEntry[index]);

        if(unitString == TOKEN_AT) {
            startTime = extractor->extractTime(index + 1, indexOfDatesAndTimes, _tokenizedUserEntry);
            if(startTime != ""){
                mainIndexOfDatesAndTimes.push(index);
                while(!indexOfDatesAndTimes.empty()){
                    mainIndexOfDatesAndTimes.push(indexOfDatesAndTimes.front());
                    indexOfDatesAndTimes.pop();
                }
                index = mainIndexOfDatesAndTimes.back();
            } else {
                std::string nextWordOriginal = _tokenizedUserEntry[index + 1];
                std::string nextWord = formatConverter->returnLowerCase(nextWordOriginal);
                if(nextWord[0] == '"' && nextWord[nextWord.length()-1] == '"' && nextWord.length() != 1){
                    if(numberOfWordsInQuote(nextWord) == 1){
                        nextWord = nextWord.substr(1,nextWord.length()-2);
                        if(timeChecker->is12HTime(nextWord)||timeChecker->is24HTime(nextWord)){
                               //what about for next day?
                               _tokenizedUserEntry[index + 1] = nextWordOriginal.substr(1,nextWordOriginal.length()-2);
                        }
                    }
                }
            }
        } else if(unitString == TOKEN_ON) {
            startDate = extractor->extractDayOrNumericDateOrDDMonDate(index + 1, indexOfDatesAndTimes, _tokenizedUserEntry);
            if(startDate != ""){
                mainIndexOfDatesAndTimes.push(index);
                while(!indexOfDatesAndTimes.empty()){
                    mainIndexOfDatesAndTimes.push(indexOfDatesAndTimes.front());
                    indexOfDatesAndTimes.pop();
                }
                index = mainIndexOfDatesAndTimes.back();
            } else {
                std::string nextWordOriginal = _tokenizedUserEntry[index + 1];
                std::string nextWord = formatConverter->returnLowerCase(nextWordOriginal);
                if(nextWord[0] == '"' && nextWord[nextWord.length()-1] == '"' && nextWord.length() != 1){
                    if(numberOfWordsInQuote(nextWord) == 1){
                        nextWord = nextWord.substr(1,nextWord.length()-2);
                        if(dateChecker->isNumericDate(nextWord)||dateChecker->isDay(nextWord)||dateChecker->isDDMonDate(nextWord)){
                               //what about for next day?
                            _tokenizedUserEntry[index + 1] = nextWordOriginal.substr(1,nextWordOriginal.length()-2);
                        }
                    }
                }
            }
        } else if(unitString == TOKEN_FROM){
            extractor->extractDateAndOrTime(index + 1, indexOfDatesAndTimes, startDate, startTime, _tokenizedUserEntry);
            if((startDate != "")||(startTime != "")){
                mainIndexOfDatesAndTimes.push(index);
                while(!indexOfDatesAndTimes.empty()){
                    mainIndexOfDatesAndTimes.push(indexOfDatesAndTimes.front());
                    indexOfDatesAndTimes.pop();
                }
                index = mainIndexOfDatesAndTimes.back();
            } else {
                std::string nextWordOriginal = _tokenizedUserEntry[index + 1];
                std::string nextWord = formatConverter->returnLowerCase(nextWordOriginal);
                if(nextWord[0] == '"' && nextWord[nextWord.length()-1] == '"' && nextWord.length() != 1){
                    if(numberOfWordsInQuote(nextWord) == 1){
                        nextWord = nextWord.substr(1,nextWord.length()-2);
                        if(timeChecker->is12HTime(nextWord)||timeChecker->is24HTime(nextWord)||
                           dateChecker->isNumericDate(nextWord)||dateChecker->isDay(nextWord)||
                           dateChecker->isDDMonDate(nextWord)){
                               //what about for next day?
                               _tokenizedUserEntry[index + 1] = nextWordOriginal.substr(1,nextWordOriginal.length()-2);
                               index = index + 1;
                        }
                    }
                }
            }
        } else if (unitString == TOKEN_TO){
            extractor->extractDateAndOrTime(index + 1, indexOfDatesAndTimes, endDate, endTime, _tokenizedUserEntry);
            if((endDate != "")||(endTime != "")){  
                mainIndexOfDatesAndTimes.push(index);
                while(!indexOfDatesAndTimes.empty()){
                    mainIndexOfDatesAndTimes.push(indexOfDatesAndTimes.front());
                    indexOfDatesAndTimes.pop();
                }
                index = mainIndexOfDatesAndTimes.back();
            } else {
                std::string nextWordOriginal = _tokenizedUserEntry[index + 1];
                std::string nextWord = formatConverter->returnLowerCase(nextWordOriginal);
                if(nextWord[0] == '"' && nextWord[nextWord.length()-1] == '"' && nextWord.length() != 1){
                    if(numberOfWordsInQuote(nextWord) == 1){
                        nextWord = nextWord.substr(1,nextWord.length()-2);
                        if(timeChecker->is12HTime(nextWord)||timeChecker->is24HTime(nextWord)||
                           dateChecker->isNumericDate(nextWord)||dateChecker->isDay(nextWord)||
                           dateChecker->isDDMonDate(nextWord)){
                               //what about for next day?
                               _tokenizedUserEntry[index + 1] = nextWordOriginal.substr(1,nextWordOriginal.length()-2);
                               index = index + 1;
                        }
                    }
                }
            }
        } else if(dateChecker->isNextDay(index, _tokenizedUserEntry)) {
            startDate = extractor->extractNextDay(index, indexOfDatesAndTimes, _tokenizedUserEntry);

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
        if(index + 1 == lengthOfVector){
            break;
        }

        unitString = formatConverter->returnLowerCase(_tokenizedUserEntry[index]);
        nextWord = formatConverter->returnLowerCase(_tokenizedUserEntry[index + 1]);

        if(unitString == TOKEN_BEFORE||unitString == TOKEN_BY) {
            if(nextWord[0] == '"' && nextWord[nextWord.length()-1] == '"' && nextWord.length() != 1){
                if(numberOfWordsInQuote(nextWord) == 1){
                    nextWord = nextWord.substr(1,nextWord.length()-2);
                    if(timeChecker->is12HTime(nextWord)||timeChecker->is24HTime(nextWord)||
                        dateChecker->isNumericDate(nextWord)||dateChecker->isDay(nextWord)||
                        dateChecker->isDDMonDate(nextWord)){
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
                    if(dateChecker->isNumericDate(nextWord)||dateChecker->isDay(nextWord)||dateChecker->isDDMonDate(nextWord)){
                        //what about for next day?
                        std::string nextWordOriginal = _tokenizedUserEntry[index + 1];
                        _tokenizedUserEntry[index + 1] = nextWordOriginal.substr(1,nextWordOriginal.length()-2);
                    }
                }
            }
        } else if(unitString == TOKEN_AT){
            std::string nextWordOriginal = _tokenizedUserEntry[index + 1];
            std::string nextWord = formatConverter->returnLowerCase(nextWordOriginal);
            if(nextWord[0] == '"' && nextWord[nextWord.length()-1] == '"' && nextWord.length() != 1){
                if(numberOfWordsInQuote(nextWord) == 1){
                    nextWord = nextWord.substr(1,nextWord.length()-2);
                    if(timeChecker->is12HTime(nextWord)||timeChecker->is24HTime(nextWord)){
                        //what about for next day?
                        _tokenizedUserEntry[index + 1] = nextWordOriginal.substr(1,nextWordOriginal.length()-2);
                    }
                }
            }
        } else if(unitString == TOKEN_FROM||unitString == TOKEN_TO){
            std::string nextWordOriginal = _tokenizedUserEntry[index + 1];
            std::string nextWord = formatConverter->returnLowerCase(nextWordOriginal);
            if(nextWord[0] == '"' && nextWord[nextWord.length()-1] == '"' && nextWord.length() != 1){
                if(numberOfWordsInQuote(nextWord) == 1){
                    nextWord = nextWord.substr(1,nextWord.length()-2);
                    if(timeChecker->is12HTime(nextWord)||timeChecker->is24HTime(nextWord)||
                       dateChecker->isNumericDate(nextWord)||dateChecker->isDay(nextWord)||
                       dateChecker->isDDMonDate(nextWord)){
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

        if(dateChecker->isValidDate(startDate) && dateChecker->isValidDate(endDate) && isValidInfo(startDate, startTime, endDate, endTime)){ //and valid info!
            //IMPLEMENT FUNCTION TO CHECK DATE IMMEDIATELY BEFORE CONSIDERING AS DATE
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


EditableTaskComponent TMParser::parseEditableTaskComponent() {
	std::string userSpecifiedComponent = _tokenizedUserEntry[1];
	if (userSpecifiedComponent == "name") {
		return Description;
	} else if (userSpecifiedComponent == "startdate") {
		return StartDate;
	} else if (userSpecifiedComponent == "starttime") {
		return StartTime;
	}  else if (userSpecifiedComponent == "enddate") {
		return EndDate;
	} else if (userSpecifiedComponent == "endtime") {
		return EndTime;
	} else {
		return InvalidComponent;
	}
}

std::string TMParser::parseDescription() {
	return _tokenizedUserEntry[2];
}

void TMParser::configureAllDatesAndTimes(std::string& startDate, std::string& startTime, std::string& endDate, std::string& endTime, TaskType& taskType){
    FormatConverter *formatConverter = FormatConverter::getInstance();
    if((startTime != ""||startDate != "") && endTime == "" && endDate == ""){
        taskType = TaskType::WithStartDateTime;
        if(startTime == ""){
            startTime = "0000";
        } else if(startDate == ""){
            startDate = formatConverter->dateFromBoostToDDMMYYYY(currentDate);
            if(startTime <= currentTime){
                startDate = addNDaysFromDate(startDate,1);
             }
        } else {
        //startTime and startDate are found
        }
        endDate = startDate;
        endTime = startTime;

    } else if((endTime != ""||endDate != "") && startTime == "" && startDate == "") {
        taskType = TaskType::WithEndDateTime;
        if(endTime == ""){
            endTime = "0000";
        } else if(endDate == ""){
            endDate = formatConverter->dateFromBoostToDDMMYYYY(currentDate);
            if(endTime <= currentTime){
                endDate = addNDaysFromDate(startDate,1);
             }
        } else {
        //endTime and endDate are found
        }
        //no startDateTime, assumed to be current date and atime
        startDate = formatConverter->dateFromBoostToDDMMYYYY(currentDate);
        startTime = currentTime;

    } else if((startDate != ""||startTime != "") && (endDate != ""||endTime != "")){
        taskType = TaskType::WithPeriod;
        if(startTime == ""){
            //there's startDate no startTime
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
            //no startDate but got startTime
            //no endDate got endTime then should be today till today/tomorrow
            //got endDate no endTime

            if(endDate == ""){
                startDate = formatConverter->dateFromBoostToDDMMYYYY(currentDate);
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
                endDate = startDate;
                if(endTime < startTime){
                    endDate = addNDaysFromDate(endDate,1);
                }
            } else {
                //all attributes found
            }
        }
    }
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

//preconditions date dd mm yyyy use dateFromUserToBoost
std::string TMParser::substractNDaysFromDate(std::string date, int n){
    FormatConverter *formatConverter = FormatConverter::getInstance();
    date = formatConverter->dateFromNumericToBoostFormat(date);
    boost::gregorian::date initialBoostDate = boost::gregorian::from_uk_string(date);
    boost::gregorian::date_duration dateDuration(n);
    boost::gregorian::date finalBoostDate = initialBoostDate - dateDuration;
    return formatConverter->dateFromBoostToDDMMYYYY(finalBoostDate);
}

//preconditions date dd mm yyyy use dateFromUserToBoost
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