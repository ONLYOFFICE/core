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
	BIFF_WORD codePage;
public:
	//BO_ATTRIB_MARKUP_BEGIN
		//BO_ATTRIB_MARKUP_ATTRIB(codePage)
	//BO_ATTRIB_MARKUP_END

};

} // namespace XLS
