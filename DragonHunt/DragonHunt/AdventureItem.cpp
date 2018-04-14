#include "AdventureItem.h"

#include "Player.h"

AdventureItem::AdventureItem()
{
	this->addChild("description", new ItemDescription(), XMLChildFlag::REQUIRED | XMLChildFlag::USESTEXT);

	this->addAttribute("name", true);
}


AdventureItem::~AdventureItem()
{
}

void AdventureItem::onChildParsed(std::string name, XMLHandler * child)
{
	if (name == "description") {
		m_description = child->getText();
	}
}

void AdventureItem::setupSequenceItems(Player * player)
{
	//create all the events
	Event pickup;
	Event inspect;

	player->addSequenceItems(&pickup);
	player->addSequenceItems(&inspect);

	this->addEvent("inspect", inspect);
	this->addEvent("pickup", pickup);
}

ItemDescription::ItemDescription()
{
}

ItemDescription::~ItemDescription()
{
}
