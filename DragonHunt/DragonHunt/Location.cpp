#include "Location.h"

#include <iostream>

Location::Location()
{
	this->addChild("description", new LocationDescription(), XMLChildFlag::REQUIRED | XMLChildFlag::USESTEXT);

	this->addAttribute("name", true);

	Event goDirection;
	this->addEvent("godirection", goDirection);
}


Location::~Location()
{
}

void Location::onChildParsed(std::string name, XMLHandler * child)
{
	//set the location's description
	if (name == "description") {
		m_description = child->getText();
	}
	
}

LocationDescription::LocationDescription()
{	
	this->addChild("itemdesc", new LocationItemDesc(), XMLChildFlag::MULTIPLE | XMLChildFlag::REQUIRED);
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
	this->addAttribute("name", true);
}

LocationItemDesc::~LocationItemDesc()
{
}

void LocationItemDesc::onChildParsed(std::string name, XMLHandler * child)
{
	//can't have children so do nothing
}
