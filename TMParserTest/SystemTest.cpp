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

		TEST_METHOD(addDatedTaskDescriptionTest)
		{
			TMExecutor* exe = TMExecutor::getInstance();
	
			

			std::string userEntry = "add test1 on 01012016 at 5pm";
			std::string expectedDescription = "test1 ";
			std::string expectedStartDate = "1 Jan 2016";
			int expectedDatedSize = 1;
		

			exe->executeMain(userEntry);
				
			TMTaskListStates *taskListStates = TMTaskListStates::getInstance();
			TMTaskList taskList = taskListStates->getCurrentTaskList();
			std::vector<TMTask> dated = taskList.getDated();
			

			Assert::AreEqual(expectedDescription,dated[0].getTaskDescription());
		}

		TEST_METHOD(addDatedTaskStartDateTest)
		{
			TMExecutor* exe = TMExecutor::getInstance();
	
			

			std::string userEntry = "add test1 on 01012016 at 5pm";
			std::string expectedDescription = "test1 ";
			std::string expectedStartDate = "01 Jan 2016";
			int expectedDatedSize = 1;
		

			exe->executeMain(userEntry);
				
			TMTaskListStates *taskListStates = TMTaskListStates::getInstance();
			TMTaskList taskList = taskListStates->getCurrentTaskList();
			std::vector<TMTask> dated = taskList.getDated();
			

			Assert::AreEqual(expectedStartDate,dated[0].getTaskTime().getStartDate());
		}

		TEST_METHOD(addDatedTaskStartTimeTest)
		{
			TMExecutor* exe = TMExecutor::getInstance();
	
			std::string userEntry = "a test1 on 01012016 at 5pm";
			std::string expectedDescription = "test1 ";
			std::string expectedStartDate = "01 Jan 2016";
			std::string expectedStartTime = "1700";
			int expectedDatedSize = 1;
		

			exe->executeMain(userEntry);
				
			TMTaskListStates *taskListStates = TMTaskListStates::getInstance();
			TMTaskList taskList = taskListStates->getCurrentTaskList();
			std::vector<TMTask> dated = taskList.getDated();
			

			Assert::AreEqual(expectedStartTime,dated[0].getTaskTime().getStartTime());
		}

		

	};
}