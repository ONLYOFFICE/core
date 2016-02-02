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
	BIFF_SHORT iOrder;
	bool fAltMethod;

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

