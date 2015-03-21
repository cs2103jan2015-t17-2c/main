#ifndef TMPARSER_H
#define TMPARSER_H

#include "TMTask.h"
#include "TMTaskTime.h"
#include "TMTimeParser.h"
#include <iostream>
#include <sstream>
#include <vector>
#include <ctype.h>

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
        Add, Delete, Undo, Complete, Incomplete, Search, Edit, Store, Invalid
    };

    std::vector<std::string> getTokenizedUserEntry(std::string);
    std::string extractCommand();
	std::vector<std::string> returnTokens();

    CommandTypes determineCommandType(std::string);
    
    //parse relevant info into the respective tasks
    //commmand must be extracted first
    std::vector<TMTask> parseTaskInfo();
    TMTask parseDeadlinedTaskInfo();
    TMTask parseTimedTaskInfo();
    TMTask parseFloatingTaskInfo();

    std::string extractDayOrDate(std::vector<std::string>&,std::vector<std::string>::iterator&);
    std::string extractDateAfterToken(std::vector<std::string>&,std::vector<std::string>::iterator&);
    std::string extractNextDayAfterToken(std::vector<std::string>&,std::vector<std::string>::iterator&);
    std::string extractDayAfterToken(std::vector<std::string>&,std::vector<std::string>::iterator&);
    std::string extractTimeAfterToken(std::vector<std::string>&,std::vector<std::string>::iterator&);


    bool isDeadlinedTask();
    bool isTimedTask();
    bool isValidDate(std::string);
    //
    bool isInteger(std::string);
    bool isPeriod(std::string);
    //to test for date in other forms. define another function to convert 
    bool isDate(std::string);
    bool isDay(std::string);
    bool is12HTime(std::string);
    bool is24HTime(std::string);
    bool isWordNext(std::string);
    bool isAM(std::string);
    bool isPM(std::string);

    int numberOfWords(std::string);
    //0 for Sunday and 6 for Saturday
    int dayOfWeek(std::string); 

    //use after command is extracted
    std::vector<int> parseTaskPositionNo();
    std::string parseSearchKey();
    std::string parseDirectory();

    std::string returnLowerCase(std::string);
    std::string dateFromUserToBoostFormat(std::string);
    std::string dateFromBoostToStandardFormat(const boost::gregorian::date&);
    std::string dateFromBoostToDelimitedDDMMYYYY(const boost::gregorian::date&);
    std::string dateFromBoostToDDMMYYYY(const boost::gregorian::date&);

    std::string timeTo24HFormat(std::string);
    std::string getCurrentTime();
    std::string substractNDaysFromDate(std::string,int);
    std::string addNDaysFromDate(std::string,int);

};
#endif