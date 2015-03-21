#ifndef TMDELETETASKS_H
#define TMDELETETASKS_H

#include "TMCommand.h"
#include "TMTaskList.h"

class TMDeleteTasks : public TMCommand {

public:
	TMDeleteTasks() {}

	void execute() {
		/*std::vector<int>::iterator iter;
		for (iter = positionIndexes.begin(); iter != positionIndexes.end(); ++iter) {
			taskList.removeTask(*iter);
		}
		positionIndexes.clear();*/
		std::cout << "DELETE TASKS CALLED." << std::endl;
	}

	void undo() {}

};
#endif