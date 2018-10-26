#include "Events.h"

#include "Logger.h"
#include <iostream>
#include <string>

Event::Event()
{
}


Event::~Event()
{
}

int Event::parseFromElement(tinyxml2::XMLElement* rootNode)
{
	//clear the previous sequence
	m_sequence.clear();

	//go through all the children of that element
	auto* rootChild = rootNode->FirstChild();
	while (rootChild != NULL) {
		if (rootChild->ToComment() != NULL) {
			//do nothing
		}
		else if (rootChild->ToElement() != NULL) {
			auto* e = rootChild->ToElement();
			//first check if that element is allowed
			auto it = m_statementPossibilities.find(e->Name());
			if (it != m_statementPossibilities.end()) {
				//found it
				//create a copy
				Statement* si = it->second->create();
				
				Logger::logEvent("Event", "Creating sequence item "+it->first+".");

				//now to loop through required arguments
				for (auto cur : si->m_requiredArgs) {
					//get argument from element
					if (e->Attribute(cur.first.c_str()) != NULL) {
						si->m_arguments.insert(std::make_pair(cur.first, e->Attribute(cur.first.c_str())));
						Logger::logEvent("Event", cur.first + " = " + e->Attribute(cur.first.c_str()));
					}
					else {
						Logger::logEvent("error", "Element at line " + std::to_string(rootChild->GetLineNum()) + " missing required argument \""+ cur.first+"\"");
						return 1;
					}
				}
				si->extraParsing(e, m_statementPossibilities);
				m_sequence.push_back(si);
			}
			else {
				Logger::logEvent("error", "Unknown element at line " + std::to_string(rootChild->GetLineNum()));
				return 1;
			}
		}
		else {
			Logger::logEvent("error", "Unknown node at line "+std::to_string(rootChild->GetLineNum()));
			return 1;
		}
		rootChild = rootChild->NextSibling();
	}

	return 0;
}

void Event::addStatementPossibility(std::string name, Statement * si)
{
	m_statementPossibilities.insert(std::make_pair(name, si));
}

void Event::addControlGroup(std::string name, ControlGroup * controlGroup, bool addNotCounterPart)
{
}

void Event::execute()
{
	for (size_t i = 0; i < m_sequence.size(); i++) {
		Logger::logEvent("Event", "Running statement "+std::to_string(i));
		m_sequence[i]->execute();
	}
}

void Event::destroyStatementsOnly()
{
	for (size_t i = 0; i < m_sequence.size(); i++) {
		delete m_sequence[i];
		m_sequence.erase(m_sequence.begin() + i);
		i--;
	}
}

void Event::destroy()
{
	destroyStatementsOnly();

	for (auto it : m_statementPossibilities) {
		it.second->destroy();
		delete it.second;
	}
}

SequenceItem::SequenceItem()
{
}

SequenceItem::~SequenceItem()
{
}

int SequenceItem::execute()
{
	return onCall();
}

void SequenceItem::extraParsing(tinyxml2::XMLElement* ele, std::unordered_map<std::string, Statement*> m_statementPossibilities)
{
}

ControlGroup::ControlGroup()
{
}

ControlGroup::~ControlGroup()
{
}

int ControlGroup::execute()
{
	//check if condition is true
	if (this->isTrue() && (!m_inverted)) {
		for (size_t i = 0; i < m_sequence.size(); i++) {
			Logger::logEvent("Event", "Running statement " + std::to_string(i));
			m_sequence[i]->execute();
		}
	}
	else if (m_inverted && (!this->isTrue())) {
		for (size_t i = 0; i < m_sequence.size(); i++) {
			Logger::logEvent("Event", "Running statement " + std::to_string(i));
			m_sequence[i]->execute();
		}
	}
	return 0;
}

void ControlGroup::extraParsing(tinyxml2::XMLElement* ele, std::unordered_map<std::string, Statement*> m_statementPossibilities)
{
	//search if this control group is inverted
	if (ele->Attribute("not") != NULL && (!strcmp(ele->Attribute("not"), "yes"))) {
		m_inverted = true;
	}

	//go through all the children of that element
	auto* rootChild = ele->FirstChild();
	while (rootChild != NULL) {
		if (rootChild->ToComment() != NULL) {
			//do nothing
		}
		else if (rootChild->ToElement() != NULL) {
			auto* e = rootChild->ToElement();
			//first check if that element is allowed
			auto it = m_statementPossibilities.find(e->Name());
			if (it != m_statementPossibilities.end()) {
				//found it
				//create a copy
				Statement* si = it->second->create();

				Logger::logEvent("Event", "Creating sequence item " + it->first + ".");

				//now to loop through required arguments
				for (auto cur : si->getRequiredArguments()) {
					//get argument from element
					if (e->Attribute(cur.first.c_str()) != NULL) {
						si->getArguments()->insert(std::make_pair(cur.first, e->Attribute(cur.first.c_str())));
						Logger::logEvent("Event", cur.first + " = " + e->Attribute(cur.first.c_str()));
					}
					else {
						Logger::logEvent("error", "Element at line " + std::to_string(rootChild->GetLineNum()) + " missing required argument \"" + cur.first + "\"");
						return;
					}
				}
				si->extraParsing(e, m_statementPossibilities);
				m_sequence.push_back(si);
			}
			else {
				Logger::logEvent("error", "Unknown element at line " + std::to_string(rootChild->GetLineNum()));
				return;
			}
		}
		else {
			Logger::logEvent("error", "Unknown node at line " + std::to_string(rootChild->GetLineNum()));
			return;
		}
		rootChild = rootChild->NextSibling();
	}
}

void ControlGroup::destroy()
{
	for (auto s : m_sequence) {
		delete s;
	}
}

Statement::Statement()
{
}

Statement::~Statement()
{
}

void Statement::requireArgument(std::string name)
{
	m_requiredArgs.insert(std::make_pair(name, true));
}

std::string Statement::getArgument(std::string name)
{
	auto it = m_arguments.find(name);
	if (it != m_arguments.end())
		return it->second;
	return NULL;
}
