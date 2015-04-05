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

    //DD/MM    DD/MM/YY    DD/MM/YYYY    
    bool isSlashedNumericDate(std::string);

    //DD.MM    DD.MM.YY    DD.MM.YYYY
    bool isDottedNumericDate(std::string);
    bool isOneNumericDateForm(std::string, char);

    bool isDDMonDate(std::string); //3-mar 3-mar-14

    bool isMonth(std::string);
    bool isNumericMonth(std::string);
    bool isDay(std::string);
    bool isNextDay(int, std::vector<std::string>);
};
#endif DATECHECKER_H