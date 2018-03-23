#pragma once

#include <string>
#include <unordered_map>

class Player
{
public:
	Player();
	~Player();

	//the player class will be managing triggers and event calling
	void triggerEvent(std::string name);

	//mostly called by events to check if a certain thing was triggered
	bool wasTriggered(std::string name);

private:
	//map of all the GLOBAL triggers (not prefixed by !)
	//name, wasTriggered
	std::unordered_map<std::string, bool> m_triggers;
};

