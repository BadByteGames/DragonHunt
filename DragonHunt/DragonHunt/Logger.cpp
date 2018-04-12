#include "Logger.h"
#include <fstream>
#include <chrono>
#include <ctime>
#include <time.h>
#include <iostream>
#include <cstring>
#include <algorithm>

std::string Logger::m_filename = "";

Logger::Logger()
{
}


Logger::~Logger()
{
}

void Logger::init(std::string loggerFile)
{
	m_filename = loggerFile;
	//empty contents of files
	std::ofstream o(m_filename);
	o.close();
}

void Logger::logEvent(std::string subsystemName, std::string message)
{
	std::ofstream o(m_filename, std::fstream::app);

	//get time stamp
	auto now = std::chrono::system_clock::now();
	time_t nowTime = std::chrono::system_clock::to_time_t(now);
	char str[26];
#ifdef _MSVC
	ctime_s(str, sizeof(str), &nowTime);
#else
	strcpy(str, ctime(&nowTime));
#endif
	//remove newlines from time stamp
	std::remove(str, str+sizeof(str) / sizeof(char), '\n');

	//add special message if error
	if (subsystemName == "error") {
		std::cout << "An error occurred, please check runtime.log for details." << std::endl;
	}

	//add time stamp to file
	o << "["<< str<<"][" << subsystemName << "]: " << message << std::endl;
	o.close();
}
