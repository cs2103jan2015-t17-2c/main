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

	};
}