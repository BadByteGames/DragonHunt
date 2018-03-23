#include "CCHandler.h"

#include <iostream>

CCHandler::CCHandler()
{
}


CCHandler::~CCHandler()
{
}

void CCHandler::onChildParsed(std::string name, XMLHandler * child)
{
	//if it is li, add its values
	if (name == "li") {
		m_lines.push_back(child->getText());
	}
}

li::li()
{
}

li::~li()
{
}

void li::onChildParsed(std::string name, XMLHandler * child)
{
	//can't have children so do nothing
}
