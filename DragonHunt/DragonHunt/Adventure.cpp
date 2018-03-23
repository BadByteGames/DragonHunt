#include "Adventure.h"
#include <iostream>

#include "Logger.h"
#include "Location.h"
#include "CCHandler.h"

Adventure::Adventure()
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
		std::cout << "Element \"adventure\" was not found." << std::endl;
		Logger::logEvent("Adventure", "Element \"adventure\" was not found.");
		return;
	}

	//this is already a hander so its fine

	//add attribute name, version, and start
	this->addAttribute("name", true);
	this->addAttribute("version", true);
	this->addAttribute("start", true);

	Location* locationHandler = new Location();
	locationHandler->addAttribute("name", true);

	CCHandler* ccHandler = new CCHandler();
	ccHandler->addChild("li", new li(), XMLChildFlag::MULTIPLE | XMLChildFlag::REQUIRED | XMLChildFlag::USESTEXT);

	this->addChild("location", locationHandler, XMLChildFlag::MULTIPLE | XMLChildFlag::REQUIRED);
	this->addChild("cc", ccHandler);

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
			std::cout << "By pressing ENTER you accept this license and acknoledge reading it." << std::endl;
			std::string filler;
			std::getline(std::cin, filler);
		}
	}

	this->destroy();
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
