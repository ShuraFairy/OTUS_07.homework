#pragma once

#include <iostream>
#include <vector>
#include <fstream>
#include <filesystem>
#include "observer.h"
#include "commands.h"

class LoggerOutputToConsole : public Observer
{
public:

	LoggerOutputToConsole(Commands* loger) { loger->attach(this); }
	~LoggerOutputToConsole() = default;
	void update(const long long int& , const std::vector<std::string>& );

private:
	long long int time_of_first_command{0};
	LoggerOutputToConsole() = default;
	LoggerOutputToConsole(const LoggerOutputToConsole& ) = delete;
	LoggerOutputToConsole& operator=(const LoggerOutputToConsole& ) = delete;
	LoggerOutputToConsole(LoggerOutputToConsole&& ) = delete;
	LoggerOutputToConsole& operator=(LoggerOutputToConsole&& ) = delete;
};

class LoggerOutputToFile : public Observer
{
public:

	LoggerOutputToFile(Commands* loger) { loger->attach(this); }
	~LoggerOutputToFile()=default;
	void update(const long long int& , const std::vector<std::string>& );
private:
	LoggerOutputToFile() = default;
	LoggerOutputToFile(const LoggerOutputToFile &) = delete;
	LoggerOutputToFile& operator=(const LoggerOutputToFile &) = delete;
	LoggerOutputToFile(LoggerOutputToFile && ) = delete;
	LoggerOutputToFile& operator=(LoggerOutputToFile&& ) = delete;
};
