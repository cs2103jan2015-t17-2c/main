//@author A0111712Y

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
    bool isCommandAdd(std::string);
    bool isCommandDelete(std::string);
    bool isCommandUndo(std::string);
    bool isCommandRedo(std::string);
    bool isCommandComplete(std::string);
    bool isCommandIncomplete(std::string);
    bool isCommandSearch(std::string);
    bool isCommandEdit(std::string);
    bool isCommandStore(std::string);
    bool isCommandDoneAll(std::string);
    bool isCommandBlock(std::string);
    bool isCommandConfirm(std::string);
    bool isCommandQuit(std::string);

    //parse relevant info into the respective tasks
    //commmand must be extracted first
    TMTask parseTaskInfo();
    TMTask parseDeadlinedTaskInfo();
    TMTask parseTimedTaskInfo();
    TMTask parseUndatedTaskInfo();

    //to block task with multiple timings >= 2
    std::vector<TMTask> parseMultipleTimingTaskInfo();
    void configureEndDateTime(int&, std::queue<int>&, std::queue<int>&, std::string&, std::string&);

    void configureTaskDescription(std::string&);
    void configureTaskDescription(std::string&, std::queue<int>&);
    void configureQueuesAndIndex(std::queue<int>&, std::queue<int>&, int&, bool);
    
    void editDateOrTimeInInvertedCommas(std::string, int, bool, bool);
    bool isNextDayInInvertedCommas(std::string);
    bool isSpacedDateInInvertedCommas(std::string);
    bool isDateToBeEdited(std::string);

    void configureAllDatesAndTimes(std::string&, std::string&, std::string&, std::string&, TaskType&);

    //checks if ddmm has passed for the current year
    //adds one more year if not 29 feb else adds until the next nearest 29 feb is found
    void configureDayMonth(std::string&);

    //checks if start ddmm has passed for current year
    //adds one more year if passed else
    //compares end ddmm with start ddmm
    //if end ddmm is less than or equal to start ddmm end 1 more year to enddate
    void configureStartDayMonthEndDayMonth(std::string&, std::string&);
    
    //to convert inexplicit times such as 8 to 10 into 24 hour format
    void configureStartTimeEndTimeWithoutPeriods(std::string&, std::string&);
    void configureStartTimeWithoutPeriods(std::string&);
    void configureEndTimeWithoutPeriods(std::string&);

    void configureStartDateEndDate(std::string&, std::string&);
    void configureStartTimeEndTime(std::string&, std::string&);
    
    //precondition: ddmmyyyy format
    bool isStartDateLessThanEndDate(std::string, std::string);

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

    void addErrorMessage(std::string);
    std::string getErrorMessage();

    bool isWordBeforeOrByOrFromOrTo(std::string);

    std::string next29FebFrom(std::string);
    std::string last29FebFrom(std::string);
};
#endif