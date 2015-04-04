#ifndef TMPARSER_H
#define TMPARSER_H

#include "TMTask.h"
#include "TMTaskTime.h"
#include <iostream>
#include <sstream>
#include <vector>
#include <queue>
#include <ctype.h>
#include <map>
#include "TMConstants.h"
#include "DateChecker.h"
#include "TaskChecker.h"
#include "TimeChecker.h"
#include "FormatConverter.h"
#include "Extractor.h"
#include "CurrentDateAndTime.h"
//#include <boost/assign/list_of.hpp>

//user input date must be ddmmyyyy
//time input if in 12hour format must be followed immediately by pm and am
//else in 24 hours format hhmm

class TMParser {
private:
	std::string _originalUserInput;
	std::vector<std::string> _tokenizedUserEntry;
    std::vector<std::string> _errorMessages;
	static TMParser* theOne;
	TMParser();
    static TaskChecker* taskChecker;
    static DateChecker* dateChecker;
    static TimeChecker* timeChecker;
    static FormatConverter* formatConverter;
    static Extractor* extractor;

public:
    static TMParser* getInstance();
    
	void TMParser::initialize(std::string userEntry);

    enum CommandTypes {
        Add, Delete, Edit, SearchKeyword, SaveAt, Block, Confirm, Complete, CompleteAllToday, Incomplete,  Undo, Redo, Invalid
    };

    std::vector<std::string> getTokenizedUserEntry(std::string);
    std::string extractCommand();
	std::vector<std::string> returnTokens();

    CommandTypes determineCommandType(std::string);
    
    //parse relevant info into the respective tasks
    //commmand must be extracted first
    TMTask parseTaskInfo();
    TMTask parseDeadlinedTaskInfo();
    TMTask parseTimedTaskInfo();
    TMTask parseUndatedTaskInfo();
    //to block task with multiple timings >= 2
    std::vector<TMTask> parseMultipleTimingTaskInfo();
	EditableTaskComponent parseEditableTaskComponent();
	std::string parseDescription();

    //need to check if there is sufficient space at the back first
    void configureAllDatesAndTimes(std::string&, std::string&, std::string&, std::string&, TaskType&);

    //checks if startDate is not later than endDate and 
    //if startDate == endDate, startTime is no later than endTime
    bool isValidInfo(std::string, std::string, std::string, std::string);

    bool isInteger(std::string);
    //bool isPeriod(std::string);
    //to test for date in other forms. define another function to convert 

    int numberOfWords(std::string);

    //use after command is extracted
    //check if position no. is unique
    std::vector<int> parseTaskPositionNo();
    std::string parseSearchKey();
    std::string parseDirectory();

    std::string substractNDaysFromDate(std::string,int);
    std::string addNDaysFromDate(std::string,int);

    void addErrorMessage(std::string);
    std::string getErrorMessage();

    int numberOfWordsInQuote(std::string);
};
#endif