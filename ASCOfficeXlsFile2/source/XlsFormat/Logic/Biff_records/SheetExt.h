#pragma once

#include "BiffRecord.h"
#include <Logic/Biff_structures/SheetExtOptional.h>

namespace XLS
{


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
	
	static const ElementType	type = typeSheetExt;

	int serialize(std::wostream & stream);
//-----------------------------
	_UINT32	cb;
	unsigned char	icvPlain;
	
	SheetExtOptional sheetExtOptional;
};

} // namespace XLS

