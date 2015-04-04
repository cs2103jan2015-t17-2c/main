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
    bool isInteger(std::string);
    bool isNumericDate(std::string);
    bool isDDMonDate(std::string);
    bool isMonth(std::string);
    bool isDay(std::string);
    bool isNextDay(int, std::vector<std::string>);
};
#endif DATECHECKER_H