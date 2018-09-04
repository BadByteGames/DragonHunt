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

//class that takes in name and repalces it with value
class Synonym : public XMLHandler
{
public:
	Synonym();
	~Synonym();
};

class Adventure : public XMLHandler
{
friend class Player;
public:
	Adventure();
	~Adventure();

	void loadFromFile(std::string originFile);

	void callTrigger(std::string name);

	virtual void onChildParsed(std::string name, XMLHandler * child);
private:
	void parserLoop();

	void setCurrentLocation(std::string name, Location* location);

	XMLDocument m_doc;
	std::vector<std::string> m_lines;

	std::unordered_map<std::string, Location> m_locations;

	//name, value
	std::unordered_map<std::string, std::string> m_synonyms;

	Location* m_currentLocation;

	Player m_player;
};

