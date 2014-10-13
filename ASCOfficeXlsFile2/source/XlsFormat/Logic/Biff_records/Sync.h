#pragma once

#include "BiffRecord.h"

namespace XLS
{;


// Logical representation of Sync record in BIFF8
class Sync: public BiffRecord
{
	BIFF_RECORD_DEFINE_TYPE_INFO(Sync)
	BASE_OBJECT_DEFINE_CLASS_NAME(Sync)
public:
	Sync();
	~Sync();

	BaseObjectPtr clone();

	void writeFields(CFRecord& record);
	void readFields(CFRecord& record);
private:
	ForwardOnlyParam<unsigned __int16> rw;
	ForwardOnlyParam<unsigned __int16> col;
	BIFF_BSTR ref_;
public:
	BO_ATTRIB_MARKUP_BEGIN
		BO_ATTRIB_MARKUP_ATTRIB(rw)
		BO_ATTRIB_MARKUP_ATTRIB(col)
		BO_ATTRIB_MARKUP_ATTRIB_NAME(ref_, L"ref")
	BO_ATTRIB_MARKUP_END

};

} // namespace XLS

