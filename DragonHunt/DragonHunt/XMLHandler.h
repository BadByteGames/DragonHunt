#pragma once
#include <string>
#include <unordered_map>
#include <tinyxml2\tinyxml2.h>
#include <map>

enum XMLChildFlag {REQUIRED = 0x1, MULTIPLE = 0x2, USESTEXT = 0x4};

class XMLHandler
{
public:
	XMLHandler();
	~XMLHandler();

	void addAttribute(std::string name, bool required = false);

	void addChild(std::string name, XMLHandler * child, unsigned int childFlags = 0);

	std::string getAttribute(std::string name);

	//call on the root node to clean it up and destroy its children
	void destroy();

	//sets the root element of the handler (only call publicy if root) then populates variables
	int parseFromElement(tinyxml2::XMLElement* root, bool usesText=false);

	std::string getText() const { return m_text; }

	//called whenever a child is added in the parsing phase
	//passes a pointer to XMLHandler which you WILL need to make a copy of because of class definition constrictions
	virtual void onChildParsed(std::string name, XMLHandler * child) = 0;

private:
	//used to parse attributes
	int populateAttributes(tinyxml2::XMLElement* elementToParse);

	int populateChildren(tinyxml2::XMLElement* elementToParse, bool usesText);

	//name , required
	std::unordered_map<std::string, bool> m_attributeRules;

	//name, value
	std::unordered_map<std::string, std::string> m_attributes;

	//name, rules
	std::unordered_map<std::string, unsigned int> m_childrenRules;

	//name, value
	std::unordered_map<std::string, XMLHandler*> m_childrenHandlers;

	//name, bool (exists)
	std::multimap<std::string, bool> m_children;

protected:
	// protected so we can modify in children classes
	std::string m_text;
};

