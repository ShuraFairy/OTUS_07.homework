#include "commands.h"

bool Commands::empty()
{
	return total_commands.empty();
}

void Commands::addCommands()
{
	using namespace std::chrono;
	if (empty()) {
		time_of_first_command = duration_cast<seconds>(system_clock::now().time_since_epoch()).count();
	}
	total_commands.emplace_back(std::move(commands));
}

void Commands::clear()
{
	total_commands.clear();
	number_commands = number_commands_of_argument;
}

void Commands::notifyAndClear()
{	
	notify();
	clear();
}

void Commands::doWork()
{
	while (std::cin) 
	{
		while (0 < number_commands) 
		{
			std::getline(std::cin, commands);

			if (std::cin.eof()) 
				break;			

			if ((commands != "{") && (commands != "}") &&
				(!commands.empty())) 
			{
				if (number_brackets > 0) 
					addCommands();
				else {
					number_commands--;
					addCommands();
				}
			}

			if (commands == "{") 
			{
				if ((number_brackets == 0) && (!total_commands.empty())) 
					notifyAndClear();
				number_brackets++;
			}
			if (commands == "}") 
			{
				if (number_brackets <= 0) {
					std::cerr << "Warning: Unexpected bracket\n";
				}
				else 
				{
					number_brackets--;
					if (number_brackets <= 0) 
						notifyAndClear();
				}
			}
		}

		if (number_brackets == 0) 
			notifyAndClear();
	}
}

void Commands::attach(Observer* observer)
{
	subscriber.push_back(observer);
}

void Commands::detach()
{
	subscriber.clear();
}

void Commands::notify()
{
	for (auto elem : subscriber) 
		elem->update(time_of_first_command, total_commands);
}

int Commands::getNumberCommands() const
{
	return number_commands;
}

int Commands::getNumberCommandsOfArgument() const
{
	return number_commands_of_argument;
}

int Commands::getNumberSubscriber() const
{
	return subscriber.size();
}

int Commands::getNumberTotalCommands() const
{
	return total_commands.size();
}

void Commands::setTotalCommansForTest(const std::vector<std::string>& commands)
{
	total_commands = commands;
}

void Commands::doWorkForTest(const std::vector<std::string>& _commands) {
	for (const auto & command : _commands) {
		commands = command; //std::getline(std::cin, _commands);
		if (commands == "EOF") {//std::cin.eof()
			number_commands--;
			break;
		}

		if ((commands != "{") &&
			(commands != "}") &&
			(!commands.empty())) {
			if (number_brackets > 0) {
				addCommands();
			}
			else {
				number_commands--;
				addCommands();
			}
		}

		if (commands == "{") {
			if ((number_brackets == 0) &&
				(!total_commands.empty())
				) {
				notifyAndClear();
			}
			number_brackets++;
		}
		if (commands == "}") {
			if (number_brackets <= 0) {
				std::cerr << "Warning: Unexpected bracket\n";
			}
			else {
				number_brackets--;
				if (number_brackets <= 0) {
					notifyAndClear();
				}
			}
		}

	}

	if (number_brackets == 0) {
		notifyAndClear();
	}
}
