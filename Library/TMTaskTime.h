#ifndef TMTASKTIME_H
#define TMTASKTIME_H
#include <string>
#include <iostream>
#include <locale>
#include <boost\date_time.hpp>

class TMTaskTime{
private:
    boost::gregorian::date _startDate;
    std::string _startTime;
    boost::gregorian::date _endDate;
    std::string _endTime;

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