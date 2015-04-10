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
static const std::string CMD_VIEW_DEFAULT = "viewd";
static const std::string CMD_VIEW_DEADLINE = "viewdd";
static const std::string CMD_VIEW_UNDATED = "viewu";
static const std::string CMD_VIEW_ARCHIVED = "viewa";
static const std::string CMD_VIEW_SEARCH = "views";

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
static const std::string CMD_SHORTCUT_VIEW_DEFAULT = "vd";
static const std::string CMD_SHORTCUT_VIEW_DEADLINE = "vdd";
static const std::string CMD_SHORTCUT_VIEW_UNDATED = "vu";
static const std::string CMD_SHORTCUT_VIEW_ARCHIVED = "va";
static const std::string CMD_SHORTCUT_VIEW_SEARCH = "vs";

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

static const std::string STATUS_DISPLAY_INDEXES = "Tasks ";
static const std::string STATUS_DISPLAY_INVALID_INDEXES = " is/are invalid position indexe(s).";
static const std::string STATUS_DISPLAY_NEW_INDEX = "New task ";
static const std::string ADD_SDT_SUCCESS = " (with start date and time) successfully added.";
static const std::string ADD_EDT_SUCCESS = " (with end date and time/deadline) successfully added.";
static const std::string ADD_PERIOD_SUCCESS = " (with period) successfully added.";
static const std::string ADD_UNDATED_SUCCESS = " (undated) successfully added.";
static const std::string ADD_INVALID = "The task you have specified has invalid component(s). Please specify a valid task.";
static const std::string CLASH_WARNING = " has clash(es) with tasks on hand.\nTasks involved have been highlighted in blue.";
static const std::string BLOCK_SUCCESS = " have been blocked.";
static const std::string BLOCK_FAILURE = "Number of tasks with invalid timings: ";
static const std::string ARCHIVE_SUCCESS = " tasks(s) successfully marked as completed and archived.";
static const std::string CONFIRM_SUCCESS = " is/are confirmed.";
static const std::string DELETE_SUCCESS = " tasks successfully deleted.";
static const std::string EDIT_SUCCESS = "Successfully edited task.";
static const std::string INVALID_COMMAND_PROMPT = "The command you have entered is invalid, please enter a valid one.";
static const std::string CURRENT_DIRECTORY_NAME = "Current file directory is now at: ";
static const std::string CURRENT_FILE_NAME = "File name: ";
static const std::string INVAILD_DIRECTORY_SPECIFIED = "The directory you have specified is invalid";
static const std::string SEARCH_CONCLUSION_KEYWORD_PART1 = "Number of tasks containing the keyword <";
static const std::string SEARCH_CONCLUSION_KEYWORD_PART2 = "> is: ";
static const std::string UNCOMPLETE_SUCCESS =" tasks successfully marked as uncomplete.";
static const std::string UNDO_SUCCESS = "Undo last action successfully.";
static const std::string UNDO_FAILURE = "Initial state of the system is reached. Unable to undo.";
static const std::string REDO_SUCCESS = "Redo successfully.";
static const std::string REDO_FAILURE = "Latest state of the system is reached. Unable to redo.";
static const std::string WARNING_REPEATED_INDEXES_SPECIFIED = "Please do not specify an index more than once.";

static const std::string DATED_TASK_DISPLAY_FORMAT = "<Task Type> <Task Description> <Start Date> <Start Time> <End Date> <End Time> <Completion> <Clash> <Confirmation> <Unconfirmed Batch Number>";
static const std::string UNDATED_TASK_DISPLAY_FORMAT = "<Task Type> <Task Description> <Completion>";
static const std::string DATED_HEADER = "Number of dated tasks: ";
static const std::string UNDATED_HEADER = "Number of undated tasks: ";
static const std::string ARCHIVED_HEADER = "Number of completed/archived tasks: ";
static const std::string LOAD_SUCCESS = "Database loaded successfully.";
static const std::string USER_INFO_TIMEMASTER_FILE = "This file directs the program where to load existing data from. Please do not delete.";

#endif