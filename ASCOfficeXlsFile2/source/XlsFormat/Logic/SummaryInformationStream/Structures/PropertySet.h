#pragma once

#include "Property.h"
#include <Binary/BinSmartPointers.h>

namespace OLEPS
{;

class PropertySet
{
public:
	PropertySet(XLS::CFStreamPtr stream, const unsigned int property_set_offset);
	
	const unsigned __int16 GetCodePage();

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
	unsigned __int16 code_page; // no need to initialize because this info is a must for every SummaryInformation stream in a compound file
};

typedef boost::shared_ptr<PropertySet> PropertySetPtr;

} // namespace OLEPS
