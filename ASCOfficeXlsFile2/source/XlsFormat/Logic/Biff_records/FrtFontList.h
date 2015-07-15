#pragma once

#include "BiffRecord.h"

namespace XLS
{;


// Logical representation of FrtFontList record in BIFF8
class FrtFontList: public BiffRecord
{
	BIFF_RECORD_DEFINE_TYPE_INFO(FrtFontList)
	BASE_OBJECT_DEFINE_CLASS_NAME(FrtFontList)
public:
	FrtFontList();
	~FrtFontList();

	BaseObjectPtr clone();

	void writeFields(CFRecord& record);
	void readFields(CFRecord& record);

	static const ElementType	type = typeFrtFontList;

//-----------------------------
//	BIFF_WORD userName;
public:
	//BO_ATTRIB_MARKUP_BEGIN
//		//BO_ATTRIB_MARKUP_ATTRIB(userName)
	//BO_ATTRIB_MARKUP_END

};

} // namespace XLS

