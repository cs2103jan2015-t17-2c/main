//@author A0114130E
#ifndef TMINVALIDCOMMAND_H
#define TMINVALIDCOMMAND_H

#include "TMCommand.h"

class TMInvalidCommand : public TMCommand {
public:
	void execute() {
		outcome = INVALID_COMMAND_PROMPT;
	}

};
#endif