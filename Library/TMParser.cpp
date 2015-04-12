//@author A0111712Y

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

//Precondition: tokens of string delimited by spaces
//Postcondition: returns vector of strings
std::vector<std::string> TMParser::getTokenizedUserEntry(std::string userEntry) {
    std::vector<std::string> tokenizedUserEntry;
    //to keep track of current position
    int positionOfFrontChar = 0;
    int positionOfBackChar = 0;
    std::string token;
    if(userEntry == "") {
        return tokenizedUserEntry;
    } else {
        positionOfFrontChar = userEntry.find_first_not_of(" ",positionOfFrontChar);
        
        //if userEntry are all spaces
        if(positionOfFrontChar == std::string::npos) {
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

//Precondition: getTokenizedUserEntry(userEntry) first
//Postcondition: first token is extracted from vector of string and is no longer there
std::string TMParser::extractCommand() {
    if(_tokenizedUserEntry.size() == 0) {
        return "";
    }

	std::string  command = _tokenizedUserEntry[0];
    _tokenizedUserEntry.erase(_tokenizedUserEntry.begin());

    return command;
}

//Precondition: use only after extractCommand()
//Postcondition: returns token that was initially after command but in the first position now
std::string TMParser::extractTokenAfterCommand() { 
    if(_tokenizedUserEntry.size() == 0){
        addErrorMessage(ERROR_INDEX_OF_TASK_NOT_SPECIFIED);
        return "";
    } else if (_tokenizedUserEntry.size() == 1) {
        addErrorMessage(ERROR_MISSING_NEW_TASK_INFO);
        return "";
    }

	std::string token = _tokenizedUserEntry[0];

    if(!isPositiveInteger(token)) {
        addErrorMessage(ERROR_INDEX_SPECIFIED_NOT_POSITIVE_INT);
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
    if(isCommandAdd(command)) {
        return CommandTypes::Add;
    } else if (isCommandDelete(command)) {
        return CommandTypes::Delete;
    } else if (isCommandUndo(command)) {
		return CommandTypes::Undo;
	} else if (isCommandRedo(command)) {
        return CommandTypes::Redo;
    } else if (isCommandComplete(command)) {
        return CommandTypes::Complete;
    } else if (isCommandIncomplete(command)) {
        return CommandTypes::Incomplete;
    } else if (isCommandSearch(command)) {
        return CommandTypes::SearchKeyword;
    } else if (isCommandEdit(command)) {
        return CommandTypes::Edit;
    } else if (isCommandStore(command)) {
        return CommandTypes::SaveAt;
	} else if (isCommandDoneAll(command)) {
        return CommandTypes::CompleteAllToday;
    } else if (isCommandBlock(command)) {
        return CommandTypes::Block;
	} else if (isCommandConfirm(command)) {
        return CommandTypes::Confirm; 
	} else if (isCommandQuit(command)) {
        return CommandTypes::Exit;
    } else {
        return CommandTypes::Invalid;
    }
}

bool TMParser::isCommandAdd(std::string command) {
    if (command == CMD_ADD||command == CMD_SHORTCUT_ADD) {
        return true;
    } else {
        return false;
    }
}

bool TMParser::isCommandDelete(std::string command) {
    if (command == CMD_DELETE||command == CMD_SHORTCUT_DELETE||command == CMD_SHORTCUT_DEL) {
        return true;
    } else {
        return false;
    }
}

bool TMParser::isCommandUndo(std::string command) {
    if (command == CMD_UNDO||command == CMD_SHORTCUT_UNDO) {
        return true;
    } else {
        return false;
    }
}

bool TMParser::isCommandRedo(std::string command) {
    if (command == CMD_REDO||command == CMD_SHORTCUT_REDO) {
        return true;
    } else {
        return false;
    }
}

bool TMParser::isCommandComplete(std::string command) {
    if (command == CMD_COMPLETE||command == CMD_SHORTCUT_COMPLETE) {
        return true;
    } else {
        return false;
    }
}

bool TMParser::isCommandIncomplete(std::string command) {
    if (command == CMD_INCOMPLETE||command == CMD_SHORTCUT_INCOMPLETE) {
        return true;
    } else {
        return false;
    }
}

bool TMParser::isCommandSearch(std::string command) {
    if (command == CMD_SEARCH||command == CMD_SHORTCUT_SEARCH) {
        return true;
    } else {
        return false;
    }
}

bool TMParser::isCommandEdit(std::string command) {
    if (command == CMD_EDIT||command == CMD_SHORTCUT_EDIT) {
        return true;
    } else {
        return false;
    }
}

bool TMParser::isCommandStore(std::string command) {
    if (command == CMD_STORE||command == CMD_SHORTCUT_STORE) {
        return true;
    } else {
        return false;
    }
}

bool TMParser::isCommandDoneAll(std::string command) {
    if (command == CMD_DONEALL||command == CMD_SHORTCUT_DONEALL) {
        return true;
    } else {
        return false;
    }
}

bool TMParser::isCommandBlock(std::string command) {
    if (command == CMD_BLOCK||command == CMD_SHORTCUT_BLOCK) {
        return true;
    } else {
        return false;
    }
}

bool TMParser::isCommandConfirm(std::string command) {
    if (command == CMD_CONFIRM) {
        return true;
    } else {
        return false;
    }
}

bool TMParser::isCommandQuit(std::string command) {
    if (command == CMD_QUIT||
        command == CMD_SHORTCUT_QUIT||
        command == CMD_EXIT||
        command == CMD_CLOSE) {
            return true;
    } else {
        return false;
    }
}

//Preconditions: taskInfo contains only the entry after command.
//               use extractEntryAfterCommand 1st
//               use only when in adding or editing information
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

//Precondition: task is deadlined task use isDeadlinedTask to check
//Postcondition: returns a deadline task if valid date else invalid
TMTask TMParser::parseDeadlinedTaskInfo() {
    std::queue<int> indexOfDatesAndTimes;
    TaskType taskType = TaskType::WithEndDateTime;
    std::string dateToMeet;
    std::string timeToMeet;
    std::string taskDescription;
    int lengthOfTokenizedUserEntry = _tokenizedUserEntry.size();

    FormatConverter *formatConverter = FormatConverter::getInstance();
    TaskChecker *taskChecker = TaskChecker::getInstance();
    TimeChecker *timeChecker = TimeChecker::getInstance();
    DateChecker *dateChecker = DateChecker::getInstance();
    Extractor *extractor = Extractor::getInstance();

    for(int index = 0; index < lengthOfTokenizedUserEntry; index++){
        std::string unitString = formatConverter->returnLowerCase(_tokenizedUserEntry[index]);

        if (taskChecker->isWordBefore(unitString)||taskChecker->isWordBy(unitString)) {
            if(index + 1 == lengthOfTokenizedUserEntry){
                break;
            }
            std::string nextWord = formatConverter->returnLowerCase(_tokenizedUserEntry[index + 1]);
            
            if (dateChecker->isDateOrDayOrNextDayOrTomorrow(nextWord, index, _tokenizedUserEntry)) {
                indexOfDatesAndTimes.push(index);
                timeToMeet = TIME_23_COLON_59;
                dateToMeet = extractor->extractDayOrNumericDateOrDelimitedDate(index + 1, indexOfDatesAndTimes, _tokenizedUserEntry);
                configureDayMonth(dateToMeet);

                if(taskChecker->isWordBefore(unitString)) {
                    dateToMeet = substractNDaysFromDate(dateToMeet,1);
                }
                index = indexOfDatesAndTimes.back();

            } else if (timeChecker->isTime(nextWord)) {
                indexOfDatesAndTimes.push(index);
                timeToMeet = extractor->extractTime(index + 1, indexOfDatesAndTimes, _tokenizedUserEntry);
                index = indexOfDatesAndTimes.back();

            } else if (dateChecker->isToday(nextWord)) {
                //intepreted as before end of today or by today
                indexOfDatesAndTimes.push(index);
                dateToMeet = extractor->extractToday(index + 1, indexOfDatesAndTimes, _tokenizedUserEntry);
                timeToMeet = TIME_23_COLON_59;
                index = indexOfDatesAndTimes.back();

            } else {
                editDateOrTimeInInvertedCommas(nextWord, index + 1, CHECK_DATE_YES, CHECK_TIME_YES);
            }
        } else if (unitString == TOKEN_ON) {
            if(index + 1 == lengthOfTokenizedUserEntry){
                break;
            }
            std::string nextWord = formatConverter->returnLowerCase(_tokenizedUserEntry[index + 1]);
            if(dateChecker->isDateOrDay(nextWord, index, _tokenizedUserEntry)) {
                indexOfDatesAndTimes.push(index);
                dateToMeet = extractor->extractDayOrNumericDateOrDelimitedDate(index + 1,indexOfDatesAndTimes, _tokenizedUserEntry);
                configureDayMonth(dateToMeet);
                index = indexOfDatesAndTimes.back();
                if(timeToMeet == "") {
                    timeToMeet = TIME_23_COLON_59;
                }
            } else {
                editDateOrTimeInInvertedCommas(nextWord, index + 1, CHECK_DATE_YES, CHECK_TIME_NO);
            }
        } else if(dateChecker->isNextDay(index, _tokenizedUserEntry)) {
            dateToMeet = extractor->extractNextDay(index, indexOfDatesAndTimes, _tokenizedUserEntry);
            index = indexOfDatesAndTimes.back();

        } else if(dateChecker->isToday(unitString)) {
            dateToMeet = extractor->extractToday(index, indexOfDatesAndTimes, _tokenizedUserEntry);
            index = indexOfDatesAndTimes.back();

        } else if(dateChecker->isTomorrow(unitString)) {
            dateToMeet = extractor->extractTomorrow(index, indexOfDatesAndTimes, _tokenizedUserEntry);
            index = indexOfDatesAndTimes.back();

        } else {
            editDateOrTimeInInvertedCommas(unitString, index, CHECK_DATE_YES, CHECK_TIME_NO);
        }
    }
    
    configureTaskDescription(taskDescription, indexOfDatesAndTimes);

    if(timeChecker->isTimeWithoutPeriod(timeToMeet)) {
        configureEndTimeWithoutPeriods(timeToMeet);
    }

    if(dateToMeet == "") {
        dateToMeet = currentDateInString();
        if(timeToMeet <= currentTime()){
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

//Precondition: isTimedTask returns true
//Postcondition: returns a timedTask if valid info i.e. startDate <= endDate
//               i.e. if startDate == endDate startTime <= endTime
TMTask TMParser::parseTimedTaskInfo() {
    TaskType taskType;
    std::queue<int> indexOfDatesAndTimes;
    std::queue<int> mainIndexOfDatesAndTimes;
    std::vector<std::string>::iterator iter;
    std::string startTime;
    std::string startDate;
    std::string endTime;
    std::string endDate;
    std::string taskDescription;
    std::string unitString;
    std::string nextWord;
    int lengthOfTokenizedUserEntry = _tokenizedUserEntry.size();

    FormatConverter *formatConverter = FormatConverter::getInstance();
    TaskChecker *taskChecker = TaskChecker::getInstance();
    TimeChecker *timeChecker = TimeChecker::getInstance();
    DateChecker *dateChecker = DateChecker::getInstance();
    Extractor *extractor = Extractor::getInstance();
    
    for(int index = 0; index < lengthOfTokenizedUserEntry; index++){

        unitString = formatConverter->returnLowerCase(_tokenizedUserEntry[index]);

        if(taskChecker->isWordAt(unitString)) {
            if(index + 1 == lengthOfTokenizedUserEntry){
                break;
            }
            nextWord = formatConverter->returnLowerCase(_tokenizedUserEntry[index + 1]);

            if (timeChecker->isTime(nextWord)){
                startTime = extractor->extractTime(index + 1, indexOfDatesAndTimes, _tokenizedUserEntry);
                configureQueuesAndIndex(mainIndexOfDatesAndTimes, indexOfDatesAndTimes, index, AFTER_TOKEN_YES);
            } else {
                editDateOrTimeInInvertedCommas(nextWord, index + 1, CHECK_DATE_NO, CHECK_TIME_YES);
            }
        } else if(taskChecker->isWordOn(unitString)) {
            if(index + 1 == lengthOfTokenizedUserEntry){
                break;
            }
            nextWord = formatConverter->returnLowerCase(_tokenizedUserEntry[index + 1]);

            if (dateChecker->isDateOrDay(nextWord, index, _tokenizedUserEntry)) {
                startDate = extractor->extractDayOrNumericDateOrDelimitedDate(index + 1, indexOfDatesAndTimes, _tokenizedUserEntry);
                configureQueuesAndIndex(mainIndexOfDatesAndTimes, indexOfDatesAndTimes, index, AFTER_TOKEN_YES);
                index = mainIndexOfDatesAndTimes.back();
            } else {
                editDateOrTimeInInvertedCommas(nextWord, index + 1, CHECK_DATE_YES, CHECK_TIME_NO);
            }
        } else if(taskChecker->isWordFrom(unitString)) {
            if(index + 1 == lengthOfTokenizedUserEntry){
                break;
            }

            nextWord = formatConverter->returnLowerCase(_tokenizedUserEntry[index + 1]);
            if (taskChecker->isDateOrTime(nextWord, index, _tokenizedUserEntry)) {
                extractor->extractDateAndOrTime(index + 1, indexOfDatesAndTimes, startDate, startTime, _tokenizedUserEntry);
                configureQueuesAndIndex(mainIndexOfDatesAndTimes, indexOfDatesAndTimes, index, AFTER_TOKEN_YES);
                index = mainIndexOfDatesAndTimes.back();

            } else {
                editDateOrTimeInInvertedCommas(nextWord, index + 1, CHECK_DATE_YES, CHECK_TIME_YES);
            }
        } else if(taskChecker->isWordTo(unitString)) {
            if(index + 1 == lengthOfTokenizedUserEntry){
                break;
            }
            nextWord = formatConverter->returnLowerCase(_tokenizedUserEntry[index + 1]);

            if (taskChecker->isDateOrTime(nextWord, index, _tokenizedUserEntry)) {
                extractor->extractDateAndOrTime(index + 1, indexOfDatesAndTimes, endDate, endTime, _tokenizedUserEntry);
                configureQueuesAndIndex(mainIndexOfDatesAndTimes, indexOfDatesAndTimes, index, AFTER_TOKEN_YES);
                index = mainIndexOfDatesAndTimes.back();
            } else {
                editDateOrTimeInInvertedCommas(nextWord, index + 1, CHECK_DATE_YES, CHECK_TIME_YES);
            }
        } else if(dateChecker->isNextDay(index, _tokenizedUserEntry)) {
            startDate = extractor->extractNextDay(index, indexOfDatesAndTimes, _tokenizedUserEntry);
            configureQueuesAndIndex(mainIndexOfDatesAndTimes, indexOfDatesAndTimes, index, AFTER_TOKEN_NO);
            index = mainIndexOfDatesAndTimes.back();
        
        } else if (dateChecker->isToday(unitString)) {
            startDate = extractor->extractToday(index, indexOfDatesAndTimes, _tokenizedUserEntry);
            configureQueuesAndIndex(mainIndexOfDatesAndTimes, indexOfDatesAndTimes, index, AFTER_TOKEN_NO);

        } else if (dateChecker->isTomorrow(unitString)) {
            startDate = extractor->extractTomorrow(index, indexOfDatesAndTimes, _tokenizedUserEntry);
            configureQueuesAndIndex(mainIndexOfDatesAndTimes, indexOfDatesAndTimes, index, AFTER_TOKEN_NO);

        } else {
            editDateOrTimeInInvertedCommas(unitString, index, CHECK_DATE_YES, CHECK_TIME_NO);
        }
    }

    configureAllDatesAndTimes(startDate, startTime, endDate, endTime, taskType);
    configureTaskDescription(taskDescription, mainIndexOfDatesAndTimes);
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

//Precondition: task information does not meet any of the 2 other task types' criteria
//Postconditions: returns a task with only task description
TMTask TMParser::parseUndatedTaskInfo() {
    TaskType taskType = TaskType::Undated;
    TMTaskTime taskTime;
    std::string taskDescription;
    std::string unitString;
    std::string nextWord;

    int lengthOfVector = _tokenizedUserEntry.size();
    TaskChecker *taskChecker = TaskChecker::getInstance();
    FormatConverter *formatConverter = FormatConverter::getInstance();
    TimeChecker *timeChecker = TimeChecker::getInstance();
    DateChecker *dateChecker = DateChecker::getInstance();
    Extractor *extractor = Extractor::getInstance();

    for(int index = 0; index < lengthOfVector; index++){

        unitString = formatConverter->returnLowerCase(_tokenizedUserEntry[index]);

        if (isWordBeforeOrByOrFromOrTo(unitString)) {
            if(index + 1 == lengthOfVector){
                break;
            }
            nextWord = formatConverter->returnLowerCase(_tokenizedUserEntry[index + 1]);
            editDateOrTimeInInvertedCommas(nextWord, index + 1, CHECK_DATE_YES, CHECK_TIME_YES);

        } else if(taskChecker->isWordOn(unitString)) {
            if(index + 1 == lengthOfVector){
                break;
            }
            nextWord = formatConverter->returnLowerCase(_tokenizedUserEntry[index + 1]);
            editDateOrTimeInInvertedCommas(nextWord, index + 1, CHECK_DATE_YES, CHECK_TIME_NO);

        } else if(taskChecker->isWordAt(unitString)) {
            if(index + 1 == lengthOfVector){
                break;
            }
            nextWord = formatConverter->returnLowerCase(_tokenizedUserEntry[index + 1]);
            editDateOrTimeInInvertedCommas(nextWord, index + 1, CHECK_DATE_NO, CHECK_TIME_YES);

        } else {
            editDateOrTimeInInvertedCommas(unitString, index, CHECK_DATE_YES, CHECK_TIME_NO);
        }
    }
    
    configureTaskDescription(taskDescription);
    TMTask task(taskDescription,taskTime,taskType);
    return task;
}

//Precondition: calls the function block
//Postcondition: returns a vector of multiple task timings if they were typed in
std::vector<TMTask> TMParser::parseMultipleTimingTaskInfo() {
    std::vector<TMTask> tasks;
    std::vector<TMTaskTime> taskTimings;
    std::queue<int> indexOfDatesAndTimes;
    std::queue<int> mainIndexOfDatesAndTimes;
    std::vector<TaskType> taskTypes;
    std::string taskDescription;
    bool oneTimingFound = false;
    int lengthOfTokenizedUserEntry = _tokenizedUserEntry.size();

    TaskChecker *taskChecker = TaskChecker::getInstance();
    FormatConverter *formatConverter = FormatConverter::getInstance();
    DateChecker *dateChecker = DateChecker::getInstance();
    Extractor *extractor = Extractor::getInstance();

    for(int index = 0; index < lengthOfTokenizedUserEntry; index++) {
        std::string startTime;
        std::string startDate;
        std::string endTime;
        std::string endDate;
        TaskType taskType;
        std::string unitString;

        unitString = formatConverter->returnLowerCase(_tokenizedUserEntry[index]);

        if(oneTimingFound) {
            if(unitString == TOKEN_AND) {
                if(index + 1 == lengthOfTokenizedUserEntry) {
                    break;
                }
                extractor->extractDateAndOrTime(index + 1, indexOfDatesAndTimes, startDate, startTime, _tokenizedUserEntry);
                std::string nextWord = formatConverter->returnLowerCase(_tokenizedUserEntry[index + 1]);

                if(startDate != ""||startTime != "") {
                    configureQueuesAndIndex(mainIndexOfDatesAndTimes, indexOfDatesAndTimes, index, AFTER_TOKEN_YES);
                    configureEndDateTime(index, mainIndexOfDatesAndTimes, indexOfDatesAndTimes, endDate, endTime);
                } else {
                    editDateOrTimeInInvertedCommas(nextWord, index + 1, CHECK_DATE_YES, CHECK_TIME_YES);
                }
            } else if (taskChecker->isWordTo(unitString)) {
                if(index + 1 == lengthOfTokenizedUserEntry) {
                    break;
                }
                extractor->extractDateAndOrTime(index + 1, indexOfDatesAndTimes, endDate, endTime, _tokenizedUserEntry);
                std::string nextWord = formatConverter->returnLowerCase(_tokenizedUserEntry[index + 1]);
                
                if(endDate != ""||endTime != "") {
                    configureQueuesAndIndex(mainIndexOfDatesAndTimes, indexOfDatesAndTimes, index, AFTER_TOKEN_YES);
                } else {
                    editDateOrTimeInInvertedCommas(nextWord, index + 1, CHECK_DATE_YES, CHECK_TIME_YES);
                }
            }
        } else {
            if((taskChecker->isWordTo(unitString))) {
                if(index + 1 == lengthOfTokenizedUserEntry) {
                    break;
                }
                extractor->extractDateAndOrTime(index + 1, indexOfDatesAndTimes, endDate, endTime, _tokenizedUserEntry);
                std::string nextWord = formatConverter->returnLowerCase(_tokenizedUserEntry[index + 1]);

                if(endDate != ""||endTime != "") {
                    if(!oneTimingFound){
                        oneTimingFound = true;
                    }
                    configureQueuesAndIndex(mainIndexOfDatesAndTimes, indexOfDatesAndTimes, index, AFTER_TOKEN_YES);
                } else {
                    editDateOrTimeInInvertedCommas(nextWord, index + 1, CHECK_DATE_YES, CHECK_TIME_YES);
                }
            } else {
                extractor->extractDateAndOrTime(index, indexOfDatesAndTimes, startDate, startTime, _tokenizedUserEntry);
                if(startDate != ""||startTime != "") {
                    if(!oneTimingFound){
                        oneTimingFound = true;
                    }
                    configureQueuesAndIndex(mainIndexOfDatesAndTimes, indexOfDatesAndTimes, index, AFTER_TOKEN_NO);
                    configureEndDateTime(index, mainIndexOfDatesAndTimes, indexOfDatesAndTimes, endDate, endTime);
                } else {
                    editDateOrTimeInInvertedCommas(unitString, index, CHECK_DATE_YES, CHECK_TIME_YES);
                }
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
    
    configureTaskDescription(taskDescription, mainIndexOfDatesAndTimes);

    int lengthOfTaskTimings = taskTimings.size();
    for(int i = 0; i < lengthOfTaskTimings; i++){
        TMTask task(taskDescription,taskTimings[i],taskTypes[i]);
        tasks.push_back(task);
    }
    return tasks;
}

//Postcondition: returns configured parameters such as extracted end date or time
void TMParser::configureEndDateTime(int& index, std::queue<int>& mainIndexOfDatesAndTimes, std::queue<int>& indexOfDatesAndTimes, std::string& endDate, std::string& endTime) {
    int lengthOfTokenizedUserEntry = _tokenizedUserEntry.size();
    FormatConverter *formatConverter = FormatConverter::getInstance();
    TaskChecker *taskChecker = TaskChecker::getInstance();
    Extractor *extractor = Extractor::getInstance();

    if(index + 1 != lengthOfTokenizedUserEntry) {
        std::string nextWord = _tokenizedUserEntry[index + 1];
        nextWord = formatConverter->returnLowerCase(nextWord);
        if(taskChecker->isWordTo(nextWord)) {
            index = index + 1;
            if(index + 1 != lengthOfTokenizedUserEntry) {
                std::string tokenAfterTo = formatConverter->returnLowerCase(_tokenizedUserEntry[index + 1]);
                extractor->extractDateAndOrTime(index + 1, indexOfDatesAndTimes, endDate, endTime, _tokenizedUserEntry);
                                
                if(endDate != ""||endTime != "") {
                    configureQueuesAndIndex(mainIndexOfDatesAndTimes, indexOfDatesAndTimes, index, AFTER_TOKEN_YES);
                } else {
                    editDateOrTimeInInvertedCommas(tokenAfterTo, index + 1, CHECK_DATE_YES, CHECK_TIME_YES);
                }
            }
        }
    }
    return;
}

//Precondition: only task description found. used for undated task
//Postcondition: edits taskDescription
void TMParser::configureTaskDescription(std::string& taskDescription) {
    int lengthOfVector = _tokenizedUserEntry.size();

    for(int i = 0; i < lengthOfVector; i++) {
        std::string token = _tokenizedUserEntry[i];
        taskDescription += token;
        if(i != lengthOfVector - 1) {
            taskDescription += " ";
        }
    }
    return;
}

//Precondition: used for deadline, timed and multiple timing tasks
//Postcondition: edits taskDescription loops over marked times or dates
void TMParser::configureTaskDescription(std::string& taskDescription, std::queue<int>& mainIndexOfDatesAndTimes) {
    int lengthOfTokenizedUserEntry = _tokenizedUserEntry.size();

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
            taskDescription = taskDescription + token + " ";
        }
    }

    if (taskDescription != "") {
        taskDescription.erase(taskDescription.end() - 1);
    }
    return;
}

//Postcondition: configures all the queues and indices
void TMParser::configureQueuesAndIndex(std::queue<int>& mainIndexOfDatesAndTimes, std::queue<int>& indexOfDatesAndTimes, int& index, bool afterToken) {
    if(afterToken) {
        mainIndexOfDatesAndTimes.push(index);
    }

    while(!indexOfDatesAndTimes.empty()){
        mainIndexOfDatesAndTimes.push(indexOfDatesAndTimes.front());
        indexOfDatesAndTimes.pop();
    }
    index = mainIndexOfDatesAndTimes.back();
}

//Precondition: nextWord must be lowercase
//Postcondition: edits dates and times encapsulated in inverted commas
void TMParser::editDateOrTimeInInvertedCommas(std::string nextWord, int index, bool checkDate, bool checkTime) {
    TimeChecker *timeChecker = TimeChecker::getInstance();
    DateChecker *dateChecker = DateChecker::getInstance();

    if(nextWord[0] == '"' && nextWord[nextWord.length()-1] == '"' && nextWord.length() > 2) {

        nextWord = nextWord.substr(1,nextWord.length()-2);
        std::string nextWordOriginal = _tokenizedUserEntry[index];

        if(checkDate && checkTime) {
            if(timeChecker->isTime(nextWord)||isDateToBeEdited(nextWord)) {
                _tokenizedUserEntry[index] = nextWordOriginal.substr(1,nextWordOriginal.length()-2);
            }
        } else if(checkDate) {
            if(isDateToBeEdited(nextWord)){
                _tokenizedUserEntry[index] = nextWordOriginal.substr(1,nextWordOriginal.length()-2);
            }
        } else if(checkTime) {
            if(timeChecker->isTime(nextWord)){
                _tokenizedUserEntry[index] = nextWordOriginal.substr(1,nextWordOriginal.length()-2);
            }
        }
    }

    return;
}

//Precondition: called by isDateToBeEdited
//Postcondition: returns true if next day encapsulated in inverted commas e.d. "next mon"
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

//Precondition: called by isDateToBeEdited
//Postcondition: returns true if spaced date encapsulated in inverted commas e.d. "1 Jan"
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

bool TMParser::isDateToBeEdited(std::string nextWord) {
    DateChecker *dateChecker = DateChecker::getInstance();

    if (dateChecker->isNumericDate(nextWord)||
        dateChecker->isDay(nextWord)||
        dateChecker->isDelimitedDate(nextWord)||
        isNextDayInInvertedCommas(nextWord)||
        dateChecker->isToday(nextWord)||
        dateChecker->isTomorrow(nextWord)||
        isSpacedDateInInvertedCommas(nextWord)) {
            return true;
    } else {
        return false;
    }
}

void TMParser::configureAllDatesAndTimes(std::string& startDate, std::string& startTime, std::string& endDate, std::string& endTime, TaskType& taskType){
    FormatConverter *formatConverter = FormatConverter::getInstance();
    TimeChecker *timeChecker = TimeChecker::getInstance();

    if ((startTime != ""||startDate != "") && endTime == "" && endDate == ""){
        taskType = TaskType::WithStartDateTime;
        configureDayMonth(startDate);
        if (timeChecker->isTimeWithoutPeriod(startTime)) {
            configureStartTimeWithoutPeriods(startTime);
        }

        if (startTime == "") {
            startTime = TIME_00_COLON_00;    
        } else if (startDate == "") {
            startDate = currentDateInString();            
            if (startTime <= currentTime()){
                startDate = addNDaysFromDate(startDate,1);
             }
        }

        endDate = startDate;
        endTime = startTime;
    } else if((endTime != ""||endDate != "") && startTime == "" && startDate == "") {
        taskType = TaskType::WithPeriod;
        startDate = currentDateInString();
        startTime = currentTime();
        configureDayMonth(endDate);
        if(timeChecker->isTimeWithoutPeriod(endTime)) {
            configureEndTimeWithoutPeriods(endTime);
        }
        
        if (endTime == "") {
            endTime = TIME_23_COLON_59;
        } else if (endDate == "") {
            endDate = currentDateInString();
            
            if(endTime <= currentTime()){
                endDate = addNDaysFromDate(endDate,1);
             }
        }
    } else if((startDate != ""||startTime != "") && (endDate != ""||endTime != "")) {
        taskType = TaskType::WithPeriod;
        
        if(startTime == "" && endDate == "") {
            //startDate endTime
            //startTime = 0000 endDate = startDate
            startTime = TIME_00_COLON_00;

            //finds the earliest date in the future
            configureDayMonth(startDate);

             if(timeChecker->isTimeWithoutPeriod(endTime)) {
                configureEndTimeWithoutPeriods(endTime);
             }
             endDate = startDate;

             //if endTime = 0000 plus 1 more day
             if(endTime == TIME_00_COLON_00) {
                endDate = addNDaysFromDate(endDate, 1);
             }
        } else if(startTime == "" && endTime == "") {
            //startDate endDate
            startTime = TIME_00_COLON_00;
            endTime = TIME_23_COLON_59;
            configureStartDateEndDate(startDate, endDate);

        } else if (startTime == "" && !(endTime == ""||endDate == "")) {
            //startDate endDate endTime
            //startTime == 00:00
            startTime = TIME_00_COLON_00;
            configureStartDateEndDate(startDate, endDate);

            if (timeChecker->isTimeWithoutPeriod(endTime)) {
                configureEndTimeWithoutPeriods(endTime);
            }   
        } else if(startDate == "" && endDate == "") {
            //startTime endTime
            startDate = currentDateInString();
            endDate = startDate;
            configureStartTimeEndTime(startTime, endTime);

            //if time passed then start date is tomorrow's date   
            if(startTime <= currentTime()) {
                startDate = addNDaysFromDate(startDate, 1);
                endDate = addNDaysFromDate(endDate, 1);
            }  
            
            if(endTime <= startTime){
                endDate = addNDaysFromDate(endDate, 1);
            }             
        } else if (startDate == "" && endTime == "") {
            //startTime endDate
            //today or tomorrow if time has passed
            //endDate may be 4 or 8
            startDate = currentDateInString();
            configureDayMonth(endDate);
            endTime = TIME_23_COLON_59;
            
            if(timeChecker->isTimeWithoutPeriod(startTime)) {
                configureStartTimeWithoutPeriods(startTime);
            }
            
            if(startTime <= currentTime()) {
                startDate = addNDaysFromDate(startDate, 1);
            }
        } else if (startDate == "" && !(endDate == ""||endTime == "")) {
            //startTime endDate endTime
            //startDate = endDate or one day earlier
            configureDayMonth(endDate);
            startDate = endDate;

            if (timeChecker->isTimeWithoutPeriod(startTime)||
                timeChecker->isTimeWithoutPeriod(endTime)) {
                    configureStartTimeEndTime(startTime, endTime);
                    if(startTime >= endTime){
                        startDate = substractNDaysFromDate(startDate,1);
                    }
            }
        } else if (!(startDate == ""||startTime == "") && endTime == "") {
            //startDate startTime endDate 2359
            
            endTime = TIME_23_COLON_59;
            configureStartTimeEndTime(startTime, endTime);
            configureStartDateEndDate(startDate, endDate);
        } else if (!(startDate == ""||startTime == "") && endDate == "") {
            //startDate startTime endTime
            //if today's date find next year's date > currentDate
            configureDayMonth(startDate);
            endDate = startDate;

            configureStartTimeEndTime(startTime, endTime);
            
            if(endTime <= startTime){
                endDate = addNDaysFromDate(endDate,1);
            }
        } else {
            //all attributes found
            if (startDate.length() == DATE_DDMMYYYY_LENGTH && endDate.length() == DATE_DDMMYYYY_LENGTH) {
                //if startTime and endTime != 5
                //if startTime > endTime check if which is is without period
                //if start without period change start to whatever it is equivalent to end time
                //if end without period change end to am or pm if it is equivalent to end time
                configureStartTimeEndTime(startTime, endTime);
                if(startTime > endTime && startDate == endDate) {
                    addErrorMessage(ERROR_UNABLE_TO_FIND_APPROPRIATE_TIME);
                } else if (!isStartDateLessThanEndDate(startDate, endDate)) {
                    addErrorMessage(ERROR_STARTDATE_LATER_THAN_ENDDATE);
                }
            } else {
                configureStartDateEndDate(startDate, endDate);
                if (timeChecker->isTimeWithoutPeriod(startTime)) {
                    configureStartTimeWithoutPeriods(startTime);
                }
                if (timeChecker->isTimeWithoutPeriod(endTime)) {
                    configureEndTimeWithoutPeriods(endTime);
                }
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

    std::string stringCurrentDate = currentDateInString();
    std::string stringCurrentYear = stringCurrentDate.substr(4);
    stringDate = stringDate + stringCurrentYear;

    if(stringDate.substr(0,4) == DATE_NUMERIC_DDMM_29_FEB && !dateChecker->isCurrentlyLeapYear()) {
        stringDate = next29FebFrom(stringDate);
    }

    if (!dateChecker->isUnoccurredDate(stringDate)) {
        stringDate = addNYearsFromDate(stringDate, 1);
    }

    return;
}

//preconditions date is only 4 digits in length
void TMParser::configureStartDayMonthEndDayMonth(std::string& startDate, std::string& endDate) {
    if(startDate.length() != DATE_DDMM_LENGTH||endDate.length() != DATE_DDMM_LENGTH) {
        return;
    }
    
    FormatConverter *formatConverter = FormatConverter::getInstance();
    DateChecker *dateChecker = DateChecker::getInstance();
    std::string stringCurrentYear = currentDateInString().substr(4);
    std::string stringStartDate = startDate + stringCurrentYear;
    std::string stringEndDate = endDate + stringCurrentYear;

    if(startDate == DATE_NUMERIC_DDMM_29_FEB && !dateChecker->isLeapYear(stringCurrentYear)) {
        stringStartDate = next29FebFrom(stringStartDate);
    }

    if(endDate == DATE_NUMERIC_DDMM_29_FEB && !dateChecker->isLeapYear(stringCurrentYear)) {
        stringEndDate = next29FebFrom(stringEndDate);
    }

    if(!dateChecker->isUnoccurredDate(stringStartDate)) {
        stringStartDate = addNYearsFromDate(stringStartDate, 1);
    }

    startDate = stringStartDate;

    std::string delimitedStartDate = formatConverter->dateFromNumericToBoostFormat(stringStartDate);
    std::string delimitedEndDate = formatConverter->dateFromNumericToBoostFormat(stringEndDate);
    boost::gregorian::date boostStartDate = boost::gregorian::from_uk_string(delimitedStartDate);
    boost::gregorian::date boostEndDate = boost::gregorian::from_uk_string(delimitedEndDate);

    while(boostEndDate <= boostStartDate) {
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

    if(startTime >= EIGHT_O_CLOCK && startTime <= ELEVEN_O_CLOCK) {
        startTimeWithPeriod = stringStartTime + TIME_PERIOD_AM;
        if(endTime == TWELVE_O_CLOCK || (endTime >= 1 && endTime <= EIGHT_O_CLOCK)) {
            endTimeWithPeriod = stringEndTime + TIME_PERIOD_PM;
        } else {
            if(endTime > startTime) {
                endTimeWithPeriod = stringEndTime + TIME_PERIOD_AM;
            } else {
                endTimeWithPeriod = stringEndTime + TIME_PERIOD_PM;
            }
        }
    } else {
        startTimeWithPeriod = stringStartTime + TIME_PERIOD_PM;
        if(endTime >= NINE_O_CLOCK && endTime <= ELEVEN_O_CLOCK) {
            endTimeWithPeriod = stringEndTime + TIME_PERIOD_PM;
        } else {
            if(startTime == TWELVE_O_CLOCK) {
                if(startTime > endTime) {
                    endTimeWithPeriod = stringEndTime + TIME_PERIOD_PM;
                } else {
                    endTimeWithPeriod = stringEndTime + TIME_PERIOD_AM;
                }
            } else {
                if(startTime < endTime && endTime != TWELVE_O_CLOCK) {
                    endTimeWithPeriod = stringEndTime + TIME_PERIOD_PM;
                } else {
                    endTimeWithPeriod = stringEndTime + TIME_PERIOD_AM;
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

    if(startTime >= EIGHT_O_CLOCK && startTime <= ELEVEN_O_CLOCK) {
        startTimeWithPeriod = stringStartTime + TIME_PERIOD_AM;
        stringStartTime = formatConverter->timeFrom12HourAMToHHMM(startTimeWithPeriod);
    } else {
        startTimeWithPeriod = stringStartTime + TIME_PERIOD_PM;
        stringStartTime = formatConverter->timeFrom12HourPMToHHMM(startTimeWithPeriod);
    }
    
    return;
}

void TMParser::configureEndTimeWithoutPeriods(std::string& stringEndTime) {
    int endTime = std::stoi(stringEndTime);
    std::string endTimeWithPeriod;
    FormatConverter *formatConverter = FormatConverter::getInstance();

    if(endTime == TWELVE_O_CLOCK || (endTime >= ONE_O_CLOCK && endTime <= EIGHT_O_CLOCK)) {
        endTimeWithPeriod = stringEndTime + TIME_PERIOD_PM;
        stringEndTime = formatConverter->timeFrom12HourPMToHHMM(endTimeWithPeriod);
    } else {
        endTimeWithPeriod = stringEndTime + TIME_PERIOD_AM;
        stringEndTime = formatConverter->timeFrom12HourAMToHHMM(endTimeWithPeriod);
    }

    return;
}

void TMParser::configureStartDateEndDate(std::string& startDate, std::string& endDate) {
    DateChecker *dateChecker = DateChecker::getInstance();
    std::string currentDateYYYY = currentDateInString().substr(4,4);
    if (startDate.length() == DATE_DDMM_LENGTH && endDate.length() == DATE_DDMM_LENGTH) {
        configureStartDayMonthEndDayMonth(startDate, endDate);
    } else if(startDate.length() == DATE_DDMM_LENGTH && endDate.length() == DATE_DDMMYYYY_LENGTH) {
        //if startDate >= endDate minus 1 year from startDate
        std::string endDateYYYY = endDate.substr(4,4); 
        if (endDateYYYY < currentDateYYYY) {
            startDate = startDate + endDateYYYY;
        } else {
            startDate = startDate + currentDateYYYY;
            if (startDate.substr(0,4) == DATE_NUMERIC_DDMM_29_FEB && !dateChecker->isLeapYear(startDate.substr(4,4))) {
                startDate = last29FebFrom(startDate);
            } else if(!dateChecker->isUnoccurredDate(startDate)) {
                startDate = addNYearsFromDate(startDate, 1);
        }

        }

        if (startDate.substr(0,4) == DATE_NUMERIC_DDMM_29_FEB && !dateChecker->isLeapYear(startDate.substr(4,4))) {
            startDate = last29FebFrom(startDate);
        }

        while(!isStartDateLessThanEndDate(startDate, endDate)) {
            startDate = subtractNYearsFromDate(startDate, 1);
        }
    } else if(startDate.length() == DATE_DDMMYYYY_LENGTH && endDate.length() == DATE_DDMM_LENGTH) {
        //if endDate <= startDate add 1 year to endDate
        std::string startDateYYYY = startDate.substr(4,4);
        if (startDateYYYY >= currentDateYYYY) {
            endDate = endDate + startDateYYYY;
        } else {
            endDate = endDate + currentDateYYYY;
            if (endDate.substr(0,4) == DATE_NUMERIC_DDMM_29_FEB && !dateChecker->isLeapYear(endDate.substr(4,4))) {
                endDate = next29FebFrom(endDate);
            } else if(!dateChecker->isUnoccurredDate(endDate)) {
                endDate = addNYearsFromDate(endDate, 1);
            }
        }

        if (endDate.substr(0,4) == DATE_NUMERIC_DDMM_29_FEB && !dateChecker->isLeapYear(endDate.substr(4,4))) {
            endDate = next29FebFrom(endDate);
        }

        while(!isStartDateLessThanEndDate(startDate, endDate)) {
            endDate = addNYearsFromDate(endDate, 1);
        }
    }
    return;
}

void TMParser::configureStartTimeEndTime(std::string& startTime, std::string& endTime) {
    TimeChecker *timeChecker = TimeChecker::getInstance();
    FormatConverter *formatConverter = FormatConverter::getInstance();

    if(timeChecker->isTimeWithoutPeriod(startTime) && timeChecker->isTimeWithoutPeriod(endTime)) {
        configureStartTimeEndTimeWithoutPeriods(startTime, endTime);
        //IF STARTTIME HAS PASSED <= CURRENTTIME ADD ONE DAY TO STARTDATE AND ENDDATE
    } else if (timeChecker->isTimeWithoutPeriod(startTime) && endTime.length() == TIME_HH_COLON_MM_LENGTH) {
        std::string timeInAM = formatConverter->timeFrom12HourAMToHHMM(startTime + TIME_PERIOD_AM);
        std::string timeInPM = formatConverter->timeFrom12HourPMToHHMM(startTime + TIME_PERIOD_PM);
        
        if(endTime == TIME_00_COLON_00) {
            startTime = timeInPM;
        } else if (timeInPM < endTime) {
            startTime = timeInPM;
        } else if (timeInAM < endTime) {
            startTime = timeInAM;
        } else {
            startTime = timeInPM;
        } 
    } else if (startTime.length() == TIME_HH_COLON_MM_LENGTH && timeChecker->isTimeWithoutPeriod(endTime)) {
        std::string timeInAM = formatConverter->timeFrom12HourAMToHHMM(endTime + TIME_PERIOD_AM);
        std::string timeInPM = formatConverter->timeFrom12HourPMToHHMM(endTime + TIME_PERIOD_PM);

        if(startTime == TIME_00_COLON_00) {
            if(timeInAM != TIME_00_COLON_00) {
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
}

bool TMParser::isStartDateLessThanEndDate(std::string startDate, std::string endDate) {
    DateChecker *dateChecker = DateChecker::getInstance();
    boost::gregorian::date boostStartDate;
    boost::gregorian::date boostEndDate;

    FormatConverter *formatConverter = FormatConverter::getInstance();
    startDate = formatConverter->dateFromNumericToBoostFormat(startDate);
    endDate = formatConverter->dateFromNumericToBoostFormat(endDate);
    if(dateChecker->isValidDate(startDate) && dateChecker->isValidDate(endDate)) {
        boostStartDate = boost::gregorian::from_uk_string(startDate);
        boostEndDate = boost::gregorian::from_uk_string(endDate);
    } else {
        return false;
    }

    if(boostStartDate < boostEndDate) {
        return true;
    } else {
        return false;
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
            addErrorMessage(ERROR_STARTTIME_LATER_THAN_ENDTIME);
            return false;
        }
    } else {
        addErrorMessage(ERROR_STARTDATE_LATER_THAN_ENDDATE);
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
        } else if (positionOfFirstDash != std::string::npos && token[0] != '-' && token[token.length()-1] != '-') {
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
std::string TMParser::addNDaysFromDate(std::string date, int n){
    FormatConverter *formatConverter = FormatConverter::getInstance();
    date = formatConverter->dateFromNumericToBoostFormat(date);
    boost::gregorian::date initialBoostDate = boost::gregorian::from_uk_string(date);
    boost::gregorian::date_duration dateDuration(n);
    boost::gregorian::date finalBoostDate = initialBoostDate + dateDuration;
    return formatConverter->dateFromBoostToDDMMYYYY(finalBoostDate);
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
//postconditions ddmmyyyy
//if 29 feb it will find the first leap year if not this year
std::string TMParser::addNYearsFromDate(std::string date, int n) {
    FormatConverter *formatConverter = FormatConverter::getInstance();
    DateChecker *dateChecker = DateChecker::getInstance();
    std::string ddmm = date.substr(0,4);

    date = formatConverter->dateFromNumericToBoostFormat(date);
    boost::gregorian::date initialBoostDate = boost::gregorian::from_uk_string(date);
    boost::gregorian::date finalBoostDate;

    
    boost::gregorian::year_iterator year(initialBoostDate);

    for(int i = 0; i < n; i++) {
            ++year;
        }

    finalBoostDate = *year;

    if(ddmm == DATE_NUMERIC_DDMM_28_FEB) {
        if(boost::gregorian::gregorian_calendar::is_leap_year(finalBoostDate.year())) {
            boost::gregorian::date_duration oneDay(1);
            finalBoostDate = finalBoostDate - oneDay;
        }
    } else if(ddmm == DATE_NUMERIC_DDMM_29_FEB) {
        while(!boost::gregorian::gregorian_calendar::is_leap_year(finalBoostDate.year())) {
            ++year;
            finalBoostDate = *year;
        }

    }

    return formatConverter->dateFromBoostToDDMMYYYY(finalBoostDate);
}

std::string TMParser::subtractNYearsFromDate(std::string date, int n) {
    FormatConverter *formatConverter = FormatConverter::getInstance();
    DateChecker *dateChecker = DateChecker::getInstance();
    std::string ddmm = date.substr(0,4);

    date = formatConverter->dateFromNumericToBoostFormat(date);
    boost::gregorian::date initialBoostDate = boost::gregorian::from_uk_string(date);
    boost::gregorian::date finalBoostDate;

    boost::gregorian::year_iterator year(initialBoostDate);

    for(int i = 0; i < n; i++) {
            --year;
    }

    finalBoostDate = *year;

    if(ddmm == DATE_NUMERIC_DDMM_28_FEB) {
        if(boost::gregorian::gregorian_calendar::is_leap_year(finalBoostDate.year())) {
            boost::gregorian::date_duration oneDay(1);
            finalBoostDate = finalBoostDate - oneDay;
        }
    } else if(ddmm == DATE_NUMERIC_DDMM_29_FEB) {
        while(!boost::gregorian::gregorian_calendar::is_leap_year(finalBoostDate.year())) {
            --year;
            finalBoostDate = *year;
        }
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

//precondition: unitString must be in lowercase
bool TMParser::isWordBeforeOrByOrFromOrTo(std::string unitString) {
    TaskChecker *taskChecker = TaskChecker::getInstance();

    if (taskChecker->isWordBefore(unitString)||
        taskChecker->isWordBy(unitString)||
        taskChecker->isWordFrom(unitString)||
        taskChecker->isWordTo(unitString)) {
            return true;
    } else {
        return false;
    }
}

std::string TMParser::next29FebFrom(std::string date) {
    std::string currentYear = currentDateInString().substr(4,4);
    int year = std::stoi(currentYear);
    int yearIn4Years = year + 4;
    int nextLeapYear = yearIn4Years - (yearIn4Years % 4);
    return DATE_NUMERIC_DDMM_29_FEB + std::to_string(nextLeapYear);
}

std::string TMParser::last29FebFrom(std::string date) {
    std::string currentYear = currentDateInString().substr(4,4);
    int year = std::stoi(currentYear);
    int nextLeapYear = year - (year%4);
    return DATE_NUMERIC_DDMM_29_FEB + std::to_string(nextLeapYear);
}