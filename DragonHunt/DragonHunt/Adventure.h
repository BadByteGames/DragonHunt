#pragma once
#include <string>
#include <tinyxml2\tinyxml2.h> // xml stuff

#include "XMLHandler.h"//custom xml stuff

#include "Location.h" //locations
#include "Player.h"

#ifndef FORMATVERSION
#define FORMATVERSION "1.0"
#endif // ! FORMATVERSION


using tinyxml2::XMLDocument;
using tinyxml2::XMLComment;
using tinyxml2::XMLElement;
using tinyxml2::XMLNode;

class Trigger : public SequenceItem {
public:
	Trigger(Player* player, Adventure* adv);
	~Trigger();

	virtual SequenceItem* create() override;
	virtual int onCall() override;

private:
	Player * m_player;
	Adventure * m_adv;
};

class Adventure : public XMLHandler
{
public:
	Adventure();
	~Adventure();

	void loadFromFile(std::string originFile);

	void callTrigger(std::string name);

	virtual void onChildParsed(std::string name, XMLHandler * child);
private:

	XMLDocument m_doc;
	std::vector<std::string> m_lines;

	std::unordered_map<std::string, Location> m_locations;

	Player m_player;
};

