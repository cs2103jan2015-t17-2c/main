#include "TMTask.h"
#include "TMRecurringTask.h"
#include "TMTaskTime.h"

class TMParser {
private:
    enum CommandTypes {
        Add, Delete, Undo, Complete, Incomplete, Search, Edit, Store
    };

    TMParser::CommandTypes _commandType;

public:
    std::string extractCommand();


    CommandTypes determineCommand(std::string);

};