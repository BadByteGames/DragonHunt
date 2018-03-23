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
	//if it is li, print out its text
	if (name == "li") {
		std::cout << child->getText() << std::endl;
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
