#include "TMExecutor.h"
int main () {
	while (true) {
	TMTaskListStates *taskListStates = TMTaskListStates::getInstance();
	 TMExecutor* exe = TMExecutor::getInstance();
	std::cout << "ENTER INPUT" << std::endl;
	std::string unmanaged;
	std::cin >> unmanaged;
	 exe->executeMain(unmanaged);
	}
	
	return 0;
	}