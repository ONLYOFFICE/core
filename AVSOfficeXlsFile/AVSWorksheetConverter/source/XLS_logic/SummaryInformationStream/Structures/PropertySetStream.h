#pragma once

#include "XLS_bin/BinSmartPointers.h"
#include "PropertySet.h"

namespace OLEPS
{;

class PropertySetStream
{
public:
	PropertySetStream(XLS::CFStreamPtr stream);
	
	const WORD GetCodePage();

private:
	std::vector<PropertySetPtr> property_sets;
};

} // namespace OLEPS
