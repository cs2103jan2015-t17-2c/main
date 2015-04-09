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
        
        if(unitString == TOKEN_BEFORE||unitString == TOKEN_BY||unitString == TOKEN_SHORTCUT_BEFORE) {
            stringAfterBefore = formatConverter->returnLowerCase(tokenizedUserEntry[index + 1]);

            if(dateChecker->isNumericDate(stringAfterBefore)||
                dateChecker->isDay(stringAfterBefore)||
                dateChecker->isOneDelimitedDate(stringAfterBefore)||
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
    
    for(int index = 0; index < lengthOfTokenizedUserEntry; index++){
        //check if current iter is at the last string
        if(index + 1 == lengthOfTokenizedUserEntry){
            break;
        }
        
        unitString = formatConverter->returnLowerCase(tokenizedUserEntry[index]);
        stringAfterToken = formatConverter->returnLowerCase(tokenizedUserEntry[index + 1]);
        
        if(unitString == TOKEN_AT||unitString == TOKEN_SHORTCUT_AT){
            if(timeChecker->is12HTime(stringAfterToken)||timeChecker->is24HTime(stringAfterToken)
                ||timeChecker->isTimeWithoutPeriod(stringAfterToken)){
                    return true;
            }
        }
        
        if(unitString == TOKEN_ON){
            if(dateChecker->isNumericDate(stringAfterToken)||dateChecker->isDay(stringAfterToken)
                ||dateChecker->isOneDelimitedDate(stringAfterToken)
                ||dateChecker->isSpacedDate(index + 1,tokenizedUserEntry)){
                    return true;
            }
        }
        
        if(unitString == TOKEN_FROM||unitString == TOKEN_SHORTCUT_FROM){
            if(dateChecker->isNumericDate(stringAfterToken)||
                dateChecker->isDay(stringAfterToken)||
                dateChecker->isOneDelimitedDate(stringAfterToken)||
                dateChecker->isSpacedDate(index + 1, tokenizedUserEntry)){
                //check for time after date
                return true;
            } else if(timeChecker->is12HTime(stringAfterToken)||timeChecker->is24HTime(stringAfterToken)||timeChecker->isTimeWithoutPeriod(stringAfterToken)){
                return true;
            } else if(dateChecker->isNextDay(index + 1, tokenizedUserEntry)){
                //check for day after next, then check for time
                return true;
            } else if(dateChecker->isTomorrow(stringAfterToken)) {
                return true;
            } else if(dateChecker->isToday(stringAfterToken)) {
                return true;
            }
        }
        
        if(unitString == TOKEN_TO||unitString == TOKEN_SHORTCUT_TO){

            if(dateChecker->isNumericDate(stringAfterToken)||
                dateChecker->isDay(stringAfterToken)||
                dateChecker->isOneDelimitedDate(stringAfterToken)||
                dateChecker->isSpacedDate(index + 1, tokenizedUserEntry)) {
                //check for time after date
                return true;
            } else if(timeChecker->is12HTime(stringAfterToken)||timeChecker->is24HTime(stringAfterToken)||timeChecker->isTimeWithoutPeriod(stringAfterToken)){
                return true;
            } else if(dateChecker->isNextDay(index + 1, tokenizedUserEntry)) {
                //check for day after next, then check for time
                return true;
            } else if(dateChecker->isTomorrow(stringAfterToken)) {
                return true;
            } else if(dateChecker->isToday(stringAfterToken)) {
                return true;
            }
        }
        
        if(dateChecker->isNextDay(index, tokenizedUserEntry)){
            return true;
        }

        if(dateChecker->isToday(unitString)) {
            return true;
        }

        if(dateChecker->isTomorrow(unitString)) {
            return true;
        }
    }

    return false;
}