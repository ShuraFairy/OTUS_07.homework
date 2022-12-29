#include <gtest/gtest.h>
#include "commands.h"
#include "logger.h"

TEST(TestLoggerOutputToConsole, subscribe)
{
	Commands commands(3);
	LoggerOutputToConsole loggerOutputToConsole(&commands);

	EXPECT_EQ(commands.getNumberSubscriber(), 1);
}

TEST(TestCommands, commands_without_brackets)
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
	EXPECT_EQ(buffer.str(), strm.str());
	EXPECT_EQ(test_commands.getNumberTotalCommands(), 0);
}

TEST(TestCommands, commands_with_EOF) {
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
	EXPECT_EQ(buffer.str(), strm.str());
	EXPECT_EQ(test_commands.getNumberTotalCommands(), 0);
}

TEST(TestCommands, commands_with_brackets) 
{
	// Test cmd1,cmd2 { cmd3,cmd4 }	
	std::vector<std::string> commands{ "cmd1","cmd2","{", "cmd3","cmd4","}"	};

	Commands test_commands(static_cast<int>(commands.size()));
	LoggerOutputToConsole loggerOutputToConsole(&test_commands);
	std::stringstream buffer, strm;
	strm << "\tbulk: cmd1,cmd2\n\n"
		<< "\tbulk: cmd3,cmd4\n\n";
	std::streambuf* sbuf = std::cout.rdbuf();
	std::cout.rdbuf(buffer.rdbuf());
	test_commands.doWorkForTest(commands);
	std::cout.rdbuf(sbuf);

	EXPECT_EQ(buffer.str(), strm.str());
	EXPECT_EQ(test_commands.getNumberTotalCommands(), 0);
}

TEST(TestCommands, total_commands) {
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

	EXPECT_EQ(buffer.str(), strm.str());
	EXPECT_EQ(test_commands.getNumberTotalCommands(), 0);
}

int main(int argc, char** argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
