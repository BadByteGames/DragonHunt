#pragma once

#include "XMLHandler.h"

class Player;

class LocationItemDesc : public XMLHandler {
public:
	LocationItemDesc();
	~LocationItemDesc();

	virtual void onChildParsed(std::string name, XMLHandler * child);
};

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

	void setupSequences(Player* player);

	std::string m_description;
};

