#pragma once

#include "BiffStructTagged.h"

namespace XLS
{;

class CFRecord;

class PhRuns : public BiffStructTagged
{
	BASE_OBJECT_DEFINE_CLASS_NAME(PhRuns)
public:
	BiffStructurePtr clone();

	virtual void setXMLAttributes(MSXML2::IXMLDOMElementPtr xml_tag);
	virtual void getXMLAttributes(MSXML2::IXMLDOMElementPtr xml_tag);
	virtual void load(CFRecord& record);
	virtual void store(CFRecord& record);

private:
	short ichFirst;
	short ichMom;
	short cchMom;
};

} // namespace XLS

