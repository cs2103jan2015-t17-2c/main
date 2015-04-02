#include "stdafx.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace TMParserTest{	

	TEST_CLASS(TMParserTest){

	public:

		TEST_METHOD(timeTo24HFormatTest){
            TMParser *parser = TMParser::getInstance(); 
			Assert::AreEqual((std::string)"1200",parser->timeTo24HFormat("12pm"));
		}

        TEST_METHOD(dateFromNumericToStandardFormat){
            TMParser *parser = TMParser::getInstance(); 
            std::string date("01012015");
            Assert::AreEqual((std::string)"01 Jan 2015",parser->dateFromNumericToStandardFormat(date));
        }

        TEST_METHOD(timeTo24HFormat){
            TMParser *parser = TMParser::getInstance();
            std::string time("12am");
            Assert::AreEqual((std::string)"0000",parser->timeTo24HFormat(time));
        }

	};
}