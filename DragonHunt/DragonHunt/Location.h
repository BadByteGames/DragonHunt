#pragma once

#include "XMLHandler.h"

class Location : public XMLHandler
{
public:
	Location();
	~Location();

	virtual void onChildParsed(std::string name, XMLHandler * child);
};

