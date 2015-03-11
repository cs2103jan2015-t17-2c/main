#ifndef TMTASKTIME_H
#define TMTASKTIME_H
#include <string>
#include <boost\date_time.hpp>

class TMTaskTime{
private:
    boost::gregorian::date _startDate;
    boost::posix_time::ptime _startTime;
    boost::gregorian::date _endDate;
    boost::posix_time::ptime _endTime;

public:
    TMTaskTime();
    TMTaskTime(std::string, int, std::string, int);

    std::string getStartDate();
    int getStartTime();
    std::string getEndDate();
    int getEndTime();

    void setStartDate(std::string);
    void setStartTime(int);
    void setEndDate(std::string);
    void setEndTime(int);
};
#endif