#include"TMTimeParser.h"

TMTimeParser::TMTimeParser() {
}

//extract information from user's entry if not floating task
std::string TMTimeParser::extractStartTime(std::string remainingEntry) {
    std::string startTime;
    //search for keyword at/from
    std::size_t atToken = remainingEntry.find("at");

    //followed by numerical token from string
    if(atToken != std::string::npos) {
        std::string remainingEntryAfterAt = remainingEntry.substr(atToken+2);
        int endOfNextToken = remainingEntryAfterAt.find_first_of(" ");
        std::string nextToken = remainingEntryAfterAt.substr(0,endOfNextToken);
        int lengthOfNextToken = nextToken.length();

        int index;
        bool isNumericToken = true;
        for(index = 0; index < lengthOfNextToken; index++) {
            if(nextToken[index] == '-'){
                break;
            } else if (!isdigit(nextToken[index])) {
                isNumericToken = false;
            } else {
                continue;
            }
        }

        if(isNumericToken) {
            startTime = nextToken.substr(0,index); //takes care of the case where there's both
        } else {
            startTime = "";                             //start and end time as well as start time only
        }
    } else {                                    //a preset time time at an ungodly hour e.g. 4am
        startTime = "";
    }

    return startTime;
}

std::string TMTimeParser::extractEndTime(std::string remainingEntry) {
    std::string endTime;
    //search for '-'/ 'to' check if there is a time before and after it
    std::size_t dashToken = remainingEntry.find("-");
    if(dashToken != std::string::npos) {
        int startOfNextToken = dashToken + 1;
        int endOfNextToken = remainingEntry.find_first_of(" ",startOfNextToken);
        endTime = remainingEntry.substr(startOfNextToken,endOfNextToken-startOfNextToken);
    } else {
        endTime = "";
    }

    return endTime;
    //otherwise endTime = startTime
}
 
std::string TMTimeParser::extractStartDate(std::string remainingEntry) {
    std::string startDate;
    //search for 'from'
    std::size_t fromToken = remainingEntry.find("from");
    std::size_t onToken = remainingEntry.find("on");
    if(fromToken != std::string::npos) {
        int startOfTokenAfterFrom = remainingEntry.find_first_not_of(" ",fromToken+4);
        int endOfTokenAfterFrom = remainingEntry.find_first_of(" ",startOfTokenAfterFrom);
        //need to check if token is desired target
        startDate = remainingEntry.substr(startOfTokenAfterFrom,endOfTokenAfterFrom-startOfTokenAfterFrom);
    } else if (onToken != std::string::npos) {
        int startOfTokenAfterOn = remainingEntry.find_first_not_of(" ",onToken+2);
        int endOfTokenAfterOn = remainingEntry.find_first_of(" ",startOfTokenAfterOn);
        //need to check if token is desired target
        startDate = remainingEntry.substr(startOfTokenAfterOn,endOfTokenAfterOn-startOfTokenAfterOn);
    } else {
        boost::gregorian::date today = boost::gregorian::day_clock::local_day();
        std::string tempDate = boost::gregorian::to_iso_string(today);
        startDate = tempDate.substr(6,2)
                    + "-"
                    + tempDate.substr(4,2)
                    + "-"
                    + tempDate.substr(0,4);
    }

    return startDate;
    //if found check if trailing word is a day and word after the day is 'to'/'-'
    //search for 'on'
    //if found check if the trailing word is e.g. Friday/
    // 13022015/130215
    // 13 Feb 2015/ 13 February 2015
}

std::string TMTimeParser::extractEndDate(std::string remainingEntry) {
    std::string endDate;
    //search for'to' check if there is a date before and after it
    std::size_t toToken = remainingEntry.find("to");
    if(toToken != std::string::npos) {
        int startOfTokenAfterTo = remainingEntry.find_first_not_of(" ",toToken+2);
        int endOfTokenAfterTo = remainingEntry.find_first_of(" ",startOfTokenAfterTo);
        endDate = remainingEntry.substr(startOfTokenAfterTo,endOfTokenAfterTo-startOfTokenAfterTo);
    } else {
        endDate = "";
    }

    return endDate;
    //otherwise endDate = startDate
}