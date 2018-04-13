#pragma once

#include "XMLHandler.h"

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
	
	void setupSequences(Player* player);

	bool getVisited() const { return m_visited; }

	std::string getDescription() const { return m_description; }

private:
	std::string m_description;

	bool m_visited = false;
};

