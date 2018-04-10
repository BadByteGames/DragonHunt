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

Print::Print()
{
	requireArgument("text");
}

Print::~Print()
{
}

SequenceItem * Print::create()
{
	Print* p = new Print();
	p->copyArgs(this);
	return p;
}

int Print::onCall()
{
	std::cout << this->getArgument("text") << std::endl;
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

void Player::addSequenceItems(Event * evnt)
{
	evnt->addSequencePossibility("print", new Print());
	evnt->addSequencePossibility("trigger", new Trigger(this));
}
