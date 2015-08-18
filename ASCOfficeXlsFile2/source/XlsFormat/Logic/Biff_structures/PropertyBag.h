#pragma once

#include "SharedProperty.h"

namespace OSHARED
{

class CFRecord;

class PropertyBag : public XLS::BiffStructure
{
	BASE_STRUCTURE_DEFINE_CLASS_NAME(PropertyBag)
public:
    PropertyBag();

	XLS::BiffStructurePtr clone();

	static const XLS::ElementType	type = XLS::typePropertyBag;

	virtual void load(XLS::CFRecord& record);
	virtual void store(XLS::CFRecord& record);

	unsigned short id;
    std::vector<SharedProperty> properties;
};

} // namespace OSHARED

