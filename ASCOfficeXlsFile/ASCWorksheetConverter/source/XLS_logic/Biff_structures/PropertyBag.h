#pragma once

#include "BiffStructure.h"
#include <XLS_logic/Biff_structures/Property.h>

namespace OSHARED
{;

class CFRecord;

class PropertyBag : public XLS::BiffStructure
{
	BASE_OBJECT_DEFINE_CLASS_NAME(PropertyBag)
public:
	XLS::BiffStructurePtr clone();

	virtual void toXML(MSXML2::IXMLDOMElementPtr parent);
	virtual const bool fromXML(MSXML2::IXMLDOMElementPtr parent);
	virtual void load(XLS::CFRecord& record);
	virtual void store(XLS::CFRecord& record);

private:
	WORD id;
	std::vector<Property> properties;
};

} // namespace OSHARED

