#pragma once

#include "BiffRecord.h"

namespace XLS
{


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
	_UINT16		colFirst;
	_UINT16		colLast;
	_UINT16		coldx;
	IXFCell		ixfe;

	bool fHidden;
	bool fUserSet;
	bool fBestFit;
	bool fPhonetic;
	BIFF_BYTE iOutLevel;
	bool fCollapsed;
};

} // namespace XLS

