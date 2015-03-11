#ifndef TMTIMEPARSER_H
#define TMTIMEPARSER_H

#include <boost\date_time.hpp>

class TMTimeParser {
private:
    std::string _startDate;
    std::string _startTime;
    std::string _endDate;
    std::string _endTime;

public:
    TMTimeParser();

    std::string getStartDate();
    std::string getStartTime();
    std::string getEndDate();
    std::string getEndTime();
    
    //edit individual pre-existing information
    void setStartDate(std::string);
    void setStartTime(std::string);
    void setEndDate(std::string);
    void setEndTime(std::string);

    //extract information from user's entry
    void extractStartTime(std::string);
    void extractEndTime(std::string);
    void extractStartDate(std::string);
    void extractEndDate(std::string);
};
#endif