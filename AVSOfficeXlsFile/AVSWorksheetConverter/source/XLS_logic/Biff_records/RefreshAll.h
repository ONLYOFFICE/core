#pragma once

#include "BiffRecord.h"

namespace XLS
{;


// Logical representation of RefreshAll record in BIFF8
class RefreshAll: public BiffRecord
{
	BIFF_RECORD_DEFINE_TYPE_INFO(RefreshAll)
	BASE_OBJECT_DEFINE_CLASS_NAME(RefreshAll)
public:
	RefreshAll();
	~RefreshAll();

	BaseObjectPtr clone();

	void writeFields(CFRecord& record);
	void readFields(CFRecord& record);
private:
	Boolean<WORD> refreshAll;
public:
	BO_ATTRIB_MARKUP_BEGIN
		BO_ATTRIB_MARKUP_ATTRIB(refreshAll)
	BO_ATTRIB_MARKUP_END

};

} // namespace XLS

