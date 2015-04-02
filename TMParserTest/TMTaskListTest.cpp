#include "stdafx.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace LibraryTest
{
	TEST_CLASS(TMTaskListTest)

{	public:
		
		TEST_METHOD(addFloatingTaskTest) {
		
			TMExecutor* exe = TMExecutor::getInstance();
	
			std::vector<TMTask> undated;

			std::string userEntry = "add test1";
			std::string expected = "test1";
			int expectedUndatedSize = 1;
		

			exe->executeMain(userEntry);
				
			TMTaskListStates *taskListStates = TMTaskListStates::getInstance();
			TMTaskList taskList = taskListStates->getCurrentTaskList();

			

			Assert::AreEqual(expectedUndatedSize,taskList.getUndatedSize());
			
		}

		TEST_METHOD(addDatedTaskTest)
		{
			TMExecutor* exe = TMExecutor::getInstance();
	
			

			std::string userEntry = "add test1 on 01012016";
			std::string expectedDescription = "test1 ";
			std::string expectedStartDate = "1 Jan 2016";
			int expectedDatedSize = 1;
		

			exe->executeMain(userEntry);
				
			TMTaskListStates *taskListStates = TMTaskListStates::getInstance();
			TMTaskList taskList = taskListStates->getCurrentTaskList();
			std::vector<TMTask> dated = taskList.getDated();
			

			Assert::AreEqual(expectedDescription,dated[0].getTaskDescription());
		}

		/*TEST_METHOD(TestisTwoClash_1) {  
			//test 1 (Task B starts before Task A ends)
			TMTaskTime timeA("27 03 2015", "1300", "27 03 2015", "1700");
			TMTask taskA("Task A", timeA, TaskType::WithPeriod);
			TMTaskTime timeB("27 03 2015", "1500", "27 03 2015", "1600");
			TMTask taskB("Task B", timeB, TaskType::WithPeriod);
			TMTaskList *taskList = TMTaskList::getInstance();
			bool actual = taskList->isTwoClash(taskA, taskB);
			bool expected = true;
			Assert::AreEqual(expected, actual, false);
		}

		TEST_METHOD(TestisTwoClash_2) {  
			//test 2 (Task B starts some time after Task A ends)
			TMTaskTime timeA("27 03 2015", "1300", "27 03 2015", "1700");
			TMTask taskA("Task A", timeA, TaskType::WithPeriod);
			TMTaskTime timeB("27 03 2015", "1730", "27 03 2015", "1800");
			TMTask taskB("Task B", timeB, TaskType::WithPeriod);
			TMTaskList *taskList = TMTaskList::getInstance();
			bool actual = taskList->isTwoClash(taskA, taskB);
			bool expected = false;
			Assert::AreEqual(expected, actual, false);
		}

		TEST_METHOD(TestisTwoClash_3) {  
			//test 3 (Task B starts right after Task A ends, boundary value)
			TMTaskTime timeA("27 03 2015", "1300", "27 03 2015", "1700");
			TMTask taskA("Task A", timeA, TaskType::WithPeriod);
			TMTaskTime timeB("27 03 2015", "1700", "27 03 2015", "1800");
			TMTask taskB("Task B", timeB, TaskType::WithPeriod);
			TMTaskList *taskList = TMTaskList::getInstance();
			bool actual = taskList->isTwoClash(taskA, taskB);
			bool expected = false;
			Assert::AreEqual(expected, actual, false);
		}*/

	};
}