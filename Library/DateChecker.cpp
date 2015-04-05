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

bool DateChecker::isNumericDate(std::string token) {
    //DDMMYYYY DDMMYYYY
    int lengthOfToken = token.size();
    if(lengthOfToken == 8||lengthOfToken == 6) {
        if(!isPositiveInteger(token)){
            return false;
        }
        FormatConverter *formatConverter = FormatConverter::getInstance();
        if(!isValidDate(formatConverter->dateFromNumericToBoostFormat(token))){
            return false;
        }

        return true;
    }

    std::string day = token.substr(0,2);
    std::string month = token.substr(2,2);
    std::string year = token.substr(4);

    if(year.length() == 2){
        year = "20" + year;
    }

    std::string date = day + " " + month + " " + year;

    if(isValidDate(date)) {
        return true;
    } else {
        return false;
    }
}

bool DateChecker::isSlashedNumericDate(std::string token) {
    return isOneNumericDateForm(token,'/');
}

bool DateChecker::isDottedNumericDate(std::string token) {
    return isOneNumericDateForm(token,'.');
}

bool DateChecker::isOneNumericDateForm(std::string token, char key) {
    std::string day = "";
    std::string month = "";
    std::string year = "";

    int positionOfNextKey = token.find_first_of(key);

    if(positionOfNextKey == std::string::npos||token[0] == key||token[token.length()-1] == key){
        return false;
    }

    day = token.substr(0,positionOfNextKey);
    int lengthOfDay = day.length();
    if(!isPositiveInteger(day)||!(lengthOfDay == 1||lengthOfDay == 2)){
        return false;
    }

    token = token.substr(positionOfNextKey + 1); 
    positionOfNextKey = token.find_first_of(key);
    
    if(positionOfNextKey != std::string::npos){
        month = token.substr(0,positionOfNextKey);
        if(!isNumericMonth(month)){
            return false;
        }
        year = token.substr(positionOfNextKey + 1);
        int lengthOfYear = year.length();
        if(!isPositiveInteger(year)||!(lengthOfYear == 2||lengthOfYear == 4)){
            return false;
        }
        if(lengthOfYear == 2){
            year = "20" + year;
        }
    } else {
        month = token;
        if(!isNumericMonth(month)){
            return false;
        }
        FormatConverter *formatConverter = FormatConverter::getInstance();
        std::string dateToday = formatConverter->dateFromBoostToDDMMYYYY(currentDate);
        year = dateToday.substr(4,4);
    }

    std::string date = day + "-" + month + "-" + year;
    
    if(!isValidDate(date)){
        return false;
    }

    return true;
}

bool DateChecker::isDDMonDate(std::string token) {
    std::string dd = "";
    std::string month = "";
    std::string yyyy = "";

    int positionOfNextDash = token.find_first_of("-");

    if(positionOfNextDash == std::string::npos){
        return false;
    }

    dd = token.substr(0,positionOfNextDash);//dd = dd
    int lengthOfdd = dd.length();
    //check whether length is 1 or 2 whether from 1-31?
    if(!isPositiveInteger(dd)||!(lengthOfdd == 1||lengthOfdd == 2)){
        return false;
    }

    token = token.substr(positionOfNextDash + 1); //stringDate = mm-yyyy 
    positionOfNextDash = token.find_first_of("-");
    
    if(positionOfNextDash != std::string::npos){
        month = token.substr(0,positionOfNextDash);
        if(!isMonth(month)){
            return false;
        }
        yyyy = token.substr(positionOfNextDash + 1);
        int lengthOfyyyy = yyyy.length();
        if(!isPositiveInteger(yyyy)||!(lengthOfyyyy == 2||lengthOfyyyy == 4)){
            return false;
        }
        if(lengthOfyyyy == 2){
            yyyy = "20" + yyyy;
        } else {
        }
    } else {
        month = token;
        FormatConverter *formatConverter = FormatConverter::getInstance();
        std::string dateToday = formatConverter->dateFromBoostToDDMMYYYY(currentDate);
        yyyy = dateToday.substr(4,4);
        if(!isMonth(month)){
            return false;
        }
    }

    std::string date = dd + "-" + month + "-" + yyyy;
    
    if(!isValidDate(date)){
        return false;
    }

    return true;
}

bool DateChecker::isMonth(std::string token) {
    FormatConverter *formatConverter = FormatConverter::getInstance();
    token = formatConverter->returnLowerCase(token);
    if(token == "jan"||
       token == "january"||
       token == "feb"||
       token == "february"||
       token == "mar"||
       token == "march"||
       token == "apr"||
       token == "april"||
       token == "may"||
       token == "jun"||
       token == "jul"||
       token == "july"||
       token == "aug"||
       token == "august"||
       token == "sep"||
       token == "september"||
       token == "oct"||
       token == "october"||
       token == "nov"||
       token == "november"||
       token == "dec"||
       token == "december") {
           return true;
    } else {
        return false;
    }
}

bool DateChecker::isNumericMonth(std::string token) {
    if(token == "1"||token == "01"||
       token == "2"||token == "02"||
       token == "3"||token == "03"||
       token == "4"||token == "04"||
       token == "5"||token == "05"||
       token == "6"||token == "06"||
       token == "7"||token == "07"||
       token == "8"||token == "08"||
       token == "9"||token == "09"||
       token == "10"||
       token == "11"||
       token == "12") {
           return true;
    } else {
        return false;
    }
}

bool DateChecker::isDay(std::string token) {
    FormatConverter *formatConverter = FormatConverter::getInstance();
    token = formatConverter->returnLowerCase(token);
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

bool DateChecker::isNextDay(int index, std::vector<std::string> tokenizedUserEntry) {
    FormatConverter *formatConverter = FormatConverter::getInstance();
    std::string firstWord = formatConverter->returnLowerCase(tokenizedUserEntry[index]);

    if(firstWord == TOKEN_NEXT){
        int lengthOfTokenizedUserEntry = tokenizedUserEntry.size();
        if(index + 1 != lengthOfTokenizedUserEntry) {
            std::string secondWord = formatConverter->returnLowerCase(tokenizedUserEntry[index + 1]);
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