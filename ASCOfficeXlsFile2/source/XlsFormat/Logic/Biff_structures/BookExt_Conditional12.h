#pragma once

#include "BiffStructure.h"

namespace XLS
{

class CFRecord;

class BookExt_Conditional12 : public BiffStructure
{
	BASE_STRUCTURE_DEFINE_CLASS_NAME(BookExt_Conditional12)
public:
	BiffStructurePtr clone();

	static const ElementType	type = typeBookExt_Conditional12;
	
	virtual void load(CFRecord& record);
	virtual void store(CFRecord& record);

	bool fPublishedBookItems;
	bool fShowPivotChartFilter;
};

} // namespace XLS

