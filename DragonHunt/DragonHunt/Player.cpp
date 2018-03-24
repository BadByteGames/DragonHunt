#include "Player.h"

#include "Logger.h"

Player::Player()
{
}


Player::~Player()
{
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
		}
		else {
			m_triggers.insert(std::make_pair(name, true));
			Logger::logEvent("player", name + " triggered");
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
