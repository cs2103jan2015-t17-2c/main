#ifndef TMSAVEAT_H
#define TMSAVEAT_H

#include "TMCommand.h"

class TMSaveAt : public TMCommand {
public:
	void execute();
	void undo();
};
#endif