#ifndef TMTIMEPARSER_H
#define TMTIMEPARSER_H
#include <boost\date_time.hpp>

class TMTimeParser {
public:
    TMTimeParser();

    //extract information from user's entry
    std::string extractStartTime(std::string);
    std::string extractEndTime(std::string);
    std::string extractStartDate(std::string);
    std::string extractEndDate(std::string);

};
#endif