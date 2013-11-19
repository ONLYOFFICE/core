#pragma once

#include "BiffStructTagged.h"
#include <XLS_logic/Biff_structures/CellRangeRef.h>

namespace XLS
{;

class CFRecord;

class SqRef : public BiffStructTagged
{
	BASE_OBJECT_DEFINE_CLASS_NAME(SqRef)
public:
	BiffStructurePtr clone();

	virtual void setXMLAttributes(MSXML2::IXMLDOMElementPtr xml_tag);
	virtual void getXMLAttributes(MSXML2::IXMLDOMElementPtr xml_tag);
	virtual void load(CFRecord& record);
	virtual void store(CFRecord& record);

private:
	std::vector<Ref8U> rgrefs;
};

} // namespace XLS

