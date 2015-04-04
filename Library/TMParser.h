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
#include <boost/assign/list_of.hpp>

//user input date must be ddmmyyyy
//time input if in 12hour format must be followed immediately by pm and am
//else in 24 hours format hhmm

class TMParser {
private:
    boost::gregorian::date _dateToday;
	std::string _originalUserInput;
	std::vector<std::string> _tokenizedUserEntry;
    std::vector<std::string> _errorMessages;
	static TMParser* theOne;
	TMParser();

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
    //NEED TO IMPLEMENT FUNCTION TO REMOVE OPEN AND CLOSE INVERTED COMMAS
    TMTask parseTaskInfo();
    TMTask parseDeadlinedTaskInfo();
    TMTask parseTimedTaskInfo();
    TMTask parseUndatedTaskInfo();
    //to block task with multiple timings >= 2
    std::vector<TMTask> parseMultipleTimingTaskInfo();
	EditableTaskComponent parseTaskComponent();

    //need to check if there is sufficient space at the back first
    std::string extractDayOrNumericDateOrDDMonDate(int, std::queue<int>&);
    std::string extractNumericDate(int, std::queue<int>&);
    std::string extractDDMonDate(int, std::queue<int>&);
    std::string extractNextDay(int, std::queue<int>&);
    std::string getDateFromNextDay(int);
    std::string extractDay(int, std::queue<int>&);
    std::string extractTime(int, std::queue<int>&);

    void extractDateAndOrTime(int, std::queue<int>&, std::string&,std::string&);
    void configureAllDatesAndTimes(std::string&, std::string&, std::string&, std::string&, TaskType&);

    bool isDeadlinedTask();
    bool isTimedTask();
    //search for keyword block (time&/date) and (time&/date)
    bool isMultipleTimingTask();

    bool isValidDate(std::string);
    //checks if startDate is not later than endDate and 
    //if startDate == endDate, startTime is no later than endTime
    bool isValidInfo(std::string, std::string, std::string, std::string);

    bool isInteger(std::string);
    //bool isPeriod(std::string);
    //to test for date in other forms. define another function to convert 
    bool isNumericDate(std::string);
    bool isDDMonDate(std::string);
    bool isDay(std::string);
    //12pm 1230pm 130am
    bool is12HTime(std::string);
    //0800 
    bool is24HTime(std::string);
    bool isNextDay(int);
    //NOT USED
    bool isNext(std::string);
    bool isAM(std::string);
    bool isPM(std::string);
    //
    bool isMonth(std::string);

    int numberOfWords(std::string);
    //0 for Sunday and 6 for Saturday
    int dayOfWeek(std::string); 

    //use after command is extracted
    //check if position no. is unique
    std::vector<int> parseTaskPositionNo();
    std::string parseSearchKey();
    std::string parseDirectory();

    std::string returnLowerCase(std::string);

    std::string dateFromNumericToBoostFormat(std::string);
    std::string dateFromNumericToStandardFormat(std::string);
    std::string dateFromBoostToStandardFormat(const boost::gregorian::date&);
    std::string dateFromBoostToDelimitedDDMMYYYY(const boost::gregorian::date&);
    std::string dateFromBoostToDDMMYYYY(const boost::gregorian::date&);

    std::string monthFromWrittenToNumeric(std::string);

    //incomplete
    //preconditions check if is 12H or 24H first
    std::string timeTo24HFormat(std::string);
    std::string getCurrentTime();
    std::string substractNDaysFromDate(std::string,int);
    std::string addNDaysFromDate(std::string,int);

    TMTask convertStringToTMTask(std::string);
    TaskType convertStringToTaskType(std::string);
    std::string convertTaskTypeToString(TaskType);

    void addErrorMessage(std::string);
    std::string getErrorMessage();

    int numberOfWordsInQuote(std::string);
};
#endif