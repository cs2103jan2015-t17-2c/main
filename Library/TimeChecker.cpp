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

bool TimeChecker::is12HTime(std::string timeToken){
    //format 2am 10am 230pm 1230am
    //check for am or pm first

    if(isAM(timeToken)||isPM(timeToken)){

    } else {

        return false;
    }

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

bool TimeChecker::isAM(std::string token){
    std::string lastTwoCharacters = token.substr(token.length()-2,2);
    FormatConverter *formatConverter = FormatConverter::getInstance();
    lastTwoCharacters = formatConverter->returnLowerCase(lastTwoCharacters);
    if(lastTwoCharacters == "am"){
        return true;
    } else {
        return false;
    }
}

bool TimeChecker::isPM(std::string token){
    std::string lastTwoCharacters = token.substr(token.length()-2,2);
    FormatConverter *formatConverter = FormatConverter::getInstance();
    lastTwoCharacters = formatConverter->returnLowerCase(lastTwoCharacters);
    if(lastTwoCharacters == "pm"){
        return true;
    } else {
        return false;
    }
}

bool TimeChecker::is24HTime(std::string timeToken) {
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
    } else if (lengthOfTimeToken == 4) {
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
    } else if(lengthOfTimeToken == 5) {
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

bool TimeChecker::isInteger(std::string token) {
    for(std::string::iterator it = token.begin(); it < token.end(); it++) {
        if(!isdigit(*it)) {
            return false;
        }
    }
    return true;
}