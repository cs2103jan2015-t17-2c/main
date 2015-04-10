#include "FormatConverter.h"

FormatConverter *FormatConverter::theOne;

FormatConverter::FormatConverter() {
}

FormatConverter* FormatConverter::getInstance() {
	if (theOne == NULL) {
		theOne = new FormatConverter();
	}
	return theOne;
}

std::string FormatConverter::returnLowerCase(std::string word) {
    int length = word.size();
    char character;

    for(int i = 0; i < length; i++) {
        character = tolower(word[i]);
        word[i] = character;
    }

    return word;
}

//preconditions: isNumericDate is used to check if the token is valid
//postconditions: returns a string dd-mm-yyyy
std::string FormatConverter::dateFromNumericToBoostFormat(std::string stringDate) {
    std::string dd;
    std::string mm;
    std::string yyyy;
    int lengthOfStringDate = stringDate.length();
    
    //DDMMYY
    if(lengthOfStringDate == DATE_DDMMYY_LENGTH){
        dd = stringDate.substr(0,2);
        mm = stringDate.substr(2,2);
        yyyy = currentDateInString().substr(4,2) + stringDate.substr(4,2);

    //DDMMYYYY
    } else if(lengthOfStringDate == DATE_DDMMYYYY_LENGTH) {
        
        dd = stringDate.substr(0,2);
        mm = stringDate.substr(2,2);
        yyyy = stringDate.substr(4,4);
    } 

    return dd + "-" + mm + "-" + yyyy;
}

//preconditions: valid date format ddmmyy ddmmyyyy
//postconditions: returns string e.g. 01 Jan 2015
std::string FormatConverter::dateFromNumericToStandardFormat(std::string stringDate){
    std::string dateInBoostFormat = dateFromNumericToBoostFormat(stringDate);
    boost::gregorian::date boostDate = boost::gregorian::from_uk_string(dateInBoostFormat);
    return dateFromBoostToStandardFormat(boostDate);
}

//precondition: boost date must be valid
//postcondition: returns string e.g. 01 Jan 2015
std::string FormatConverter::dateFromBoostToStandardFormat(const boost::gregorian::date& date) {
    std::ostringstream os;
    boost::gregorian::date_facet* facet(new boost::gregorian::date_facet("%d %b %Y"));
    os.imbue(std::locale(std::cout.getloc(), facet));
    os << date;
    return os.str();
}

//precondition: boost date must be valid
//postcondition: returns string date with spaces in between i.e. dd mm yyyy
std::string FormatConverter::dateFromBoostToDelimitedDDMMYYYY(const boost::gregorian::date& date){
    std::ostringstream os;
    boost::gregorian::date_facet* facet(new boost::gregorian::date_facet("%d %m %Y"));
    os.imbue(std::locale(std::cout.getloc(), facet));
    os << date;
    return os.str();
}

//precondition: boost date must be valid
//postcondition: returns string date ddmmyyyy
std::string FormatConverter::dateFromBoostToDDMMYYYY(const boost::gregorian::date& date){
    std::ostringstream os;
    boost::gregorian::date_facet* facet(new boost::gregorian::date_facet("%d%m%Y"));
    os.imbue(std::locale(std::cout.getloc(),facet));
    os << date;
    return os.str();
}

//precondition: check if is 24HTime
//postcondition: 
std::string FormatConverter::timeFrom24HourToHHMM(std::string time){
    std::string HHMM;

    if (time.length() == 4){
        HHMM = time.substr(0,2) + ":" + time.substr(2,2);
    } else {
        HHMM = time;
    }

    return HHMM;
}

//preconditions check if 12H AM
std::string FormatConverter::timeFrom12HourAMToHHMM(std::string time) {
    std::string HHMM;
    int intTime;
    
    if(time.length() == 3) {
        HHMM = "0" + time.substr(0,1) + ":" + "00";
    } else if(time.length() == 4) {
        intTime = (std::stoi(time.substr(0,2)))%12;
        if(intTime == 0){
            HHMM = "00:00";
        } else {
            HHMM = time.substr(0,2) + ":" + "00";
        }
    } else if(time.length() == 5){
        HHMM = "0" + time.substr(0,1) + ":" + time.substr(1,2);
    } else {
        std::string hour = time.substr(0,2);
        if(hour == "12"){
            HHMM = "00:" + time.substr(2,2);
        } else {
            HHMM = time.substr(0,2) + ":" + time.substr(2,2);
        }
    }

    return HHMM;
}

//preconditions check if 12H PM
std::string FormatConverter::timeFrom12HourPMToHHMM(std::string time) {
    std::string HHMM;
    int intTime;
    
    if(time.length() == 3) {
        intTime = 12 + std::stoi(time.substr(0,1));
        HHMM = std::to_string(intTime) + ":00";
    } else if(time.length() == 4){
        intTime = (std::stoi(time.substr(0,2)))%12 + 12;
        HHMM = std::to_string(intTime) + ":00";
    } else if(time.length() == 5){
        intTime = 1200 + std::stoi(time.substr(0,3));
        HHMM = std::to_string(intTime);
        HHMM = HHMM.substr(0,2) + ":" + HHMM.substr(2,2);
    } else {
        int intTime = 1200 + (std::stoi(time.substr(0,4)))%1200;
        HHMM = std::to_string(intTime);
        HHMM = HHMM.substr(0,2) + ":" + HHMM.substr(2,2);
    }

    return HHMM;
}

std::string FormatConverter::monthFromWrittenToNumeric(std::string month){
    month = returnLowerCase(month);
    if(month == "jan"||month == "january"){
        return "01";
    } else if (month == "feb"||month == "february"){
        return "02";
    } else if (month == "mar"||month == "march"){
        return "03";
    } else if (month == "apr"||month == "april"){
        return "04";
    } else if (month == "may"){
        return "05";
    } else if (month == "jun"||month == "june"){
        return "06";
    } else if (month == "jul"||month == "july"){
        return "07";
    } else if (month == "aug"||month == "august"){
        return "08";
    } else if (month == "sep"||month == "september"){
        return "09";
    } else if (month == "oct"||month == "october"){
        return "10";
    } else if (month == "nov"||month == "november"){
        return "11";
    } else if (month == "dec"||month == "december") {
        return "12";
    }
}

TMTask FormatConverter::convertStringToTMTask(std::string listEntry){
    
	std::string stringTaskType;
	std::string taskDescription;
    std::string token;

    std::string startDay;
    std::string startMonth;
    std::string startYear;
    std::string startTime;

    std::string endDay;
    std::string endMonth;
    std::string endYear;
    std::string endTime;

    bool isCompleted;
	bool isClashed;
    bool isConfirmed;
	int unconfirmedBatchNumber;

    TaskType taskType;
    std::cout << "IN FUNCTION" << std::endl;
    std::cout << listEntry << std::endl;
    std::istringstream iss(listEntry);

	iss >> stringTaskType;
	taskType = convertStringToTaskType(stringTaskType);
    std::cout << stringTaskType << std::endl;
    
	if (taskType != TaskType::Undated) {

        iss >> token;
       std::cout << token << std::endl;
        while(token != "||") {  
            std::cout << token << std::endl;
            taskDescription = taskDescription + token + " ";
            iss >> token;
        }

        if(taskDescription != "") {
            taskDescription.erase(taskDescription.end()-1);
        }

		iss >> startDay;
		iss >> startMonth;
		iss >> startYear;
		iss >> startTime;

		iss >> endDay;
		iss >> endMonth;
		iss >> endYear;
		iss >> endTime;

		iss >> isCompleted;
		iss >> isClashed;
		iss >> isConfirmed;
		iss >> unconfirmedBatchNumber;

		std::string startDate = startDay + "-" + startMonth + "-" + startYear;
		std::string endDate = endDay + "-" + endMonth + "-" + endYear;

		TMTaskTime taskTime(startDate, startTime, endDate, endTime);
		TMTask task(taskDescription, taskTime, taskType);

		if(isCompleted){
			task.setAsCompleted();
		}

		if(!isConfirmed){
			task.setAsUnconfirmed();
		}

		if(isClashed){
			task.setAsClashed();
		}

		task.setUnconfirmedBatchNumber(unconfirmedBatchNumber);
        std::cout << "reached end\n";
		return task;
	} else {
		iss >> token;

        while(token != "||"){
		    std::cout << token << std::endl;
            taskDescription = taskDescription + token + " ";
            iss >> token;
        }

        if(taskDescription != "") {
            taskDescription.erase(taskDescription.end()-1);
        }

		iss >> isCompleted;

		TMTaskTime taskTime;
		TMTask task(taskDescription, taskTime, taskType);
		
		if(isCompleted){
			task.setAsCompleted();
		}
        std::cout << "reached end\n";
		
		return task;
	}
}

//precondition: the string must be one of the 5 types in the exact same format
TaskType FormatConverter::convertStringToTaskType(std::string taskType){
    std::map<std::string, TaskType> taskTypeMap = boost::assign::map_list_of
        ("WithStartDateTime", TaskType::WithStartDateTime) 
        ("WithEndDateTime", TaskType::WithEndDateTime)
        ("WithPeriod", TaskType::WithPeriod)
        ("Undated", TaskType::Undated)
        ("Invalid", TaskType::Invalid);

    return taskTypeMap[taskType];
}

//precondition:: taskType is one of the 5 types
std::string FormatConverter::convertTaskTypeToString(TaskType taskType){
    std::map<TaskType, std::string> stringMap = boost::assign::map_list_of
        (TaskType::WithStartDateTime, "WithStartDateTime")
        (TaskType::WithEndDateTime, "WithEndDateTime")
        (TaskType::WithPeriod, "WithPeriod")
        (TaskType::Undated, "Undated")
        (TaskType::Invalid, "Invalid");

    return stringMap[taskType];
}