#pragma once
#include "XMLHandler.h"

class Player;
class AdventureItem;

class Take : public SequenceItem {
public:
	Take(Player * player, AdventureItem* parent);
	Take(Player * player, std::string parentName);
	~Take();

	virtual Statement* create() override;
	virtual int onCall() override;

private:
	Player*m_player;
	AdventureItem*m_adventureItem;
	std::string m_parentName;
};

class Drop : public SequenceItem {
public:
	Drop(Player * player, AdventureItem* parent);
	Drop(Player * player, std::string parentName);
	~Drop();

	virtual Statement* create() override;
	virtual int onCall() override;

private:
	Player * m_player;
	AdventureItem*m_adventureItem;
	std::string m_parentName;
};


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

