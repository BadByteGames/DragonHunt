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

	//add item specific statements
	pickup.addStatementPossibility("take", new Take(player, this));
	inspect.addStatementPossibility("take", new Take(player, this));

	this->addEvent("inspect", inspect);
	this->addEvent("pickup", pickup);
}

ItemDescription::ItemDescription()
{
}

ItemDescription::~ItemDescription()
{
}

Take::Take(Player * player, AdventureItem* parent):m_player(player), m_adventureItem(parent)
{
}

Take::Take(Player * player, std::string parentName) : m_player(player), m_parentName(parentName)
{
}

Take::~Take()
{
}

Statement * Take::create()
{
	return new Take(m_player,m_adventureItem->getAttribute("name"));
}

int Take::onCall()
{
	m_player->giveItem(m_parentName);
	return 0;
}
