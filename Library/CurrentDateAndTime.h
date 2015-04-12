//@author A0111712Y

#ifndef CURRENTDATEANDTIME_H
#define CURRENTDATEANDTIME_H

#include <boost\date_time.hpp>

static std::string currentTime(){
    boost::posix_time::ptime currentDateAndTime(boost::posix_time::second_clock::local_time());
    std::ostringstream os;
    boost::posix_time::time_facet* facet(new boost::posix_time::time_facet("%H:%M"));
    os.imbue(std::locale(std::cout.getloc(), facet));
    os << currentDateAndTime;
    return os.str();
}

static boost::gregorian::date currentDate() {
    return boost::gregorian::day_clock::local_day();
}

static std::string currentDateInString() {
    std::string stringDate = boost::gregorian::to_iso_string(currentDate());
    return stringDate.substr(6,2) + stringDate.substr(4,2) + stringDate.substr(0,4);
}

#endif