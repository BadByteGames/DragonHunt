#include "XMLHandler.h"

#include "Logger.h"

#include <iostream>

XMLHandler::XMLHandler()
{
}


XMLHandler::~XMLHandler()
{
}

void XMLHandler::addAttribute(std::string name, bool required)
{
	//add attributes
	m_attributeRules.insert(std::make_pair(name, required));
}

void XMLHandler::addChild(std::string name, XMLHandler * child, unsigned int childFlags)
{
}

std::string XMLHandler::getAttribute(std::string name)
{
	//find attribute name
	auto it = m_attributes.find(name);
	if (it != m_attributes.end()) return it->second;

	//return NULL by default
	return NULL;
}

void XMLHandler::destroy()
{
	//delete the children "rules"
	for (auto it : m_childrenHandlers) {
		//destroy all its children then delete the child
		it.second->destroy();
		delete it.second;
	}

	//delete the actual children
	for (auto it : m_children) {
		it.second->destroy();
		delete it.second;
	}
}

int XMLHandler::parseFromElement(tinyxml2::XMLElement * root)
{
	Logger::logEvent("XMLHandler", "began parsing element " + std::string(root->Name()) +" at line "+std::to_string(root->GetLineNum()));
	//populate attributes
	if(populateAttributes(root)) return 1;
	Logger::logEvent("XMLHandler", "finished parsing element " + std::string(root->Name()));

	return 0;
}

int XMLHandler::populateAttributes(tinyxml2::XMLElement * elementToParse)
{
	//loop through attributes
	for (auto it = m_attributeRules.begin(); it != m_attributeRules.end(); it++) {
		//gets attribute
		const char * val = elementToParse->Attribute(it->first.c_str());
		if (val == NULL && it->second) {
			std::cout << "An error occurred, please check runtime.log for details" << std::endl;
			Logger::logEvent("error", "expected attribute \"" + it->first + "\" at line " + std::to_string(elementToParse->GetLineNum()) + " (" +elementToParse->Name()+")");
			return 1;
		}
		else {
			Logger::logEvent("XMLHandler", it->first + " = " + val);
			m_attributes.insert(std::make_pair(it->first, val));
		}
	}

	return 0;
}

int XMLHandler::populateChildren(tinyxml2::XMLElement * elementToParse)
{
	return 0;
}
