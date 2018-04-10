#pragma once

#include <string>
#include <unordered_map>

#include "Events.h"

class Adventure;
class Player;

class Trigger : public SequenceItem {
public:
	Trigger(Player* player);
	~Trigger();

	virtual SequenceItem* create() override;
	virtual int onCall() override;

private:
	Player * m_player;
};

class Print : public SequenceItem {
public:
	Print();
	~Print();

	virtual SequenceItem* create() override;
	virtual int onCall() override;
};

class Player
{
public:
	Player();
	~Player();

	void init(Adventure* adv);

	//the player class will be managing triggers and event calling
	void triggerEvent(std::string name);

	//mostly called by events to check if a certain thing was triggered
	bool wasTriggered(std::string name);

	//adds all default sequenceitem
	void addSequenceItems(Event* evnt);

private:
	//map of all the GLOBAL triggers (not prefixed by !)
	//name, wasTriggered
	std::unordered_map<std::string, bool> m_triggers;

	Adventure* m_adv;
};

