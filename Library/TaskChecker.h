#ifndef TASKCHECKER_H
#define TASKCHECKER_H

#include <string>
#include <vector>
#include "TMConstants.h"
#include "FormatConverter.h"
#include "DateChecker.h"
#include "TimeChecker.h"

class TaskChecker {
private:
    static TaskChecker* theOne;
    TaskChecker();

public:
    static TaskChecker* getInstance();

    bool isDeadlinedTask(std::vector<std::string>);
    bool isTimedTask(std::vector<std::string>);

    bool isWordBefore(std::string);
    bool isWordBy(std::string);
    bool isWordAt(std::string);
    bool isWordOn(std::string);
    bool isWordFrom(std::string);
    bool isWordTo(std::string); 

    bool isDateOrTime(std::string, int, std::vector<std::string>);
};
#endif TASKCHECKER_H