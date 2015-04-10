#include "TMDateTime.h"

TMDateTime::TMDateTime(){
    boost::gregorian::date startDate(boost::gregorian::not_a_date_time);
    _date = startDate;
    _time = "";
}

TMDateTime::TMDateTime(std::string date, std::string time){
    if(date == ""){
        _date = boost::gregorian::date(boost::gregorian::not_a_date_time);
    } else {
        _date = boost::gregorian::from_uk_string(date);
    }

    _time = time;
}

std::string TMDateTime::getDate(){
    if(_date.is_not_a_date()){
        return "";
    } else {
        std::locale facet(std::locale::classic(), new boost::gregorian::date_facet("%d %b %Y"));
        std::ostringstream stream;
        stream.imbue(facet);
        stream << _date;
        return stream.str();
    }
}

boost::gregorian::date TMDateTime::getBoostDate(){
    return _date;
}

std::string TMDateTime::getTime(){
    return _time;
}

void TMDateTime::setDate(std::string newDate){
    _date = boost::gregorian::from_uk_string(newDate);
}

void TMDateTime::setTime(std::string newTime){
    _time = newTime;
}