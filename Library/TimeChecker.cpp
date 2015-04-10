#include "TimeChecker.h"

TimeChecker* TimeChecker::theOne;

TimeChecker::TimeChecker() {
}

TimeChecker* TimeChecker::getInstance() {
    if (theOne == NULL) {
		theOne = new TimeChecker();
	}
	return theOne;
}

//valid 12HTime format 2am 10am 230pm 1230am
//check for am or pm first
bool TimeChecker::is12HTime(std::string timeToken){
    if (!isAM(timeToken) && !isPM(timeToken)) {
        return false;
    }

    int lengthOfTimeToken = timeToken.size();

    if (lengthOfTimeToken == TIME_WITH_PERIOD_LENGTH_3) {
        int hour = std::stoi(timeToken.substr(0,1));

        if (hour < ONE_O_CLOCK || hour > NINE_O_CLOCK) {
            return false;
        }
    } else if (lengthOfTimeToken == TIME_WITH_PERIOD_LENGTH_4) {
        int hour = std::stoi(timeToken.substr(0,2));

        if(hour > TWELVE_O_CLOCK || hour < TEN_O_CLOCK) {
            return false;
        }
    } else if (lengthOfTimeToken == TIME_WITH_PERIOD_LENGTH_5) {
        int hour = std::stoi(timeToken.substr(0,1));

        if(hour < ONE_O_CLOCK || hour > NINE_O_CLOCK) {
            return false;
        }
        
        int minute = std::stoi(timeToken.substr(1,2));

        if(minute > FIFTY_NINE_MINUTES || minute < ZERO_MINUTE) {
            return false;
        }
    } else if (lengthOfTimeToken == TIME_WITH_PERIOD_LENGTH_6) {
        int hour = std::stoi(timeToken.substr(0,2));

        if(hour > TWELVE_O_CLOCK || hour < TEN_O_CLOCK) {
            return false;
        }
        
        int minute = std::stoi(timeToken.substr(2,2));

        if(minute > FIFTY_NINE_MINUTES || minute < ZERO_MINUTE) {
            return false;
        }
    } else {
        return false;
    }
}

bool TimeChecker::isAM(std::string token){
    if (token.length() < TIME_WITH_PERIOD_LENGTH_3 || token.length() > TIME_WITH_PERIOD_LENGTH_6) {
        return false;
    }

    std::string partOfTokenBeforeLastTwoCharacters = token.substr(0, token.length()-2);

    if(!isPositiveInteger(partOfTokenBeforeLastTwoCharacters)) {
        return false;
    }

    std::string lastTwoCharacters = token.substr(token.length()-2,2);
    FormatConverter *formatConverter = FormatConverter::getInstance();
    lastTwoCharacters = formatConverter->returnLowerCase(lastTwoCharacters);

    if(lastTwoCharacters == TIME_PERIOD_AM){
        return true;
    } else {
        return false;
    }
}

bool TimeChecker::isPM(std::string token){
    if (token.length() < TIME_WITH_PERIOD_LENGTH_3 || token.length() > TIME_WITH_PERIOD_LENGTH_6) {
        return false;
    }

    std::string partOfTokenBeforeLastTwoCharacters = token.substr(0, token.length() - 2);

    if(!isPositiveInteger(partOfTokenBeforeLastTwoCharacters)) {
        return false;
    }

    std::string lastTwoCharacters = token.substr(token.length()-2,2);
    FormatConverter *formatConverter = FormatConverter::getInstance();
    lastTwoCharacters = formatConverter->returnLowerCase(lastTwoCharacters);

    if(lastTwoCharacters == TIME_PERIOD_PM){
        return true;
    } else {
        return false;
    }
}

bool TimeChecker::isTimeWithoutPeriod(std::string token) {
    int length = token.length();
    if(length < TIME_WITHOUT_PERIOD_LENGTH_1 || length > TIME_WITHOUT_PERIOD_LENGTH_2) {
        return false;
    }

    if(!isPositiveInteger(token)) {
        return false;
    }

    int intToken = std::stoi(token);

    if(intToken >= ONE_O_CLOCK && intToken <= TWELVE_O_CLOCK) {
        return true;
    } else {
        return false;
    }
}

//valid time format: 1030 / 10:30 / 0030/ 00:30 / 0000/ 00:00
bool TimeChecker::is24HTime(std::string timeToken) {
    
    unsigned int lengthOfTimeToken = timeToken.size();
    
    if (lengthOfTimeToken == TIME_HHMM_LENGTH) {
        if (!isInteger(timeToken)) {
            return false;
        }
        
        int hour = std::stoi(timeToken.substr(0,2));
        if (hour < ZERO_HOURS || hour > TWENTY_THREE_HOURS) {
            return false;
        }
        
        int minute = std::stoi(timeToken.substr(2,2));
        if(minute < ZERO_MINUTE || minute > FIFTY_NINE_MINUTES) {
            return false;
        }
    } else if(lengthOfTimeToken == TIME_HH_COLON_MM_LENGTH) {
        if (timeToken[2] != CHAR_COLON) {
            return false;
        }
        
        if (!isInteger(timeToken.substr(0,2)) || !isInteger(timeToken.substr(3,2))) {
            return false;
        }
        
        int hour = std::stoi(timeToken.substr(0,2));
        if (hour < ZERO_HOURS || hour > TWENTY_THREE_HOURS) {
            return false;
        }

        int minute = std::stoi(timeToken.substr(3,2));
        if (minute < ZERO_MINUTE || minute > FIFTY_NINE_MINUTES) {
            return false;
        }
    } else {
        return false;
    }

    return true;
}

bool TimeChecker::isPositiveInteger(std::string token) {
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

bool TimeChecker::isInteger(std::string token) {
    for(std::string::iterator it = token.begin(); it < token.end(); it++) {
        if(!isdigit(*it)) {
            return false;
        }
    }

    return true;
}