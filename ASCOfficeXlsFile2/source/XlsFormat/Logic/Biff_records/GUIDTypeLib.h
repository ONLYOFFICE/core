#pragma once

#include "BiffRecord.h"

namespace XLS
{


// Logical representation of GUIDTypeLib record in BIFF8
class GUIDTypeLib: public BiffRecord
{
	BIFF_RECORD_DEFINE_TYPE_INFO(GUIDTypeLib)
	BASE_OBJECT_DEFINE_CLASS_NAME(GUIDTypeLib)
public:
	GUIDTypeLib();
	~GUIDTypeLib();

	BaseObjectPtr clone();

	void writeFields(CFRecord& record);
	void readFields(CFRecord& record);

	static const ElementType	type = typeGUIDTypeLib;

//-----------------------------
	BIFF_BSTR guid;
public:
	//BO_ATTRIB_MARKUP_BEGIN
		//BO_ATTRIB_MARKUP_ATTRIB(guid)
	//BO_ATTRIB_MARKUP_END

};

} // namespace XLS

