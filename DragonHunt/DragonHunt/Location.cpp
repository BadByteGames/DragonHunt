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
	//init events
	Event goDirection;
	Event trigger;
	Event arriveFirstTime;
	Event arrive;
	Event leave;

	//add sequence jtems
	player->addSequenceItems(&goDirection);
	player->addSequenceItems(&trigger);
	player->addSequenceItems(&arriveFirstTime);
	player->addSequenceItems(&arrive);
	player->addSequenceItems(&leave);

	//add item to adventure
	this->addEvent("arrivefirsttime", arriveFirstTime);
	this->addEvent("arrive", arrive);
	this->addEvent("leave", leave);
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