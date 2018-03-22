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

	//creates an xml handler 
	static XMLHandler* createElementHandler(std::string name);

	void addAttribute(std::string name, bool required = false);

	void addChildren(XMLHandler * child, unsigned int childFlags = 0);

	std::string getAttribute(std::string name);
	
	std::vector<XMLHandler*> getChildren(std::string name);

	//call on the root node to clean it up and destroy its children
	void destroy();

	//sets the root element of the handler (only call if root) then populates variables
	void parseFromElement(tinyxml2::XMLElement* root);

private:
	//call upon creation of child
	void init(std::string name, tinyxml2::XMLElement* elementToParse);

	//name , required
	std::unordered_map<std::string, bool> m_attributeRules;

	//name, value
	std::unordered_map<std::string, std::string> m_attributes;

	//name, rules
	std::unordered_map<std::string, unsigned int> m_childrenRules;

	//name, value
	std::unordered_map<std::string, XMLHandler*> m_childrenHandlers;

	//name, element
	std::multimap<std::string, tinyxml2::XMLElement*> m_children;
};

