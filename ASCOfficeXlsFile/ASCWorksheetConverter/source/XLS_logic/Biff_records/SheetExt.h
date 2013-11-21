#pragma once

#include "BiffRecord.h"
#include <XLS_logic/Biff_structures/SheetExtOptional.h>

namespace XLS
{;


// Logical representation of SheetExt record in BIFF8
class SheetExt: public BiffRecord
{
	BIFF_RECORD_DEFINE_TYPE_INFO(SheetExt)
	BASE_OBJECT_DEFINE_CLASS_NAME(SheetExt)
public:
	SheetExt();
	~SheetExt();

	BaseObjectPtr clone();

	void writeFields(CFRecord& record);
	void readFields(CFRecord& record);
private:
	BIFF_DWORD cb;
	BIFF_BYTE icvPlain;
	SheetExtOptional sheetExtOptional;
public:
	BO_ATTRIB_MARKUP_BEGIN
	BO_ATTRIB_MARKUP_ATTRIB(icvPlain)
	if(0x00000028 == cb)
	{
		BO_ATTRIB_MARKUP_COMPLEX(sheetExtOptional)
	}
	BO_ATTRIB_MARKUP_END

};

} // namespace XLS

