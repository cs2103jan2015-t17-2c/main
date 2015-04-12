//@author A0111712Y

#include "stdafx.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace TMParserTest{	

	TEST_CLASS(TMParserTest){

	public:

		TEST_METHOD(timeTo24HFormatTest){
            TMParser *parser = TMParser::getInstance(); 
            FormatConverter *formatConverter = FormatConverter::getInstance();
			Assert::AreEqual((std::string)"12:00",formatConverter->timeFrom12HourPMToHHMM("12pm"));
		}

        TEST_METHOD(dateFromNumericToStandardFormat){
            TMParser *parser = TMParser::getInstance();
            FormatConverter *formatConverter = FormatConverter::getInstance();
            std::string date("01012015");
            Assert::AreEqual((std::string)"01 Jan 2015",formatConverter->dateFromNumericToStandardFormat(date));
        }

        TEST_METHOD(timeTo24HFormat){
            TMParser *parser = TMParser::getInstance();
            FormatConverter *formatConverter = FormatConverter::getInstance();
            std::string time("12am");
            Assert::AreEqual((std::string)"00:00",formatConverter->timeFrom12HourAMToHHMM(time));
        }

        TEST_METHOD (parseTimedTask) {
            TMParser *parser = TMParser::getInstance();
            std::string sampleString = "add sample task from 010515 9 to 5";
            std::vector<std::string> actualTokenizedUserEntry = parser->getTokenizedUserEntry(sampleString);
            int expectedSize;
            expectedSize = 8;
            int actualSize;
            actualSize = actualTokenizedUserEntry.size();

            Assert::AreEqual(expectedSize, actualSize);            
        }

        TEST_METHOD(parseTimedTask_1) {
            TMParser *parser = TMParser::getInstance();
            std::string sampleString = "add sample task from 010515 9 to 5";
            parser->initialize(sampleString);
		    std::string command = parser->extractCommand();
            TMTask task = parser -> parseTaskInfo();
            Assert::AreEqual((std::string)"add", command);
            std::string expectedTaskDescription = "sample task";
            std::string actualTaskDescription = task.getTaskDescription();
            Assert::AreEqual(expectedTaskDescription, actualTaskDescription);
        }
	};
}