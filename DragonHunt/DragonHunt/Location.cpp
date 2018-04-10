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