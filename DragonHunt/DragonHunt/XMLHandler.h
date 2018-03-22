#pragma once
#include <string>
#include <unordered_map>
#include <tinyxml2\tinyxml2.h>
#include <map>

enum XMLChildFlag {REQUIRED = 0x1, MULTIPLE = 0x2};

//if a text element, will have different parser looking for text filling
enum XMLElementFlag {TEXTELEMENT = 0x1};

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
	int parseFromElement(tinyxml2::XMLElement* root);

	//called whenever a child is added in the parsing phase
	virtual void onChildParsed(std::string name, XMLHandler * child) = 0;

private:
	//used to parse attributes
	int populateAttributes(tinyxml2::XMLElement* elementToParse);

	int populateChildren(tinyxml2::XMLElement* elementToParse);

	//name , required
	std::unordered_map<std::string, bool> m_attributeRules;

	//name, value
	std::unordered_map<std::string, std::string> m_attributes;

	//name, rules
	std::unordered_map<std::string, unsigned int> m_childrenRules;

	//name, value
	std::unordered_map<std::string, XMLHandler*> m_childrenHandlers;

	//name, element
	std::multimap<std::string, XMLHandler*> m_children;
};

