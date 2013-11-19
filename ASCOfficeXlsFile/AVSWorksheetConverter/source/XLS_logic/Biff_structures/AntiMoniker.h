#pragma once

#include "BiffStructTagged.h"

namespace OSHARED
{;

class CFRecord;

class AntiMoniker : public XLS::BiffStructTagged
{
	BASE_OBJECT_DEFINE_CLASS_NAME(AntiMoniker)
public:
	XLS::BiffStructurePtr clone();

	virtual void setXMLAttributes(MSXML2::IXMLDOMElementPtr xml_tag);
	virtual void getXMLAttributes(MSXML2::IXMLDOMElementPtr xml_tag);
	virtual void load(XLS::CFRecord& record);
	virtual void store(XLS::CFRecord& record);

private:
	DWORD count;
};

} // namespace OSHARED

