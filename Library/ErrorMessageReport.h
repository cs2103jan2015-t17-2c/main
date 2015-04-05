#ifndef ERRORMESSAGEREPORT_H
#define ERRORMESSAGEREPORT_H

#include <vector>
#include <string>

class ErrorMessageReport {
private:
    std::vector<std::string> _errorMessageReport;
    static ErrorMessageReport* theOne;
	ErrorMessageReport();

public:
    static ErrorMessageReport* getInstance();
    //clears report
    void ErrorMessageReport::initialize();
 
    void addErrorMessage(std::string);
    std::string getErrorMessageReport();
};
#endif