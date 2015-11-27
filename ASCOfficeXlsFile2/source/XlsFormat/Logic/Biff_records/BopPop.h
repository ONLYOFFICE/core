#pragma once

#include "BiffRecord.h"

namespace XLS
{


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

	static const ElementType	type = typeBopPop;

//-----------------------------
	BIFF_BYTE		pst;
	Boolean<unsigned char> fAutoSplit;
	BIFF_WORD		split;
	BIFF_SHORT		iSplitPos;
	BIFF_SHORT		pcSplitPercent;
	BIFF_SHORT		pcPie2Size;
	BIFF_SHORT		pcGap;
	BIFF_DOUBLE		numSplitValue;
	BIFF_BOOL		fHasShadow;


	BaseObjectPtr	m_Custom;
};

} // namespace XLS

