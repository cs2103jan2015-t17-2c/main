#ifndef TMCONSTANTS_H
#define TMCONSTANTS_H

#include <string>

static const std::string CMD_ADD = "add";
static const std::string CMD_DELETE = "delete";
static const std::string CMD_UNDO = "undo";
static const std::string CMD_REDO = "redo";
static const std::string CMD_COMPLETE = "complete";
static const std::string CMD_INCOMPLETE = "incomplete";
static const std::string CMD_SEARCH = "search";
static const std::string CMD_EDIT = "edit";
static const std::string CMD_STORE = "store";
static const std::string CMD_DONEALL = "doneall";
static const std::string CMD_BLOCK = "block";
static const std::string CMD_CONFIRM = "confirm";
static const std::string CMD_QUIT = "quit";
static const std::string CMD_EXIT = "exit";
static const std::string CMD_CLOSE = "close";

static const std::string CMD_SHORTCUT_ADD = "a";
static const std::string CMD_SHORTCUT_DELETE = "d";
static const std::string CMD_SHORTCUT_UNDO = "u";
static const std::string CMD_SHORTCUT_REDO = "r";
static const std::string CMD_SHORTCUT_COMPLETE = "c";
static const std::string CMD_SHORTCUT_INCOMPLETE = "i";
static const std::string CMD_SHORTCUT_SEARCH = "se";
static const std::string CMD_SHORTCUT_EDIT= "e";
static const std::string CMD_SHORTCUT_STORE = "st";
static const std::string CMD_SHORTCUT_DONEALL = "da";
static const std::string CMD_SHORTCUT_QUIT = "q";

static const std::string DAY_YESTERDAY = "yesterday";
static const std::string DAY_TODAY = "today";
static const std::string DAY_SHORTCUT_TODAY = "tdy";
static const std::string DAY_TOMORROW = "tomorrow";
static const std::string DAY_SHORTCUT_TOMORROW = "tmr";
static const std::string DAY_MONDAY = "monday";
static const std::string DAY_TUESDAY = "tuesday";
static const std::string DAY_WEDNESDAY = "wednesday";
static const std::string DAY_THURSDAY = "thursday";
static const std::string DAY_FRIDAY = "friday";
static const std::string DAY_SATURDAY = "saturday";
static const std::string DAY_SUNDAY = "sunday";

static const std::string DAY_MON = "mon";
static const std::string DAY_TUE = "tue";
static const std::string DAY_WED = "wed";
static const std::string DAY_THU = "thu";
static const std::string DAY_FRI = "fri";
static const std::string DAY_SAT = "sat";
static const std::string DAY_SUN = "sun";

static const std::string TOKEN_BEFORE = "before";
static const std::string TOKEN_SHORTCUT_BEFORE = "bfr";
static const std::string TOKEN_BY = "by";
static const std::string TOKEN_ON = "on";
static const std::string TOKEN_NEXT = "next";
static const std::string TOKEN_SHORTCUT_NEXT = "nxt";
static const std::string TOKEN_AT = "at";
static const std::string TOKEN_SHORTCUT_AT = "@";
static const std::string TOKEN_FROM = "from";
static const std::string TOKEN_SHORTCUT_FROM = "fr";
static const std::string TOKEN_TO = "to";
static const std::string TOKEN_SHORTCUT_TO = "2";
static const std::string TOKEN_DASH = "-";

static const char DELIMITER_SLASH = '/';
static const char DELIMITER_FULLSTOP = '.';
static const char DELIMITER_DASH = '-';

/* in 3 hours, etc. for deadlined task
const std::string PERIOD_HOUR = "hour";
const std::string PERIOD_HOURS = "hours";
const std::string PERIOD_DAY = "day";
const std::string PERIOD_DAYS = "days";
const std::string PERIOD_WEEK = "week";
const std::string PERIOD_WEEKS = "weeks";
const std::string PERIOD_MONTH = "month";
const std::string PERIOD_MONTHS = "months";
const std::string PERIOD_YEAR = "year";
const std::string PERIOD_YEARS = "years";
*/
#endif