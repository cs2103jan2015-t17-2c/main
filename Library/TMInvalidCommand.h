#ifndef TMINVALIDCOMMAND_H
#define TMINVALIDCOMMAND_H

#include "TMCommand.h"


const std::string INVALID_COMMAND_PROMPT = "The command you have entered is invalid, please enter a valid one.";

class TMInvalidCommand : public TMCommand {
public:
	void execute() {
		outcome = INVALID_COMMAND_PROMPT;
	}

};
#endif