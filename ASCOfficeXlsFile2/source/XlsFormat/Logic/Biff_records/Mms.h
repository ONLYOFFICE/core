#pragma once

#include "BiffRecord.h"

namespace XLS
{;

class CFStreamCacheReader;


// Logical representation of Mms record in BIFF8
class Mms: public BiffRecord
{
	BIFF_RECORD_DEFINE_TYPE_INFO(Mms)
	BASE_OBJECT_DEFINE_CLASS_NAME(Mms)
public:
	Mms();
	~Mms();

	BaseObjectPtr clone();

	void writeFields(CFRecord& record);
	void readFields(CFRecord& record);
public:
	//BO_ATTRIB_MARKUP_BEGIN
	//BO_ATTRIB_MARKUP_END

};

} // namespace XLS

