#pragma once

#include "XMLHandler.h"

#include <vector>

class li : public XMLHandler {
public:
	li();
	~li();

	virtual void onChildParsed(std::string name, XMLHandler * child);
};

class CCHandler : public XMLHandler
{
public:
	CCHandler();
	~CCHandler();

	virtual void onChildParsed(std::string name, XMLHandler * child);

	std::vector<std::string> getLines() const { return m_lines; }

private:
	std::vector<std::string> m_lines;
};

