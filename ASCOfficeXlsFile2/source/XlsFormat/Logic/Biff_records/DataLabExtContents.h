#pragma once

#include "BiffRecord.h"
#include <Logic/Biff_structures/BiffString.h>

namespace XLS
{


// Logical representation of DataLabExtContents record in BIFF8
class DataLabExtContents: public BiffRecord
{
	BIFF_RECORD_DEFINE_TYPE_INFO(DataLabExtContents)
	BASE_OBJECT_DEFINE_CLASS_NAME(DataLabExtContents)
public:
	DataLabExtContents();
	~DataLabExtContents();

	BaseObjectPtr clone();

	void writeFields(CFRecord& record);
	void readFields(CFRecord& record);

	static const ElementType	type = typeDataLabExtContents;

	int serialize(std::wostream & _stream);

	BIFF_BOOL fSerName;
	BIFF_BOOL fCatName;
	BIFF_BOOL fValue;
	BIFF_BOOL fPercent;
	BIFF_BOOL fBubSizes;

	XLUnicodeStringMin2 rgchSep;

//-----------------------------

	bool is_area;
};

} // namespace XLS

