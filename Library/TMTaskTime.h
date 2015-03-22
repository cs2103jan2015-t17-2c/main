#ifndef TMTASKTIME_H
#define TMTASKTIME_H
#include "TMDateTime.h"
#include <string>
#include <boost\date_time.hpp>


class TMTaskTime {
private:
    TMDateTime _startDateTime;
    TMDateTime _endDateTime;

public:
    TMTaskTime();
    TMTaskTime(std::string, std::string, std::string, std::string);

    std::string getStartDate();
    std::string getStartTime();
    std::string getEndDate();
    std::string getEndTime();

    void setStartDate(std::string);
    void setStartTime(std::string);
    void setEndDate(std::string);
    void setEndTime(std::string);
};
#endif