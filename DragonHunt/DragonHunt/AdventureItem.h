#pragma once
#include "XMLHandler.h"

class Player;

class ItemDescription : public XMLHandler {
public:
	ItemDescription();
	~ItemDescription();

};

class AdventureItem : public XMLHandler
{
public:
	AdventureItem();
	~AdventureItem();

	virtual void onChildParsed(std::string name, XMLHandler * child);

	void setupSequenceItems(Player* player);

	std::string getDescription() const { return m_description; }

private:
	std::string m_description = "";
};

