#pragma once

#include "BiffRecord.h"
#include <Logic/Biff_structures/BiffString.h>

namespace XLS
{


// Logical representation of Sort record in BIFF8
class Sort: public BiffRecord
{
	BIFF_RECORD_DEFINE_TYPE_INFO(Sort)
	BASE_OBJECT_DEFINE_CLASS_NAME(Sort)
public:
	Sort();
	~Sort();

	BaseObjectPtr clone();

	void writeFields(CFRecord& record);
	void readFields(CFRecord& record);

	static const ElementType	type = typeSort;

//-----------------------------

	bool fCol;
	bool fKey1Dsc;
	bool fKey2Dsc;
	bool fKey3Dsc;
	bool fCaseSensitive;
	_INT16 iOrder;
	bool fAltMethod;

	XLUnicodeStringNoCch stKey1;
	XLUnicodeStringNoCch stKey2;
	XLUnicodeStringNoCch stKey3;
};

} // namespace XLS

