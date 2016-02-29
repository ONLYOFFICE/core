#pragma once

#include "BiffRecord.h"

namespace XLS
{

class CFStreamCacheReader;

// Logical representation of InterfaceHdr record in BIFF8
class InterfaceHdr: public BiffRecord
{
	BIFF_RECORD_DEFINE_TYPE_INFO(InterfaceHdr)
	BASE_OBJECT_DEFINE_CLASS_NAME(InterfaceHdr)
public:
	InterfaceHdr();
	~InterfaceHdr();

	BaseObjectPtr clone();

	void writeFields(CFRecord& record);
	void readFields(CFRecord& record);
//-----------------------------
	_UINT16 codePage;
};

} // namespace XLS
