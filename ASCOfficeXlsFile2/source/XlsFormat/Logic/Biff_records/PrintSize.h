#pragma once

#include "BiffRecord.h"

namespace XLS
{


// Logical representation of PrintSize record in BIFF8
class PrintSize: public BiffRecord
{
	BIFF_RECORD_DEFINE_TYPE_INFO(PrintSize)
	BASE_OBJECT_DEFINE_CLASS_NAME(PrintSize)
public:
	PrintSize();
	~PrintSize();

	BaseObjectPtr clone();

	void writeFields(CFRecord& record);
	void readFields(CFRecord& record);

	static const ElementType	type = typePrintSize;

//-----------------------------
	_UINT16 printSize;

};

} // namespace XLS

