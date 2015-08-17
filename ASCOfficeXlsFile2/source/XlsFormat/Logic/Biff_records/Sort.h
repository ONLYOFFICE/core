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

	BIFF_BOOL fCol;
	BIFF_BOOL fKey1Dsc;
	BIFF_BOOL fKey2Dsc;
	BIFF_BOOL fKey3Dsc;
	BIFF_BOOL fCaseSensitive;
	BIFF_SHORT iOrder;
	BIFF_BOOL fAltMethod;

	XLUnicodeStringNoCch stKey1;
	XLUnicodeStringNoCch stKey2;
	XLUnicodeStringNoCch stKey3;

public:
	//BO_ATTRIB_MARKUP_BEGIN
		//BO_ATTRIB_MARKUP_ATTRIB(fCol)
		//BO_ATTRIB_MARKUP_ATTRIB(fKey1Dsc)
		//BO_ATTRIB_MARKUP_ATTRIB(fKey2Dsc)
		//BO_ATTRIB_MARKUP_ATTRIB(fKey3Dsc)
		//BO_ATTRIB_MARKUP_ATTRIB(fCaseSensitive)
		//BO_ATTRIB_MARKUP_ATTRIB(iOrder)
		//BO_ATTRIB_MARKUP_ATTRIB(fAltMethod)

		//BO_ATTRIB_MARKUP_ATTRIB(stKey1)
		//BO_ATTRIB_MARKUP_ATTRIB(stKey2)
		//BO_ATTRIB_MARKUP_ATTRIB(stKey3)
	//BO_ATTRIB_MARKUP_END

};

} // namespace XLS

