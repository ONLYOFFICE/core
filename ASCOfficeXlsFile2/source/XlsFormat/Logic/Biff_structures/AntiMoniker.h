#pragma once

#include "BiffStructure.h"

namespace OSHARED
{;

class CFRecord;

class AntiMoniker : public XLS::BiffStructure
{
	BASE_OBJECT_DEFINE_CLASS_NAME(AntiMoniker)
public:
	XLS::BiffStructurePtr clone();

	//virtual void setXMLAttributes(MSXML2::IXMLDOMElementPtr xml_tag);
	//virtual void getXMLAttributes(MSXML2::IXMLDOMElementPtr xml_tag);
	virtual void load(XLS::CFRecord& record);
	virtual void store(XLS::CFRecord& record);

private:
	unsigned int count;
};

} // namespace OSHARED

