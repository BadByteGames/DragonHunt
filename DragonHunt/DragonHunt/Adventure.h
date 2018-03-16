#pragma once
#include <string>
#include <tinyxml2\tinyxml2.h> // xml stuff

#ifndef FORMATVERSION
#define FORMATVERSION "1.0"
#endif // ! FORMATVERSION


using namespace tinyxml2;
class Adventure
{
public:
	Adventure();
	~Adventure();

	void loadFromFile(std::string originFile);

private:
	XMLDocument m_doc;
	std::string m_name;
	std::string m_startLocation;

};

