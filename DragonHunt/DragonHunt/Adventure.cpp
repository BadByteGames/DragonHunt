#include "Adventure.h"
#include <iostream>

#include "Logger.h"
#include "Location.h"
#include "CCHandler.h"

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
		std::cout << "An error occurred, check runtime.log for details." << std::endl;
		Logger::logEvent("error", "Element \"adventure\" was not found.");
		return;
	}

	//add attribute name, version, and start
	this->addAttribute("name", true);
	this->addAttribute("version", true);
	this->addAttribute("start", true);

	this->addChild("location", new Location(), XMLChildFlag::MULTIPLE | XMLChildFlag::REQUIRED);
	this->addChild("cc", new CCHandler());

	//create events
	Event autoCall;
	Event trigger;

	//make sure to add trigger sequence item
	autoCall.addSequencePossibility("trigger", new Trigger(&m_player, this));
	trigger.addSequencePossibility("trigger", new Trigger(&m_player, this));

	this->addEvent("autocall", autoCall);
	this->addEvent("trigger", trigger);

	if (this->parseFromElement(m_doc.FirstChildElement())) {
		Logger::logEvent("Adventure", "Adventure parsing failed");
	}
	else {
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
	}

	if(wasEventDefined("autocall"))
		this->executeEvent("autocall");

	this->destroy();
}

void Adventure::callTrigger(std::string name)
{
	if (wasEventDefined("trigger", "triggername:" + name)) {
		executeEvent("trigger", "triggername:" + name);
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
}

Trigger::Trigger(Player* player,Adventure* adventure):m_player(player),m_adv(adventure)
{
	requireArgument("triggername");
}

Trigger::~Trigger()
{
}

SequenceItem * Trigger::create()
{
	return new Trigger(m_player, m_adv);
}

int Trigger::onCall()
{
	m_player->triggerEvent(getArgument("triggername"),m_adv);
	return 0;
}
