#pragma once

#include "BiffStructure.h"
#include "BiffString.h"

namespace XLS
{

class CFRecord;

class LbsDropData : public BiffStructure
{
	BASE_STRUCTURE_DEFINE_CLASS_NAME(LbsDropData)
public:
	BiffStructurePtr clone();

	static const ElementType	type = typeLbsDropData;

	virtual void load(CFRecord& record);
	virtual void store(CFRecord& record);

	unsigned char wStyle;
	bool fFiltered;

	unsigned short cLine;
	unsigned short dxMin;

	XLUnicodeString str;

};

typedef boost::shared_ptr<LbsDropData> LbsDropDataPtr;

} // namespace XLS

