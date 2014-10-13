#pragma once

#include "BiffRecord.h"
#include <Logic/Biff_structures/DropDownObjId.h>

namespace XLS
{;


// Logical representation of DropDownObjIds record in BIFF8
class DropDownObjIds: public BiffRecord
{
	BIFF_RECORD_DEFINE_TYPE_INFO(DropDownObjIds)
	BASE_OBJECT_DEFINE_CLASS_NAME(DropDownObjIds)
public:
	DropDownObjIds();
	~DropDownObjIds();

	BaseObjectPtr clone();

	void writeFields(CFRecord& record);
	void readFields(CFRecord& record);
private:
	BiffStructurePtrVector rgidObj;
public:
	BO_ATTRIB_MARKUP_BEGIN
		BO_ATTRIB_MARKUP_VECTOR_COMPLEX(rgidObj, DropDownObjId)
	BO_ATTRIB_MARKUP_END

};

} // namespace XLS

