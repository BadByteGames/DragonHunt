#include "Events.h"

#include <iostream>
#include <string>

Event::Event()
{
}


Event::~Event()
{
}

EventList::EventList()
{
}

EventList::~EventList()
{
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
	for (auto it : other->m_arguments) {
		this->m_arguments.insert(std::make_pair(it.first, it.second));
	}
}

Print::Print()
{
	requireArgument("text");
}

Print::~Print()
{
}

SequenceItem * Print::create()
{
	Print* p = new Print();
	p->copyArgs(this);
	return p;
}

int Print::onCall()
{
	std::cout << this->getArgument("text") << std::endl;
	return 0;
}
