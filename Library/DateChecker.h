#ifndef DATECHECKER_H
#define DATECHECKER_H

#include <string>
#include <boost\date_time.hpp>
#include "TMConstants.h"
#include "CurrentDateAndTime.h"
#include "FormatConverter.h"

class DateChecker {
private:
    static DateChecker* theOne;
    DateChecker();

public:
    static DateChecker* getInstance();

    bool isValidDate(std::string);
    bool isPositiveInteger(std::string);

    //DDMMYY DDMMYYYY
    bool isNumericDate(std::string);
    
    //check if date has passed
    bool isOneDelimitedDate(std::string);
    char returnDelimiter(std::string);
    bool isDelimitedDate(std::string, char);
    bool isSpacedDate(int, std::vector<std::string>);

    //DDMMYYYY
    bool isUnoccurredDate(std::string);

    bool isMonth(std::string);
    bool isNumericMonth(std::string);
    bool isDay(std::string);
    bool isNextDay(int, std::vector<std::string>);
    bool isTomorrow(std::string);
    bool isToday(std::string);
};
#endif DATECHECKER_H