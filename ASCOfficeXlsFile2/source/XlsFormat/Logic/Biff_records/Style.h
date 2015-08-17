#pragma once

#include "BiffRecord.h"
#include <Logic/Biff_structures/BiffString.h>
#include <Logic/Biff_structures/BuiltInStyle.h>

namespace XLS
{


// Logical representation of Style record in BIFF8
class Style: public BiffRecord
{
	BIFF_RECORD_DEFINE_TYPE_INFO(Style)
	BASE_OBJECT_DEFINE_CLASS_NAME(Style)
public:
	Style();
	~Style();

	BaseObjectPtr clone();

	void writeFields(CFRecord& record);
	void readFields(CFRecord& record);

	static const ElementType	type = typeStyle;

	int serialize(std::wostream & stream);
//-----------------------------
	BIFF_WORD		ixfe;
	BIFF_BOOL		fBuiltIn;
	BuiltInStyle 	builtInData;
	XLUnicodeString user;

};

} // namespace XLS

