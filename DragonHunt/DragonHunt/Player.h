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

class Suicide : public SequenceItem {
public:
	Suicide(Player* player);
	~Suicide();

	virtual Statement* create() override;
	virtual int onCall() override;

private:
	Player * m_player;
};

class Triggered : public ControlGroup {
public:
	Triggered(Player* player);
	~Triggered();
	
	virtual Statement* create() override;
	virtual bool isTrue() override;

private:
	Player * m_player;
};

class AtLocation : public ControlGroup {
public:
	AtLocation(Player* player);
	~AtLocation();

	virtual Statement* create() override;
	virtual bool isTrue() override;

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

	//checks wheter or not the current location is this
	bool atLocation(std::string name);

	//adds all default sequenceitem
	void addSequenceItems(Event* evnt);

	void setMacro(std::string name, std::string value);

	void gotToLocation(std::string name);

	void die();

	std::string evaluateMacros(std::string original);

	bool isDead() const { return m_isDead; }

private:
	//map of all the GLOBAL triggers (not prefixed by !)
	//name, wasTriggered
	std::unordered_map<std::string, bool> m_triggers;

	//a map storing the values of all the macros
	//name, value
	std::unordered_map<std::string, std::string> m_macros;

	//wheter or not the player is dead
	bool m_isDead;

	Adventure* m_adv;
};

