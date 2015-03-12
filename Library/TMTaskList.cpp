#include <fstream>
#include <vector>
#include <algorithm>
#include <string>
#include "TMTaskList.h"
#include "TMTask.h"
#include "TMParser.h"


using namespace std;


void TMTaskList :: addTask (TMTask task)
{
	
	if (task.getTaskType() == TaskType::Timed  || task.getTaskType() == TaskType::WithDeadline)
	{
		timedAndDeadline.push_back(task);
	}
	if (task.getTaskType() == TaskType::Floating)
	{
		floating.push_back(task);
	}
}

void TMTaskList :: removeTask(int positionIndex)
{	
	if (positionIndex <= timedAndDeadline.size())
	{
		timedAndDeadline.erase(timedAndDeadline.begin() + positionIndex);
	}
	else
	{
		int floatingTaskNumber = positionIndex - timedAndDeadline.size();
		floating.erase(floating.begin() + floatingTaskNumber);
	}
}

/*void TMTaskList :: archiveAllTasks()
{
	ofstream writeToFile;
	writeToFile.open("TMStorage.txt");
	writeToFile << "Tasks With Deadline:" << endl;
	writeToFile << timedAndDeadline.size() <<endl;
	for(int i =0; i < timedAndDeadline.size(); ++i)
	{
		writeToFile << timedAndDeadline[i].getTaskDescription() << " " << timedAndDeadline[i].getTaskTime().getStartDate() << 
			" " << timedAndDeadline[i].getTaskTime().getStartTime() << 
			" " << timedAndDeadline[i].getTaskTime().getEndDate() <<
			" " << timedAndDeadline[i].getTaskTime().getEndTime() <<endl;
	}
	
	writeToFile << "Tasks Without Deadline:" <<endl;
	writeToFile <<floating.size()<<endl;
	for(int j=0; j < floating.size(); ++j)
	{
		writeToFile << floating[j].getTaskDescription() << endl;
	}
	writeToFile.close();
}

void TMTaskList :: loadFromFile()
{
	ifstream readFromFile;
	string unusedLines;
	string entryTimedAndDeadline;
	string entryFloating;
	TMParser parseTimedAndDeadline,parseFloating;
	readFromFile.open("TMStorage.txt");
	getline(readFromFile,unusedLines);
	int sizeTimed;
	cin>>sizeTimed;
	for (int i=0; i < sizeTimed; ++i)
	{
		getline(readFromFile,entryTimedAndDeadline);
		parseTimedAndDeadline.parseTaskInfo(entryTimedAndDeadline);
	}
	getline(readFromFile,unusedLines);
	int sizeFloating;
	cin>>sizeFloating;
	for (int j=0;j < sizeFloating;++j)
	{
		getline(readFromFile,entryFloating);
		parseFloating.parseTaskInfo(entryFloating);
	}
}
*/

string TMTaskList :: searchTasks(string keyword)
{
	vector<TMTask> searchResults;
	int positionTimed, positionFloating;
	//Search for keyword in timedAndDeadline vector
	for (int i = 0; i<timedAndDeadline.size(); ++i) {
		positionTimed = timedAndDeadline[i].getTaskDescription().find(keyword);
		if (positionTimed != string::npos) {
			searchResults.push_back(timedAndDeadline[i]);
		}
	}

	//Search for keyword in floating vector
	for (int j = 0; j<floating.size(); ++j) {
		positionFloating = floating[j].getTaskDescription().find(keyword);
		if (positionFloating != string::npos) {
			searchResults.push_back(floating[j]);
		}
	}
	
	for (int k=0; k<searchResults.size(); ++k) {
		cout << searchResults[k].getTaskDescription() << " " 
		<< searchResults[k].getTaskTime().getStartDate() << " "
		<< searchResults[k].getTaskTime().getStartTime() << " " 
		<< searchResults[k].getTaskTime().getEndDate() << " " 
		<< searchResults[k].getTaskTime().getEndTime() <<endl;
	}
}