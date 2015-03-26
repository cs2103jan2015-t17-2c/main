#include "stdafx.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace LibraryTest
{
	TEST_CLASS(TMTaskListTest)
	{
	public:
		
		TEST_METHOD(addFloatingTaskTest)
		{
			TMParser *parser = TMParser::getInstance(); 
			TMTaskList *taskList = TMTaskList::getInstance();
			TMCommandCreator cmdCreator;

			std::string userEntry = "add test1";
			std::string expected = "test1";
			int expectedUndatedSize = 1;
			int expectedDatedSize = 0;

			parser->initialize(userEntry);
			std::string command = parser->extractCommand();
			TMCommand* commandObjPtr = cmdCreator.createNewCommandObj(parser->determineCommandType(command));
			commandObjPtr->execute();

			Assert::AreEqual(expectedUndatedSize,taskList->getUndatedSize());
			Assert::AreEqual(expectedDatedSize,taskList->getDatedSize());
		}

		TEST_METHOD(addDatedTaskTest)
		{
			TMParser *parser = TMParser::getInstance(); 
			TMTaskList *taskList = TMTaskList::getInstance();
			TMCommandCreator cmdCreator;

			std::string userEntry = "add test1 on friday at 4pm";
			std::string expected = "test1";
			int expectedUndatedSize = 0;
			int expectedDatedSize = 1;

			parser->initialize(userEntry);
			std::string command = parser->extractCommand();
			TMCommand* commandObjPtr = cmdCreator.createNewCommandObj(parser->determineCommandType(command));
			commandObjPtr->execute();

			//Assert::AreEqual(expectedUndatedSize,taskList->getUndatedSize());
			Assert::AreEqual(expectedDatedSize,taskList->getDatedSize());
		}

		TEST_METHOD(TestisTwoClash_1) {  
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
		}

	};
}