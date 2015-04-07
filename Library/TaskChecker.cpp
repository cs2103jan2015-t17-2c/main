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
        
        if(unitString == TOKEN_BEFORE||unitString == TOKEN_BY) {
            stringAfterBefore = formatConverter->returnLowerCase(tokenizedUserEntry[index + 1]);

            if(dateChecker->isNumericDate(stringAfterBefore)||dateChecker->isDay(stringAfterBefore)||dateChecker->isOneDelimitedDate(stringAfterBefore)) {
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
        
        if(unitString == TOKEN_AT){
            if(timeChecker->is12HTime(stringAfterToken)||timeChecker->is24HTime(stringAfterToken)||timeChecker->isTimeWithoutPeriod(stringAfterToken)){
                return true;
            }
        }
        
        if(unitString == TOKEN_ON){
            if(dateChecker->isNumericDate(stringAfterToken)||dateChecker->isDay(stringAfterToken)||dateChecker->isOneDelimitedDate(stringAfterToken)){
                return true;
            }
        }
        
        if(unitString == TOKEN_FROM){
            if(dateChecker->isNumericDate(stringAfterToken)||dateChecker->isDay(stringAfterToken)||dateChecker->isOneDelimitedDate(stringAfterToken)){
                //check for time after date
                return true;
            } else if(timeChecker->is12HTime(stringAfterToken)||timeChecker->is24HTime(stringAfterToken)||timeChecker->isTimeWithoutPeriod(stringAfterToken)){
                return true;
            } else if(dateChecker->isNextDay(index + 1, tokenizedUserEntry)){
                //check for day after next, then check for time
                return true;
            }
        }
        
        if(unitString == TOKEN_TO){

            if(dateChecker->isNumericDate(stringAfterToken)||dateChecker->isDay(stringAfterToken)||dateChecker->isOneDelimitedDate(stringAfterToken)) {
                //check for time after date
                return true;
            } else if(timeChecker->is12HTime(stringAfterToken)||timeChecker->is24HTime(stringAfterToken)||timeChecker->isTimeWithoutPeriod(stringAfterToken)){
                return true;
            } else if(dateChecker->isNextDay(index + 1, tokenizedUserEntry)) {
                //check for day after next, then check for time
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

//user input must be date and/or time first followed by and date and/or time
//CHECK FOR PERIOD TOO 2-4PM AND MORE
bool TaskChecker::isMultipleTimingTask(std::vector<std::string> tokenizedUserEntry){
    FormatConverter *formatConverter = FormatConverter::getInstance();
    DateChecker *dateChecker = DateChecker::getInstance();
    TimeChecker *timeChecker = TimeChecker::getInstance();
    std::string unitString;
    std::string stringAfterAnd;
    int lengthOfTokenizedUserEntry = tokenizedUserEntry.size();
    bool oneTimingIsFound = false;
    bool moreThanOneTimingIsFound = false;

    for(int index = 0; index < lengthOfTokenizedUserEntry; index++){

        unitString = formatConverter->returnLowerCase(tokenizedUserEntry[index]);

        if(!oneTimingIsFound){
            if(dateChecker->isNumericDate(unitString)||dateChecker->isDay(unitString)||
                dateChecker->isOneDelimitedDate(unitString)||timeChecker->is12HTime(unitString)||
                timeChecker->is24HTime(unitString)||timeChecker->isTimeWithoutPeriod(unitString)||
                dateChecker->isTomorrow(unitString)||dateChecker->isToday(unitString)){
                    oneTimingIsFound = true;
            } else if (dateChecker->isNextDay(index, tokenizedUserEntry)){
                oneTimingIsFound = true;   
            }
        } else {
            if(unitString == "and"){
                if(index + 1 == lengthOfTokenizedUserEntry){
                    break;
                }

                stringAfterAnd = formatConverter->returnLowerCase(tokenizedUserEntry[index + 1]);
                if(dateChecker->isNumericDate(stringAfterAnd)||dateChecker->isDay(stringAfterAnd)||
                    dateChecker->isOneDelimitedDate(stringAfterAnd)||timeChecker->is12HTime(stringAfterAnd)||
                    timeChecker->is24HTime(stringAfterAnd)||timeChecker->isTimeWithoutPeriod(stringAfterAnd)||
                    dateChecker->isTomorrow(stringAfterAnd)||dateChecker->isToday(stringAfterAnd)){
                    moreThanOneTimingIsFound = true;

                } else if (dateChecker->isNextDay(index + 1, tokenizedUserEntry)){
                    moreThanOneTimingIsFound = true;
                }
            }
        }
    }

    if(moreThanOneTimingIsFound){
        return true;
    } else if(!moreThanOneTimingIsFound && oneTimingIsFound){
        //
        //addErrorMessage("Only 1 Timing found.\n");
        return false;
    } else {
        return false;
    }
}