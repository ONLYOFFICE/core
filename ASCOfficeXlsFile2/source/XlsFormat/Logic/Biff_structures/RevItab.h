#pragma once

#include "BiffStructure.h"
#include "BiffString.h"

namespace XLS
{

class CFRecord;

class RevItab : public BiffStructure
{
	BASE_STRUCTURE_DEFINE_CLASS_NAME(RevItab)
public:
	RevItab();
	RevItab(CFRecord& record);
	BiffStructurePtr clone();

	static const ElementType	type = typeRevItab;

	virtual void load(CFRecord& record);
	virtual void store(CFRecord& record);

	unsigned char type1;
	unsigned short tabid;
	XLUnicodeString sheet;
};

} // namespace XLS

