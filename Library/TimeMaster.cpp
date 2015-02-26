#include "TimeMaster.h"

int main() {

	return 0;
}

TimeMaster::TimeMaster(void)
{
}


TimeMaster::~TimeMaster(void)
{
	if(listIsNotEmpty(timedTasks)) {
		clearList(timedTasks);
	}

	if(listIsNotEmpty(floatingTasks)) {
		clearList(floatingTasks);
	}
}

bool TimeMaster::listIsNotEmpty(std::list<Task> &listOfTasks) {
	return !listOfTasks.empty();
}

void TimeMaster::clearList(std::list<Task> &listOfTasks) {
	listOfTasks.clear();
}