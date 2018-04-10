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
			auto it = m_sequencePossibilities.find(e->Name());
			if (it != m_sequencePossibilities.end()) {
				//found it
				//create a copy
				SequenceItem* si = it->second->create();
				
				Logger::logEvent("Event", "Creating sequence item "+it->first+".");

				//now to loop through required arguments
				for (auto cur : si->m_requiredArgs) {
					//get argument from element
					if (e->Attribute(cur.first.c_str()) != NULL) {
						si->m_arguments.insert(std::make_pair(cur.first, e->Attribute(cur.first.c_str())));
						Logger::logEvent("Event", cur.first + " = " + e->Attribute(cur.first.c_str()));
					}
					else {
						std::cout << "An error occurred, please check runtime.log for details" << std::endl;
						Logger::logEvent("error", "Element at line " + std::to_string(rootChild->GetLineNum()) + " missing required argument \""+ cur.first+"\"");
						return 1;
					}
				}

				m_sequence.push_back(si);
			}
			else {
				std::cout << "An error occurred, please check runtime.log for details" << std::endl;
				Logger::logEvent("error", "Unknown element at line " + std::to_string(rootChild->GetLineNum()));
				return 1;
			}
		}
		else {
			std::cout << "An error occurred, please check runtime.log for details" << std::endl;
			Logger::logEvent("error", "Unknown node at line "+std::to_string(rootChild->GetLineNum()));
			return 1;
		}
		rootChild = rootChild->NextSibling();
	}

	return 0;
}

void Event::addSequencePossibility(std::string name, SequenceItem * si)
{
	m_sequencePossibilities.insert(std::make_pair(name, si));
}

void Event::execute()
{
	for (size_t i = 0; i < m_sequence.size(); i++) {
		Logger::logEvent("Event", "Running sequence item "+std::to_string(i));
		m_sequence[i]->onCall();
	}
}

void Event::destroySequenceOnly()
{
	for (size_t i = 0; i < m_sequence.size(); i++) {
		delete m_sequence[i];
		m_sequence.erase(m_sequence.begin() + i);
		i--;
	}
}

void Event::destroy()
{
	destroySequenceOnly();

	for (auto it : m_sequencePossibilities) {
		delete it.second;
	}
}

SequenceItem::SequenceItem()
{
}

SequenceItem::~SequenceItem()
{
}

void SequenceItem::requireArgument(std::string name)
{
	m_requiredArgs.insert(std::make_pair(name, true));
}

std::string SequenceItem::getArgument(std::string name)
{
	auto it = m_arguments.find(name);
	if (it != m_arguments.end())
		return it->second;
	return NULL;
}

void SequenceItem::copyArgs(SequenceItem * other)
{
	this->m_arguments.clear();
	for (auto it : other->m_arguments) {
		this->m_arguments.insert(std::make_pair(it.first, it.second));
	}
}
