#pragma once

#include "BiffRecord.h"

namespace XLS
{;


// Logical representation of AxesUsed record in BIFF8
class AxesUsed: public BiffRecord
{
	BIFF_RECORD_DEFINE_TYPE_INFO(AxesUsed)
	BASE_OBJECT_DEFINE_CLASS_NAME(AxesUsed)
public:
	AxesUsed();
	~AxesUsed();

	BaseObjectPtr clone();

	void writeFields(CFRecord& record);
	void readFields(CFRecord& record);
private:
	BIFF_WORD cAxes;
public:
	BO_ATTRIB_MARKUP_BEGIN
		BO_ATTRIB_MARKUP_ATTRIB(cAxes)
	BO_ATTRIB_MARKUP_END

};

} // namespace XLS

