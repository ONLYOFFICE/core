#pragma once

#include "BiffStructure.h"
#include <XLS_logic/Biff_structures/CellRangeRef.h>
#include <XLS_bin/CFRecordType.h>
#include <XLS_logic/Biff_structures/BitMarkedStructs.h>

namespace XLS
{;

class CFRecord;

class FrtRefHeaderNoGrbit : public BiffStructure
{
	BASE_OBJECT_DEFINE_CLASS_NAME(FrtRefHeaderNoGrbit)
public:
	FrtRefHeaderNoGrbit(const CFRecordType::TypeId type);
	FrtRefHeaderNoGrbit(const CFRecordType::TypeId type, const Ref8U& ref8_init);

	BiffStructurePtr clone();

	virtual void toXML(MSXML2::IXMLDOMElementPtr parent);
	virtual const bool fromXML(MSXML2::IXMLDOMElementPtr parent);
	virtual void load(CFRecord& record);
	virtual void store(CFRecord& record);

private:
	CFRecordType::TypeId rt;
	Ref8U ref8;
};

} // namespace XLS

