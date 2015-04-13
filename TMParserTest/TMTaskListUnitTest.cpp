//@author A0114130E
#include "stdafx.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace LibraryTest
{
	TEST_CLASS(TMTaskListUnitTest)
	{
	public:
		
		TEST_METHOD(TestisTwoClash_1) {  
			//test 1 (Task B starts before Task A ends)
			TMTaskTime timeA("27 03 2015", "1300", "27 03 2015", "1700");
			TMTask taskA("Task A", timeA, TaskType::WithPeriod);
			
			TMTaskTime timeB("27 03 2015", "1500", "27 03 2015", "1600");
			TMTask taskB("Task B", timeB, TaskType::WithPeriod);
			
			TMTaskListStates *taskListStates = TMTaskListStates::getInstance();
			TMTaskList taskList = taskListStates->getCurrentTaskList();
			
			bool actual = taskList.isTwoClash(taskA, taskB);
			bool expected = true;
			
			Assert::AreEqual(expected, actual, false);
		}

		TEST_METHOD(TestisTwoClash_2) {  
			//test 2 (Task B starts some time after Task A ends)
			TMTaskTime timeA("27 03 2015", "1300", "27 03 2015", "1700");
			TMTask taskA("Task A", timeA, TaskType::WithPeriod);
			
			TMTaskTime timeB("27 03 2015", "1730", "27 03 2015", "1800");
			TMTask taskB("Task B", timeB, TaskType::WithPeriod);
			
			TMTaskListStates *taskListStates = TMTaskListStates::getInstance();
			TMTaskList taskList = taskListStates->getCurrentTaskList();
			
			bool actual = taskList.isTwoClash(taskA, taskB);
			bool expected = false;

			Assert::AreEqual(expected, actual, false);
		}

		TEST_METHOD(TestisTwoClash_3) {  
			//test 3 (Task B starts at the end time of Task A, boundary value)
			TMTaskTime timeA("27 03 2015", "1300", "27 03 2015", "1700");
			TMTask taskA("Task A", timeA, TaskType::WithPeriod);
			TMTaskTime timeB("27 03 2015", "1700", "27 03 2015", "1800");
			TMTask taskB("Task B", timeB, TaskType::WithPeriod);
			
			TMTaskListStates *taskListStates = TMTaskListStates::getInstance();
			TMTaskList taskList = taskListStates->getCurrentTaskList();
			
			bool actual = taskList.isTwoClash(taskA, taskB);
			bool expected = false;

			Assert::AreEqual(expected, actual, false);
		}

		TEST_METHOD(TestisBefore_1) {  
			//test 1 (TimeA is before TimeB)
			TMDateTime timeA("27 03 2015", "1300");
			TMDateTime timeB("27 03 2015", "1500");
			
			TMTaskListStates *taskListStates = TMTaskListStates::getInstance();
			TMTaskList taskList = taskListStates->getCurrentTaskList();
			
			bool actual = taskList.isBefore(timeA, timeB);
			bool expected = true;

			Assert::AreEqual(expected, actual, false);
		}

		TEST_METHOD(TestisBefore_2) {  
			//test 1 (TimeA == TimeB, boundary value)
			TMDateTime timeA("27 03 2015", "1300");
			TMDateTime timeB("27 03 2015", "1300");
			
			TMTaskListStates *taskListStates = TMTaskListStates::getInstance();
			TMTaskList taskList = taskListStates->getCurrentTaskList();
			
			bool actual = taskList.isBefore(timeA, timeB);
			bool expected = false;

			Assert::AreEqual(expected, actual, false);
		}

		TEST_METHOD(TestisBefore_3) {  
			//test 1 (TimeA is after TimeB)
			TMDateTime timeA("27 03 2015", "1300");
			TMDateTime timeB("27 03 2015", "1000");
			
			TMTaskListStates *taskListStates = TMTaskListStates::getInstance();
			TMTaskList taskList = taskListStates->getCurrentTaskList();
			
			bool actual = taskList.isBefore(timeA, timeB);
			bool expected = false;

			Assert::AreEqual(expected, actual, false);
		}

		TEST_METHOD(TestisValidPositionIndex_1) {  
			//test 1 (Test a value outside the lower boundary value)
			TMTaskTime time_1("27 03 2015", "1300", "27 03 2015", "1300");
			TMTaskTime time_2("27 03 2015", "1500", "27 03 2015", "1500");
			TMTaskTime time_3("27 03 2015", "1200", "27 03 2015", "1700");
			TMTaskTime time_4;

			TMTask taskA("Start date time task", time_1, TaskType::WithStartDateTime);
			TMTask taskB("Deadline", time_2, TaskType::WithEndDateTime);
			TMTask taskC("Period", time_3, TaskType::WithPeriod);
			TMTask taskD("Undated", time_4, TaskType::Undated);
			
			TMTaskList taskList;
			
			taskList.addTask(taskA);
			taskList.addTask(taskB);
			taskList.addTask(taskC);
			taskList.addTask(taskD);

			bool actual = taskList.isValidPositionIndex(-1);
			bool expected = false;
			
			Assert::AreEqual(expected, actual, false);
		}

		TEST_METHOD(TestisValidPositionIndex_2) {  
			//test 2 (Test a value at the lower boundary value)
			TMTaskTime time_1("27 03 2015", "1300", "27 03 2015", "1300");
			TMTaskTime time_2("27 03 2015", "1500", "27 03 2015", "1500");
			TMTaskTime time_3("27 03 2015", "1200", "27 03 2015", "1700");
			TMTaskTime time_4;

			TMTask taskA("Start date time task", time_1, TaskType::WithStartDateTime);
			TMTask taskB("Deadline", time_2, TaskType::WithEndDateTime);
			TMTask taskC("Period", time_3, TaskType::WithPeriod);
			TMTask taskD("Undated", time_4, TaskType::Undated);
			
			TMTaskList taskList;
			
			taskList.addTask(taskA);
			taskList.addTask(taskB);
			taskList.addTask(taskC);
			taskList.addTask(taskD);

			bool actual = taskList.isValidPositionIndex(1);
			bool expected = true;
			
			Assert::AreEqual(expected, actual, false);
		}

		TEST_METHOD(TestisValidPositionIndex_3) {  
			//test 3 (Test a value within the lower and upper boundary value)
			TMTaskTime time_1("27 03 2015", "1300", "27 03 2015", "1300");
			TMTaskTime time_2("27 03 2015", "1500", "27 03 2015", "1500");
			TMTaskTime time_3("27 03 2015", "1200", "27 03 2015", "1700");
			TMTaskTime time_4;

			TMTask taskA("Start date time task", time_1, TaskType::WithStartDateTime);
			TMTask taskB("Deadline", time_2, TaskType::WithEndDateTime);
			TMTask taskC("Period", time_3, TaskType::WithPeriod);
			TMTask taskD("Undated", time_4, TaskType::Undated);
			
			TMTaskList taskList;

			taskList.addTask(taskA);
			taskList.addTask(taskB);
			taskList.addTask(taskC);
			taskList.addTask(taskD);

			bool actual = taskList.isValidPositionIndex(3);
			bool expected = true;
			
			Assert::AreEqual(expected, actual, false);
		}

		TEST_METHOD(TestisValidPositionIndex_4) {  
			//test 4 (Test a value at the upper boundary value)
			TMTaskTime time_1("27 03 2015", "1300", "27 03 2015", "1300");
			TMTaskTime time_2("27 03 2015", "1500", "27 03 2015", "1500");
			TMTaskTime time_3("27 03 2015", "1200", "27 03 2015", "1700");
			TMTaskTime time_4;

			TMTask taskA("Start date time task", time_1, TaskType::WithStartDateTime);
			TMTask taskB("Deadline", time_2, TaskType::WithEndDateTime);
			TMTask taskC("Period", time_3, TaskType::WithPeriod);
			TMTask taskD("Undated", time_4, TaskType::Undated);
			
			TMTaskList taskList;
			
			taskList.addTask(taskA);
			taskList.addTask(taskB);
			taskList.addTask(taskC);
			taskList.addTask(taskD);

			bool actual = taskList.isValidPositionIndex(4);
			bool expected = true;
			
			Assert::AreEqual(expected, actual, false);
		}

		TEST_METHOD(TestisValidPositionIndex_5) {  
			//test 5 (Test a value outside the upper boundary value)
			TMTaskTime time_1("27 03 2015", "1300", "27 03 2015", "1300");
			TMTaskTime time_2("27 03 2015", "1500", "27 03 2015", "1500");
			TMTaskTime time_3("27 03 2015", "1200", "27 03 2015", "1700");
			TMTaskTime time_4;

			TMTask taskA("Start date time task", time_1, TaskType::WithStartDateTime);
			TMTask taskB("Deadline", time_2, TaskType::WithEndDateTime);
			TMTask taskC("Period", time_3, TaskType::WithPeriod);
			TMTask taskD("Undated", time_4, TaskType::Undated);
			
			TMTaskList taskList;
			
			taskList.addTask(taskA);
			taskList.addTask(taskB);
			taskList.addTask(taskC);
			taskList.addTask(taskD);

			bool actual = taskList.isValidPositionIndex(8);
			bool expected = false;
			
			Assert::AreEqual(expected, actual, false);
		}

		TEST_METHOD(TestgetPositionIndexFromTask_1) {  
			//test 1 (Input a valid task)
			TMTaskTime time_1("27 03 2015", "1300", "27 03 2015", "1300");
			TMTaskTime time_2("27 03 2015", "1500", "27 03 2015", "1500");
			TMTaskTime time_3("27 03 2015", "1200", "27 03 2015", "1700");
			TMTaskTime time_4;

			TMTask taskA("Start date time task", time_1, TaskType::WithStartDateTime);
			TMTask taskB("Deadline", time_2, TaskType::WithEndDateTime);
			TMTask taskC("Period", time_3, TaskType::WithPeriod);
			TMTask taskD("Undated", time_4, TaskType::Undated);
			
			TMTaskList taskList;
			
			taskList.addTask(taskA);
			taskList.addTask(taskB);
			taskList.addTask(taskC);
			taskList.addTask(taskD);

			int actual = taskList.getPositionIndexFromTask(taskC);
			int expected = 1;
			
			Assert::AreEqual(expected, actual, false);
		}

		TEST_METHOD(TestgetPositionIndexFromTask_2) {  
			//test 2 (Input an invalid task)
			TMTaskTime time_1("27 03 2015", "1300", "27 03 2015", "1300");
			TMTaskTime time_2("27 03 2015", "1500", "27 03 2015", "1500");
			TMTaskTime time_3("27 03 2015", "1200", "27 03 2015", "1700");
			TMTaskTime time_4;

			TMTask taskA("Start date time task", time_1, TaskType::WithStartDateTime);
			TMTask taskB("Deadline", time_2, TaskType::WithEndDateTime);
			TMTask taskC("Period", time_3, TaskType::WithPeriod);
			TMTask taskD("Undated", time_4, TaskType::Undated);
			
			TMTaskList taskList;
			
			taskList.addTask(taskA);
			taskList.addTask(taskB);
			taskList.addTask(taskC);
			taskList.addTask(taskD);

			TMTask taskE("Deadline", time_1, TaskType::WithEndDateTime);

			int actual = taskList.getPositionIndexFromTask(taskE);
			int expected = 0;
			
			Assert::AreEqual(expected, actual, false);
		}

		TEST_METHOD(TestgetTaskFromPositionIndex_1) {  
			//test 1 (Input an index which is outside the lower boundary value)
			TMTaskTime time_1("27 03 2015", "1300", "27 03 2015", "1300");
			TMTaskTime time_2("27 03 2015", "1500", "27 03 2015", "1500");
			TMTaskTime time_3("27 03 2015", "1200", "27 03 2015", "1700");
			TMTaskTime time_4;

			TMTask taskA("Start date time task", time_1, TaskType::WithStartDateTime);
			TMTask taskB("Deadline", time_2, TaskType::WithEndDateTime);
			TMTask taskC("Period", time_3, TaskType::WithPeriod);
			TMTask taskD("Undated", time_4, TaskType::Undated);
			
			TMTaskList taskList;
			
			taskList.addTask(taskA);
			taskList.addTask(taskB);
			taskList.addTask(taskC);
			taskList.addTask(taskD);

			std::string actual = taskList.getTaskFromPositionIndex(-2).getTaskTypeAsString();
			std::string expected = "Invalid";
			
			Assert::AreEqual(expected, actual, false);
		}

		TEST_METHOD(TestgetTaskFromPositionIndex_2) {  
			//test 2 (Input an index which on the lower boundary value)
			TMTaskTime time_1("27 03 2015", "1300", "27 03 2015", "1300");
			TMTaskTime time_2("27 03 2015", "1500", "27 03 2015", "1500");
			TMTaskTime time_3("27 03 2015", "1200", "27 03 2015", "1700");
			TMTaskTime time_4;

			TMTask taskA("Start date time task", time_1, TaskType::WithStartDateTime);
			TMTask taskB("Deadline", time_2, TaskType::WithEndDateTime);
			TMTask taskC("Period", time_3, TaskType::WithPeriod);
			TMTask taskD("Undated", time_4, TaskType::Undated);
			
			TMTaskList taskList;
			
			taskList.addTask(taskA);
			taskList.addTask(taskB);
			taskList.addTask(taskC);
			taskList.addTask(taskD);

			std::string actual = taskList.getTaskFromPositionIndex(1).getTaskTypeAsString();
			std::string expected = "WithPeriod";
			
			Assert::AreEqual(expected, actual, false);
		}

		TEST_METHOD(TestgetTaskFromPositionIndex_3) {  
			//test 3 (Input an index which within the lower and upper boundary value)
			TMTaskTime time_1("27 03 2015", "1300", "27 03 2015", "1300");
			TMTaskTime time_2("27 03 2015", "1500", "27 03 2015", "1500");
			TMTaskTime time_3("27 03 2015", "1200", "27 03 2015", "1700");
			TMTaskTime time_4;

			TMTask taskA("Start date time task", time_1, TaskType::WithStartDateTime);
			TMTask taskB("Deadline", time_2, TaskType::WithEndDateTime);
			TMTask taskC("Period", time_3, TaskType::WithPeriod);
			TMTask taskD("Undated", time_4, TaskType::Undated);
			
			TMTaskList taskList;
			
			taskList.addTask(taskA);
			taskList.addTask(taskB);
			taskList.addTask(taskC);
			taskList.addTask(taskD);

			std::string actual = taskList.getTaskFromPositionIndex(3).getTaskTypeAsString();
			std::string expected = "WithEndDateTime";
			
			Assert::AreEqual(expected, actual, false);
		}

		TEST_METHOD(TestgetTaskFromPositionIndex_4) {  
			//test 4 (Input an index which on the upper boundary value)
			TMTaskTime time_1("27 03 2015", "1300", "27 03 2015", "1300");
			TMTaskTime time_2("27 03 2015", "1500", "27 03 2015", "1500");
			TMTaskTime time_3("27 03 2015", "1200", "27 03 2015", "1700");
			TMTaskTime time_4;

			TMTask taskA("Start date time task", time_1, TaskType::WithStartDateTime);
			TMTask taskB("Deadline", time_2, TaskType::WithEndDateTime);
			TMTask taskC("Period", time_3, TaskType::WithPeriod);
			TMTask taskD("Undated", time_4, TaskType::Undated);
			
			TMTaskList taskList;
			
			taskList.addTask(taskA);
			taskList.addTask(taskB);
			taskList.addTask(taskC);
			taskList.addTask(taskD);

			std::string actual = taskList.getTaskFromPositionIndex(4).getTaskTypeAsString();
			std::string expected = "Undated";
			
			Assert::AreEqual(expected, actual, false);
		}

		TEST_METHOD(TestgetTaskFromPositionIndex_5) {  
			//test 5 (Input an index which outside the upper boundary value)
			TMTaskTime time_1("27 03 2015", "1300", "27 03 2015", "1300");
			TMTaskTime time_2("27 03 2015", "1500", "27 03 2015", "1500");
			TMTaskTime time_3("27 03 2015", "1200", "27 03 2015", "1700");
			TMTaskTime time_4;

			TMTask taskA("Start date time task", time_1, TaskType::WithStartDateTime);
			TMTask taskB("Deadline", time_2, TaskType::WithEndDateTime);
			TMTask taskC("Period", time_3, TaskType::WithPeriod);
			TMTask taskD("Undated", time_4, TaskType::Undated);
			
			TMTaskList taskList;
			
			taskList.addTask(taskA);
			taskList.addTask(taskB);
			taskList.addTask(taskC);
			taskList.addTask(taskD);

			std::string actual = taskList.getTaskFromPositionIndex(9).getTaskTypeAsString();
			std::string expected = "Invalid";
			
			Assert::AreEqual(expected, actual, false);
		}

		TEST_METHOD(TestaddTask_1) {  
			//test 1 (test if a task of type WithStartDateTime is added corrected to the dated vector)
			TMTaskTime time_1("27 03 2015", "1300", "27 03 2015", "1300");
			TMTask taskA("Start date time task", time_1, TaskType::WithStartDateTime);
		
			TMTaskList taskList;
			
			taskList.addTask(taskA);
		
			std::ostringstream actual;
			actual << taskList.getDatedSize() << " " << taskList.getUndatedSize() << " " << taskList.getArchivedSize();
			std::string expected = "1 0 0";
			
			Assert::AreEqual(expected, actual.str(), false);
		}

		TEST_METHOD(TestaddTask_2) {  
			//test 2 (test if a task of type WithEndDateTime is added corrected to the dated vector)
			TMTaskTime time_1("27 03 2015", "1300", "27 03 2015", "1300");
			TMTask taskA("End date time task", time_1, TaskType::WithEndDateTime);
		
			TMTaskList taskList;
			
			taskList.addTask(taskA);
		
			std::ostringstream actual;
			actual << taskList.getDatedSize() << " " << taskList.getUndatedSize() << " " << taskList.getArchivedSize();
			std::string expected = "1 0 0";
			
			Assert::AreEqual(expected, actual.str(), false);
		}

		TEST_METHOD(TestaddTask_3) {  
			//test 3 (test if a task of type WithPeriod is added corrected to the dated vector)
			TMTaskTime time_1("27 03 2015", "1200", "27 03 2015", "1300");
			TMTask taskA("With period", time_1, TaskType::WithPeriod);
		
			TMTaskList taskList;
			
			taskList.addTask(taskA);
		
			std::ostringstream actual;
			actual << taskList.getDatedSize() << " " << taskList.getUndatedSize() << " " << taskList.getArchivedSize();
			std::string expected = "1 0 0";
			
			Assert::AreEqual(expected, actual.str(), false);
		}

		TEST_METHOD(TestaddTask_4) {  
			//test 4 (test if a task of type Undated is added corrected to the undated vector)
			TMTaskTime time_1;
			TMTask taskA("Undated", time_1, TaskType::Undated);
		
			TMTaskList taskList;
			
			taskList.addTask(taskA);
		
			std::ostringstream actual;
			actual << taskList.getDatedSize() << " " << taskList.getUndatedSize() << " " << taskList.getArchivedSize();
			std::string expected = "0 1 0";
			
			Assert::AreEqual(expected, actual.str(), false);
		}

		TEST_METHOD(TestaddTask_5) {  
			//test 5 (test whether two period tasks which clashes are marked as clash)
			TMTaskTime time_1("27 03 2015", "1200", "27 03 2015", "1500");
			TMTaskTime time_2("27 03 2015", "1000", "27 03 2015", "1900");
			TMTask taskA("Period task 1", time_1, TaskType::WithPeriod);
			TMTask taskB("Period task 2", time_2, TaskType::WithPeriod);
		
			TMTaskList taskList;
			
			taskList.addTask(taskA);
			taskList.addTask(taskB);
		
			std::ostringstream actual;
			TMTask task1 = taskList.getTaskFromPositionIndex(1);
			TMTask task2 = taskList.getTaskFromPositionIndex(2);
			actual << task1.isClashed() << " " << task2.isClashed();
			std::string expected = "1 1";
			
			Assert::AreEqual(expected, actual.str(), false);
		}

		TEST_METHOD(TestaddTask_6) {  
			//test 6 (test whether a deadline task which ends within the time of the period task clashes with a period task)
			TMTaskTime time_1("27 03 2015", "1200", "27 03 2015", "1500");
			TMTaskTime time_2("27 03 2015", "1300", "27 03 2015", "1300");
			TMTask taskA("Period task", time_1, TaskType::WithPeriod);
			TMTask taskB("Deadline task", time_2, TaskType::WithEndDateTime);
		
			TMTaskList taskList;
			
			taskList.addTask(taskA);
			taskList.addTask(taskB);
		
			std::ostringstream actual;
			TMTask task1 = taskList.getTaskFromPositionIndex(1);
			TMTask task2 = taskList.getTaskFromPositionIndex(2);
			actual << task1.isClashed() << " " << task2.isClashed();
			std::string expected = "0 0";
			
			Assert::AreEqual(expected, actual.str(), false);
		}

		TEST_METHOD(TestaddTask_7) {  
			//test 7 (test whether a start date time task which starts within the time of the period task clashes with a period task)
			TMTaskTime time_1("27 03 2015", "1200", "27 03 2015", "1500");
			TMTaskTime time_2("27 03 2015", "1300", "27 03 2015", "1300");
			TMTask taskA("Period task", time_1, TaskType::WithPeriod);
			TMTask taskB("Start date time task", time_2, TaskType::WithStartDateTime);
		
			TMTaskList taskList;
			
			taskList.addTask(taskA);
			taskList.addTask(taskB);
		
			std::ostringstream actual;
			TMTask task1 = taskList.getTaskFromPositionIndex(1);
			TMTask task2 = taskList.getTaskFromPositionIndex(2);
			actual << task1.isClashed() << " " << task2.isClashed();
			std::string expected = "0 0";
			
			Assert::AreEqual(expected, actual.str(), false);
		}

		


	};
}