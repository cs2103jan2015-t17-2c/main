#ifndef TMDATETIME_H
#define TMDATETIME_H
#include <string>
#include <iostream>
#include <locale>
#include <boost\date_time.hpp>

class TMDateTime{
private:
    boost::gregorian::date _date;
    std::string _time;

public:
    TMDateTime();
    TMDateTime(std::string, std::string);

    std::string getDate();
    boost::gregorian::date getBoostDate();
    std::string getTime();

    void setDate(std::string);
    void setTime(std::string);
};
#endif TMDATETIME_H