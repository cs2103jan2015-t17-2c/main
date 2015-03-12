#include <fstream>
#include <vector>
#include <string>
#include "TMmemory.h"
#include "TMTask.h"


using namespace std;


void TMTaskList :: addTask (TMTask task)
{
	
	if (task.getTaskType() == TMTask::TaskType::Timed  || task.getTaskType() == TMTask::TaskType::WithDeadline)
	{
		timedAndDeadline.push_back(task);
	}
	if (task.getTaskType() == TMTask::TaskType::Floating)
	{
		floating.push_back(task);
	}
	if (task.getTaskType() == TMTask::TaskType::Repeated)
	{
		repeatingTasks.push_back(task);
	}
}

void TMTaskList :: removeTask()
{
	int taskNumber;
	cin>>taskNumber;
	
	if (taskNumber <= timedAndDeadline.size())
	{
		timedAndDeadline.erase(timedAndDeadline.begin()+taskNumber);
	}
	else
	{
		int floatingTaskNumber = taskNumber - timedAndDeadline.size();
		floating.erase(floating.begin()+floatingTaskNumber);
	}
}

void TMTaskList :: archiveAllTasks()
{
	ofstream writeToFile;
	writeToFile.open("TMStorage.txt");
	writeToFile << "Tasks With Deadline:" << endl;
	writeToFile << timedAndDeadline.size() <<endl;
	for(int i =0; i < timedAndDeadline.size(); ++i)
	{
		writeToFile << timedAndDeadline[i].getTaskDescription() << endl << timedAndDeadline[i].getTaskTime << endl;
	}
	
	writeToFile << "Tasks Without Deadline:" <<endl;
	writeToFile <<floating.size()<<endl;
	for(int j=0; j < floating.size(); ++j)
	{
		writeToFile << floating[j].getTaskDescription() << endl << floating[j].getTaskTime << endl;
	}
	writeToFile.close();
}

void TMTaskList :: loadFromFile()
{
	ifstream readFromFile;
	string unusedLines;
	readFromFile.open("TMStorage.txt");
	getline(readFromFile,unusedLines);
	int sizeTimed;
	cin>>sizeTimed;
	for (int i=0; i < sizeTimed; ++i)
	{
		getline(readFromFile,timedAndDeadline[i].taskDescription);
		getline(readFromFile,unusedLines); //for now this function will only load task description
	}
	getline(readFromFile,unusedLines);
	int sizeFloating;
	cin>>sizeFloating;
	for (int j=0;j < sizeFloating;++j)
	{
		getline(readFromFile,floating[j].taskDescription);
		getline(readFromFile,unusedLines);
	}
}


string TMTaskList :: searchTasks (string keyword)
{
	vector<TMTask> searchResults;
	for (int i = 0; i<timedAndDeadline.size();++i){
		if(timedAndDeadline[i].getTaskDescription() == keyword)
		{searchResults.push_back(timedAndDeadline[i]);}
	}
	for (int j = 0; j<floating.size();++j){
		if(floating[j].getTaskDescription() == keyword)
		{searchResults.push_back(floating[j]);}
	}
	
	for (int k=0; k<searchResults.size();++k){
		printEntry(searchResults); //UI
	}
}