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
	Print(Player* player);
	~Print();

	virtual SequenceItem* create() override;
	virtual int onCall() override;

private:
	Player * m_player;
};

class SetMacro : public SequenceItem {
public:
	SetMacro(Player* player);
	~SetMacro();

	virtual SequenceItem* create() override;
	virtual int onCall() override;

private:
	Player * m_player;
};

class GoToLocation : public SequenceItem {
public:
	GoToLocation(Player* player);
	~GoToLocation();

	virtual SequenceItem* create() override;
	virtual int onCall() override;

private:
	Player * m_player;
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

	void setMacro(std::string name, std::string value);

	void gotToLocation(std::string name);

	std::string evaluateMacros(std::string original);

private:
	//map of all the GLOBAL triggers (not prefixed by !)
	//name, wasTriggered
	std::unordered_map<std::string, bool> m_triggers;

	//a map storing the values of all the macros
	//name, value
	std::unordered_map<std::string, std::string> m_macros;

	Adventure* m_adv;
};

