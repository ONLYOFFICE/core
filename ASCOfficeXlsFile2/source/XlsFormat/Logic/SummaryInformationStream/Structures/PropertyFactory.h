#pragma once

#include "Binary/BinSmartPointers.h"
#include "Property.h"

namespace OLEPS
{

class PropertyFactory 
{
private:
	PropertyFactory();

public:

	static PropertyPtr ReadProperty(const unsigned int prop_type, XLS::CFStreamPtr stream, const unsigned int property_offset);
	
};

} // namespace OLEPS
