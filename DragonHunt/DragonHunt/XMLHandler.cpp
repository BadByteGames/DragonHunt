#include "XMLHandler.h"

#include "Logger.h"

#include <iostream>

XMLHandler::XMLHandler()
{
	m_text = "";
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
	m_childrenHandlers.insert(std::make_pair(name, child));
	m_childrenRules.insert(std::make_pair(name, childFlags));
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

	//clean up the events
	for (auto it : m_events) {
		it.second.destroy();
	}
}

int XMLHandler::parseFromElement(tinyxml2::XMLElement * root, bool usesText)
{
	Logger::logEvent("XMLHandler", "began parsing element " + std::string(root->Name()) +" at line "+std::to_string(root->GetLineNum()));
	//populate attributes
	if(populateAttributes(root)) return 1;
	if (populateChildren(root, usesText)) return 1;
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

int XMLHandler::populateChildren(tinyxml2::XMLElement * elementToParse, bool usesText)
{
	//for this one we basically loop through everything and see what it is
	auto p = elementToParse->FirstChild();
	while (p != NULL) {
		//so we know that p is a node so next we will go through everything and see If we can identify it
		if (p->ToElement() != NULL) {
			auto currentElement = p->ToElement();
			//make sure that this element is one of the allowed
			auto it = m_childrenRules.find(currentElement->Name());
			if (it != m_childrenRules.end()) {
				//check if multiple are allowed
				if (it->second & XMLChildFlag::MULTIPLE) {
					//no checks are needed 
				}
				else {
					//check the element wasn't already found as there can only be one
					auto secondIt = m_children.find(currentElement->Name());
					if (secondIt != m_children.end()) {
						std::cout << "An error occurred, please check runtime.log for details" << std::endl;
						Logger::logEvent("error", "Element at line " + std::to_string(p->GetLineNum()) + ": \"" + p->Value() + "\" already exists, please remove it.");
						return 1;
					}
				}
				//seems legit so now we can do stuff
				//mark as added
				m_children.insert(std::make_pair(currentElement->Name(), true));
				//call that child's parser
				auto handler = m_childrenHandlers.find(currentElement->Name())->second;
				if(handler->parseFromElement(currentElement, it->second & XMLChildFlag::USESTEXT)) return 1;
				//next, call this object's "onchildparsed"
				onChildParsed(currentElement->Name(), handler);
				//make sure to clear that child's text
				handler->m_text = "";
			}
			//check if that element is an event
			else if (m_events.find(currentElement->Name()) != m_events.end()) {
				Logger::logEvent("XMLHandler", "began parsing element " + std::string(currentElement->Name()) + " at line " + std::to_string(currentElement->GetLineNum()));
				//check if wasn't defined
				if (!wasEventDefined(currentElement->Name())) {
					//found an event so just call its sequence builder
					auto evnt = m_events.find(currentElement->Name());
					if (evnt->second.parseFromElement(currentElement)) {
						return 1;
					}

					//next we add it to the defined events
					m_eventDefined.insert(std::make_pair(currentElement->Name(), true));
				}
				else {
					std::cout << "An error occurred, please check runtime.log for details" << std::endl;
					Logger::logEvent("error", "Event at line " + std::to_string(currentElement->GetLineNum()) + " was already defined.");
					return 1;
				}
				Logger::logEvent("XMLHandler", "finished parsing element " + std::string(currentElement->Name()));
			}
			else {
				std::cout << "An error occurred, please check runtime.log for details" << std::endl;
				Logger::logEvent("error", "Unknown element at line " + std::to_string(p->GetLineNum()) + ": " + p->Value());
				return 1;
			}
		}
		else if (p->ToComment() != NULL) {
			//its a comment, do nothing
		}
		else if (p->ToText()!=NULL && usesText) {
			//just print out the text for now
			m_text += p->ToText()->Value();
		}
		else {
			//Uh-oh, its an unkown
			std::cout << "An error occurred, please check runtime.log for details" << std::endl;
			Logger::logEvent("error", "Unknown node at line " + std::to_string(p->GetLineNum())+": "+p->Value());
			return 1;
		}
		//progress p's index
		p = p->NextSibling();
	}


	//lastly, we make sure that all required elements were met
	for (auto it : m_childrenRules) {
		if (it.second & XMLChildFlag::REQUIRED) {
			if (m_children.find(it.first) == m_children.end()) {
				//we didn't find a required item
				std::cout << "An error occurred, please check runtime.log for details" << std::endl;
				//create string to allow for addition
				Logger::logEvent("error", "Element \"" + std::string(elementToParse->Name()) + "\" (line " + std::to_string(elementToParse->GetLineNum())+") expected a child element \"" + it.first+"\"");
				return 1;
			}
		}
	}

	return 0;
}

void XMLHandler::addEvent(std::string name, Event evnt)
{
	m_events.insert(std::make_pair(name, evnt));
}

bool XMLHandler::wasEventDefined(std::string name)
{
	if (m_eventDefined.find(name) != m_eventDefined.end())
		return m_eventDefined.find(name)->second;
	return false;
}
