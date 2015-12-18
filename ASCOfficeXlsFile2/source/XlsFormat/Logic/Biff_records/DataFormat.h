#pragma once

#include "BiffRecord.h"

namespace XLS
{


// Logical representation of DataFormat record in BIFF8
class DataFormat: public BiffRecord
{
	BIFF_RECORD_DEFINE_TYPE_INFO(DataFormat)
	BASE_OBJECT_DEFINE_CLASS_NAME(DataFormat)
public:
	DataFormat();
	~DataFormat();

	BaseObjectPtr clone();

	void writeFields(CFRecord& record);
	void readFields(CFRecord& record);

	static const ElementType	type = typeDataFormat;

	BIFF_WORD xi;
	BIFF_WORD yi;
	BIFF_WORD iss;
	BIFF_BOOL fUnknown;
};

} // namespace XLS

