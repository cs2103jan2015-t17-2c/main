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
};
#endif TASKCHECKER_H