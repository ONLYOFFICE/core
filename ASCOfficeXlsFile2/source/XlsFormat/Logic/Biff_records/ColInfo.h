#pragma once

#include "BiffRecord.h"

namespace XLS
{;


// Logical representation of ColInfo record in BIFF8
class ColInfo: public BiffRecord
{
	BIFF_RECORD_DEFINE_TYPE_INFO(ColInfo)
	BASE_OBJECT_DEFINE_CLASS_NAME(ColInfo)
public:
	ColInfo();
	~ColInfo();

	BaseObjectPtr clone();

	void writeFields(CFRecord& record);
	void readFields(CFRecord& record);

	static const ElementType	type = typeColInfo;

//-----------------------------
	Col256U colFirst;
	Col256U colLast;
	BIFF_WORD coldx;
	IXFCell ixfe;

	BIFF_BOOL fHidden;
	BIFF_BOOL fUserSet;
	BIFF_BOOL fBestFit;
	BIFF_BOOL fPhonetic;
	BIFF_BYTE iOutLevel;
	BIFF_BOOL fCollapsed;
};

} // namespace XLS

