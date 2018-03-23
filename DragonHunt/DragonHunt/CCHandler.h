#pragma once

#include "XMLHandler.h"

class li : public XMLHandler {
public:
	li();
	~li();

	virtual void onChildParsed(std::string name, XMLHandler * child);
};

class CCHandler : public XMLHandler
{
public:
	CCHandler();
	~CCHandler();

	virtual void onChildParsed(std::string name, XMLHandler * child);
};

