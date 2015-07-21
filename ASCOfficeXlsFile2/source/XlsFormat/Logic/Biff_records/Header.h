#pragma once

#include "BiffRecord.h"
#include <Logic/Biff_structures/BiffString.h>

namespace XLS
{;


// Logical representation of Header record in BIFF8
class Header: public BiffRecord
{
	BIFF_RECORD_DEFINE_TYPE_INFO(Header)
	BASE_OBJECT_DEFINE_CLASS_NAME(Header)
public:
	Header();
	~Header();

	BaseObjectPtr clone();

	void writeFields(CFRecord& record);
	void readFields(CFRecord& record);

	static const ElementType	type = typeHeader;
//-----------------------------
	XLUnicodeString ast;

};

} // namespace XLS

