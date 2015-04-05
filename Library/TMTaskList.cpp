#include "TMTaskList.h"

const std::string ADD_SDT_SUCCESS = "Successfully added a task with start date and time!";
const std::string ADD_EDT_SUCCESS = "Successfully added a task with end date and time!";
const std::string ADD_PERIOD_SUCCESS = "Successfully added a period task!";
const std::string ADD_UNDATED_SUCCESS = "Successfully added an undated task!";
const std::string ADD_INVALID = "Task you have specified has invalid component(s). Please specify a valid task.";
const std::string CLASH_WARNING = "Task added has clashes with tasks on hand.\nInvolved tasks have been highlighted in blue.";
const std::string UPDATE_SUCCESS = "Task successfully edited.";
const std::string UPDATE_TIME_SUCCESS = "Task time successfully changed.";
const std::string DELETE_SUCCESS = "Task successfully removed from database!";
const std::string ARCHIVE_SUCCESS = "Task is successfully completed and archived.";
const std::string ARCHIVE_FAILURE = "Cannot archive an already archived task.";
const std::string INVALID_INDEX = "Invalid index specified to be archived.";
const std::string DATED_TASK_DISPLAY_FORMAT = "<Task Type> <Task Description> <Start Date> <Start Time> <End Date> <End Time> <Completion> <Clash> <Confirmation> <Unconfirmed Batch Number>";
const std::string UNDATED_TASK_DISPLAY_FORMAT = "<Task Type> <Task Description> <Completion>";
const std::string DATED_HEADER = "Number of dated tasks: ";
const std::string UNDATED_HEADER = "Number of undated tasks: ";
const std::string ARCHIVED_HEADER = "Number of completed/archived tasks: ";
const std::string LOAD_SUCCESS = "Database loaded successfully.";
const std::string USER_INFO_TIMEMASTER_FILE = "This file directs the program where to load existing data from. Please do not delete.";

	TMTaskList::TMTaskList() {
		boost::gregorian::date dateToday =  boost::gregorian::day_clock::local_day();
		std::locale facet(std::locale::classic(), new boost::gregorian::date_facet("%d %b %Y"));
		std::ostringstream stream;
		stream.imbue(facet);
		std::string  strDateToday;
		stream << dateToday;
		strDateToday = stream.str();
		std::string year, defaultFileName;
		
		year = strDateToday.substr(strDateToday.size()-4, 4);
		defaultFileName = year + " Schedule.txt"; 
		_fileName = defaultFileName; 
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
		return (positionIndex <= int(_dated.size() + _undated.size()));
	}

	bool TMTaskList::isInArchived(int positionIndex) {
		return (positionIndex <= int(_dated.size() + _undated.size() + _archived.size()));
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
	std::string TMTaskList::addTask (TMTask task) {
		TaskType type = task.getTaskType();
		std::string outcome;
		switch (type) {
			
		case WithStartDateTime:
			_dated.push_back(task);
			chronoSort();
			return ADD_SDT_SUCCESS;
			break;

		case WithEndDateTime:
			_dated.push_back(task);
			chronoSort();
			return ADD_EDT_SUCCESS;
			break;
		
		case WithPeriod:
			if (int(_dated.size()) != 0 && hasClash(task)) {
				setClashes(task, _dated.begin());
				task.setAsClashed();
				outcome = CLASH_WARNING;
			} else {
				outcome = ADD_PERIOD_SUCCESS;
			}

			_dated.push_back(task);
			chronoSort();
			return outcome;
			break;

		case Undated: 
			_undated.push_back(task);
			alphaSort();
			return ADD_UNDATED_SUCCESS;
			break;

		case Invalid:
			return ADD_INVALID;
			break;
		}

		return ADD_INVALID;
	}

	
	std::string TMTaskList::updateTask(int positionIndex, TMTask alteredTask) {
		
		if (isValidPositionIndex(positionIndex)) {
			removeTask(positionIndex);
			addTask(alteredTask);
			return UPDATE_SUCCESS;
		} else {
			return INVALID_INDEX;
		}
	}

	std::string TMTaskList::removeTask(int positionIndex) {	
		
		if (isInDated(positionIndex)) {
			TMTask deleteTask = getTaskFromPositionIndex(positionIndex);
			_dated.erase(_dated.begin() + positionIndex - 1);
			if (deleteTask.getTaskType() == TaskType::WithPeriod) {
				updateClashes(deleteTask);
			}
			return DELETE_SUCCESS;

		} else if (isInUndated(positionIndex)) {
			int floatingTaskNumber = positionIndex - _dated.size();
			_undated.erase(_undated.begin() + floatingTaskNumber - 1);
			return DELETE_SUCCESS;

		} else if (isInArchived(positionIndex)) {
			int archivedTaskNumber = positionIndex - _dated.size() - _undated.size();
			_archived.erase(_archived.begin() + archivedTaskNumber - 1);
			return DELETE_SUCCESS;
		}

		return INVALID_INDEX;
	}

	std::string TMTaskList::archiveOneTask(int positionIndex) {
		TMTask &task = getTaskFromPositionIndex(positionIndex);
		task.setAsCompleted();
		_archived.push_back(task);
		
		if (isInDated(positionIndex) || isInUndated(positionIndex)) {
			std::string str = removeTask(positionIndex);
			return ARCHIVE_SUCCESS;
		} else if (isInArchived(positionIndex)) {
			return ARCHIVE_FAILURE;
		}

		return INVALID_INDEX;
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
	void TMTaskList::writeToFile() {
		std::ofstream outFile;
		std::vector<TMTask>::iterator iter;
		if (_directoryName == "") {
			outFile.open(_fileName);
		} else {
			std::string fileDirectory = _directoryName + "\\" + _fileName;
			outFile.open(fileDirectory);
		}
	

		outFile << DATED_HEADER  << _dated.size() << "\n";
		outFile << DATED_TASK_DISPLAY_FORMAT << "\n";
		for (iter = _dated.begin(); iter != _dated.end(); ++iter) {
			outFile << iter->getTaskTypeAsString() <<
			" "	<< iter->getTaskDescription() << "||" 
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
				" "	<< iter->getTaskDescription() << "||" 
				" " << iter->isCompleted() << "\n";
		}

		//outFile << "\n";

		outFile << ARCHIVED_HEADER << _archived.size() << "\n";
		for (iter = _archived.begin(); iter != _archived.end(); iter++) {
			
			if (iter->getTaskType() != TaskType::Undated) {
				outFile << iter->getTaskTypeAsString() <<
				" "	<< iter->getTaskDescription() << "||" 
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
				" "	<< iter->getTaskDescription() << "||" 
				" " << iter->isCompleted() << "\n";
			}
		}

		outFile.close();
	}

	std::string TMTaskList::loadFromFile(std::string pathName) {

		std::ifstream contentReference(pathName);
		std::vector<std::string> linesFromFile;
		std::string line;


		while (getline(contentReference, line)) {
			if (line != "\n") {
				linesFromFile.push_back(line);
			}
		}
		std::cout << "SIZE OF LINESFROMFILE: " << linesFromFile.size() << std::endl;
		contentReference.close();

		std::vector<std::string>::iterator iter;
		FormatConverter* converter = FormatConverter::getInstance();

		//std::search(searchLine.begin(), searchLine.end(), keyword.begin(), keyword.end()) != searchLine.end()

		//Removes first two irrelevant lines in the text file.
		iter = linesFromFile.erase(linesFromFile.begin());
		iter = linesFromFile.erase(linesFromFile.begin());
		std::cout << "SIZE OF LINESFROMFILE: " << linesFromFile.size() << std::endl;
		std::cout << "BEGIN: " << *(linesFromFile.begin()) << std::endl;

		//Assumes first line in saved text file is as unmodified by user.
		//Loop stops when M2 is detected
		//Dated tasks
		for (iter = linesFromFile.begin(); !isFoundInLine(UNDATED_HEADER, *iter); ++iter) {
			std::cout << "hi" << std::endl;
			TMTask task = converter->convertStringToTMTask(*iter);
			_dated.push_back(task);
		}

		//Undated tasks
		iter = linesFromFile.erase(linesFromFile.begin());
		iter = linesFromFile.erase(linesFromFile.begin());
		std::cout << "SIZE OF LINESFROMFILE: " << linesFromFile.size() << std::endl;
		std::cout << "BEGIN: " << *(linesFromFile.begin()) << std::endl;

		while (!isFoundInLine(ARCHIVED_HEADER, *iter)) {
			std::cout << "hi" << std::endl;
			TMTask task = converter->convertStringToTMTask(*iter);
			_undated.push_back(task);
			++iter;
		}

		//Completed tasks
		iter = linesFromFile.erase(linesFromFile.begin());

		while (iter != linesFromFile.end()) {
			TMTask task = converter->convertStringToTMTask(*iter);
			_archived.push_back(task);
			++iter;
		}

		return LOAD_SUCCESS;
	} 

	void TMTaskList::setFileDirectory(std::string directory) {
		_directoryName = directory;
	}

	std::string TMTaskList::getFileDirectory() {
		return _directoryName;
	}

	void TMTaskList::leaveReferenceUponExit() {
		std::ofstream outFile;
		outFile.open("TimeMaster.txt");
		outFile << _directoryName + _fileName << '\n';
		outFile << USER_INFO_TIMEMASTER_FILE << '\n';
		outFile.close();
		}

	bool TMTaskList::isFoundInLine(std::string text, std::string line) {
		if (std::search(line.begin(), line.end(), text.begin(), text.end()) == line.end()) {
			return false;
		}
	return true;
	}

	void TMTaskList::determineLoadOrCreateFile() {
		std::string pathName;
		std::ifstream directoryReference("TimeMaster.txt");
		getline(directoryReference, pathName);
		if ((pathName) != "") {
			loadFromFile(pathName);
		}
		return;
	}