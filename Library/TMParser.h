#ifndef TMPARSER_H
#define TMPARSER_H

#include "TMTask.h"
#include "TMTaskTime.h"
#include <iostream>
#include <sstream>
#include <vector>
#include <queue>
#include <ctype.h>

//user input date must be ddmmyyyy
//time input if in 12hour format must be followed immediately by pm and am
//else in 24 hours format hhmm

class TMParser {
private:
    boost::gregorian::date _dateToday;
	std::string _originalUserInput;
	std::vector<std::string> _tokenizedUserEntry;
	static TMParser* theOne;
	TMParser();

public:
    static TMParser* getInstance();
    
	void TMParser::initialize(std::string userEntry);

    enum CommandTypes {
        Add, Delete, Edit, SearchKeyword, SearchDateTime, SaveAt, Block, Confirm, Complete, CompleteAllToday, Incomplete,  Undo, Redo, Invalid
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

    //need to check if there is sufficient space at the back first
    std::string extractDayOrNumericDateOrDDMonDate(int, std::queue<int>&);
    std::string extractNumericDateAfterToken(int, std::queue<int>&);
    std::string extractDDMonDateAfterToken(int, std::queue<int>&);
    std::string extractNextDayAfterToken(int, std::queue<int>&);
    std::string getDateFromNextDay(int);
    std::string extractNextDay(int, std::queue<int>&);
    std::string extractDayAfterToken(int, std::queue<int>&);
    std::string extractTimeAfterToken(int, std::queue<int>&);
    void extractDateAndOrTime(int, std::queue<int>&, std::string&,std::string&);


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
    //
    bool isDDMonDate(std::string);
    bool isDay(std::string);
    bool is12HTime(std::string);
    bool is24HTime(std::string);
    bool isNextDay(int);
    bool isNext(std::string);
    bool isAM(std::string);
    bool isPM(std::string);
    //
    bool isMonth(std::string);

    int numberOfWords(std::string);
    //0 for Sunday and 6 for Saturday
    int dayOfWeek(std::string); 

    //use after command is extracted
    std::vector<int> parseTaskPositionNo();
    std::string parseSearchKey();
    std::string parseDirectory();

    std::string returnLowerCase(std::string);

    std::string dateFromNumericToBoostFormat(std::string);
    std::string dateFromBoostToStandardFormat(const boost::gregorian::date&);
    std::string dateFromBoostToDelimitedDDMMYYYY(const boost::gregorian::date&);
    std::string dateFromBoostToDDMMYYYY(const boost::gregorian::date&);

    std::string monthFromWrittenToNumeric(std::string);

    std::string timeTo24HFormat(std::string);
    std::string getCurrentTime();
    std::string substractNDaysFromDate(std::string,int);
    std::string addNDaysFromDate(std::string,int);
};
#endif