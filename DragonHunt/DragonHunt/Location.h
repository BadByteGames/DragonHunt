#pragma once

#include "XMLHandler.h"
#include "AdventureItem.h"

class Player;

class LocationDescription : public XMLHandler {

public:
	LocationDescription();
	~LocationDescription();

	virtual void onChildParsed(std::string name, XMLHandler * child);
};

class Location : public XMLHandler
{
public:
	Location();
	~Location();

	virtual void onChildParsed(std::string name, XMLHandler * child);

	void setVisited() { m_visited = true; }
	
	bool wasEventDefinedForItem(std::string name, std::string eventName);

	void executeEventForItem(std::string name, std::string eventName);

	bool hasItem(std::string name);

	void setupSequences(Player* player);

	bool getVisited() const { return m_visited; }

	std::string getDescription() const { return m_description; }

private:
	std::string m_description;

	std::unordered_map<std::string, AdventureItem> m_items;

	bool m_visited = false;
};

