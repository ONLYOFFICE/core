#pragma once

#include "Property.h"
#include <Binary/BinSmartPointers.h>

#include <vector>

namespace OLEPS
{

class PropertySet
{
public:
	PropertySet(XLS::CFStreamPtr stream, const unsigned int property_set_offset);
	
	const unsigned short GetCodePage();

private:
#pragma pack(1)
	struct PropertyIdentifierAndOffset
	{
		unsigned int PropertyIdentifier;
		unsigned int Offset;
	};
#pragma pack()

private:
	std::vector<PropertyPtr> properties;
	unsigned short code_page; // no need to initialize because this info is a must for every SummaryInformation stream in a compound file
};

typedef boost::shared_ptr<PropertySet> PropertySetPtr;

} // namespace OLEPS
