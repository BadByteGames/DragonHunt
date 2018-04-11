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

ControlGroup::ControlGroup()
{
}

ControlGroup::~ControlGroup()
{
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
