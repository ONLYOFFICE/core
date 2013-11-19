#pragma once

#include "Property.h"
#include "XLS_bin\BinSmartPointers.h"

namespace OLEPS
{;

class PropertySet
{
public:
	PropertySet(XLS::CFStreamPtr stream, const DWORD property_set_offset);
	
	const WORD GetCodePage();

private:
#pragma pack(1)
	struct PropertyIdentifierAndOffset
	{
		DWORD PropertyIdentifier;
		DWORD Offset;
	};
#pragma pack()

private:
	std::vector<PropertyPtr> properties;
	WORD code_page; // no need to initialize because this info is a must for every SummaryInformation stream in a compound file
};

typedef boost::shared_ptr<PropertySet> PropertySetPtr;

} // namespace OLEPS
