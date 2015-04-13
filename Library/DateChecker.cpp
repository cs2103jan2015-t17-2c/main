//@author A0111712Y

#include "DateChecker.h"

DateChecker* DateChecker::theOne;

DateChecker::DateChecker() {
    
}

DateChecker* DateChecker::getInstance() {
    if (theOne == NULL) {
		theOne = new DateChecker();
	}
	return theOne;
}

//Precondition: DDMMYY DDMMYYYY
//Postcondition: returns true if valid dates else false
bool DateChecker::isNumericDate(std::string token) {
    int lengthOfToken = token.size();

    if(lengthOfToken == DATE_DDMMYYYY_LENGTH||lengthOfToken == DATE_DDMMYY_LENGTH) {
        
        if(!isPositiveInteger(token)){
            return false;
        }

        FormatConverter *formatConverter = FormatConverter::getInstance();

        if(!isValidDate(formatConverter->dateFromNumericToBoostFormat(token))){
            return false;
        }
        
        std::string day = token.substr(0,2);
        std::string month = token.substr(2,2);
        std::string year = token.substr(4);

        if(year.length() == YEAR_YY_LENGTH){
            FormatConverter *formatConverter = FormatConverter::getInstance();
            std::string firstTwoDigitsOfCurrentYear = currentDateInString().substr(4,2);
            year = firstTwoDigitsOfCurrentYear + year;
        }

        std::string date = day + " " + month + " " + year;

        if(isValidDate(date)) {
            return true;
        } else {
            return false;
        }
    } else {
        return false;
    }

}

//Preconditions: dd(delimiter)month(delimiter)(optional:year)
//Postconditions: returns true if valid date
bool DateChecker::isDelimitedDate(std::string token){
    if(isOneDelimitedDate(token, DELIMITER_DASH)) {
        return true;
    } else if(isOneDelimitedDate(token, DELIMITER_FULLSTOP)) {
        return true;
    } else if(isOneDelimitedDate(token, DELIMITER_SLASH)) {
        return true;
    } else {
        return false;
    }
}

bool DateChecker::isOneDelimitedDate(std::string token, char key) {
    FormatConverter *formatConverter = FormatConverter::getInstance();
    std::string day;
    std::string month;
    std::string year;
    int tokenLength = token.length();

    for(int i = 0; i < tokenLength; i++) {
        if(!(isalnum(token[i])||token[i] == key)) {
            return false;
        }
    }
    int positionOfNextKey = token.find_first_of(key);

    if(positionOfNextKey == std::string::npos||token[0] == key||token[tokenLength-1] == key){
        return false;
    }

    day = token.substr(0,positionOfNextKey);
    int lengthOfDay = day.length();

    if(!isPositiveInteger(day)||!(lengthOfDay == DAY_D_LENGTH||lengthOfDay == DAY_DD_LENGTH)){
        return false;
    }

    token = token.substr(positionOfNextKey + 1); 
    positionOfNextKey = token.find_first_of(key);
    
    if(positionOfNextKey != std::string::npos){
        month = token.substr(0,positionOfNextKey);
        if(!isNumericMonth(month) && !isMonth(month)){
            return false;
        }
        year = token.substr(positionOfNextKey + 1);
        int lengthOfYear = year.length();

        if(!isInteger(year)||!(lengthOfYear == YEAR_YY_LENGTH||lengthOfYear == YEAR_YYYY_LENGTH)){
            return false;
        }

        if(lengthOfYear == YEAR_YY_LENGTH){
            std::string firstTwoDigitsOfCurrentYear = currentDateInString().substr(4,2);
            year = firstTwoDigitsOfCurrentYear + year;
        }
    } else {
        month = token;
        if(!isNumericMonth(month) && !isMonth(month)) {
            return false;
        }

        if(isMonth(month)) {
            month = formatConverter->monthFromWrittenToNumeric(month);
        }

        //only certain years have 29 Feb
        if((day + month) == DATE_NUMERIC_DDMM_29_FEB) {
            return true;
        }
        year = currentDateInString().substr(4,4);
    }

    std::string date = day + "-" + month + "-" + year;
    
    if(!isValidDate(date)){
        return false;
    }
    return true;    
}

//preconditions check isDelimitedDate first
char DateChecker::returnDelimiter(std::string token) {
     if(isOneDelimitedDate(token, DELIMITER_DASH)) {
        return DELIMITER_DASH;
    } else if(isOneDelimitedDate(token, DELIMITER_FULLSTOP)) {
        return DELIMITER_FULLSTOP;
    } else {
        return DELIMITER_SLASH;
    }
}

bool DateChecker::isSpacedDate(int index, std::vector<std::string> tokenizedUserEntry) {
    FormatConverter *formatConverter = FormatConverter::getInstance();
    std::string firstToken = tokenizedUserEntry[index];
    int vectorSize = tokenizedUserEntry.size();

    if(!isPositiveInteger(firstToken)) {
        return false;
    }

    if(index + 1 == vectorSize) {
        return false;
    }

    std::string secondToken = formatConverter->toLowerCase(tokenizedUserEntry[index + 1]);
    if(!isMonth(secondToken)) {
        return false;
    }

    std::string possibleDate = firstToken + DELIMITER_DASH + secondToken;
    return isOneDelimitedDate(possibleDate,DELIMITER_DASH); 
}

//Preconditions: dd mm yyyy
bool DateChecker::isValidDate(std::string date) {
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

//Precondition: valid date in the form of DDMMYYYY
//Postcondition: returns true if date is valid
bool DateChecker::isUnoccurredDate(std::string date) {
    FormatConverter *formatConverter = FormatConverter::getInstance();
    boost::gregorian::date boostDate;
    date = formatConverter->dateFromNumericToBoostFormat(date);
    if(isValidDate(date)) {
        boostDate = boost::gregorian::from_uk_string(date);
    } else {
        return false;
    }

    if(boostDate > currentDate()) {
        return true;
    } else {
        return false;
    }
}

//Postcondition: return true is the year in which the program is run is a leap year
bool DateChecker::isCurrentlyLeapYear() {
    if(boost::gregorian::gregorian_calendar::is_leap_year(currentDate().year())) {
        return true;
    } else {
        return false;
    }
}

//Precondition: year in yyyy
//Postcondition: returns true if year is divisible by 4 (i.e. leap year)
bool DateChecker::isLeapYear(std::string year) {
    int intYear = std::stoi(year);
    return intYear%4 == 0;
}

//Postcondition: returns true if token matches today or its shortcut
bool DateChecker::isToday(std::string token) {
    FormatConverter *formatConverter = FormatConverter::getInstance();
    token = formatConverter->toLowerCase(token);

    if(token == DAY_TODAY||token == DAY_SHORTCUT_TODAY) {
        return true;
    } else {
        return false;
    }
}

//Postcondition: returns true if token matches tomorrow or its shortcut
bool DateChecker::isTomorrow(std::string token) {
    FormatConverter *formatConverter = FormatConverter::getInstance();
    token = formatConverter->toLowerCase(token);

    if(token == DAY_TOMORROW||token == DAY_SHORTCUT_TOMORROW) {
        return true;
    } else {
        return false;
    }
}

//Postcondition: returns true if token matches one of the weekdays or its shortcut
bool DateChecker::isDay(std::string token) {
    FormatConverter *formatConverter = FormatConverter::getInstance();
    token = formatConverter->toLowerCase(token);

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

//Postcondition: returns true if token at index == "next" and the trailing token is a weekday
bool DateChecker::isNextDay(int index, std::vector<std::string> tokenizedUserEntry) {
    FormatConverter *formatConverter = FormatConverter::getInstance();
    std::string firstWord = formatConverter->toLowerCase(tokenizedUserEntry[index]);

    if(firstWord == TOKEN_NEXT||firstWord == TOKEN_SHORTCUT_NEXT) {
        int lengthOfTokenizedUserEntry = tokenizedUserEntry.size();

        if(index + 1 != lengthOfTokenizedUserEntry) {
            std::string secondWord = formatConverter->toLowerCase(tokenizedUserEntry[index + 1]);

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

//Postcondition: returns true if token is a month or its shortcut
bool DateChecker::isMonth(std::string token) {
    FormatConverter *formatConverter = FormatConverter::getInstance();
    token = formatConverter->toLowerCase(token);

    if(token == MONTH_JAN||
       token == MONTH_JANUARY||
       token == MONTH_FEB||
       token == MONTH_FEBRUARY||
       token == MONTH_MAR||
       token == MONTH_MARCH||
       token == MONTH_APR||
       token == MONTH_APRIL||
       token == MONTH_MAY||
       token == MONTH_JUN||
       token == MONTH_JUNE||
       token == MONTH_JUL||
       token == MONTH_JULY||
       token == MONTH_AUG||
       token == MONTH_AUGUST||
       token == MONTH_SEP||
       token == MONTH_SEPTEMBER||
       token == MONTH_OCT||
       token == MONTH_OCTOBER||
       token == MONTH_NOV||
       token == MONTH_NOVEMBER||
       token == MONTH_DEC||
       token == MONTH_DECEMBER) {
           return true;
    } else {
        return false;
    }
}

//Postcondition: returns true if token is a numeric month
bool DateChecker::isNumericMonth(std::string token) {
    if(token == MONTH_NUMERIC_JAN_M||token == MONTH_NUMERIC_JAN_MM||
       token == MONTH_NUMERIC_FEB_M||token == MONTH_NUMERIC_FEB_MM||
       token == MONTH_NUMERIC_MAR_M||token == MONTH_NUMERIC_MAR_MM||
       token == MONTH_NUMERIC_APR_M||token == MONTH_NUMERIC_APR_MM||
       token == MONTH_NUMERIC_MAY_M||token == MONTH_NUMERIC_MAY_MM||
       token == MONTH_NUMERIC_JUN_M||token == MONTH_NUMERIC_JUN_MM||
       token == MONTH_NUMERIC_JUL_M||token == MONTH_NUMERIC_JUL_MM||
       token == MONTH_NUMERIC_AUG_M||token == MONTH_NUMERIC_AUG_MM||
       token == MONTH_NUMERIC_SEP_M||token == MONTH_NUMERIC_SEP_MM||
       token == MONTH_NUMERIC_OCT_MM||
       token == MONTH_NUMERIC_NOV_MM||
       token == MONTH_NUMERIC_DEC_MM) {
           return true;
    } else {
        return false;
    }
}

//Postcondition: returns true if token is a string of integers that has a value > 0
bool DateChecker::isPositiveInteger(std::string token) {
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

//Postcondition: returns true if token is a string of integers that has a nonnegative value
bool DateChecker::isInteger(std::string token) {
    for(std::string::iterator it = token.begin(); it < token.end(); it++) {
        if(!isdigit(*it)) {
            return false;
        }
    }
    int integer = std::stoi(token);

    if(integer >= 0) {
        return true;
    } else {
        return false;
    }
}

bool DateChecker::isDateOrDayOrNextDayOrTomorrowOrToday(std::string nextWord, int index, std::vector<std::string> tokenizedUserEntry) {
    if (isDateOrDayOrNextDayOrTomorrow(nextWord, index, tokenizedUserEntry)||
        isToday(nextWord)) {
            return true;
    } else {
        return false;
    }
}

bool DateChecker::isDateOrDayOrNextDayOrTomorrow(std::string nextWord, int index, std::vector<std::string> tokenizedUserEntry) {
    if (isDateOrDay(nextWord, index, tokenizedUserEntry)||
        isNextDay(index + 1, tokenizedUserEntry)||
        isTomorrow(nextWord)) {
            return true;
    } else {
        return false;
    }
}

bool DateChecker::isDateOrDay(std::string nextWord, int index, std::vector<std::string> tokenizedUserEntry) {
    if (isNumericDate(nextWord)||
        isDay(nextWord)||
        isDelimitedDate(nextWord)||
        isSpacedDate(index + 1, tokenizedUserEntry)) {
            return true;
    } else {
        return false;
    }
}