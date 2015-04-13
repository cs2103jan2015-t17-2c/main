//@author A0111712Y

#include "TaskChecker.h"

TaskChecker* TaskChecker::theOne;

TaskChecker::TaskChecker() {

}

TaskChecker* TaskChecker::getInstance() {
    if (theOne == NULL) {
		theOne = new TaskChecker();
	}
	return theOne;
}

bool TaskChecker::isDeadlinedTask(std::vector<std::string> tokenizedUserEntry) {
    FormatConverter *formatConverter = FormatConverter::getInstance();
    DateChecker *dateChecker = DateChecker::getInstance();
    TimeChecker *timeChecker = TimeChecker::getInstance();
    std::string unitString;
    std::string stringAfterBefore;
    int lengthOfTokenizedUserEntry = tokenizedUserEntry.size();

    for(int index = 0; index < lengthOfTokenizedUserEntry; index++){
        if(index + 1 == lengthOfTokenizedUserEntry){
            break;
        }
        
        unitString = formatConverter->returnLowerCase(tokenizedUserEntry[index]);
        
        if (isWordBefore(unitString)||isWordBy(unitString)) {
            stringAfterBefore = formatConverter->returnLowerCase(tokenizedUserEntry[index + 1]);

            if (dateChecker->isNumericDate(stringAfterBefore)||
                dateChecker->isDay(stringAfterBefore)||
                dateChecker->isDelimitedDate(stringAfterBefore)||
                dateChecker->isSpacedDate(index + 1, tokenizedUserEntry)) {
                return true;
            } else if (timeChecker->is12HTime(stringAfterBefore)||timeChecker->is24HTime(stringAfterBefore)||timeChecker->isTimeWithoutPeriod(stringAfterBefore)) {
                return true;
            } else if (dateChecker->isNextDay(index + 1, tokenizedUserEntry)){ 
                return true;
            } else if (dateChecker->isTomorrow(stringAfterBefore)) {
                return true;
            } else if (dateChecker->isToday(stringAfterBefore)) {
                return true;
            }
        }
    }

    return false;
}

bool TaskChecker::isTimedTask(std::vector<std::string> tokenizedUserEntry) {
    FormatConverter *formatConverter = FormatConverter::getInstance();
    DateChecker *dateChecker = DateChecker::getInstance();
    TimeChecker *timeChecker = TimeChecker::getInstance();
    std::string unitString;
    std::string stringAfterToken;
    int lengthOfTokenizedUserEntry = tokenizedUserEntry.size();
    
    for(int index = 0; index < lengthOfTokenizedUserEntry; index++) {
        
        unitString = formatConverter->returnLowerCase(tokenizedUserEntry[index]);
        
        if(isWordAt(unitString)){
            if(index + 1 == lengthOfTokenizedUserEntry){
                break;
            }

            stringAfterToken = formatConverter->returnLowerCase(tokenizedUserEntry[index + 1]);

            if (timeChecker->is12HTime(stringAfterToken)||
                timeChecker->is24HTime(stringAfterToken)||
                timeChecker->isTimeWithoutPeriod(stringAfterToken)){
                    return true;
            }
        } else if (isWordOn(unitString)) {
            if(index + 1 == lengthOfTokenizedUserEntry){
                break;
            }

            stringAfterToken = formatConverter->returnLowerCase(tokenizedUserEntry[index + 1]);

            if (dateChecker->isNumericDate(stringAfterToken)||
                dateChecker->isDay(stringAfterToken)||
                dateChecker->isDelimitedDate(stringAfterToken)||
                dateChecker->isSpacedDate(index + 1,tokenizedUserEntry)){
                    return true;
            }
        } else if (isWordFrom(unitString)||isWordTo(unitString)) {
            if(index + 1 == lengthOfTokenizedUserEntry){
                break;
            }
            
            stringAfterToken = formatConverter->returnLowerCase(tokenizedUserEntry[index + 1]);

            if(dateChecker->isNumericDate(stringAfterToken)||
                dateChecker->isDay(stringAfterToken)||
                dateChecker->isDelimitedDate(stringAfterToken)||
                dateChecker->isSpacedDate(index + 1, tokenizedUserEntry)) {
                //check for time after date
                return true;
            } else if(timeChecker->is12HTime(stringAfterToken)||
                timeChecker->is24HTime(stringAfterToken)||
                timeChecker->isTimeWithoutPeriod(stringAfterToken)) {

                return true;
            } else if(dateChecker->isNextDay(index + 1, tokenizedUserEntry)) {
                //check for day after next, then check for time
                return true;
            } else if(dateChecker->isTomorrow(stringAfterToken)) {
                return true;
            } else if(dateChecker->isToday(stringAfterToken)) {
                return true;
            }

        } else if (dateChecker->isNextDay(index, tokenizedUserEntry)) {
            return true;
        } else if (dateChecker->isToday(unitString)) {
            return true;
        } else if (dateChecker->isTomorrow(unitString)) {
            return true;
        }
    }

    return false;
}

//Preconditions: token must be in lowercase
bool TaskChecker::isWordBefore(std::string token) {
    if (token == TOKEN_BEFORE) {
        return true;
    } else if (token == TOKEN_BY) {
        return true;
    } else if (token == TOKEN_SHORTCUT_BEFORE) {
        return true;
    } else if (token == TOKEN_B4) {
        return true;
    } else {
        return false;
    }
}

//Preconditions: token must be in lowercase
bool TaskChecker::isWordBy(std::string token) {
    if (token == TOKEN_BY) {
        return true;
    } else {
        return false;
    }
}

bool TaskChecker::isWordAt (std::string token) {
    if (token == TOKEN_AT) {
        return true;
    } else if (token == TOKEN_SHORTCUT_AT) {
        return true;
    } else {
        return false;
    }
}

bool TaskChecker::isWordOn (std::string token) {
    if (token == TOKEN_ON) {
        return true;
    } else {
        return false;
    }
}

bool TaskChecker::isWordFrom (std::string token) {
    if (token == TOKEN_FROM) {
        return true;
    } else if (token == TOKEN_SHORTCUT_FROM) {
        return true;
    } else {
        return false;
    }
}

bool TaskChecker::isWordTo (std::string token) {
    if (token == TOKEN_TO) {
        return true;
    } else if (token == TOKEN_SHORTCUT_TO) {
        return true;
    } else if (token == TOKEN_DASH) {
        return true;
    } else {
        return false;
    }
}

bool TaskChecker::isDateOrTime(std::string nextWord, int index, std::vector<std::string> tokenizedUserEntry) {
    DateChecker *dateChecker = DateChecker::getInstance();
    TimeChecker *timeChecker = TimeChecker::getInstance();

    if (dateChecker->isDateOrDayOrNextDayOrTomorrowOrToday(nextWord, index, tokenizedUserEntry)) {
        return true;
    } else if (timeChecker->isTime(nextWord)) {
        return true;
    } else {
        return false;
    }
}