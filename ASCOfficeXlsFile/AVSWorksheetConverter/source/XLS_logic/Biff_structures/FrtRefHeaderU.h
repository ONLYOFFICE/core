#pragma once

#include "BiffStructTagged.h"
#include <XLS_logic/Biff_structures/CellRangeRef.h>
#include <XLS_bin/CFRecordType.h>
#include <XLS_logic/Biff_structures/BitMarkedStructs.h>

namespace XLS
{;

class CFRecord;

class FrtRefHeaderU : public BiffStructTagged
{
	BASE_OBJECT_DEFINE_CLASS_NAME(FrtRefHeaderU)
public:
	BiffStructurePtr clone();

	virtual void setXMLAttributes(MSXML2::IXMLDOMElementPtr xml_tag);
	virtual void getXMLAttributes(MSXML2::IXMLDOMElementPtr xml_tag);
	virtual void load(CFRecord& record);
	virtual void store(CFRecord& record);

private:
	CFRecordType::TypeId rt;
	FrtFlags grbitFrt;
	Ref8U ref8;
};

} // namespace XLS

