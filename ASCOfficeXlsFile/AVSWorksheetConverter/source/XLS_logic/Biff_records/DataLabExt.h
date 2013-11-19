#pragma once

#include "BiffRecord.h"

namespace XLS
{;


// Logical representation of DataLabExt record in BIFF8
class DataLabExt: public BiffRecord
{
	BIFF_RECORD_DEFINE_TYPE_INFO(DataLabExt)
	BASE_OBJECT_DEFINE_CLASS_NAME(DataLabExt)
public:
	DataLabExt();
	~DataLabExt();

	BaseObjectPtr clone();

	void writeFields(CFRecord& record);
	void readFields(CFRecord& record);

public:
	BO_ATTRIB_MARKUP_BEGIN
	BO_ATTRIB_MARKUP_END

};

} // namespace XLS

