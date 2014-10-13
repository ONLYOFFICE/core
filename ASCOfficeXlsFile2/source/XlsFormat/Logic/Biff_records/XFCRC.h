#pragma once

#include "BiffRecord.h"

namespace XLS
{;


// Logical representation of XFCRC record in BIFF8
class XFCRC: public BiffRecord
{
	BIFF_RECORD_DEFINE_TYPE_INFO(XFCRC)
	BASE_OBJECT_DEFINE_CLASS_NAME(XFCRC)
public:
	XFCRC();
	~XFCRC();

	BaseObjectPtr clone();

	void writeFields(CFRecord& record);
	void readFields(CFRecord& record);
private:
	BIFF_WORD cxfs;
	BIFF_DWORD crc;
public:
	BO_ATTRIB_MARKUP_BEGIN
		BO_ATTRIB_MARKUP_ATTRIB(cxfs)
		BO_ATTRIB_MARKUP_ATTRIB(crc)
	BO_ATTRIB_MARKUP_END

};

} // namespace XLS

