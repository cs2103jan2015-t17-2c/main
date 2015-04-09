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

    void extractDateAndOrTime(int, std::queue<int>&, std::string&,std::string&, std::vector<std::string>);
    std::string extractDayOrNumericDateOrDelimitedDate(int, std::queue<int>&, std::vector<std::string>);

    std::string extractDay(int, std::queue<int>&, std::vector<std::string>);
    std::string extractToday(int, std::queue<int>&, std::vector<std::string>);
    std::string extractTomorrow(int, std::queue<int>&, std::vector<std::string>);
    std::string extractNumericDate(int, std::queue<int>&, std::vector<std::string>);
    std::string extractDelimitedDate(int, std::queue<int>&, std::vector<std::string>, char);
    std::string extractSpacedDate(int, std::queue<int>&, std::vector<std::string>);
    std::string extractNextDay(int, std::queue<int>&, std::vector<std::string>);

    std::string getDateFromNextDay(int, std::vector<std::string>);
    

    //is without period return 1 or 2 digit string else return HHMM
    std::string extractTime(int, std::queue<int>&, std::vector<std::string>);
    
    //0 for Sunday and 6 for Saturday
    int dayOfWeek(std::string); 
};
#endif