#include "Player.h"

#include "Logger.h"
#include "Adventure.h"
#include <iostream>

Trigger::Trigger(Player* player) :m_player(player)
{
	requireArgument("triggername");
}

Trigger::~Trigger()
{
}

SequenceItem * Trigger::create()
{
	return new Trigger(m_player);
}

int Trigger::onCall()
{
	m_player->triggerEvent(getArgument("triggername"));
	return 0;
}

Print::Print(Player* player) :m_player(player)
{
	requireArgument("text");
}

Print::~Print()
{
}

SequenceItem * Print::create()
{
	Print* p = new Print(m_player);
	return p;
}

int Print::onCall()
{
	std::cout << m_player->evaluateMacros(this->getArgument("text")) << std::endl;
	Logger::logEvent("print", m_player->evaluateMacros(this->getArgument("text")));
	return 0;
}

Player::Player()
{
}


Player::~Player()
{
}

void Player::init(Adventure * adv)
{
	m_adv = adv;
}

void Player::triggerEvent(std::string name)
{
	//checks if the trigger is local or not
	if (name.length() > 0 && name[0] == '!') {
		//destination trigger is local
	}
	else {
		//create a global trigger event in the m_triggers if not already existing
		auto it = m_triggers.find(name);
		if (it != m_triggers.end()) {
			//already exists
			it->second = true;
			Logger::logEvent("player", name + " triggered");
			m_adv->callTrigger(name);
		}
		else {
			m_triggers.insert(std::make_pair(name, true));
			Logger::logEvent("player", name + " triggered");
			m_adv->callTrigger(name);
		}
	}
}

bool Player::wasTriggered(std::string name)
{
	//checks if global trigger event has been triggered and if not, creates a new trigger
	auto it = m_triggers.find(name);
	if (it != m_triggers.end()) {
		//already exists, just return value
		return it->second;
	}
	else {
		//create a new trigger of that name
		m_triggers.insert(std::make_pair(name, false));

		return false;
	}
}

bool Player::atLocation(std::string name)
{
	return name==m_adv->m_currentLocation->getAttribute("name");
}

void Player::addSequenceItems(Event * evnt)
{
	evnt->addStatementPossibility("print", new Print(this));
	evnt->addStatementPossibility("trigger", new Trigger(this));
	evnt->addStatementPossibility("setmacro", new SetMacro(this));
	evnt->addStatementPossibility("gotolocation", new GoToLocation(this));
	evnt->addStatementPossibility("suicide", new Suicide(this));

	evnt->addStatementPossibility("triggered", new Triggered(this));
	evnt->addStatementPossibility("atlocation", new AtLocation(this));
}

void Player::setMacro(std::string name, std::string value)
{
	//proper macro name
	std::string macroName = "%" + name + "%";
	
	//check if that macro already exists
	auto macro = m_macros.find(macroName);
	if (macro != m_macros.end()) {
		//macro already exists so change its value
		macro->second = value;
	}
	else {
		//insert a new macro
		m_macros.insert(std::make_pair(macroName, value));
	}
}

void Player::gotToLocation(std::string name)
{
	auto l_pair = m_adv->m_locations.find(name);
	if (l_pair != m_adv->m_locations.end()) {
		m_adv->setCurrentLocation(l_pair->first, &l_pair->second);
	}
}

void Player::die()
{
	m_isDead = true;

	if (m_adv->wasEventDefined("ondeath")) {
		m_adv->executeEvent("ondeath");
	}
	
}

void Player::giveItem(std::string name)
{
	m_items.insert(std::make_pair(name, m_adv->m_currentLocation->getItem(name)));
	m_adv->m_currentLocation->removeItem(name);
}

void Player::dropItem(std::string name) {
	auto it = m_items.find(name);
	if (it != m_items.end()) {
		m_adv->m_currentLocation->addItem(it->first, it->second);
		m_items.erase(name);
	}
}

bool Player::hasItem(std::string name)
{
	return m_items.find(name) != m_items.end();
}

std::string Player::evaluateMacros(std::string original)
{
	std::string returnValue = original;

	//loop through replacing macros
	for (auto macro : m_macros) {
		//find macro in the returnValue
		size_t location = returnValue.find(macro.first);
		if (location != returnValue.npos) {
			returnValue.replace(location, macro.first.length(), macro.second);
		}
	}

	return returnValue;
}

AdventureItem Player::getItem(std::string name)
{
	auto it = m_items.find(name);
	if (it != m_items.end())
		return it->second;
	return AdventureItem();
}

SetMacro::SetMacro(Player * player):m_player(player)
{
	requireArgument("name");
	requireArgument("value");
}

SetMacro::~SetMacro()
{
}

SequenceItem * SetMacro::create()
{
	return new SetMacro(m_player);
}

int SetMacro::onCall()
{
	m_player->setMacro(getArgument("name"), getArgument("value"));
	return 0;
}

GoToLocation::GoToLocation(Player * player) :m_player(player)
{
	requireArgument("name");
}

GoToLocation::~GoToLocation()
{
}

SequenceItem * GoToLocation::create()
{
	return new GoToLocation(m_player);
}

int GoToLocation::onCall()
{
	m_player->gotToLocation(getArgument("name"));
	return 0;
}

Triggered::Triggered(Player * player) : m_player(player)
{
	requireArgument("triggername");
}

Triggered::~Triggered()
{
}

Statement * Triggered::create()
{
	return new Triggered(m_player);
}

bool Triggered::isTrue()
{
	return m_player->wasTriggered(getArgument("triggername"));
}

AtLocation::AtLocation(Player * player):m_player(player)
{
	requireArgument("name");
}

AtLocation::~AtLocation()
{
}

Statement * AtLocation::create()
{
	return new AtLocation(m_player);
}

bool AtLocation::isTrue()
{
	return m_player->atLocation(getArgument("name"));
}

Suicide::Suicide(Player * player):m_player(player)
{
}

Suicide::~Suicide()
{
}

Statement * Suicide::create()
{
	return new Suicide(m_player);
}

int Suicide::onCall()
{
	m_player->die();
	return 0;
}
