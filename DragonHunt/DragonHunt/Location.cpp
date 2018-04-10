#include "Location.h"

#include "Player.h"

#include <iostream>

Location::Location()
{
	this->addChild("description", new LocationDescription(), XMLChildFlag::REQUIRED | XMLChildFlag::USESTEXT);

	this->addAttribute("name", true);

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

void Location::setupSequences(Player * player)
{
	Event goDirection;
	Event trigger;

	player->addSequenceItems(&goDirection);
	player->addSequenceItems(&trigger);

	this->addEvent("godirection", goDirection);
	this->addEvent("trigger", trigger);
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
