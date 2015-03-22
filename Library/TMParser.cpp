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
const std::string TOKEN_FROM = "from";
const std::string TOKEN_TO = "to";

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
                        //std::cout << "close inverted commas missing\n";
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

//Preconditions: getTokenizedUserEntry(userEntry) first
//Postconditions: first token is extracted from vector of string and is no longer is there
std::string TMParser::extractCommand() {
    std::string command;
    command = _tokenizedUserEntry[0];
    _tokenizedUserEntry.erase(_tokenizedUserEntry.begin());

    return command;
}

std::vector<std::string> TMParser::returnTokens() {
	return _tokenizedUserEntry;
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
std::vector<TMTask> TMParser::parseTaskInfo() {
    std::vector<TMTask> task;
    //std::cout << "parseTaskInfo\n";
    if(isDeadlinedTask()){
        //std::cout << "IS DEADLINE\n";
        task.push_back(parseDeadlinedTaskInfo());
    } else if(isTimedTask()) {
        
       // std::cout << "IS TIMED\n";
        task.push_back(parseTimedTaskInfo());
    } else {
        
       // std::cout << "floating task\n";
        task.push_back(parseFloatingTaskInfo());
		//std::cout << "DONE WITH FLOATING" << std::endl;
    }
    return task;
}

//Preconditions:task is deadlined task use isDeadlinedTask to check
TMTask TMParser::parseDeadlinedTaskInfo() {
    TaskType taskType = TaskType::WithDeadline;
    std::string dateToMeet = "";
    std::string timeToMeet = "";
    std::vector<std::string> remainingEntry = _tokenizedUserEntry;
    std::string taskDescription = "";

    std::string unitString;
    std::string stringAfterBefore;
    std::vector<std::string>::iterator iter;

    bool iterMinusOne = false;

    for(iter = remainingEntry.begin(); iter < remainingEntry.end(); iter++){
        if(iterMinusOne){
            iter--;
            iterMinusOne = false;
        }

        unitString = returnLowerCase(*iter);
        //
        //std::cout << unitString << std::endl;
        if(unitString == TOKEN_BEFORE){
            stringAfterBefore = *(iter+1);
            stringAfterBefore = returnLowerCase(stringAfterBefore);

            //
            //std::cout << "Before\n";
            if(isDate(stringAfterBefore)) {//before date DDMMYYYY
                dateToMeet = stringAfterBefore;
                dateToMeet = dateFromUserToBoostFormat(dateToMeet);
                dateToMeet = substractNDaysFromDate(dateToMeet,1);
                iter = remainingEntry.erase(iter,iter+2);
                timeToMeet = "2359";
                if(iter == remainingEntry.end()){
                    break;
                } else if (iter == remainingEntry.begin()){
                    iterMinusOne = true;
                } else {
                    iter--;
                }
                //convert to one day earlier at 2359 DONE
            } else if (isDay(stringAfterBefore)) {//before monday
                //returns user the earliest x-day from today
                stringAfterBefore = returnLowerCase(stringAfterBefore);
                int dayInInteger = dayOfWeek(stringAfterBefore);
                boost::gregorian::first_day_of_the_week_after fdaf(dayInInteger);
                boost::gregorian::date dateInBoost = fdaf.get_date(_dateToday);
                dateToMeet = dateFromBoostToDelimitedDDMMYYYY(dateInBoost);
                dateToMeet = substractNDaysFromDate(dateToMeet,1);
                timeToMeet = "2359";
                //convert to one day earlier at 2359 DONE
                iter = remainingEntry.erase(iter,iter+2);
                if(iter == remainingEntry.end()){
                    break;
                } else if (iter == remainingEntry.begin()){
                    iterMinusOne = true;
                } else {
                    iter--;
                }
            } else if (isWordNext(stringAfterBefore)) {//before next monday
                if(iter+2 != remainingEntry.end()){
                    std::string stringAfterNext = *(iter+2);
                    stringAfterNext = returnLowerCase(stringAfterNext);
                    //
                    std::cout << stringAfterNext << std::endl;
                    if(isDay(stringAfterNext)) {
                        std::string stringDay = stringAfterNext;
                        boost::gregorian::date_duration oneWeek(7);
                        boost::gregorian::date oneWeekFromToday = _dateToday + oneWeek;
                        boost::gregorian::greg_weekday day(dayOfWeek(stringDay));
                        boost::gregorian::date dateTM = boost::gregorian::next_weekday(oneWeekFromToday, day);
                        std::string tempDate = boost::gregorian::to_iso_string(dateTM);
                        dateToMeet = tempDate.substr(6,2)
                                    + tempDate.substr(4,2)
                                    + tempDate.substr(0,4);
                        dateToMeet = dateFromUserToBoostFormat(dateToMeet);
                        dateToMeet = substractNDaysFromDate(dateToMeet,1);
                        timeToMeet = "2359";
                        iter = remainingEntry.erase(iter,iter+3);
                        if(iter == remainingEntry.end()){
                            break;
                        } else if (iter == remainingEntry.begin()){
                            iterMinusOne = true;
                        } else {
                            iter--;
                        }
                    } else {
                        //before next (day missing)
                        //treat as task description
                    }
                } else {
                    //treats as task description
                }
            } else if (is12HTime(stringAfterBefore)||is24HTime(stringAfterBefore)) {
                //before time
                //std::cout << "IS TIME\n";
                timeToMeet = timeTo24HFormat(stringAfterBefore);
                //check for word on
                //if exists, check date, next x-day, x-day
                //assume (before time) (on day) must check (on day) separately
                //else today if time hasn't passed. how to check that?
                if(iter+2 != remainingEntry.end()){
                    std::string stringAfterTime = *(iter+2);
                    //
                    //std::cout << stringAfterTime << std::endl;
                    stringAfterTime = returnLowerCase(stringAfterTime);
                    if(stringAfterTime == TOKEN_ON){//before time on date/day
                        if(iter+3 != remainingEntry.end()){
                            std::string stringAfterOn = *(iter+3);
                            stringAfterOn = returnLowerCase(stringAfterOn);
                            if(isDate(stringAfterOn)){
                                dateToMeet = stringAfterOn;
                                iter = remainingEntry.erase(iter,iter+4);
                                if(iter == remainingEntry.end()){
                                    break;
                                } else if (iter == remainingEntry.begin()){
                                    iterMinusOne = true;
                                } else {
                                    iter--;
                                }
                            } else if(isDay(stringAfterOn)){
                                //returns user the earliest x-day from today
                                stringAfterOn = returnLowerCase(stringAfterOn);
                                int dayInInteger = dayOfWeek(stringAfterOn);
                                boost::gregorian::first_day_of_the_week_after fdaf(dayInInteger);
                                boost::gregorian::date dateInBoost = fdaf.get_date(_dateToday);
                                dateToMeet = dateFromBoostToDDMMYYYY(dateInBoost);
                                iter = remainingEntry.erase(iter,iter+4);
                                if(iter == remainingEntry.end()){
                                    break;
                                } else if (iter == remainingEntry.begin()){
                                    iterMinusOne = true;
                                } else {
                                    iter--;
                                }
                            } else {
                                //found before time but no day
                                //check if current time more than found time
                                //if so tomorrow otherwise today
                                std::string currentTime = getCurrentTime();
                                if(timeToMeet >= currentTime){
                                    boost::gregorian::date currentDate = _dateToday;
                                    dateToMeet = dateFromBoostToDDMMYYYY(currentDate);
                                } else {
                                    boost::gregorian::date currentDate = _dateToday;
                                    dateToMeet = dateFromBoostToDelimitedDDMMYYYY(currentDate);
                                    dateToMeet = addNDaysFromDate(dateToMeet,1);
                                }
                                iter = remainingEntry.erase(iter,iter+2);
                                if(iter == remainingEntry.end()){
                                    break;
                                } else if (iter == remainingEntry.begin()){
                                    iterMinusOne = true;
                                } else {
                                    iter--;
                                }
                            }
                        }
                    } else if(isWordNext(stringAfterTime)){
                        if(iter+3 != remainingEntry.end()){
                        std::string stringAfterNext = *(iter+3);
                        stringAfterNext = returnLowerCase(stringAfterNext);
                            if(isDay(stringAfterNext)){
                                std::string stringDay = returnLowerCase(stringAfterNext);
                                boost::gregorian::date_duration oneWeek(7);
                                boost::gregorian::date oneWeekFromToday = _dateToday + oneWeek;
                                boost::gregorian::greg_weekday day(dayOfWeek(stringDay));
                                boost::gregorian::date dateTM = boost::gregorian::next_weekday(oneWeekFromToday, day);
                                std::string tempDate = boost::gregorian::to_iso_string(dateTM);
                                dateToMeet = tempDate.substr(6,2)
                                            + "-"
                                            + tempDate.substr(4,2)
                                            + "-"
                                            + tempDate.substr(0,4);
                                iter = remainingEntry.erase(iter,iter+4);
                                if(iter == remainingEntry.end()){
                                    break;
                                } else if (iter == remainingEntry.begin()){
                                    iterMinusOne = true;
                                } else {
                                    iter--;
                                }
                            } else {
                                    //
                                    //
                            }
                        } else {
                                //
                                //
                        }
                    } else {
                        //dateToMeet = today if time hasn't passed otherwise tomorrow
                        std::string currentTime = getCurrentTime();
                        //
                        //std::cout << currentTime << std::endl;
                        if(timeToMeet >= currentTime){
                            boost::gregorian::date currentDate = _dateToday;
                            dateToMeet = dateFromBoostToDDMMYYYY(currentDate);
                        } else {
                            boost::gregorian::date currentDate = _dateToday;
                            dateToMeet = dateFromBoostToDelimitedDDMMYYYY(currentDate);
                            dateToMeet = addNDaysFromDate(dateToMeet,1);
                        }

                        iter = remainingEntry.erase(iter,iter+2);

                        if(iter == remainingEntry.end()){
                            break;
                        } else if (iter == remainingEntry.begin()){
                            iterMinusOne = true;
                        } else {
                            iter--;
                        }
                    }
                } else {
                    //dateToMeet = today if time hasn't passed otherwise tomorrow
                    std::string currentTime = getCurrentTime();
                    //
                    //std::cout << currentTime << std::endl;
                    //std::cout << timeToMeet << std::endl;
                    if(timeToMeet >= currentTime){
                        boost::gregorian::date currentDate = _dateToday;
                        dateToMeet = dateFromBoostToDDMMYYYY(currentDate);
                        //
                        //std::cout << dateToMeet << std::endl;
                    } else {
                        boost::gregorian::date currentDate = _dateToday;
                        dateToMeet = dateFromBoostToDelimitedDDMMYYYY(currentDate);
                        dateToMeet = addNDaysFromDate(dateToMeet,1);
                    }
                    //
                    iter = remainingEntry.erase(iter,iter+2);
                    //
                    /*
                    if(iter != remainingEntry.end()){
                        std::cout << *iter << std::endl;
                    } else {
                        std::cout << "end of string\n";
                    }

                    for (std::vector<std::string>::iterator it = remainingEntry.begin() ; it != remainingEntry.end(); ++it){
                        std::cout << ' ' << *it;
                    }
                    */
                    if(iter == remainingEntry.end()){
                        break;
                    } else if (iter == remainingEntry.begin()){
                        iterMinusOne = true;
                    } else {
                        iter--;
                    }
                }
            } else {
                //cannot find anything after word before
                //continue;
            }
        } else {
            //word in main loop is not before. continue searching
        }
    }


    int sizeOfVector = remainingEntry.size();
    //
    std::cout << "size: " << sizeOfVector << std::endl;
    for(int i = 0; i < sizeOfVector; i++) {
        taskDescription += remainingEntry[i];
        if(i != sizeOfVector - 1) {
            taskDescription += " ";
        }
    }

    //
    
    //DDMMYYYY to DD MM YYYY
    dateToMeet = dateToMeet.substr(0,2) + " " + dateToMeet.substr(2,2) + " " + dateToMeet.substr(4,4);
    //
    //std::cout << "final dateTM: " << dateToMeet << std::endl;
    if(isValidDate(dateToMeet)){
        TMTaskTime taskTime(dateToMeet,timeToMeet,"","");
        TMTask task(taskDescription,taskTime,taskType);
        return task;
    } else {
        //when date is invalid
        TMTaskTime taskTime;
        TMTask task("",taskTime,TaskType::Invalid);
        return task;
    }
}

//for now find start time and start date only
//change to period

TMTask TMParser::parseTimedTaskInfo(){
    TaskType taskType = TaskType::Timed;
    std::string startTime = "";
    //std::string dayToMeet = "";
    //missing endTime and endDate need to use function to determine period of time and dates
    std::string startDate = "";
    std::string endTime = "";
    std::string endDate = "";
    std::vector<std::string> remainingEntry = _tokenizedUserEntry;
    std::string taskDescription = "";
    std::string unitString;
    std::vector<std::string>::iterator iter;
    bool startTimeExtracted = false;
    bool startDateExtracted = false;
    bool endTimeExtracted = false;
    bool endDateExtracted = false;
    bool iterMinusOne = false;

    //seach for at (time)
    for(iter = remainingEntry.begin(); iter < remainingEntry.end()-1; iter++){
        if(iterMinusOne){
            iter--;
            iterMinusOne = false;
        }
        unitString = *iter;
        unitString = returnLowerCase(unitString);

        if(unitString == TOKEN_AT){
            std::string stringAfterAt = *(iter+1);
            //
            //std::cout << stringAfterAt << std::endl;
            stringAfterAt = returnLowerCase(stringAfterAt);
            //
            //std::cout << stringAfterAt << std::endl;
            if(is12HTime(stringAfterAt)||is24HTime(stringAfterAt)){
                startTime = stringAfterAt;
                //
                //std::cout << stringAfterAt << std::endl;
                startTime = timeTo24HFormat(startTime);
                //std::cout << startTime << std::endl;
                iter = remainingEntry.erase(iter,iter+2);
                startTimeExtracted = true;
                if(iter == remainingEntry.end()){
                    break;
                } else if (iter == remainingEntry.begin()){
                    iterMinusOne = true;
                } else {
                    iter--;
                }
            } else {
                //at (not time) continue searching
            }
        } else if(unitString == TOKEN_ON){
            std::string stringAfterOn = *(iter+1);
            stringAfterOn = returnLowerCase(stringAfterOn);
            if(isDate(stringAfterOn)){
                startDate = stringAfterOn;
                iter = remainingEntry.erase(iter,iter+2);
                startDateExtracted = true;
                if(iter == remainingEntry.end()){
                    break;
                } else if (iter == remainingEntry.begin()){
                    iterMinusOne = true;
                } else {
                    iter--;
                }
            } else if(isDay(stringAfterOn)){
                stringAfterOn = returnLowerCase(stringAfterOn);
                int dayInInteger = dayOfWeek(stringAfterOn);
                boost::gregorian::first_day_of_the_week_after fdaf(dayInInteger);
                boost::gregorian::date dateInBoost = fdaf.get_date(_dateToday);
                startDate = dateFromBoostToDDMMYYYY(dateInBoost);
                iter = remainingEntry.erase(iter,iter+2);
                startDateExtracted = true;
                if(iter == remainingEntry.end()){
                    break;
                } else if (iter == remainingEntry.begin()){
                    iterMinusOne = true;
                } else {
                    iter--;
                }
            } else {
                //found on but cannot find following date or day
                //need to check another condition: next x-day
            }
        } else if(unitString == TOKEN_FROM){
            //checks for startTime and startDate
            std::string stringAfterFrom = *(iter + 1);
            stringAfterFrom = returnLowerCase(stringAfterFrom);

            if(isDay(stringAfterFrom)) {
                //convert day to date first
                int dayInInteger = dayOfWeek(stringAfterFrom);
                boost::gregorian::first_day_of_the_week_after fdaf(dayInInteger);
                boost::gregorian::date dateInBoost = fdaf.get_date(_dateToday);
                startDate = dateFromBoostToDDMMYYYY(dateInBoost);
                //startDate extracted
                startDateExtracted = true;
                //check for time after day
                if(iter + 2 != remainingEntry.end()) {
                    std::string stringAfterDay = *(iter + 2);
                    stringAfterDay = returnLowerCase(stringAfterDay);
                    //found before day time
                    if(is12HTime(stringAfterDay)||is24HTime(stringAfterDay)) {
                        startTime = stringAfterDay;
                        startTime = timeTo24HFormat(startTime);
                        startTimeExtracted = true;
                        iter = remainingEntry.erase(iter,iter + 3);
                        if(iter == remainingEntry.end()){
                            break;
                        } else if (iter == remainingEntry.begin()){
                            iterMinusOne = true;
                        } else {
                            iter--;
                        }
                    } else {
                        //found only from Day
                        iter = remainingEntry.erase(iter,iter + 2);
                        if(iter == remainingEntry.end()){
                            break;
                        } else if (iter == remainingEntry.begin()){
                            iterMinusOne = true;
                        } else {
                            iter--;
                        }
                    }
                } else {
                    //found only "from Day"
                    iter = remainingEntry.erase(iter,iter+2);
                    if(iter == remainingEntry.end()){
                        break;
                    } else if (iter == remainingEntry.begin()){
                        iterMinusOne = true;
                    } else {
                        iter--;
                    }
                }
            } else if(isDate(stringAfterFrom)) {
                //check for time after date
                startDate = stringAfterFrom;
                startDateExtracted = true;
                if(iter + 2 != remainingEntry.end()) {
                std::string stringAfterDay = *(iter + 2);
                stringAfterDay = returnLowerCase(stringAfterDay);
                //found before day time
                    if(is12HTime(stringAfterDay)||is24HTime(stringAfterDay)) {
                        startTime = stringAfterDay;
                        startTime = timeTo24HFormat(startTime);
                        startTimeExtracted = true;
                        iter = remainingEntry.erase(iter,iter + 3);
                        if(iter == remainingEntry.end()){
                            break;
                        } else if (iter == remainingEntry.begin()){
                            iterMinusOne = true;
                        } else {
                            iter--;
                        }
                    } else {
                        //found only from Day
                        iter = remainingEntry.erase(iter,iter + 2);
                        if(iter == remainingEntry.end()){
                            break;
                        } else if (iter == remainingEntry.begin()){
                            iterMinusOne = true;
                        } else {
                           iter--;
                        }
                    }
                } else {
                    //found only "from Date"
                    iter = remainingEntry.erase(iter,iter+2);
                    if(iter == remainingEntry.end()){
                        break;
                    } else if (iter == remainingEntry.begin()){
                        iterMinusOne = true;
                    } else {
                        iter--;
                    }
                }
            } else if(isWordNext(stringAfterFrom)) {
                //check for day after next, then check for time
                if(iter + 2 != remainingEntry.end()) {
                    //
                    std::string stringAfterNext = *(iter + 2);
                    stringAfterNext = returnLowerCase(stringAfterNext);
                    if(isDay(stringAfterNext)) {
                        //get date from next Day
                        std::string stringDay = stringAfterNext;
                        boost::gregorian::date_duration oneWeek(7);
                        boost::gregorian::date oneWeekFromToday = _dateToday + oneWeek;
                        boost::gregorian::greg_weekday day(dayOfWeek(stringDay));
                        boost::gregorian::date dateTM = boost::gregorian::next_weekday(oneWeekFromToday, day);
                        std::string tempDate = boost::gregorian::to_iso_string(dateTM);
                        startDate = tempDate.substr(6,2)
                                    + tempDate.substr(4,2)
                                    + tempDate.substr(0,4);
                        startDateExtracted = true;
                        //check for time
                        if(iter + 3 != remainingEntry.end()) {
                            //
                            std::string stringAfterDay = *(iter + 3);
                            stringAfterDay = returnLowerCase(stringAfterDay);
                            if(is12HTime(stringAfterDay)||is24HTime(stringAfterDay)) {
                                startTime = stringAfterDay;
                                startTime = timeTo24HFormat(startTime);
                                iter = remainingEntry.erase(iter, iter + 4);
                                startTimeExtracted = true;
                                if(iter == remainingEntry.end()){
                                    break;
                                } else if (iter == remainingEntry.begin()){
                                    iterMinusOne = true;
                                } else {
                                    iter--;
                                }
                            } else {
                                iter = remainingEntry.erase(iter, iter +3);
                                if(iter == remainingEntry.end()){
                                    break;
                                } else if (iter == remainingEntry.begin()){
                                     iterMinusOne = true;
                                } else {
                                    iter--;
                                }
                            }
                        } else {
                            iter = remainingEntry.erase(iter, iter + 3);
                            if(iter == remainingEntry.end()){
                                break;
                            } else if (iter == remainingEntry.begin()){
                                iterMinusOne = true;
                            } else {
                                iter--;
                            }
                        }
                    } else {
                        //found from next .. task descr. continue
                    }
                } else {
                    //found from next. treat as task description. continue
                }
            } else if(is12HTime(stringAfterFrom)||is24HTime(stringAfterFrom)){
                startTime = stringAfterFrom;
                startTime = timeTo24HFormat(startTime);
                startTimeExtracted = true;
                if(iter + 2 != remainingEntry.end()){
                    std::string stringAfterTime = *(iter+2);
                    stringAfterTime = returnLowerCase(stringAfterTime);
                    if(isDate(stringAfterTime)){
                        startDate = stringAfterTime;
                        iter = remainingEntry.erase(iter,iter+3);
                        startDateExtracted = true;
                        if(iter == remainingEntry.end()){
                        break;
                        } else if (iter == remainingEntry.begin()){
                            iterMinusOne = true;
                        } else {
                            iter--;
                        }
                    } else if(isDay(stringAfterTime)){
                        stringAfterTime = returnLowerCase(stringAfterTime);
                        int dayInInteger = dayOfWeek(stringAfterTime);
                        boost::gregorian::first_day_of_the_week_after fdaf(dayInInteger);
                        boost::gregorian::date dateInBoost = fdaf.get_date(_dateToday);
                        startDate = dateFromBoostToDDMMYYYY(dateInBoost);
                        iter = remainingEntry.erase(iter,iter+3);
                        startDateExtracted = true;
                        if(iter == remainingEntry.end()){
                            break;
                        } else if (iter == remainingEntry.begin()){
                            iterMinusOne = true;
                        } else {
                            iter--;
                        }
                    } else if(isWordNext(stringAfterTime)){
                        if(iter + 3 != remainingEntry.end()) {
                            std::string stringAfterNext = *(iter + 3);
                            stringAfterNext = returnLowerCase(stringAfterNext);
                            if(isDay(stringAfterNext)) {
                                //get date from next Day
                                std::string stringDay = stringAfterNext;
                                boost::gregorian::date_duration oneWeek(7);
                                boost::gregorian::date oneWeekFromToday = _dateToday + oneWeek;
                                boost::gregorian::greg_weekday day(dayOfWeek(stringDay));
                                boost::gregorian::date dateTM = boost::gregorian::next_weekday(oneWeekFromToday, day);
                                std::string tempDate = boost::gregorian::to_iso_string(dateTM);
                                startDate = tempDate.substr(6,2)
                                          + tempDate.substr(4,2)
                                          + tempDate.substr(0,4);
                                startDateExtracted = true;
                                iter = remainingEntry.erase(iter, iter + 4);
                                if(iter == remainingEntry.end()){
                                    break;
                                } else if (iter == remainingEntry.begin()){
                                    iterMinusOne = true;
                                } else {
                                    iter--;
                                }
                                //check for time
                            } else {
                            //found from time next take out time next?
                                iter = remainingEntry.erase(iter,iter + 2);
                                if(iter == remainingEntry.end()){
                                    break;
                                } else if (iter == remainingEntry.begin()){
                                    iterMinusOne = true;
                                } else {
                                    iter--;
                                }
                            }
                        } else {
                        //found from next. treat as task description. continue
                            iter = remainingEntry.erase(iter,iter + 2);
                            if(iter == remainingEntry.end()){
                                break;
                            } else if (iter == remainingEntry.begin()){
                                iterMinusOne = true;
                            } else {
                                iter--;
                            }
                        }
                    } else {
                        //found only "from startTime"
                        iter = remainingEntry.erase(iter,iter + 2);
                        if(iter == remainingEntry.end()){
                            break;
                        } else if (iter == remainingEntry.begin()){
                            iterMinusOne = true;
                        } else {
                            iter--;
                        }
                    }
                } else {
                    //found only "from startTime"
                    iter = remainingEntry.erase(iter,iter + 2);
                    if(iter == remainingEntry.end()){
                        break;
                    } else if (iter == remainingEntry.begin()){
                        iterMinusOne = true;
                    } else {
                        iter--;
                    }
                }
            } else {
                //cannot find a startTime or startDate. from is treated as task desc
            }
        } else if (unitString == TOKEN_TO){
            //checks for endTime and endDate
            std::string stringAfterTo = *(iter + 1);
            stringAfterTo = returnLowerCase(stringAfterTo);

            if(isDay(stringAfterTo)) {
                //convert day to date first
                int dayInInteger = dayOfWeek(stringAfterTo);
                boost::gregorian::first_day_of_the_week_after fdaf(dayInInteger);
                boost::gregorian::date dateInBoost = fdaf.get_date(_dateToday);
                endDate = dateFromBoostToDDMMYYYY(dateInBoost);
                //endDate extracted
                endDateExtracted = true;
                //check for time after day
                if(iter + 2 != remainingEntry.end()) {
                    std::string stringAfterDay = *(iter + 2);
                    stringAfterDay = returnLowerCase(stringAfterDay);
                    //found before day time
                    if(is12HTime(stringAfterDay)||is24HTime(stringAfterDay)) {
                        endTime = stringAfterDay;
                        endTime = timeTo24HFormat(endTime);
                        endTimeExtracted = true;
                        iter = remainingEntry.erase(iter,iter + 3);
                        if(iter == remainingEntry.end()){
                            break;
                        } else if (iter == remainingEntry.begin()){
                            iterMinusOne = true;
                        } else {
                            iter--;
                        }
                    } else {
                        //found only from Day
                        iter = remainingEntry.erase(iter,iter + 2);
                        if(iter == remainingEntry.end()){
                            break;
                        } else if (iter == remainingEntry.begin()){
                            iterMinusOne = true;
                        } else {
                            iter--;
                        }
                    }
                } else {
                    //found only "from Day"
                    iter = remainingEntry.erase(iter,iter+2);
                    if(iter == remainingEntry.end()){
                        break;
                    } else if (iter == remainingEntry.begin()){
                        iterMinusOne = true;
                    } else {
                        iter--;
                    }
                }
            } else if(isDate(stringAfterTo)) {
                //check for time after date
                endDate = stringAfterTo;
                endDateExtracted = true;
                if(iter + 2 != remainingEntry.end()) {
                std::string stringAfterDate = *(iter + 2);
                stringAfterDate = returnLowerCase(stringAfterDate);
                //found before day time
                    if(is12HTime(stringAfterDate)||is24HTime(stringAfterDate)) {
                        endTime = stringAfterDate;
                        endTime = timeTo24HFormat(endTime);
                        endTimeExtracted = true;
                        iter = remainingEntry.erase(iter,iter + 3);
                        if(iter == remainingEntry.end()){
                            break;
                        } else if (iter == remainingEntry.begin()){
                            iterMinusOne = true;
                        } else {
                            iter--;
                        }
                    } else {
                        //found only from Date
                        iter = remainingEntry.erase(iter,iter + 2);
                        if(iter == remainingEntry.end()){
                            break;
                        } else if (iter == remainingEntry.begin()){
                            iterMinusOne = true;
                        } else {
                           iter--;
                        }
                    }
                } else {
                    //found only "from Day"
                    iter = remainingEntry.erase(iter,iter+2);
                    if(iter == remainingEntry.end()){
                        break;
                    } else if (iter == remainingEntry.begin()){
                        iterMinusOne = true;
                    } else {
                        iter--;
                    }
                }
            } else if(isWordNext(stringAfterTo)) {
                //check for day after next, then check for time
                if(iter + 2 != remainingEntry.end()) {
                    //
                    std::string stringAfterNext = *(iter + 2);
                    stringAfterNext = returnLowerCase(stringAfterNext);
                    if(isDay(stringAfterNext)) {
                        //get date from next Day
                        std::string stringDay = stringAfterNext;
                        boost::gregorian::date_duration oneWeek(7);
                        boost::gregorian::date oneWeekFromToday = _dateToday + oneWeek;
                        boost::gregorian::greg_weekday day(dayOfWeek(stringDay));
                        boost::gregorian::date dateTM = boost::gregorian::next_weekday(oneWeekFromToday, day);
                        std::string tempDate = boost::gregorian::to_iso_string(dateTM);
                        endDate = tempDate.substr(6,2)
                                    + tempDate.substr(4,2)
                                    + tempDate.substr(0,4);
                        endDateExtracted = true;
                        //check for time
                        if(iter + 3 != remainingEntry.end()) {
                            //
                            std::string stringAfterDay = *(iter + 3);
                            stringAfterDay = returnLowerCase(stringAfterDay);
                            if(is12HTime(stringAfterDay)||is24HTime(stringAfterDay)) {
                                endTime = stringAfterDay;
                                endTime = timeTo24HFormat(endTime);
                                iter = remainingEntry.erase(iter, iter + 4);
                                endTimeExtracted = true;
                                if(iter == remainingEntry.end()){
                                    break;
                                } else if (iter == remainingEntry.begin()){
                                    iterMinusOne = true;
                                } else {
                                    iter--;
                                }
                            } else {
                                iter = remainingEntry.erase(iter, iter +3);
                                if(iter == remainingEntry.end()){
                                    break;
                                } else if (iter == remainingEntry.begin()){
                                     iterMinusOne = true;
                                } else {
                                    iter--;
                                }
                            }
                        } else {
                            iter = remainingEntry.erase(iter, iter + 3);
                            if(iter == remainingEntry.end()){
                                break;
                            } else if (iter == remainingEntry.begin()){
                                iterMinusOne = true;
                            } else {
                                iter--;
                            }
                        }
                    } else {
                        //found from next .. task descr. continue
                    }
                } else {
                    //found from next. treat as task description. continue
                }
            } else if(is12HTime(stringAfterTo)||is24HTime(stringAfterTo)){
                endTime = stringAfterTo;
                endTime = timeTo24HFormat(endTime);
                endTimeExtracted = true;
                if(iter + 2 != remainingEntry.end()){
                    std::string stringAfterTime = *(iter+2);
                    stringAfterTime = returnLowerCase(stringAfterTime);
                    if(isDate(stringAfterTime)){
                        endDate = stringAfterTime;
                        iter = remainingEntry.erase(iter,iter+3);
                        endDateExtracted = true;
                        if(iter == remainingEntry.end()){
                        break;
                        } else if (iter == remainingEntry.begin()){
                            iterMinusOne = true;
                        } else {
                            iter--;
                        }
                    } else if(isDay(stringAfterTime)){
                        stringAfterTime = returnLowerCase(stringAfterTime);
                        int dayInInteger = dayOfWeek(stringAfterTime);
                        boost::gregorian::first_day_of_the_week_after fdaf(dayInInteger);
                        boost::gregorian::date dateInBoost = fdaf.get_date(_dateToday);
                        endDate = dateFromBoostToDDMMYYYY(dateInBoost);
                        iter = remainingEntry.erase(iter,iter+3);
                        endDateExtracted = true;
                        if(iter == remainingEntry.end()){
                            break;
                        } else if (iter == remainingEntry.begin()){
                            iterMinusOne = true;
                        } else {
                            iter--;
                        }
                    } else if(isWordNext(stringAfterTime)){
                        if(iter + 3 != remainingEntry.end()) {
                            std::string stringAfterNext = *(iter + 3);
                            stringAfterNext = returnLowerCase(stringAfterNext);
                            if(isDay(stringAfterNext)) {
                            //get date from next Day
                                std::string stringDay = stringAfterNext;
                                boost::gregorian::date_duration oneWeek(7);
                                boost::gregorian::date oneWeekFromToday = _dateToday + oneWeek;
                                boost::gregorian::greg_weekday day(dayOfWeek(stringDay));
                                boost::gregorian::date dateTM = boost::gregorian::next_weekday(oneWeekFromToday, day);
                                std::string tempDate = boost::gregorian::to_iso_string(dateTM);
                                endDate = tempDate.substr(6,2)
                                        + tempDate.substr(4,2)
                                        + tempDate.substr(0,4);
                                endDateExtracted = true;

                                iter = remainingEntry.erase(iter,iter+4);
                                if(iter == remainingEntry.end()){
                                    break;
                                } else if (iter == remainingEntry.begin()){
                                    iterMinusOne = true;
                                } else {
                                    iter--;
                                }
                                //
                            } else {
                                iter = remainingEntry.erase(iter,iter+2);
                                if(iter == remainingEntry.end()){
                                    break;
                                } else if (iter == remainingEntry.begin()){
                                    iterMinusOne = true;
                                } else {
                                    iter--;
                                }
                            }
                        } else {
                            iter = remainingEntry.erase(iter,iter+2);
                            if(iter == remainingEntry.end()){
                                break;
                            } else if (iter == remainingEntry.begin()){
                                iterMinusOne = true;
                            } else {
                                iter--;
                            }
                        }
                    } else {
                        //found only "from endTime"
                        iter = remainingEntry.erase(iter,iter + 2);
                        if(iter == remainingEntry.end()){
                            break;
                        } else if (iter == remainingEntry.begin()){
                            iterMinusOne = true;
                        } else {
                            iter--;
                        }
                    }
                } else {
                    //found only "from endTime"
                    iter = remainingEntry.erase(iter,iter + 2);
                    if(iter == remainingEntry.end()){
                        break;
                    } else if (iter == remainingEntry.begin()){
                        iterMinusOne = true;
                    } else {
                        iter--;
                    }
                }
            } else {
                //cannot find a endTime or endDate. from is treated as task desc
            }
        } else {
            //cannot find any markers
        }
    }
    //if obtained only time then check if time passed...
    //if only date obtained then set as 0000

    //assume user will surely use from and to combination
    if((startTimeExtracted||startDateExtracted) && !endTimeExtracted && !endDateExtracted){
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
    } else if((endTimeExtracted||endDateExtracted) && !startTimeExtracted && !startDateExtracted) {
        if(!endTimeExtracted){
        endTime = "0000";
        } else if(!endDateExtracted){
            std::string currentTime = getCurrentTime();
            if(startTime >= currentTime){
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
    } else if((startDateExtracted||startTimeExtracted) && (endDateExtracted||endTimeExtracted)){
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
        
        if(!endTimeExtracted){
        endTime = "0000";
        } else if(!endDateExtracted){
            std::string currentTime = getCurrentTime();
            if(startTime >= currentTime){
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

    }

    int lengthOfRemainingEntry = remainingEntry.size();

    for(int i = 0; i < lengthOfRemainingEntry; i++){
        taskDescription += remainingEntry[i];
        if(i != lengthOfRemainingEntry-1){
            taskDescription += " ";
        }
    }
    
    if((startDate != "") && (endDate != "")) {
        startDate = dateFromUserToBoostFormat(startDate);
        endDate = dateFromUserToBoostFormat(endDate);

        if(isValidDate(startDate) && isValidDate(endDate)){
            TMTaskTime taskTime(startDate,startTime,endDate,endTime);
            TMTask task(taskDescription,taskTime,taskType);

            return task;
        } else {
            TMTaskTime taskTime;
            TMTask task("",taskTime,TaskType::Invalid);
            return task;
        }
    } else if((endDate == "") && (startDate != "")) {
        startDate = dateFromUserToBoostFormat(startDate);

        if(isValidDate(startDate)) {
            TMTaskTime taskTime(startDate,startTime,"","");
            TMTask task(taskDescription,taskTime,taskType);

            return task;
        } else {
            TMTaskTime taskTime;
            TMTask task("",taskTime,TaskType::Invalid);
            return task;
        }
    } else if((startDate == "") && (endDate != "")){
        endDate = dateFromUserToBoostFormat(endDate);

        if(isValidDate(endDate)) {
            TMTaskTime taskTime("","",endDate,endTime);
            TMTask task(taskDescription,taskTime,taskType);

            return task;
        } else {
            TMTaskTime taskTime;
            TMTask task("",taskTime,TaskType::Invalid);
            return task;
        }
    } else {
        TMTaskTime taskTime;
        TMTask task("",taskTime,TaskType::Invalid);
        return task;
    }

    //return task;
}

TMTask TMParser::parseFloatingTaskInfo() {
    TaskType taskType = TaskType::Floating;
    TMTaskTime taskTime;
    std::string taskDescription;

    int lengthOfVector = _tokenizedUserEntry.size();
    for(int i = 0; i < lengthOfVector; i++){
        taskDescription += _tokenizedUserEntry[i];
        if(i != lengthOfVector - 1){
            taskDescription += " ";
        }
    }

    TMTask task(taskDescription,taskTime,taskType);

    return task;
}

//need to check if before is the last word of the string. will go out of bound?
//if before is the last word then it cannot be a deadline
bool TMParser::isDeadlinedTask() {
    std::vector<std::string> remainingEntry = _tokenizedUserEntry;
    std::string unitString;
    std::string stringAfterBefore;
    std::vector<std::string>::iterator iter;
    for(iter = remainingEntry.begin(); iter < remainingEntry.end()-1; iter++){
        unitString = returnLowerCase(*iter);
        if(unitString == TOKEN_BEFORE){
            stringAfterBefore = *(iter+1);
            stringAfterBefore = returnLowerCase(stringAfterBefore);
            if(isDate(stringAfterBefore)||isDay(stringAfterBefore)) {
                return true;
            } else if (is12HTime(stringAfterBefore)||is24HTime(stringAfterBefore)) {
                return true;
            } else if (isWordNext(stringAfterBefore)){ 
                std::string stringAfterNext;
                stringAfterNext = *(iter+2);
                stringAfterNext = returnLowerCase(stringAfterNext);
                if(isDay(stringAfterNext)){
                    return true;
                } else {
                    continue;
                }
            } else {
                continue;
            }
        }
    }

    return false;
}

bool TMParser::isTimedTask() {
    std::vector<std::string> remainingEntry = _tokenizedUserEntry;
    bool isTokenOnFound = false;
    bool isTokenAtFound = false;
    bool isTokenFromFound = false;
    bool isTokenToFound = false;
    std::string unitString;
    std::string stringAfterToken;
    std::vector<std::string>::iterator iter;

    for(iter = remainingEntry.begin(); iter < remainingEntry.end()-1; iter++){
        unitString = returnLowerCase(*iter);

        //std::cout << "before checking for token...\n";
        
        if(unitString == TOKEN_AT){
            stringAfterToken = *(iter+1);
            //
            //std::cout << "isTimedTask: " << stringAfterToken << std::endl;
            stringAfterToken = returnLowerCase(stringAfterToken);
            //
            //std::cout << "isTimedTask: " << stringAfterToken << std::endl;
            if(is12HTime(stringAfterToken)||is24HTime(stringAfterToken)){
                isTokenAtFound = true;
                //
                //std::cout << stringAfterToken << std::endl;
            } else {
                //found at but no time
            }
            //
            //std::cout << "isTimedTask: " << stringAfterToken << std::endl;
        }

        //
        //std::cout << "after checking for token at...\n";

        if(unitString == TOKEN_ON){
            stringAfterToken = *(iter+1);
            stringAfterToken = returnLowerCase(stringAfterToken);
            if(isDate(stringAfterToken)||isDay(stringAfterToken)){
                isTokenOnFound = true;
            } else if(isWordNext(stringAfterToken)){
                if(iter+2 != remainingEntry.end()){
                    std::string stringAfterNext = *(iter+2);
                    if(isDay(stringAfterNext)){
                        isTokenOnFound = true;
                    } else {
                        //found "on next" within string but no day
                    }
                } else {
                    //at the back of the string: "... on next"
                }
            } else {
                //continue searching for on and essential info
            }
        }
        
        if(unitString == TOKEN_FROM){
        std::string stringAfterFrom = *(iter + 1);
            stringAfterFrom = returnLowerCase(stringAfterFrom);

            if(isDate(stringAfterFrom)||isDay(stringAfterFrom)) {
                //check for time after date
                isTokenFromFound = true;
            } else if(is12HTime(stringAfterFrom)||is24HTime(stringAfterFrom)){
                isTokenFromFound = true;
            } else if(isWordNext(stringAfterFrom)) {
                //check for day after next, then check for time
                if(iter + 2 != remainingEntry.end()) {
                    std::string stringAfterNext = *(iter + 2);
                    stringAfterNext = returnLowerCase(stringAfterNext);
                    if(isDay(stringAfterNext)) {
                        isTokenFromFound = true;
                    } else {
                        //found from next .. task descr. continue
                    }
                } else {
                    //found from next. treat as task description. continue
                }
            } else {
                //cannot find a startTime or startDate. from is treated as task desc
            }
        }

        if(unitString == TOKEN_TO) {
            std::string stringAfterTo = *(iter + 1);
            stringAfterTo = returnLowerCase(stringAfterTo);

            if(isDate(stringAfterTo)||isDay(stringAfterTo)) {
                //check for time after date
                isTokenToFound = true;
            } else if(is12HTime(stringAfterTo)||is24HTime(stringAfterTo)){
                isTokenToFound = true;
            } else if(isWordNext(stringAfterTo)) {
                //check for day after next, then check for time
                if(iter + 2 != remainingEntry.end()) {
                    std::string stringAfterNext = *(iter + 2);
                    stringAfterNext = returnLowerCase(stringAfterNext);
                    if(isDay(stringAfterNext)) {
                        isTokenToFound = true;
                    } else {
                        //found from next .. task descr. continue
                    }
                } else {
                    //found from next. treat as task description. continue
                }
            } else {
                //cannot find a startTime or startDate. from is treated as task desc
            }
        }
        
    }
    //
    //std::cout << "Reached the end of...\n";

    if(isTokenOnFound||isTokenAtFound||isTokenFromFound||isTokenToFound){
        return true;
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
       token== DAY_SUNDAY) {
           return true;
    } else {
        return false;
    }
}

bool TMParser::is12HTime(std::string timeToken){
    //format 2am 10am 230pm 1230am
    //check for am or pm first
    //std::cout << "is12HTime\n";
    if(isAM(timeToken)||isPM(timeToken)){
        //
        //std::cout << "isAM or isPM\n";
    } else {
        //
        //std::cout << "false\n";
        return false;
    }
    //
    //std::cout << "out \n";

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
    } else if (lengthOfTimeToken = 4) {
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
    } else if(lengthOfTimeToken = 5) {
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

bool TMParser::isWordNext(std::string token) {
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
    std::string taskPositionNo = _tokenizedUserEntry[0];
    intTaskPositionNo = std::stoi(taskPositionNo);
    vectorTaskPositionNo.push_back(intTaskPositionNo);
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

//incomplete
std::string TMParser::timeTo24HFormat(std::string time){
    std::string hhmm;
    int timeIn24HFormat;
    //
    std::cout << time << std::endl;
    //if this is true then there are only 4 possibilities check is24HTime function
    if(is12HTime(time)) {
        if(isAM(time)){
            if(time.length() == 3){
                hhmm = "0" + time.substr(0,1) + "00";
            } else if(time.length() == 4){
                timeIn24HFormat = (std::stoi(time.substr(0,2)))%12;
                if(timeIn24HFormat = 0){
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