#ifndef TMVIEWFREETIME_H
#define TMVIEWFREETIME_H

#include "TMCommand.h"
#include "TMUserInterface.h"

class TMViewFreeTime : public TMCommand {
public:
	void execute() {
		TMUserInterface *ui = TMUserInterface::getInstance();
		ui->displayFreeTime();
	}
	void undo();
};

#endif
		