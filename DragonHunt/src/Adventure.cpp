#include "Adventure.h"
#include <iostream>

#include "Logger.h"
#include "Location.h"
#include "CCHandler.h"
#include "Parser.h"

Adventure::Adventure():m_player()
{
}


Adventure::~Adventure()
{
}

void Adventure::loadFromFile(std::string originFile)
{
	//open the file
	m_doc.LoadFile(originFile.c_str());

	if (m_doc.FirstChildElement("adventure") == NULL) {
		Logger::logEvent("error", "Element \"adventure\" was not found.");
		return;
	}

	m_player.init(this);

	//add attribute name, version, and start
	this->addAttribute("name", true);
	this->addAttribute("version", true);
	this->addAttribute("start", true);

	auto l = new Location();
	l->setupSequences(&m_player);
	this->addChild("location", l, XMLChildFlag::MULTIPLE | XMLChildFlag::REQUIRED);
	this->addChild("cc", new CCHandler());
	this->addChild("synonym", new Synonym(), XMLChildFlag::MULTIPLE);

	//create events
	Event autoCall;
	Event trigger;
	Event onDeath;
	Event useItem;

	//make sure to sequence items
	m_player.addSequenceItems(&autoCall);
	m_player.addSequenceItems(&trigger);
	m_player.addSequenceItems(&onDeath);
	m_player.addSequenceItems(&useItem);

	this->addEvent("begin", autoCall);
	this->addEvent("trigger", trigger);
	this->addEvent("ondeath", onDeath);
	this->addEvent("useitem", useItem);
	

	if (this->parseFromElement(m_doc.FirstChildElement())) {
		Logger::logEvent("error", "XML parsing failed");
	}
	else {
		//begin game
		if (m_locations.find(getAttribute("start")) == m_locations.end()) {
			Logger::logEvent("error", "Start location does not exist.");
			std::string a;
			std::getline(std::cin, a);
			return;
		}

		//create initial synonyms
		m_synonyms.insert(std::make_pair("pick up", "take"));
		m_synonyms.insert(std::make_pair("pull open", "open"));
		m_synonyms.insert(std::make_pair("push open", "open"));

		//removes these words
		m_synonyms.insert(std::make_pair("the", ""));

		//print out cc
		for (auto li : m_lines) {
			Logger::logEvent("cc", li);
			std::cout << li << std::endl;
		}
		if (m_lines.size() > 0) {
			std::cout << "By pressing ENTER you accept this license and acknowledge reading it." << std::endl;
			std::string filler;
			std::getline(std::cin, filler);
		}

		//we already know it exists
		auto l_pair = m_locations.find(getAttribute("start"));
		setCurrentLocation(l_pair->first, &l_pair->second);
		
		if (wasEventDefined("begin"))
			this->executeEvent("begin");

		parserLoop();
	}
}

void Adventure::callTrigger(std::string name)
{
	if (wasEventDefined("trigger", "triggername:" + name)) {
		executeEvent("trigger", "triggername:" + name);
	}
	//attempt to call it in the current location
	if (m_currentLocation->wasEventDefined("trigger", "triggername:" + name)) {
		m_currentLocation->executeEvent("trigger", "triggername:" + name);
	}
}

void Adventure::onChildParsed(std::string name, XMLHandler * child)
{
	//add cc handler if detected
	if (name == "cc") {
		//safe cast
		m_lines = ((CCHandler *)child)->getLines();
	}
	else if (name == "location") {
		//create a location object then push back
		Location l = (Location)*((Location*)child);
		m_locations.insert(std::make_pair(l.getAttribute("name"), l));
	}
	else if (name == "synonym") {
		//insert synonym
		m_synonyms.insert(std::make_pair(child->getAttribute("name"), child->getAttribute("value")));
	}
}

void Adventure::parserLoop()
{
	std::cout << "Please type \"quit\" in order to deallocate memory when quitting" << std::endl;
	bool shouldContinue = true;
	while (shouldContinue) {
		std::string input = "";
		std::cout << ">";
		std::getline(std::cin, input);
		Logger::logEvent("user", ">"+input);
		
		//replace synonyms
		for (auto s : m_synonyms) {
			size_t synonymLocation = input.find(s.first);
			if (synonymLocation != input.npos) {
				input.replace(synonymLocation, s.first.length(), s.second);
			}
		}

		Logger::logEvent("Adventure", "Received input: "+input);
		
		//check wheter user wants to quit
		if (input == "quit" || input == "exit" || m_player.isDead()) {
			shouldContinue = false;
		}
		else if(input!=""){
			//otherwise, parse the data

			//create extra tokens based off scene
			std::vector<TOKENPAIR> extraTokens;

			Parser p;
			auto results = p.parse(input, extraTokens);
			if (results.size() >= 1 && results[0].type != TOKEN::VERB) {
				std::cout << "I don't recognize that verb." << std::endl;
				Logger::logEvent("Adventure", "Unknown verb "+ results[0].value);
			}
			else if (results[0].value == "go") {
				if (results.size() >= 2 && !(m_currentLocation->wasEventDefined("godirection", "name:" + results[1].value))) {
					std::cout << "You can't go there." << std::endl;
					Logger::logEvent("Adventure", "Unknown direction \"" + results[1].value + "\"");
				}
				else if(results.size() >= 2) {
					m_currentLocation->executeEvent("godirection", "name:" + results[1].value);
				}
				else {
					std::cout << "I only understood you as far as wanting to go somewhere." << std::endl;
					Logger::logEvent("Adventure", "Direction not specified");
				}
			}
			else if (results[0].value == "take") {
				if (results.size() >= 2 && m_currentLocation->wasEventDefinedForItem(results[1].value, "pickup")) {
					m_currentLocation->executeEventForItem(results[1].value, "pickup");
				}
				else if (results.size() >= 2 && m_currentLocation->hasItem(results[1].value)) {
					std::cout << "You can't take that." << std::endl;
					Logger::logEvent("Adventure", "Item pickup event not defined. Skipping take.");
				}
				else {
					std::cout << "I see no such thing" << std::endl;
					Logger::logEvent("Adventure", "Item non-existant at location or not specified");
				}
			}
			else if (results[0].value == "drop") {
				if (results.size() >= 2 && m_player.hasItem(results[1].value)) {
					//execute drop event
					if (m_player.getItem(results[1].value).wasEventDefined("drop")) {
						m_player.getItem(results[1].value).executeEvent("drop");
					}
					else {
						//just drop the item
						m_player.dropItem(results[1].value);
					}
				}
				else if(results.size() >= 2) {
					std::cout << "You're not holding that item." << std::endl;
					Logger::logEvent("Adventure", "Item not held");
				}
				else {
					std::cout << "I only understood you as far as wanting to drop something." << std::endl;
				}
			}
			else if (results[0].value == "inspect") {
				if (results.size() >= 2 && m_currentLocation->hasItem(results[1].value)) {
					//check if inspect event defined, if not print description
					if (m_currentLocation->wasEventDefinedForItem(results[1].value, "inspect")) {
						m_currentLocation->executeEventForItem(results[1].value, "inspect");
					}
					else {
						std::cout << m_currentLocation->getItem(results[1].value).getDescription() << std::endl;
					}
				}
				else {
					std::cout << "I see no such thing" << std::endl;
					Logger::logEvent("Adventure", "Item non-existant at location or not specified");
				}
			}
			else if (results[0].value == "use") {
				if(results.size() >= 2 && m_player.hasItem(results[1].value)) {
					//check if "on" was used
					if (results.size() >= 3 && results[2].value == "on") {
						if (results.size() >= 4) {
							//attempt to use the clippers
							if (results[3].value == "me" && this->wasEventDefined("useitem","itemname:"+results[1].value)) {
								//use clippers on player
								this->executeEvent("useitem", "itemname:" + results[1].value);
							}
							else if (m_currentLocation->hasItem(results[3].value) && m_currentLocation->wasEventDefinedForItem(results[3].value,"useitem%itemname:" + results[1].value)) {
								//use on item at location
								m_currentLocation->executeEventForItem(results[3].value, "useitem%itemname:" + results[1].value);
							}
							else if (m_player.hasItem(results[3].value) && m_player.getItem(results[3].value).wasEventDefined("useitem", "itemname:" + results[1].value)) {
								//use on item in inventory
								m_player.getItem(results[3].value).executeEvent("useitem", "itemname:" + results[1].value);
							}
							else {
								std::cout << "You can't do that." << std::endl;
							}
						}
						else {
							std::cout << "I only understood you as far as wanting to use the " << results[1].value << " on something." << std::endl;
						}
					}else
					//execute use event
					if (m_player.getItem(results[1].value).wasEventDefined("use")) {
						m_player.getItem(results[1].value).executeEvent("use");
					}
					else {
						//print an error
						std::cout << "You can't use that item." << std::endl;
						Logger::logEvent("Adventure", "Use event not defined.");
					}
				}
				else if (results.size() >= 2) {
					std::cout << "You're not holding that item." << std::endl;
					Logger::logEvent("Adventure", "Item not held");
				}else {
					std::cout << "I only understood you as far as wanting to use something." << std::endl;
				}
			}
			else if (results[0].value == "enter") {
				if (results.size() >= 2 && m_currentLocation->wasEventDefinedForItem(results[1].value, "enter")) {
					m_currentLocation->executeEventForItem(results[1].value, "enter");
				}
				else if (results.size() >= 2 && m_currentLocation->hasItem(results[1].value)) {
					std::cout << "You can't enter that." << std::endl;
					Logger::logEvent("Adventure", "Item enter event not defined. Skipping enter.");
				}
				else {
					std::cout << "I see no such thing" << std::endl;
					Logger::logEvent("Adventure", "Item non-existant at location or not specified");
				}
			}
			else if (results[0].value == "open") {
				if (results.size() >= 2 && m_currentLocation->wasEventDefinedForItem(results[1].value, "open")) {
					m_currentLocation->executeEventForItem(results[1].value, "open");
				}
				else if (results.size() >= 2 && m_currentLocation->hasItem(results[1].value)) {
					std::cout << "You can't open that." << std::endl;
					Logger::logEvent("Adventure", "Item open event not defined. Skipping open.");
				}
				else {
					std::cout << "I see no such thing" << std::endl;
					Logger::logEvent("Adventure", "Item non-existant at location or not specified");
				}
			}
			else if (results[0].value == "push") {
				if (results.size() >= 2 && m_currentLocation->wasEventDefinedForItem(results[1].value, "push")) {
					m_currentLocation->executeEventForItem(results[1].value, "push");
				}
				else if (results.size() >= 2 && m_currentLocation->hasItem(results[1].value)) {
					std::cout << "You can't push that." << std::endl;
					Logger::logEvent("Adventure", "Item push event not defined. Skipping push.");
				}
				else {
					std::cout << "I see no such thing" << std::endl;
					Logger::logEvent("Adventure", "Item non-existant at location or not specified");
				}
			}
			else if (results[0].value == "pull") {
				if (results.size() >= 2 && m_currentLocation->wasEventDefinedForItem(results[1].value, "pull")) {
					m_currentLocation->executeEventForItem(results[1].value, "pull");
				}
				else if (results.size() >= 2 && m_currentLocation->hasItem(results[1].value)) {
					std::cout << "You can't pull that." << std::endl;
					Logger::logEvent("Adventure", "Item pull event not defined. Skipping pull.");
				}
				else {
					std::cout << "I see no such thing" << std::endl;
					Logger::logEvent("Adventure", "Item non-existant at location or not specified");
				}
			}
		}

	}
}

void Adventure::setCurrentLocation(std::string name, Location * location)
{
	//trigger the current location's leave event
	if (m_currentLocation != nullptr && m_currentLocation->wasEventDefined("leave")) {
		m_currentLocation->executeEvent("leave");
	}

	//check out if the new location has an arrive and arrivefirsttime event
	if ((!location->getVisited()) && location->wasEventDefined("arrivefirsttime")) {
		location->executeEvent("arrivefirsttime");
		location->setVisited();
	}

	if (location->wasEventDefined("arrive")) {
		location->executeEvent("arrive");
	}

	//print out name and desription
	std::cout << "\n" <<m_player.evaluateMacros(name)<<"\n" << std::endl;
	std::cout << m_player.evaluateMacros(location->getDescription()) << "\n"  << std::endl;

	//change current location
	m_currentLocation = location;
}

Synonym::Synonym()
{
	this->addAttribute("name", true);
	this->addAttribute("value", true);
}

Synonym::~Synonym()
{
}
