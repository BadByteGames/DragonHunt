#include "Location.h"

#include "Player.h"
#include "AdventureItem.h"

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
	else if (name == "item") {
		AdventureItem a = (AdventureItem)*((AdventureItem*)child);
		m_items.insert(std::make_pair(a.getAttribute("name"), a));
	}
	
}

bool Location::wasEventDefinedForItem(std::string name, std::string eventName)
{
	auto it = m_items.find(name);
	if (it != m_items.end() && it->second.wasEventDefined(eventName))
		return true;
	return false;
}

void Location::executeEventForItem(std::string name, std::string eventName)
{
	auto it = m_items.find(name);
	if (it != m_items.end() && it->second.wasEventDefined(eventName))
		it->second.executeEvent(eventName);
}

bool Location::hasItem(std::string name)
{
	return m_items.find(name) != m_items.end();
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

	//also setup items sequence items
	auto i = new AdventureItem();
	i->setupSequenceItems(player);
	this->addChild("item", i, XMLChildFlag::MULTIPLE);
}

AdventureItem Location::getItem(std::string name)
{
	auto it = m_items.find(name);
	if (it != m_items.end())
		return it->second;
	return AdventureItem();
}

LocationDescription::LocationDescription()
{	
}

LocationDescription::~LocationDescription()
{
}

void LocationDescription::onChildParsed(std::string name, XMLHandler * child)
{
}