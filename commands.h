#pragma once
#include <iostream>
#include <sstream>
#include <vector>
#include <chrono>
#include "observer.h"

class Subject
{
public:
	virtual	~Subject() = default;
	virtual void attach(Observer *) = 0;	
	virtual void detach() = 0;
	virtual void notify() = 0;
};

class Commands : public Subject
{
public:
	Commands() = default;

	Commands(int number_commands)
		: number_commands_of_argument{ number_commands }
	{
		clear();
	};
	~Commands() { };
	
	bool empty();	
	void addCommands();	
	void clear();	
	void notifyAndClear();
	void doWork();	
	void attach(Observer* ) override;	
	void detach() override;	
	void notify() override;

	// for tests
	int getNumberCommands() const;
	int getNumberCommandsOfArgument() const;
	int getNumberSubscriber() const;
	int getNumberTotalCommands() const;
	void setTotalCommansForTest(const std::vector<std::string>&);
	void doWorkForTest(const std::vector<std::string>&);

private:	
	int number_commands{0};
	int number_commands_of_argument{0};
	int number_brackets{0};
	long long int time_of_first_command{0};
	std::string commands{};		
	std::vector<std::string> total_commands;
	std::vector<Observer *> subscriber;
};

