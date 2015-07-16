#pragma once

#include "BiffStructure.h"
#include <Logic/Biff_structures/BiffString.h>

namespace XLS
{;

class CFRecord;

class DXFNumUsr : public BiffStructure
{
	BASE_STRUCTURE_DEFINE_CLASS_NAME(DXFNumUsr)
public:
	DXFNumUsr();
	BiffStructurePtr clone();

	//virtual void toXML(BiffStructurePtr & parent);
	//virtual const bool fromXML(BiffStructurePtr & parent);
	virtual void load(CFRecord& record);
	virtual void store(CFRecord& record);

private:
	unsigned short cb;
	XLUnicodeString fmt;
};

} // namespace XLS

