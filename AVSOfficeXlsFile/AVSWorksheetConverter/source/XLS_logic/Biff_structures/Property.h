#pragma once

#include "BiffStructure.h"

namespace OSHARED
{;

class CFRecord;

class Property : public XLS::BiffStructure
{
	BASE_OBJECT_DEFINE_CLASS_NAME(Property)
public:
	XLS::BiffStructurePtr clone();

	virtual void toXML(MSXML2::IXMLDOMElementPtr parent);
	virtual const bool fromXML(MSXML2::IXMLDOMElementPtr parent);
	virtual void load(XLS::CFRecord& record);
	virtual void store(XLS::CFRecord& record);

private:
	DWORD keyIndex;
	DWORD valueIndex;
};

} // namespace OSHARED

