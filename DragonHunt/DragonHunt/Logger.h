#pragma once
#include <string>
class Logger
{
public:
	Logger();
	~Logger();

	void init(std::string loggerFile);

	void logEvent(std::string subsystemName, std::string message);

private:
	std::string m_filename;
};

