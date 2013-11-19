#pragma once

#include "BiffRecord.h"

namespace XLS
{;


// Logical representation of UsesELFs record in BIFF8
class UsesELFs: public BiffRecord
{
	BIFF_RECORD_DEFINE_TYPE_INFO(UsesELFs)
	BASE_OBJECT_DEFINE_CLASS_NAME(UsesELFs)
public:
	UsesELFs();
	~UsesELFs();

	BaseObjectPtr clone();

	void writeFields(CFRecord& record);
	void readFields(CFRecord& record);
private:
	Boolean<WORD> useselfs;
public:
	BO_ATTRIB_MARKUP_BEGIN
		BO_ATTRIB_MARKUP_ATTRIB(useselfs)
	BO_ATTRIB_MARKUP_END

};

} // namespace XLS

