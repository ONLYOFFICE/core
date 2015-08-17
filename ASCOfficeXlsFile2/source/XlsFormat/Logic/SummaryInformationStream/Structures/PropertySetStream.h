#pragma once

#include <Binary/BinSmartPointers.h>
#include "PropertySet.h"

namespace OLEPS
{

class PropertySetStream
{
public:
	PropertySetStream(XLS::CFStreamPtr stream);
	
	const unsigned short GetCodePage();

private:
	std::vector<PropertySetPtr> property_sets;
};

} // namespace OLEPS
