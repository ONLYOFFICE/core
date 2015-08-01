#pragma once

#include "BiffStructure.h"
#include "BiffString.h"
#include "CellRangeRef.h"
#include <Logic/Biff_structures/BitMarkedStructs.h>

namespace XLS
{;

class CFRecord;

class SortCond12 : public BiffStructure
{
	BASE_STRUCTURE_DEFINE_CLASS_NAME(SortCond12)
public:
	BiffStructurePtr clone();

	static const ElementType	type = typeSortCond12;

	virtual void load(CFRecord& record);
	virtual void store(CFRecord& record);

	bool fSortDes;
	unsigned short sortOn;
	RFX rfx;

	CondDataValue condDataValue;
	CFFlag cfflag;

	long cchSt;
	XLUnicodeStringNoCch stSslist;
};

typedef boost::shared_ptr<SortCond12> SortCond12Ptr;

} // namespace XLS

