#pragma once

#include "BiffRecord.h"
#include <Logic/Biff_structures/DropDownObjId.h>

namespace XLS
{


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

	static const ElementType	type = typeDropDownObjIds;

//-----------------------------
	BiffStructurePtrVector rgidObj;

};

} // namespace XLS

