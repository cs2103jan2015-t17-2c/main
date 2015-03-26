#include "stdafx.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace TMParserTest{	

	TEST_CLASS(TMParserTest){

	public:

		TEST_METHOD(timeTo24HFormatTest)
		{
            TMParser *parser = TMParser::getInstance(); 
			Assert::AreEqual((std::string)"1200",parser->timeTo24HFormat("12pm"));
		}

	};
}