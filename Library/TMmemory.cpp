#include <fstream>
#include <vector>
#include <string>
#include "TMmemory.h"


using namespace std;


void TMTaskList :: addTask (TMTask task)
{
	if (task.TaskType = timed || task.TaskType = deadline)
	{
		timedAndDeadline.push_back(task);
	}
	if (task.Tasktype = floating)
	{
		floating.push_back(task);
	}
	if (task.TaskType = repeating)
	{
		repeatingTasks.push_back(task);
	}
}

void TMTaskList :: addRepeatingTasksToList ()
{
	if(task.numberOfRepeatsLeft != 0)
}

