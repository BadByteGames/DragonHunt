#include "Location.h"

#include <iostream>

Location::Location()
{
}


Location::~Location()
{
}

void Location::onChildParsed(std::string name, XMLHandler * child)
{
	//print out the description for debug
	if (name == "description") {
		std::cout << child->getText() << std::endl;
	}
}

LocationDescription::LocationDescription()
{
}

LocationDescription::~LocationDescription()
{
}

void LocationDescription::onChildParsed(std::string name, XMLHandler * child)
{
	//can ONLY have item desc so replace it with a special signifier
	if (name == "itemdesc") {
		m_text += "%item:" + child->getAttribute("name") + "%";
	}
}

LocationItemDesc::LocationItemDesc()
{
}

LocationItemDesc::~LocationItemDesc()
{
}

void LocationItemDesc::onChildParsed(std::string name, XMLHandler * child)
{
	//can't have children so do nothing
}
