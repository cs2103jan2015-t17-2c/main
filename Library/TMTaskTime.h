#ifndef TMTASKTIME_H
#define TMTASKTIME_H
#include <string>

class TMTaskTime{
private:
    std::string _startDate;
    int _startTime;
    std::string _endDate;
    int _endTime;

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