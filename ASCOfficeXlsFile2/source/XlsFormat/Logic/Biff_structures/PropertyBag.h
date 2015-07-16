#pragma once

#include "BiffStructure.h"
#include <Logic/Biff_structures/Property.h>

namespace OSHARED
{;

class CFRecord;

class PropertyBag : public XLS::BiffStructure
{
	BASE_STRUCTURE_DEFINE_CLASS_NAME(PropertyBag)
public:
	XLS::BiffStructurePtr clone();

	//virtual void toXML(BiffStructurePtr & parent);
	//virtual const bool fromXML(BiffStructurePtr & parent);
	virtual void load(XLS::CFRecord& record);
	virtual void store(XLS::CFRecord& record);

private:
	unsigned short id;
	std::vector<Property> properties;
};

} // namespace OSHARED

