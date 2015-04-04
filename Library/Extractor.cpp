#include "Extractor.h"

Extractor::Extractor() {
}

Extractor* Extractor::getInstance() {
	if (theOne == NULL) {
		theOne = new Extractor();
	}

    formatConverter = FormatConverter::getInstance();
    timeChecker = TimeChecker::getInstance();
    dateChecker = DateChecker::getInstance();

	return theOne;
}

std::string Extractor::extractDayOrNumericDateOrDDMonDate(int index, std::queue<int>& indexOfDatesAndTimes, std::vector<std::string> tokenizedUserEntry){
    std::string stringAfterOn = formatConverter->returnLowerCase(tokenizedUserEntry[index]);
    std::string startDate = "";

    if(dateChecker->isNumericDate(stringAfterOn)){
        startDate = extractNumericDate(index,indexOfDatesAndTimes, tokenizedUserEntry);
    } else if(dateChecker->isDay(stringAfterOn)){
        startDate = extractDay(index,indexOfDatesAndTimes, tokenizedUserEntry);
    } else if(dateChecker->isDDMonDate(stringAfterOn)){               
        startDate = extractDDMonDate(index,indexOfDatesAndTimes, tokenizedUserEntry);
    } else {
        //found on but cannot find following date or day
    }

    return startDate;
}

std::string Extractor::extractNumericDate(int index, std::queue<int>& indexOfDatesAndTimes, std::vector<std::string> tokenizedUserEntry){
    std::string startDate = formatConverter->returnLowerCase(tokenizedUserEntry[index]);
    indexOfDatesAndTimes.push(index);
    return startDate;
}

std::string Extractor::extractDDMonDate(int index, std::queue<int>& indexOfDatesAndTimes, std::vector<std::string> tokenizedUserEntry){
    std::string dd = "";
    std::string month = "";
    std::string yyyy = "";
    std::string stringDate = formatConverter->returnLowerCase(tokenizedUserEntry[index]);

    int positionOfNextDash = stringDate.find_first_of("-");
    dd = stringDate.substr(0,positionOfNextDash);
    int lengthOfdd = dd.length();
    if(lengthOfdd == 1){
        dd = "0" + dd;
    }
    stringDate = stringDate.substr(positionOfNextDash + 1); //stringDate = mm-yyyy 
    positionOfNextDash = stringDate.find_first_of("-");
    
    if(positionOfNextDash != std::string::npos){
        month = stringDate.substr(0,positionOfNextDash);
        yyyy = stringDate.substr(positionOfNextDash + 1);
    } else {
        month = stringDate;
        std::string dateToday = formatConverter->dateFromBoostToDDMMYYYY(currentDate);
        yyyy = dateToday.substr(4);
    }
    month = formatConverter->monthFromWrittenToNumeric(month);
    indexOfDatesAndTimes.push(index);
    return dd + month + yyyy;
}

//preconditions: isNextDay is true. used for deadline. comes after "before".
std::string Extractor::extractNextDay(int index, std::queue<int>& indexOfDatesAndTimes, std::vector<std::string> tokenizedUserEntry){
    //check for day after next, then check for time iter + 1 = next
    std::string date = getDateFromNextDay(index, tokenizedUserEntry);
    indexOfDatesAndTimes.push(index);
    indexOfDatesAndTimes.push(index + 1);
    //
    return date;
}

//preconditions only used after token before
std::string Extractor::getDateFromNextDay(int index, std::vector<std::string> tokenizedUserEntry){
    std::string stringDay = formatConverter->returnLowerCase(tokenizedUserEntry[index + 1]);
    std::string date;

    boost::gregorian::greg_weekday day(dayOfWeek(stringDay));
    boost::gregorian::first_day_of_the_week_after fdaf(day);
    boost::gregorian::first_day_of_the_week_after firstSundayAfterToday(boost::gregorian::Sunday);
    boost::gregorian::date dateTM = firstSundayAfterToday.get_date(currentDate);
    dateTM = fdaf.get_date(dateTM);
    std::string tempDate = boost::gregorian::to_iso_string(dateTM);

    date = tempDate.substr(6,2) + tempDate.substr(4,2) + tempDate.substr(0,4);

    return date;
}

std::string Extractor::extractDay(int index, std::queue<int>& indexOfDatesAndTimes, std::vector<std::string> tokenizedUserEntry){
    std::string day = formatConverter->returnLowerCase(tokenizedUserEntry[index]); 
    int dayInInteger = dayOfWeek(day);
    boost::gregorian::first_day_of_the_week_after fdaf(dayInInteger);
    boost::gregorian::date dateInBoost = fdaf.get_date(currentDate);
    std::string startDate = formatConverter->dateFromBoostToDDMMYYYY(dateInBoost);
    indexOfDatesAndTimes.push(index);

    return startDate;
}

std::string Extractor::extractTime(int index, std::queue<int>& indexOfDatesAndTimes, std::vector<std::string> tokenizedUserEntry){
    std::string stringAfterAt = formatConverter->returnLowerCase(tokenizedUserEntry[index]);      
    std::string time = "";

    if(timeChecker->is12HTime(stringAfterAt)) {
        if(timeChecker->isAM(stringAfterAt)){
            time = formatConverter->timeFrom12HourAMToHHMM(stringAfterAt);
            indexOfDatesAndTimes.push(index);
        } else if (timeChecker->isPM(stringAfterAt)) {
            time = formatConverter->timeFrom12HourPMToHHMM(stringAfterAt);
            indexOfDatesAndTimes.push(index);
        }
        indexOfDatesAndTimes.push(index);
    } else if (timeChecker->is24HTime(stringAfterAt)){
        time = formatConverter->timeFrom24HourToHHMM(stringAfterAt);
        indexOfDatesAndTimes.push(index);
    }

    return time;
}

void Extractor::extractDateAndOrTime(int index, std::queue<int>& indexOfDatesAndTimes, std::string& extractedDate, std::string& extractedTime, std::vector<std::string> tokenizedUserEntry){
                //checks for startTime and startDate
    std::string stringAfterToken = formatConverter->returnLowerCase(tokenizedUserEntry[index]);
    std::string date = "";
    std::string time = "";
    int lengthOfTokenizedUserEntry = tokenizedUserEntry.size();

    if(dateChecker->isDay(stringAfterToken)||dateChecker->isNumericDate(stringAfterToken)||dateChecker->isDDMonDate(stringAfterToken)) {
        date = extractDayOrNumericDateOrDDMonDate(index, indexOfDatesAndTimes, tokenizedUserEntry);
        
        if(index + 1 != lengthOfTokenizedUserEntry) {
            std::string stringAfterDate = formatConverter->returnLowerCase(tokenizedUserEntry[index + 1]);
            
            if(timeChecker->is12HTime(stringAfterDate)||timeChecker->is24HTime(stringAfterDate)) {
                time = extractTime(index + 1, indexOfDatesAndTimes, tokenizedUserEntry);
            }
        }
    } else if(dateChecker->isNextDay(index, tokenizedUserEntry)) {
        date = extractNextDay(index, indexOfDatesAndTimes, tokenizedUserEntry);

        if(index + 2 != lengthOfTokenizedUserEntry){
            std::string stringAfterNextDay = formatConverter->returnLowerCase(tokenizedUserEntry[index + 2]);

            if(timeChecker->is12HTime(stringAfterNextDay)||timeChecker->is24HTime(stringAfterNextDay)) {
                time = extractTime(index + 2, indexOfDatesAndTimes, tokenizedUserEntry);
            }

        }
    } else if(timeChecker->is12HTime(stringAfterToken)||timeChecker->is24HTime(stringAfterToken)){
        time = extractTime(index, indexOfDatesAndTimes, tokenizedUserEntry);

        if(index + 1 != lengthOfTokenizedUserEntry){

            std::string stringAfterTime = formatConverter->returnLowerCase(tokenizedUserEntry[index + 1]);
            if(dateChecker->isNumericDate(stringAfterTime)||dateChecker->isDay(stringAfterTime)||dateChecker->isDDMonDate(stringAfterTime)){
                date = extractDayOrNumericDateOrDDMonDate(index + 1, indexOfDatesAndTimes, tokenizedUserEntry);
            } else if(dateChecker->isNextDay(index + 1, tokenizedUserEntry)){
                date = extractNextDay(index + 1, indexOfDatesAndTimes, tokenizedUserEntry);
            }
        }
    } 

    extractedDate = date;
    extractedTime = time;

    return;
}

int Extractor::dayOfWeek(std::string day) {
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