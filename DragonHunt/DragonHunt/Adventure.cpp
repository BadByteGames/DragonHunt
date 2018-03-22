#include "Adventure.h"
#include <iostream>

#include "Logger.h"

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

	if (this->parseFromElement(m_doc.FirstChildElement())) {
		Logger::logEvent("Adventure", "Adventure parsing failed");
	}

	this->destroy();
}

void Adventure::onChildParsed(std::string name, XMLHandler * child)
{
}
