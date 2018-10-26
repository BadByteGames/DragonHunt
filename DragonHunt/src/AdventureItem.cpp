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
	Event drop;
	Event use;
	Event useItem; //<- Different from use (other item)
	Event enter;
	Event open;
	Event push;
	Event pull;

	//add default sequence items
	player->addSequenceItems(&pickup);
	player->addSequenceItems(&inspect);
	player->addSequenceItems(&drop);
	player->addSequenceItems(&use);
	player->addSequenceItems(&useItem);
	player->addSequenceItems(&enter);
	player->addSequenceItems(&open);
	player->addSequenceItems(&push);
	player->addSequenceItems(&pull);

	//add item specific statements
	pickup.addStatementPossibility("take", new Take(player, this));
	inspect.addStatementPossibility("take", new Take(player, this));
	drop.addStatementPossibility("take", new Take(player, this));
	use.addStatementPossibility("take", new Take(player, this));
	useItem.addStatementPossibility("take", new Take(player, this));
	enter.addStatementPossibility("take", new Take(player, this));
	open.addStatementPossibility("take", new Take(player, this));
	push.addStatementPossibility("take", new Take(player, this));
	pull.addStatementPossibility("take", new Take(player, this));

	pickup.addStatementPossibility("drop", new Drop(player, this));
	inspect.addStatementPossibility("drop", new Drop(player, this));
	drop.addStatementPossibility("drop", new Drop(player, this));
	use.addStatementPossibility("drop", new Drop(player, this));
	useItem.addStatementPossibility("drop", new Drop(player, this));
	enter.addStatementPossibility("drop", new Drop(player, this));
	open.addStatementPossibility("drop", new Drop(player, this));
	push.addStatementPossibility("drop", new Drop(player, this));
	pull.addStatementPossibility("drop", new Drop(player, this));

	this->addEvent("inspect", inspect);
	this->addEvent("pickup", pickup);
	this->addEvent("drop", drop);
	this->addEvent("use", use);
	this->addEvent("useitem", useItem);
	this->addEvent("enter", enter);
	this->addEvent("open", open);
	this->addEvent("push", push);
	this->addEvent("pull", pull);
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

SequenceItem * Take::create()
{
	return new Take(m_player,m_adventureItem->getAttribute("name"));
}

int Take::onCall()
{
	m_player->giveItem(m_parentName);
	return 0;
}

Drop::Drop(Player * player, AdventureItem* parent) :m_player(player), m_adventureItem(parent)
{
}

Drop::Drop(Player * player, std::string parentName) : m_player(player), m_parentName(parentName)
{
}

Drop::~Drop()
{
}

SequenceItem * Drop::create()
{
	return new Drop(m_player, m_adventureItem->getAttribute("name"));
}

int Drop::onCall()
{
	m_player->dropItem(m_parentName);
	return 0;
}