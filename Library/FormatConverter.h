#ifndef FORMATCONVERTER_H
#define FORMATCONVERTER_H

#include <string>
#include <boost/date_time.hpp>
#include <boost/assign/list_of.hpp>
#include "TMTask.h"

class FormatConverter {
private:
    static FormatConverter* theOne;
    FormatConverter();

public:
    static FormatConverter* getInstance();

    std::string returnLowerCase(std::string);

    std::string dateFromNumericToBoostFormat(std::string);
    std::string dateFromNumericToStandardFormat(std::string);
    std::string dateFromBoostToStandardFormat(const boost::gregorian::date&);
    std::string dateFromBoostToDelimitedDDMMYYYY(const boost::gregorian::date&);
    std::string dateFromBoostToDDMMYYYY(const boost::gregorian::date&);

    std::string monthFromWrittenToNumeric(std::string);

    //incomplete
    //preconditions check if is 24H
    std::string timeFrom24HourToHHMM(std::string);
    //preconditions check if 12H AM
    std::string timeFrom12HourAMToHHMM(std::string);
    //preconditions check if 12H PM
    std::string timeFrom12HourPMToHHMM(std::string);

    TMTask convertStringToTMTask(std::string);
    TaskType convertStringToTaskType(std::string);
    std::string convertTaskTypeToString(TaskType);
};
#endif