#define BOOST_TEST_MODULE test_version

#include <boost/test/unit_test.hpp>
#include "commands.h"
#include "logger.h"

BOOST_AUTO_TEST_SUITE(boost_test_version)

BOOST_AUTO_TEST_CASE(subscribe)
{
	Commands commands(3);
	LoggerOutputToConsole loggerOutputToConsole(&commands);

	BOOST_CHECK(commands.getNumberSubscriber() == 1);	
}

BOOST_AUTO_TEST_CASE(commands_without_brackets)
{
	// Test cmd1,cmd2,cmd3 
	std::vector<std::string> commands{ "cmd1","cmd2","cmd3" };
	Commands test_commands(static_cast<int>(commands.size()));
	LoggerOutputToConsole loggerOutputToConsole(&test_commands);

	std::stringstream buffer, strm;
	strm << "\tbulk: cmd1,cmd2,cmd3\n\n";
	std::streambuf* sbuf = std::cout.rdbuf();
	std::cout.rdbuf(buffer.rdbuf());

	test_commands.doWorkForTest(commands);

	std::cout.rdbuf(sbuf);
	BOOST_CHECK(buffer.str() == strm.str());
	BOOST_CHECK(test_commands.getNumberTotalCommands() == 0);
}

BOOST_AUTO_TEST_CASE(commands_with_EOF) {
	// Test cmd1,cmd2,EOF 
	std::vector<std::string> commands{ "cmd4","cmd5","EOF" };
	Commands test_commands(static_cast<int>(commands.size()));
	LoggerOutputToConsole loggerOutputToConsole(&test_commands);
	std::stringstream buffer, strm;
	strm << "\tbulk: cmd4,cmd5\n\n";
	std::streambuf* sbuf = std::cout.rdbuf();
	std::cout.rdbuf(buffer.rdbuf());

	test_commands.doWorkForTest(commands);

	std::cout.rdbuf(sbuf);
	BOOST_CHECK(buffer.str() == strm.str());
	BOOST_CHECK(test_commands.getNumberTotalCommands() == 0);
}

BOOST_AUTO_TEST_CASE(commands_with_brackets)
{
	// Test cmd1,cmd2 { cmd3,cmd4 }	
	std::vector<std::string> commands{ "cmd1","cmd2","{", "cmd3","cmd4","}" };

	Commands test_commands(static_cast<int>(commands.size()));
	LoggerOutputToConsole loggerOutputToConsole(&test_commands);
	std::stringstream buffer, strm;
	strm << "\tbulk: cmd1,cmd2\n\n"
		<< "\tbulk: cmd3,cmd4\n\n";
	std::streambuf* sbuf = std::cout.rdbuf();
	std::cout.rdbuf(buffer.rdbuf());
	test_commands.doWorkForTest(commands);
	std::cout.rdbuf(sbuf);

	BOOST_CHECK(buffer.str() == strm.str());
	BOOST_CHECK(test_commands.getNumberTotalCommands() == 0);
}

BOOST_AUTO_TEST_CASE(total_commands) 
{
	// Test all commands
	std::vector<std::string> commands{
		"cmd1","cmd2", "{",
		"cmd3", "cmd4","}",
		"{",
		"cmd5","cmd6",
		"{",
		"cmd7","cmd8",
		"}",
		"cmd9",
		"}"
	};

	Commands test_commands(static_cast<int>(commands.size()));
	LoggerOutputToConsole loggerOutputToConsole(&test_commands);
	std::stringstream buffer, strm;
	strm << "\tbulk: cmd1,cmd2\n\n"
		<< "\tbulk: cmd3,cmd4\n\n"
		<< "\tbulk: cmd5,cmd6,cmd7,cmd8,cmd9\n\n";
	std::streambuf* sbuf = std::cout.rdbuf();
	std::cout.rdbuf(buffer.rdbuf());
	test_commands.doWorkForTest(commands);
	std::cout.rdbuf(sbuf);

	BOOST_CHECK(buffer.str() == strm.str());
	BOOST_CHECK(test_commands.getNumberTotalCommands() == 0);
}

BOOST_AUTO_TEST_SUITE_END()
