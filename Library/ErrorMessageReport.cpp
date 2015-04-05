#include "ErrorMessageReport.h"

ErrorMessageReport* ErrorMessageReport::theOne;

ErrorMessageReport::ErrorMessageReport() {
}

ErrorMessageReport* ErrorMessageReport::getInstance() {
    if(theOne == NULL) {
        theOne = new ErrorMessageReport();
    }
    return theOne;
}

void ErrorMessageReport::initialize() {
    _errorMessageReport.clear();
}

void ErrorMessageReport::addErrorMessage(std::string errorMessage) {
    _errorMessageReport.push_back(errorMessage);
}

std::string ErrorMessageReport::getErrorMessageReport() {
    int numberOfErrorMessages = _errorMessageReport.size();
    std::string errorMessageReport;

    for(int i = 0; i < numberOfErrorMessages; i++) {
        errorMessageReport = errorMessageReport + _errorMessageReport[i];
    }

    return errorMessageReport;
}