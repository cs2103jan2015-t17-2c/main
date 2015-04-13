//@author A0111712Y

#include "Extractor.h"

Extractor* Extractor::theOne;

Extractor::Extractor() {
}

Extractor* Extractor::getInstance() {
	if (theOne == NULL) {
		theOne = new Extractor();
	}
	return theOne;
}

void Extractor::extractDateAndOrTime(int index, std::queue<int>& indexOfDatesAndTimes, std::string& extractedDate, std::string& extractedTime, std::vector<std::string> tokenizedUserEntry){
    //checks for startTime and startDate
    FormatConverter *formatConverter = FormatConverter::getInstance();
    TimeChecker *timeChecker = TimeChecker::getInstance();
    DateChecker *dateChecker = DateChecker::getInstance();
    std::string stringAfterToken = formatConverter->toLowerCase(tokenizedUserEntry[index]);

    int lengthOfTokenizedUserEntry = tokenizedUserEntry.size();

    if (dateChecker->isDay(stringAfterToken)||
        dateChecker->isToday(stringAfterToken)||
        dateChecker->isTomorrow(stringAfterToken)||
        dateChecker->isNumericDate(stringAfterToken)||
        dateChecker->isDelimitedDate(stringAfterToken)||
        dateChecker->isSpacedDate(index, tokenizedUserEntry)||
        dateChecker->isNextDay(index, tokenizedUserEntry)) {

            extractedDate = extractDayOrNumericDateOrDelimitedDate(index, indexOfDatesAndTimes, tokenizedUserEntry);
        
            index = indexOfDatesAndTimes.back();

        if(index + 1 == lengthOfTokenizedUserEntry) {
            return;
        }

        std::string stringAfterDate = formatConverter->toLowerCase(tokenizedUserEntry[index + 1]);
            
        if (timeChecker->is12HTime(stringAfterDate)||
            timeChecker->is24HTime(stringAfterDate)||
            timeChecker->isTimeWithoutPeriod(stringAfterDate)) {
                extractedTime = extractTime(index + 1, indexOfDatesAndTimes, tokenizedUserEntry);
            }
        
    } else if(timeChecker->is12HTime(stringAfterToken)||
              timeChecker->is24HTime(stringAfterToken)||
              timeChecker->isTimeWithoutPeriod(stringAfterToken)){
                  
                  extractedTime = extractTime(index, indexOfDatesAndTimes, tokenizedUserEntry);

        if(index + 1 == lengthOfTokenizedUserEntry) {
            return;
        }

        std::string stringAfterTime = formatConverter->toLowerCase(tokenizedUserEntry[index + 1]);
        
        if( dateChecker->isDay(stringAfterTime)||
            dateChecker->isToday(stringAfterTime)||
            dateChecker->isTomorrow(stringAfterTime)||
            dateChecker->isNumericDate(stringAfterTime)||
            dateChecker->isDelimitedDate(stringAfterTime)||
            dateChecker->isSpacedDate(index + 1, tokenizedUserEntry)||
            dateChecker->isNextDay(index + 1, tokenizedUserEntry)) {
                extractedDate = extractDayOrNumericDateOrDelimitedDate(index + 1, indexOfDatesAndTimes, tokenizedUserEntry);
            }
        
    }

    return;
}

std::string Extractor::extractDayOrNumericDateOrDelimitedDate(int index, std::queue<int>& indexOfDatesAndTimes, std::vector<std::string> tokenizedUserEntry){
    FormatConverter *formatConverter = FormatConverter::getInstance();
    DateChecker *dateChecker = DateChecker::getInstance();
    std::string stringAfterToken = formatConverter->toLowerCase(tokenizedUserEntry[index]);
    std::string startDate = "";
    
    if(dateChecker->isDay(stringAfterToken)){

        startDate = extractDay(index,indexOfDatesAndTimes, tokenizedUserEntry);

    } else if(dateChecker->isToday(stringAfterToken)) {

        startDate = extractToday(index, indexOfDatesAndTimes, tokenizedUserEntry);

    } else if(dateChecker->isTomorrow(stringAfterToken)) {

        startDate = extractTomorrow(index, indexOfDatesAndTimes, tokenizedUserEntry);

    } else if(dateChecker->isNumericDate(stringAfterToken)){

        startDate = extractNumericDate(index,indexOfDatesAndTimes, tokenizedUserEntry);

    } else if(dateChecker->isDelimitedDate(stringAfterToken)) {

        char delimiter = dateChecker->returnDelimiter(stringAfterToken);
        startDate = extractDelimitedDate(index,indexOfDatesAndTimes,tokenizedUserEntry,delimiter);

    } else if(dateChecker->isSpacedDate(index, tokenizedUserEntry)){

        startDate = extractSpacedDate(index, indexOfDatesAndTimes, tokenizedUserEntry);

    } else if(dateChecker->isNextDay(index, tokenizedUserEntry)) {

        startDate = extractNextDay(index, indexOfDatesAndTimes, tokenizedUserEntry);
    }

    return startDate;
}

std::string Extractor::extractDay(int index, std::queue<int>& indexOfDatesAndTimes, std::vector<std::string> tokenizedUserEntry){
    FormatConverter *formatConverter = FormatConverter::getInstance();
    std::string day = tokenizedUserEntry[index]; 
    int dayInInteger = dayOfWeek(day);
    boost::gregorian::first_day_of_the_week_after fdaf(dayInInteger);
    boost::gregorian::date dateInBoost = fdaf.get_date(currentDate());
    std::string startDate = formatConverter->dateFromBoostToDDMMYYYY(dateInBoost);
    indexOfDatesAndTimes.push(index);

    return startDate;
}

std::string Extractor::extractToday(int index, std::queue<int>& indexOfDatesAndTimes, std::vector<std::string> tokenizedUserEntry) {
    FormatConverter *formatConverter = FormatConverter::getInstance();
    indexOfDatesAndTimes.push(index);
    return currentDateInString();
}

std::string Extractor::extractTomorrow(int index, std::queue<int>& indexOfDatesAndTimes, std::vector<std::string> tokenizedUserEntry) {
    FormatConverter *formatConverter = FormatConverter::getInstance();
    indexOfDatesAndTimes.push(index);
    boost::gregorian::date today = currentDate();
    boost::gregorian::date_duration oneDay(1);
    boost::gregorian::date tomorrow = today + oneDay;
    return formatConverter->dateFromBoostToDDMMYYYY(tomorrow);
}

std::string Extractor::extractNumericDate(int index, std::queue<int>& indexOfDatesAndTimes, std::vector<std::string> tokenizedUserEntry){
    FormatConverter *formatConverter = FormatConverter::getInstance();
    std::string date = (tokenizedUserEntry[index]);
    if(date.length() == 6) {
        date = date.substr(0,4) + currentDateInString().substr(4,2) + date.substr(4,2);
    }
    indexOfDatesAndTimes.push(index);
    return date;
}

std::string Extractor::extractDelimitedDate(int index, std::queue<int>& indexOfDatesAndTimes, std::vector<std::string> tokenizedUserEntry, char key) {
    std::string day = "";
    std::string month = "";
    std::string year = "";
    FormatConverter *formatConverter = FormatConverter::getInstance();
    DateChecker *dateChecker = DateChecker::getInstance();
    std::string stringDate = tokenizedUserEntry[index];

    int positionOfNextKey = stringDate.find_first_of(key);
    day = stringDate.substr(0,positionOfNextKey);
    int lengthOfDay = day.length();

    if(lengthOfDay == 1){
        day = "0" + day;
    }

    stringDate = stringDate.substr(positionOfNextKey + 1); 
    positionOfNextKey = stringDate.find_first_of(key);
    
    if(positionOfNextKey != std::string::npos){
        month = stringDate.substr(0,positionOfNextKey);
        year = stringDate.substr(positionOfNextKey + 1);
    } else {
        month = stringDate;
    }

    if(dateChecker->isMonth(month)) {
        month = formatConverter->monthFromWrittenToNumeric(month);
    } else if(dateChecker->isNumericMonth(month) && month.length() == 1) {
        month = "0" + month;
    }

    if(year.length() == 2) {
        year = currentDateInString().substr(4,2) + year;
    }

    indexOfDatesAndTimes.push(index);
    return day + month + year;
}

//Precondition: isSpacedDate true
//Postcondition: returns date in ddmm format which will be configured at the end
//               of each parse task function
std::string Extractor::extractSpacedDate(int index, std::queue<int>& indexOfDatesAndTimes, std::vector<std::string> tokenizedUserEntry) {
    FormatConverter *formatConverter = FormatConverter::getInstance();
    DateChecker *dateChecker = DateChecker::getInstance();
    std::string day = tokenizedUserEntry[index];
    std::string month = formatConverter->toLowerCase(tokenizedUserEntry[index + 1]);

    if(day.length() == 1){
        day = "0" + day;
    }

    month = formatConverter->monthFromWrittenToNumeric(month);

    indexOfDatesAndTimes.push(index);
    indexOfDatesAndTimes.push(index + 1);

    return day + month;
}

//Precondition: isNextDay is true
//Postcondition: returns date of next weekday
std::string Extractor::extractNextDay(int index, std::queue<int>& indexOfDatesAndTimes, std::vector<std::string> tokenizedUserEntry){
    std::string stringDay = tokenizedUserEntry[index + 1];
    std::string date;

    indexOfDatesAndTimes.push(index);
    indexOfDatesAndTimes.push(index + 1);

    boost::gregorian::greg_weekday day(dayOfWeek(stringDay));
    boost::gregorian::first_day_of_the_week_after fdaf(day);
    boost::gregorian::first_day_of_the_week_after firstSundayAfterToday(boost::gregorian::Sunday);
    boost::gregorian::date dateTM = firstSundayAfterToday.get_date(currentDate());
    dateTM = fdaf.get_date(dateTM);
    std::string tempDate = boost::gregorian::to_iso_string(dateTM);

    date = tempDate.substr(6,2) + tempDate.substr(4,2) + tempDate.substr(0,4);

    return date;
}

//Precondition: check if token is time
//Postcondition: pushes index of keyword "at" into indexOfDatesAndTimes
//Postcondition: returns extracted time in HH:MM format except for time without am or pm
//               time without am or pm will be configured at the end
std::string Extractor::extractTime(int index, std::queue<int>& indexOfDatesAndTimes, std::vector<std::string> tokenizedUserEntry){
    FormatConverter *formatConverter = FormatConverter::getInstance();
    TimeChecker *timeChecker = TimeChecker::getInstance();
    std::string stringAfterAt = formatConverter->toLowerCase(tokenizedUserEntry[index]);      
    std::string time;

    if(timeChecker->is12HTime(stringAfterAt)) {
        if(timeChecker->isAM(stringAfterAt)){
            time = formatConverter->timeFrom12HourAMToHHMM(stringAfterAt);
        } else if (timeChecker->isPM(stringAfterAt)) {
            time = formatConverter->timeFrom12HourPMToHHMM(stringAfterAt);
        }
        indexOfDatesAndTimes.push(index);
    } else if (timeChecker->is24HTime(stringAfterAt)){
        time = formatConverter->timeFrom24HourToHHMM(stringAfterAt);
        indexOfDatesAndTimes.push(index);
    } else if (timeChecker->isTimeWithoutPeriod(stringAfterAt)) {
        time = stringAfterAt;
        indexOfDatesAndTimes.push(index);
    }

    return time;
}

//Precondition: string must be a weekday and in lowercase
//Postcondition: returns integer tagged to each boost day
int Extractor::dayOfWeek(std::string day) {
    FormatConverter *formatConverter = FormatConverter::getInstance();
    day = formatConverter->toLowerCase(day);

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
        return -1;
    }
}