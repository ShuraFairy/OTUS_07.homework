#include <iostream>
#include <cassert>
#include "commands.h"
#include "logger.h"

int main([[maybe_unused]] int argc, [[maybe_unused]] char const* argv[])
{
	int number_commands_of_argument{ 0 };
	if (argc <= 1) 
	{
		std::cout << "Incorrect number of arguments";
	}
	else 
	{
		try 
		{
			number_commands_of_argument = std::stoi(argv[1]);
			if (number_commands_of_argument <= 0)
				std::cout << "Number of commands must be positive";			
		}
		catch (const std::exception& error) 
		{
			std::cerr << error.what();
		}
	}

	Commands commands(number_commands_of_argument);
	LoggerOutputToConsole loggerOutputToConsole(&commands);
	LoggerOutputToFile loggerOutputToFile(&commands);
	commands.doWork();

	return EXIT_SUCCESS;
}
