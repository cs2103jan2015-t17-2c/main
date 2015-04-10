#ifndef TIMECHECKER_H
#define TIMECHECKER_H

#include <string>
#include "FormatConverter.h"

class TimeChecker {
private:
    static TimeChecker* theOne;
    TimeChecker();

public:
    static TimeChecker* getInstance();
    //1pm 12pm 1230pm 130am
    bool is12HTime(std::string);
    bool isAM(std::string);
    bool isPM(std::string);
    bool isTimeWithoutPeriod(std::string);

    //0800 
    bool is24HTime(std::string);

    bool isPositiveInteger(std::string);
    bool isInteger(std::string);
};
#endif