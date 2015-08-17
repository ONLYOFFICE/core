#pragma once

#include "BiffRecord.h"

namespace XLS
{


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
	
	static const ElementType	type = typeXFCRC;

//-----------------------------
	BIFF_WORD cxfs;
	BIFF_DWORD crc;

};

} // namespace XLS

