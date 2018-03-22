#pragma once
#include <string>
#include <tinyxml2\tinyxml2.h> // xml stuff

#include "XMLHandler.h"//custom xml stuff

#ifndef FORMATVERSION
#define FORMATVERSION "1.0"
#endif // ! FORMATVERSION


using tinyxml2::XMLDocument;
using tinyxml2::XMLComment;
using tinyxml2::XMLElement;
using tinyxml2::XMLNode;
class Adventure : public XMLHandler
{
public:
	Adventure();
	~Adventure();

	void loadFromFile(std::string originFile);

	virtual void onChildParsed(std::string name, XMLHandler * child);
private:

	XMLDocument m_doc;
};

