#pragma once

#include "BiffRecord.h"
#include <Logic/Biff_structures/BiffString.h>

namespace XLS
{;


// Logical representation of Footer record in BIFF8
class Footer: public BiffRecord
{
	BIFF_RECORD_DEFINE_TYPE_INFO(Footer)
	BASE_OBJECT_DEFINE_CLASS_NAME(Footer)
public:
	Footer();
	~Footer();

	BaseObjectPtr clone();

	void writeFields(CFRecord& record);
	void readFields(CFRecord& record);

	static const ElementType	type = typeFooter;

//-----------------------------
	XLUnicodeString ast;
};

} // namespace XLS

