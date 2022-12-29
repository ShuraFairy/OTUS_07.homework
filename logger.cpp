#include "logger.h"

void LoggerOutputToConsole::update(const long long int& time_command, const std::vector<std::string>& v_stream_command)
{
	std::vector<std::string>::size_type size = v_stream_command.size();
	time_of_first_command = time_command;
	
	if (!v_stream_command.empty()) 
	{
		std::cout << "\tbulk: ";
		for (size_t i = 0; i < size; i++) 
		{
			if (i < v_stream_command.size() - 1) 
				std::cout << v_stream_command.at(i) << ",";			
			else 
				std::cout << v_stream_command.at(i) << "\n";
		}
		std::cout << '\n';
	}
}

void LoggerOutputToFile::update(const long long int& time_command, const std::vector<std::string>& v_stream_command)
{
	namespace fs = std::filesystem;
	std::vector<std::string>::size_type size = v_stream_command.size();

	if (!v_stream_command.empty()) 
	{
		try {

			fs::path directory_with_log{ "LOG/" };
			create_directories(directory_with_log);
			auto name_log_file = std::to_string(time_command) + ".log";
			auto path_log_file = directory_with_log / name_log_file;

			std::ofstream log_file{ path_log_file };
			if (!log_file) 
				std::cerr << "OOPS, can't open \"" << path_log_file.string() << "\"\n";			

			log_file << "bulk: ";
			for (size_t i = 0; i < size; i++) 
			{
				if (i < v_stream_command.size() - 1) 
					log_file << v_stream_command.at(i) << ",";				
				else 
					log_file << v_stream_command.at(i) << "\n";				
			}
			log_file << '\n';
		}
		catch (const fs::filesystem_error& e) 
		{
			std::cerr << "EXCEPTION: " << e.what() << '\n';
			std::cerr << "    path1: \"" << e.path1().string() << "\"\n";
		}
	}
}


