#pragma once

#include "XLS_bin/BinSmartPointers.h"
#include "Property.h"

namespace OLEPS
{;

class PropertyFactory 
{
private:
	PropertyFactory();

public:

	static PropertyPtr ReadProperty(const DWORD prop_type, XLS::CFStreamPtr stream, const DWORD property_offset);
	
};

} // namespace OLEPS
