#include "TMTaskList.h"



	TMTaskList::TMTaskList() {
		
		}


	//LOGIC FUNCTIONS//
	bool TMTaskList::areEquivalent(TMTask task1, TMTask task2) {
		if (task1.getTaskDescription() != task2.getTaskDescription()) {
			return false;
		}

		if (task1.getTaskTime().getStartDate() != task2.getTaskTime().getStartDate()) { //TO IMPLEMENT FUNCTION TO COMPARE TASKTIME
			return false;
		}
		if (task1.getTaskTime().getStartTime() != task2.getTaskTime().getStartTime()) { //TO IMPLEMENT FUNCTION TO COMPARE TASKTIME
			return false;
		}
		if (task1.getTaskTime().getEndDate() != task2.getTaskTime().getEndDate()) { //TO IMPLEMENT FUNCTION TO COMPARE TASKTIME
			return false;
		}
		if (task1.getTaskTime().getEndTime() != task2.getTaskTime().getEndTime()) { //TO IMPLEMENT FUNCTION TO COMPARE TASKTIME
			return false;
		}
		if (task1.isCompleted() != task2.isCompleted()) {
			return false;
		}

		if (task1.isConfirmed() != task2.isConfirmed()) {
			return false;
		}

		if (task1.getTaskType() != task2.getTaskType()) {
			return false;
		}
	
		return true;
	}

	bool TMTaskList::hasClash(TMTask task) { //REVISIT CODE AND CHECK FOR BOUNDARY VALUES
		std::vector<TMTask>::iterator iter;
		TMDateTime start = task.getTaskTime().getStartDateTime();
		
		for (iter = _dated.begin(); iter != _dated.end(); ++iter) {
			if (startsBeforeTime(*iter, start) && iter->getTaskType() == TaskType::WithPeriod) {
				if (isTwoClash(*iter, task)) {
					return true;
				}
			} else if (iter->getTaskType() == TaskType::WithPeriod) {
				if (isTwoClash(task, *iter)) {
					return true;
				}
			}
		}	
	return false;
	}

	bool TMTaskList::isTwoClash(TMTask task1, TMTask task2) { //REVISIT CODE
		return startsBeforeTime(task2, task1.getTaskTime().getEndDateTime());
		return true;
	}

	bool TMTaskList::startsBeforeTime(TMTask task, TMDateTime time) { //REVISIT CODE
		TMDateTime dateTime = task.getTaskTime().getStartDateTime();
		return isBefore(dateTime, time);
	} 

	bool TMTaskList::endsBeforeTime(TMTask task, TMDateTime time) {
		TMDateTime dateTime = task.getTaskTime().getEndDateTime();
		return isBefore(dateTime, time);
	} //REVISIT CODE

	bool TMTaskList::isBefore(TMDateTime time1, TMDateTime time2) { //REVISIT CODE
		if (time1.getBoostDate() > time2.getBoostDate()) {
			return false;
		} else if (time1.getBoostDate() == time2.getBoostDate()) {
			if (time1.getTime() >= time2.getTime()) {
				return false;
			}
		}

		return true;
	}

	bool TMTaskList::areEquivalentDateTime(TMDateTime time1, TMDateTime time2) {
		if (time1.getDate() != time2.getDate()) {
			return false;
		} else if (time1.getTime() != time2.getTime()) {
			return false;
		}

		return true;
	}

	bool TMTaskList::isValidPositionIndex(int positionIndex) {
		return (positionIndex > 0 && positionIndex <= int(_dated.size() + _undated.size() + _archived.size()));
	}

	bool TMTaskList::isInDated(int positionIndex) {
		return (positionIndex <= int(_dated.size()));
	}

	bool TMTaskList::isInUndated(int positionIndex) {
		return (positionIndex > int(_dated.size()) && positionIndex <= int(_dated.size() + _undated.size()));
	}

	bool TMTaskList::isInArchived(int positionIndex) {
		return (positionIndex > int(_dated.size() + _undated.size()) && positionIndex <= int(_dated.size() + _undated.size() + _archived.size()));
	}

	bool TMTaskList::isUniqueBatchNum(int i) {
		std::vector<TMTask>::iterator iter;
		for (iter = _dated.begin(); iter != _dated.end(); ++iter) {
			if (iter->getUnconfirmedBatchNumber() == i) {
				return false;
			}
		}
		return true;
	}

	void TMTaskList::setClashes(TMTask task, std::vector<TMTask>::iterator beginFrom) { //REVISIT CODE AND CHECK FOR BOUNDARY VALUES
		_clashes.clear();
		std::vector<TMTask>::iterator iter;
		TMDateTime start = task.getTaskTime().getStartDateTime();
		
		for (iter = beginFrom; iter != _dated.end(); ++iter) {
			TMTask &registeredTask = *iter;
			if (startsBeforeTime(registeredTask, start) && iter->getTaskType() == TaskType::WithPeriod) {
				if (isTwoClash(registeredTask, task)) {
					registeredTask.setAsClashed();
					_clashes.push_back(registeredTask);
				}
			} else if (isTwoClash(task, registeredTask) && iter->getTaskType() == TaskType::WithPeriod) {
					registeredTask.setAsClashed();
					_clashes.push_back(registeredTask);
			}
			
		}

	}

	void  TMTaskList::updateClashes(TMTask deleteTask) {
		setClashes(deleteTask, _dated.begin());
		std::vector<TMTask>::iterator iter;
		int i = 0;
		for (iter = _dated.begin(); iter != _dated.end() && i < int(_clashes.size()); ++iter) {
			if (areEquivalent(*iter, _clashes[i])) {
				iter->setAsUnclashed();
			}
			++i;
		}
	

		for (iter = _dated.begin(); iter != _dated.end(); ++iter) {
			TMTask &focusTask = *iter;
			if (iter+1 != _dated.end()) {
				setClashes(focusTask, iter+1);
				if (int(_clashes.size()) != 0) {
					iter->setAsClashed();
				}
			}
		}

	}

	std::vector<TMTask>::iterator TMTaskList::findEarliestTaskIter(std::vector<TMTask>::iterator unsortedStart) {
									TMTask earliestTask = *unsortedStart;
									std::vector<TMTask>::iterator earliestTaskIter;
									earliestTaskIter = unsortedStart;
									std::vector<TMTask>::iterator iter;
		
									for (iter = unsortedStart; iter != _dated.end(); ++iter) {
				
										if (startsBeforeTime(*iter, earliestTask.getTaskTime().getStartDateTime())) {
											earliestTask = *iter;
											earliestTaskIter = iter;
										} else if (areEquivalentDateTime(iter->getTaskTime().getStartDateTime(), earliestTask.getTaskTime().getStartDateTime())) {
											if ( isBefore(iter->getTaskTime().getEndDateTime(), earliestTask.getTaskTime().getEndDateTime()) ) {
												earliestTask = *iter;
												earliestTaskIter = iter;
											}
										}
									}

									return earliestTaskIter;
	}

	std::vector<TMTask>::iterator TMTaskList::findSmallestAlphaTaskIter(std::vector<TMTask>::iterator unsortedStart) {
									TMTask smallestAlphaTask = *unsortedStart;
									std::vector<TMTask>::iterator smallestAlphaTaskIter;
									smallestAlphaTaskIter = unsortedStart;
									std::vector<TMTask>::iterator iter;
		
									for (iter = unsortedStart; iter != _undated.end(); ++iter) {
										if (iter->getTaskDescription() < smallestAlphaTaskIter->getTaskDescription()) {
											smallestAlphaTask = *iter;
											smallestAlphaTaskIter = iter;
										}
									}

									return smallestAlphaTaskIter;
	}

	std::string TMTaskList::toLower(std::string toBeConverted) {
		std::string aCopy = toBeConverted;
		transform(aCopy.begin(), aCopy.end(), aCopy.begin(), ::tolower);
		return aCopy;
	}

	std::vector<int> TMTaskList::searchUnconfirmedBatchNum(int i) {
		std::vector<TMTask>::iterator iter;
		std::vector<int> results;

		for (iter = _dated.begin(); iter != _dated.end(); ++iter) {
			if (iter->getUnconfirmedBatchNumber() == i && !iter->isConfirmed()) {
				results.push_back(getPositionIndexFromTask(*iter));
			}
		}
		return results;
	}

	


	//GETTER FUNCTIONS//
	int TMTaskList::getPositionIndexFromTask(TMTask task) {
		std::vector<TMTask>::iterator iter;
		int positionIndex = 1;
		
		for (iter = _dated.begin(); iter != _dated.end(); ++iter) {
			if (areEquivalent(task, *iter)) {
				return positionIndex;
			}
			positionIndex++;
		}
		
		for (iter = _undated.begin(); iter != _undated.end(); ++iter) {
			if (areEquivalent(task, *iter)) {
				return positionIndex;
			}
			positionIndex++;
		}

		for (iter = _archived.begin(); iter != _archived.end(); ++iter) {
			if (areEquivalent(task, *iter)) {
				return positionIndex;
			}
			positionIndex++;
		}

		return 0;
	}	

	TMTask TMTaskList::getTaskFromPositionIndex(int positionIndex) {
		assert(isValidPositionIndex(positionIndex));

		TMTaskTime taskTime;
		TMTask invalidTask("No such task", taskTime, TaskType::Invalid);

		if (isInDated(positionIndex)) {
			return _dated[positionIndex - 1];
		}

		if (isInUndated(positionIndex)) {
			return _undated[positionIndex - int(_dated.size()) - 1];
		}

		if (isInArchived(positionIndex)) {
			return _archived[positionIndex - int(_dated.size()) - int(_undated.size()) - 1];
		}
		
		return invalidTask;
	}

	int TMTaskList::getDatedSize() {
		return _dated.size();
	}

	int TMTaskList::getUndatedSize() {
		return _undated.size();
	}

	int TMTaskList::getArchivedSize() {
		return _archived.size();
	}

	std::vector<TMTask> TMTaskList::getDated() {
		return _dated;
	}
	
	std::vector<TMTask> TMTaskList::getUndated() {
		return _undated;
	}
	
	std::vector<TMTask> TMTaskList::getArchived() {
		return _archived;
	}

	int TMTaskList::generateUniqueBatchNum() {
		std::vector<TMTask>::iterator iter;
		int i;
		for (i = 1; !isUniqueBatchNum(i); ++i) {
			}

		return i;
	}


	//BASIC FUNCTIONS//
	void TMTaskList::addTask (TMTask &task) {
		TaskType type = task.getTaskType();
		std::string outcome;
		switch (type) {
			
		case WithStartDateTime:
			_dated.push_back(task);
			chronoSort();
			break;

		case WithEndDateTime:
			_dated.push_back(task);
			chronoSort();
			break;
		
		case WithPeriod:
			if (int(_dated.size()) != 0 && hasClash(task)) {
				setClashes(task, _dated.begin());
				task.setAsClashed();
			} 
			_dated.push_back(task);
			chronoSort();
			break;

		case Undated: 
			_undated.push_back(task);
			alphaSort();
			break;

		case Invalid:
			break;
		}

		return;
	}

	
	void  TMTaskList::updateTask(int positionIndex, TMTask &alteredTask) {
		removeTask(positionIndex);
		addTask(alteredTask);
		return;
	}

	void  TMTaskList::removeTask(int positionIndex) {	
		
		if (isInDated(positionIndex)) {
			TMTask deleteTask = getTaskFromPositionIndex(positionIndex);
			_dated.erase(_dated.begin() + positionIndex - 1);
			if (deleteTask.getTaskType() == TaskType::WithPeriod) {
				updateClashes(deleteTask);
			}
			return;

		} else if (isInUndated(positionIndex)) {
			int floatingTaskNumber = positionIndex - _dated.size();
			_undated.erase(_undated.begin() + floatingTaskNumber - 1);
			return;

		} else if (isInArchived(positionIndex)) {
			int archivedTaskNumber = positionIndex - _dated.size() - _undated.size();
			_archived.erase(_archived.begin() + archivedTaskNumber - 1);
			return;
		}

		return;
	}
	
	void  TMTaskList::archiveOneTask(int positionIndex) {
		TMTask &task = getTaskFromPositionIndex(positionIndex);
		task.setAsCompleted();
		_archived.push_back(task);
		removeTask(positionIndex);
		return;
	}

	void TMTaskList::chronoSort() {
		std::vector<TMTask>::iterator iter;
		for (iter = _dated.begin(); iter != _dated.end(); ++iter) {
			std::iter_swap(iter, findEarliestTaskIter(iter));
		}
	}

	void TMTaskList::alphaSort() {
		std::vector<TMTask>::iterator iter;
		for (iter = _undated.begin(); iter != _undated.end(); ++iter) {
			std::iter_swap(iter, findSmallestAlphaTaskIter(iter));
		}
	}	

	std::vector<int> TMTaskList::keywordSearch(std::string keyword) {
		std::string lowerKeyword = toLower(keyword);
		
		std::vector<std::string> taskDescInLower;
		std::vector<TMTask>::iterator iter;
		std::vector<int> searchResults;
		int posIndexCounter = 1;

		for (iter = _dated.begin(); iter != _dated.end(); ++iter) {
			taskDescInLower.push_back(toLower((*iter).getTaskDescription()));
		}
		for (iter = _undated.begin(); iter != _undated.end(); ++iter) {
			taskDescInLower.push_back(toLower((*iter).getTaskDescription()));
		}
		for (iter = _archived.begin(); iter != _archived.end(); ++iter) {
			taskDescInLower.push_back(toLower((*iter).getTaskDescription()));
		}

		std::vector<std::string>::iterator iterForLower;
		for (iterForLower = taskDescInLower.begin(); iterForLower != taskDescInLower.end(); ++iterForLower) {
			std::string searchLine = *iterForLower;
			if ( std::search(searchLine.begin(), searchLine.end(), lowerKeyword.begin(), lowerKeyword.end()) != searchLine.end() ) {
				searchResults.push_back(posIndexCounter);
			}
			posIndexCounter++;
		}
	
		return searchResults;
	}



	//EXPORT AND IMPORT FUNCTIONS
	
	bool TMTaskList::isFoundInLine(std::string text, std::string line) {
		bool result = (std::search(line.begin(), line.end(), text.begin(), text.end()) != line.end());
		std::cout << "RESULT: " << result << std::endl;
		return std::search(line.begin(), line.end(), text.begin(), text.end()) != line.end();
	}
	
	bool TMTaskList::isValidDirectory(const char* directory) {
		DWORD attributes = GetFileAttributesA(directory);
		if (attributes == INVALID_FILE_ATTRIBUTES) {
			return false;
		}
		
		return (attributes & FILE_ATTRIBUTE_DIRECTORY);
	}

	void TMTaskList::loadOrCreateFile() {
		std::string directoryName;
		std::ifstream directoryReference("TimeMaster.txt");
		getline(directoryReference, directoryName);
		setDefaultFileName();
		std::string pathName = directoryName + "\\" + _fileName;
		if (directoryName != "" && isValidDirectory(pathName.c_str())) {
			setDirectoryName(directoryName);
			loadFromFile();
			
		} else {
			setDirectoryName(getExePath());
			createFile();
		}
		return;
	}

	void TMTaskList::setDirectoryName(std::string directory) {
		_directoryName = directory;
	}

	void TMTaskList::setDefaultFileName() {
		boost::gregorian::date dateToday =  boost::gregorian::day_clock::local_day();
		std::locale facet(std::locale::classic(), new boost::gregorian::date_facet("%d %b %Y"));
		std::ostringstream stream;
		stream.imbue(facet);
		std::string  strDateToday;
		stream << dateToday;
		strDateToday = stream.str();
		std::string year, defaultFileName;
		
		year = strDateToday.substr(strDateToday.size()-4, 4);
		defaultFileName = "TM " + year + " Schedule.txt"; 
		_fileName = defaultFileName; 
	}

	std::string TMTaskList::getDirectoryName() {
		return _directoryName;
	}

	std::string TMTaskList::getFileName() {
		return _fileName;
	}

	std::string TMTaskList::getExeFileName() {
		char buffer[MAX_PATH];
		GetModuleFileName( NULL, buffer, MAX_PATH );
		return std::string(buffer);
	}

	std::string TMTaskList::getExePath() {
		std::string exeDirectoryAndFileName = getExeFileName();
		return exeDirectoryAndFileName.substr(0, exeDirectoryAndFileName.find_last_of( "\\/" ));
	}

	void TMTaskList::writeToFile() {
		std::ofstream outFile;
		std::vector<TMTask>::iterator iter;
		std::string fileDirectory = _directoryName + "\\" + _fileName;
		outFile.open(fileDirectory);
	
		outFile << DATED_HEADER  << _dated.size() << "\n";
		outFile << DATED_TASK_DISPLAY_FORMAT << "\n";
		for (iter = _dated.begin(); iter != _dated.end(); ++iter) {
			outFile << iter->getTaskTypeAsString() <<
			" "	<< iter->getTaskDescription() << " ||" 
			" " << iter->getTaskTime().getStartDate() << 
			" " << iter->getTaskTime().getStartTime() << 
			" " << iter->getTaskTime().getEndDate() <<
			" " << iter->getTaskTime().getEndTime() << 
			" " << iter->isCompleted() << 
			" " << iter->isClashed() << 
			" " << iter->isConfirmed() << 
			" " << iter->getUnconfirmedBatchNumber() << "\n";
		}

		//outFile << "\n";
	
		outFile << UNDATED_HEADER << _undated.size() << "\n";
		outFile << UNDATED_TASK_DISPLAY_FORMAT << "\n";
		for (iter = _undated.begin(); iter != _undated.end(); ++iter) {
			outFile << iter->getTaskTypeAsString() << 
				" "	<< iter->getTaskDescription() << " ||" 
				" " << iter->isCompleted() << "\n";
		}

		//outFile << "\n";

		outFile << ARCHIVED_HEADER << _archived.size() << "\n";
		for (iter = _archived.begin(); iter != _archived.end(); iter++) {
			
			if (iter->getTaskType() != TaskType::Undated) {
				outFile << iter->getTaskTypeAsString() <<
				" "	<< iter->getTaskDescription() << " ||" 
				" " << iter->getTaskTime().getStartDate() << 
				" " << iter->getTaskTime().getStartTime() << 
				" " << iter->getTaskTime().getEndDate() <<
				" " << iter->getTaskTime().getEndTime() << 
				" " << iter->isCompleted() << 
				" " << iter->isClashed() << 
				" " << iter->isConfirmed() << 
				" " << iter->getUnconfirmedBatchNumber() << "\n";
			} else {
				outFile << iter->getTaskTypeAsString() << 
				" "	<< iter->getTaskDescription() << " ||" 
				" " << iter->isCompleted() << "\n";
			}
		}

		outFile.close();
	}

	void TMTaskList::createFile() {
		std::string pathName = _directoryName + "\\" + _fileName;
		std::ofstream outFile;
		outFile.open(pathName);
		outFile.close();
	}

	void TMTaskList::loadFromFile() {
		std::string pathName = _directoryName + "\\" + _fileName;
		std::ifstream contentReference(pathName);
		std::vector<std::string> linesFromFile;
		std::string line;


		while (getline(contentReference, line)) {
			if (line != "\n") {
				linesFromFile.push_back(line);
			}
		}
		
		contentReference.close();

		std::vector<std::string>::iterator iter;
		FormatConverter* converter = FormatConverter::getInstance();

		

		//Dated tasks
		iter = linesFromFile.begin() + 2;	//Skips first two irrelevant lines in the text file.
		while (!isFoundInLine(UNDATED_HEADER, *iter)) {
			TMTask task = converter->convertStringToTMTask(*iter);
			addTask(task);
			iter++;
		}

		//Undated tasks
		iter = iter + 2;
		while (!isFoundInLine(ARCHIVED_HEADER, *iter)) {
			TMTask task = converter->convertStringToTMTask(*iter);
			addTask(task);
			iter++;
		}


		//Completed tasks
		iter++;
		while (iter != linesFromFile.end()) {
			TMTask task = converter->convertStringToTMTask(*iter);
			_archived.push_back(task);
			iter++;
		}

		return;
	} 

	void TMTaskList::leaveReferenceUponExit() {
		std::ofstream outFile;
		outFile.open("TimeMaster.txt");
		outFile << _directoryName << '\n';
		outFile << USER_INFO_TIMEMASTER_FILE << '\n';
		outFile.close();
		}


	