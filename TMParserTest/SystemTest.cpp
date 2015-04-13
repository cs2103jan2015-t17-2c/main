//@author A0114130E
#include "stdafx.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace LibraryTest
{
	TEST_CLASS(TMTaskListTest)

{	public:
		
		TEST_METHOD(deleteTasksTest_1) {
			//test 1 all position indexes specified are valid.
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

			TMTaskListStates *states = TMTaskListStates::getInstance();
			states->addNewState(taskList);
			
			std::string userInput = "delete 1 3";
			TMExecutor *executor = TMExecutor::getInstance();
			executor->executeMain(userInput);
		
			std::string actual = executor->getResultOfExecution();
			std::string expected = "2 task(s) successfully deleted.\n";
			
			Assert::AreEqual(expected,actual);
			
		}

		TEST_METHOD(deleteTasksTest_2) {
			//test 2 not all position indexes specified are valid)

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

			TMTaskListStates *states = TMTaskListStates::getInstance();
			states->addNewState(taskList);

			std::string userInput = "delete 7 3";
			TMExecutor *executor = TMExecutor::getInstance();
			executor->executeMain(userInput);
		
			std::string actual = executor->getResultOfExecution();
			std::string expected = "1 task(s) successfully deleted.\n7 is an/are invalid position index(es).";
			
			Assert::AreEqual(expected,actual);
			
		}

		TEST_METHOD(deleteTasksTest_3) {
			//test 3 all position indexes specified are invalid.
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

			TMTaskListStates *states = TMTaskListStates::getInstance();
			states->addNewState(taskList);

			std::string userInput = "delete 7 8";
			TMExecutor *executor = TMExecutor::getInstance();
			executor->executeMain(userInput);
		
			std::string actual = executor->getResultOfExecution();
			std::string expected = "7 8 is an/are invalid position index(es).";
			
			Assert::AreEqual(expected,actual);
			
		}

		TEST_METHOD(deleteTasksTest_4) {
			//test 4 the position index specified is invalid.
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

			TMTaskListStates *states = TMTaskListStates::getInstance();
			states->addNewState(taskList);

			std::string userInput = "delete 7";
			TMExecutor *executor = TMExecutor::getInstance();
			executor->executeMain(userInput);
		
			std::string actual = executor->getResultOfExecution();
			std::string expected = "7 is an/are invalid position index(es).";
			
			Assert::AreEqual(expected,actual);
			
		}

		TEST_METHOD(deleteTasksTest_5) {
			//test 5 the position index specified is valid.
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

			TMTaskListStates *states = TMTaskListStates::getInstance();
			states->addNewState(taskList);

			std::string userInput = "delete 2";
			TMExecutor *executor = TMExecutor::getInstance();
			executor->executeMain(userInput);
		
			std::string actual = executor->getResultOfExecution();
			std::string expected = "1 task(s) successfully deleted.\n";
			
			Assert::AreEqual(expected,actual);
			
		}

		TEST_METHOD(editTaskTest_1) {
			//test 1 (both position index and updated task details specified are valid)
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

			TMTaskListStates *states = TMTaskListStates::getInstance();
			states->addNewState(taskList);

			std::string userInput = "edit 4 Undated changed";
			TMExecutor *executor = TMExecutor::getInstance();
			executor->executeMain(userInput);
		
			std::string actual = executor->getResultOfExecution();
			std::string expected = "Successfully edited task.";
			
			Assert::AreEqual(expected,actual);
			
		}

		TEST_METHOD(editTaskTest_2) {
			//test 2 (both position index and updated task details specified are invalid)
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

			TMTaskListStates *states = TMTaskListStates::getInstance();
			states->addNewState(taskList);

			std::string userInput = "edit 5 ";
			TMExecutor *executor = TMExecutor::getInstance();
			executor->executeMain(userInput);
		
			std::string actual = executor->getResultOfExecution();
			std::string expected = "Missing new task information\nPlease enter an index or the updated task details.";
			
			Assert::AreEqual(expected,actual);
			
		}

		TEST_METHOD(editTaskTest_3) {
			//test 3 (invalid position index but valid updated task details specified)
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

			TMTaskListStates *states = TMTaskListStates::getInstance();
			states->addNewState(taskList);

			std::string userInput = "edit 7 Nonexistent";
			TMExecutor *executor = TMExecutor::getInstance();
			executor->executeMain(userInput);
		
			std::string actual = executor->getResultOfExecution();
			std::string expected = "7 is an/are invalid position index(es).";
			
			Assert::AreEqual(expected,actual);
			
		}

		TEST_METHOD(editTaskTest_4) {
			//test 4 (valid position index but no updated task details specified)
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

			TMTaskListStates *states = TMTaskListStates::getInstance();
			states->addNewState(taskList);

			std::string userInput = "edit 2";
			TMExecutor *executor = TMExecutor::getInstance();
			executor->executeMain(userInput);
		
			std::string actual = executor->getResultOfExecution();
			std::string expected = "Missing new task information\nPlease enter an index or the updated task details.";
			
			Assert::AreEqual(expected,actual);
			
		}
		

	};
}