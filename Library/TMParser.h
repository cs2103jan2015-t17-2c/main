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
#include <algorithm>
#include "TMConstants.h"
#include "DateChecker.h"
#include "TaskChecker.h"
#include "TimeChecker.h"
#include "FormatConverter.h"
#include "Extractor.h"
#include "CurrentDateAndTime.h"
#include "ErrorMessageReport.h"

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

public:
    static TMParser* getInstance();
    
	void TMParser::initialize(std::string userEntry);

    enum CommandTypes {
        Add, Delete, Edit, SearchKeyword, SaveAt, Block, Confirm, Complete, CompleteAllToday, Incomplete,  Undo, Redo, Invalid, Exit
    };

    std::vector<std::string> getTokenizedUserEntry(std::string);
    std::string extractCommand();
    std::string extractTokenAfterCommand();
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

    //used for timedtask
    void configureQueuesAndIndex(std::queue<int>&, std::queue<int>&, int&);
    void configureQueuesAndIndexAfterToken(std::queue<int>&, std::queue<int>&, int&);

    void editDateOrTimeInInvertedCommas(std::string, int, bool, bool);

    //inverted commas must be removed
    bool isNextDayInInvertedCommas(std::string);
    bool isSpacedDateInInvertedCommas(std::string);

    void configureAllDatesAndTimes(std::string&, std::string&, std::string&, std::string&, TaskType&);
    //if on same day full time specified but start time more than end time ERROR
    //if full date specified for both and start date more than end date ERROR

    //checks if ddmm has passed for the current year
    //adds one more year if not 29 FEB!
    void configureDayMonth(std::string&);

    //checks if start ddmm has passed for current year
    //adds one more year if passed else
    //compares end ddmm with start ddmm adds years till they are valid
    void configureStartDayMonthEndDayMonth(std::string&, std::string&);
    
    //if time 8 to 10 configure
    //returns time of the day
    //need to check if start time has passed after using if so add one more day
    void configureStartTimeEndTimeWithoutPeriods(std::string&, std::string&);
    void configureStartTimeWithoutPeriods(std::string&);
    void configureEndTimeWithoutPeriods(std::string&);

    //checks if startDate is not later than endDate and 
    //if startDate == endDate, startTime is no later than endTime
    bool isValidInfo(std::string, std::string, std::string, std::string);

    bool isPositiveInteger(std::string);

    int numberOfWords(std::string);

    //use after command is extracted
    //check if position no. is unique
    std::vector<int> parseTaskPositionNo();
    bool isUniqueIndex(int, std::vector<int>);
    std::string parseSearchKey();
    std::string parseDirectory();

    //postcondition: return ddmmyyyy
    std::string addNDaysFromDate(std::string,int);
    std::string substractNDaysFromDate(std::string,int);
    
    std::string addNYearsFromDate(std::string, int);
    std::string subtractNYearsFromDate(std::string, int);

    //SHOULD I REMOVE ERROR MESSAGE FROM STORAGE AND JUST PUT IT IN PARSER?
    void addErrorMessage(std::string);
    std::string getErrorMessage();

    int numberOfWordsInQuote(std::string);
};
#endif