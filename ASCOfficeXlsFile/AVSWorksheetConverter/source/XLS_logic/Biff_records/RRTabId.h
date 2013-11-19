#pragma once

#include "BiffRecord.h"
#include <XLS_logic/Biff_structures/SheetId.h>

namespace XLS
{;


// Logical representation of RRTabId record in BIFF8
class RRTabId: public BiffRecord
{
	BIFF_RECORD_DEFINE_TYPE_INFO(RRTabId)
	BASE_OBJECT_DEFINE_CLASS_NAME(RRTabId)
public:
	RRTabId();
	~RRTabId();

	BaseObjectPtr clone();

	void writeFields(CFRecord& record);
	void readFields(CFRecord& record);
private:
	BiffStructurePtrVector sheet_ids;
public:
	BO_ATTRIB_MARKUP_BEGIN
		BO_ATTRIB_MARKUP_VECTOR_COMPLEX(sheet_ids, SheetId)
	BO_ATTRIB_MARKUP_END

};

} // namespace XLS

