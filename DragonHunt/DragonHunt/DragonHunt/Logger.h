#pragma once
#include <string>

class Logger
{
public:
	Logger();
	~Logger();

	static void init(std::string loggerFile);

	static void logEvent(std::string subsystemName, std::string message);

private:
	static std::string m_filename;
};

