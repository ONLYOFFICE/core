#pragma once

#include "BiffStructure.h"
#include "BiffString.h"

namespace XLS
{;

class CFRecord;

class RevItab : public BiffStructure
{
	BASE_STRUCTURE_DEFINE_CLASS_NAME(RevItab)
public:
	RevItab();
	RevItab(CFRecord& record);
	BiffStructurePtr clone();

	//virtual void toXML(BiffStructurePtr & parent);
	//virtual const bool fromXML(BiffStructurePtr & parent);
	virtual void load(CFRecord& record);
	virtual void store(CFRecord& record);

private:
	unsigned char type;
	unsigned __int16 tabid;
	XLUnicodeString sheet;
};

} // namespace XLS

