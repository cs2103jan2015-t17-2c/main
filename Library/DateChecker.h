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
    
    //Preconditions: Time format DDMMYY DDMMYYYY
    bool isNumericDate(std::string);
    bool isOneDelimitedDate(std::string);
    bool isDelimitedDate(std::string, char);
    char returnDelimiter(std::string);
    bool isSpacedDate(int, std::vector<std::string>);

    bool isValidDate(std::string);
    bool isUnoccurredDate(std::string);

    bool isToday(std::string);
    bool isTomorrow(std::string);
    bool isDay(std::string);
    bool isNextDay(int, std::vector<std::string>);
    bool isMonth(std::string);
    bool isNumericMonth(std::string);

    bool isPositiveInteger(std::string);
};
#endif DATECHECKER_H