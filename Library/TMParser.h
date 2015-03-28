#ifndef TMPARSER_H
#define TMPARSER_H

#include "TMTask.h"
#include "TMTaskTime.h"
#include <iostream>
#include <sstream>
#include <vector>
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
        Add, Delete, Edit, SearchKeyword, SearchDateTime, SaveAt, Block, Confirm, Complete, CompleteAllToday, Incomplete,  Undo, Redo, 
		ViewFreeTime, ViewDate, ViewDeadline, ViewUndated, ViewArchived, ViewAllTasks, Invalid
    };

    std::vector<std::string> getTokenizedUserEntry(std::string);
    std::string extractCommand();
	std::vector<std::string> returnTokens();

    CommandTypes determineCommandType(std::string);
    
    //parse relevant info into the respective tasks
    //commmand must be extracted first
    std::vector<TMTask> parseTaskInfo();
    std::vector<TMTask> parseDeadlinedTaskInfo();
    std::vector<TMTask> parseTimedTaskInfo();
    std::vector<TMTask> parseUndatedTaskInfo();
    //std::vector<TMTask> returnSplitPeriodTasks(std::string, std::string, std::string, std::string, std::string, TaskType);

    //need to check if there is sufficient space at the back first
    std::string extractDayOrNumericDateOrDDMonDate(std::vector<std::string>&, std::vector<std::string>::iterator);
    std::string extractNumericDateAfterToken(std::vector<std::string>&, std::vector<std::string>::iterator);
    std::string extractDDMonDateAfterToken(std::vector<std::string>&, std::vector<std::string>::iterator);
    std::string extractNextDayAfterToken(std::vector<std::string>&, std::vector<std::string>::iterator);
    std::string extractNextDay(std::vector<std::string>&, std::vector<std::string>::iterator);
    std::string extractDayAfterToken(std::vector<std::string>&, std::vector<std::string>::iterator);
    std::string extractTimeAfterToken(std::vector<std::string>&, std::vector<std::string>::iterator);
    void extractDateAndOrTime(std::vector<std::string>&,std::vector<std::string>::iterator, std::string&,std::string&);


    bool isDeadlinedTask();
    bool isTimedTask();
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
    bool isNextDay(const std::vector<std::string>&, std::vector<std::string>::iterator);
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
    std::string getDateFromNextDay(std::vector<std::string>&,std::vector<std::string>::iterator);
};
#endif