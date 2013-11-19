#pragma once

#include "BiffRecord.h"

namespace XLS
{;


// Logical representation of BopPop record in BIFF8
class BopPop: public BiffRecord
{
	BIFF_RECORD_DEFINE_TYPE_INFO(BopPop)
	BASE_OBJECT_DEFINE_CLASS_NAME(BopPop)
public:
	BopPop();
	~BopPop();

	BaseObjectPtr clone();

	void writeFields(CFRecord& record);
	void readFields(CFRecord& record);
private:
	BIFF_BYTE pst;
	Boolean<BYTE> fAutoSplit;
	BIFF_WORD split;
	BIFF_SHORT iSplitPos;
	BIFF_SHORT pcSplitPercent;
	BIFF_SHORT pcPie2Size;
	BIFF_SHORT pcGap;
	BIFF_DOUBLE numSplitValue;
	BIFF_BOOL fHasShadow;
public:
	BO_ATTRIB_MARKUP_BEGIN
		BO_ATTRIB_MARKUP_ATTRIB(pst)
		BO_ATTRIB_MARKUP_ATTRIB(fAutoSplit)
		BO_ATTRIB_MARKUP_ATTRIB(split)
		BO_ATTRIB_MARKUP_ATTRIB(iSplitPos)
		BO_ATTRIB_MARKUP_ATTRIB(pcSplitPercent)
		BO_ATTRIB_MARKUP_ATTRIB(pcPie2Size)
		BO_ATTRIB_MARKUP_ATTRIB(pcGap)
		BO_ATTRIB_MARKUP_ATTRIB(numSplitValue)
		BO_ATTRIB_MARKUP_ATTRIB(fHasShadow)
	BO_ATTRIB_MARKUP_END

};

} // namespace XLS

