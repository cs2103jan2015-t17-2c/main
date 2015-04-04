#ifndef CURRENTDATEANDTIME_H
#define CURRENTDATEANDTIME_H

#include <boost\date_time.hpp>

std::string getCurrentTime(){
    boost::posix_time::ptime currentDateAndTime(boost::posix_time::second_clock::local_time());
    std::ostringstream os;
    boost::posix_time::time_facet* facet(new boost::posix_time::time_facet("%H%M"));
    os.imbue(std::locale(std::cout.getloc(), facet));
    os << currentDateAndTime;
    return os.str();
}

static boost::gregorian::date currentDate = boost::gregorian::day_clock::local_day();
static std::string currentTime = getCurrentTime();

#endif