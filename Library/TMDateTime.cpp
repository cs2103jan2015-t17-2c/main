#include "TMDateTime.h"

const std::locale facet(std::locale::classic(), new boost::gregorian::date_facet("%d %b %Y"));

TMDateTime::TMDateTime(){
    boost::gregorian::date startDate(boost::gregorian::not_a_date_time);
    _date = startDate;
    _time = "";
}

TMDateTime::TMDateTime(std::string date, std::string time){
    //need to try catch and throw
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
        std::stringstream stream;
        stream.imbue(facet);
        stream << _date;
        return stream.str();
    }
}

std::string TMDateTime::getTime(){
    return _time;
}

void TMDateTime::setDate(std::string newDate){
    _date = boost::gregorian::from_undelimited_string(newDate);
}

void TMDateTime::setTime(std::string newTime){
    _time = newTime;
}