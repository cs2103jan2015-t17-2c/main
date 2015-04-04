#ifndef EXTRACTOR_H
#define EXTRACTOR_H

#include <string>
#include <queue>
#include <vector>
#include "FormatConverter.h"
#include "TimeChecker.h"
#include "DateChecker.h"
#include "CurrentDateAndTime.h"

class Extractor {
private:
    static Extractor* theOne;
    Extractor();

public:
    static Extractor* getInstance();

    std::string extractDayOrNumericDateOrDDMonDate(int, std::queue<int>&, std::vector<std::string>);
    std::string extractNumericDate(int, std::queue<int>&, std::vector<std::string>);
    std::string extractDDMonDate(int, std::queue<int>&, std::vector<std::string>);
    std::string extractNextDay(int, std::queue<int>&, std::vector<std::string>);
    std::string getDateFromNextDay(int, std::vector<std::string>);
    std::string extractDay(int, std::queue<int>&, std::vector<std::string>);
    std::string extractTime(int, std::queue<int>&, std::vector<std::string>);

    void extractDateAndOrTime(int, std::queue<int>&, std::string&,std::string&, std::vector<std::string>);

    //0 for Sunday and 6 for Saturday
    int dayOfWeek(std::string); 
};
#endif