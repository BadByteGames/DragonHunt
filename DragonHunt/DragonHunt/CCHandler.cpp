#include "CCHandler.h"

#include <iostream>

CCHandler::CCHandler()
{
	this->addChild("li", new li(), XMLChildFlag::MULTIPLE | XMLChildFlag::REQUIRED | XMLChildFlag::USESTEXT);
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
