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
	};
}