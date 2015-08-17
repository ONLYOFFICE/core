#pragma once

#include "BiffRecord.h"

namespace XLS
{


// Logical representation of Begin record in BIFF8
class Begin: public BiffRecord
{
	BIFF_RECORD_DEFINE_TYPE_INFO(Begin)
	BASE_OBJECT_DEFINE_CLASS_NAME(Begin)
public:
	Begin();
	~Begin();

	BaseObjectPtr clone();

	void writeFields(CFRecord& record);
	void readFields(CFRecord& record);

	static const ElementType	type = typeBegin;

//-----------------------------
public:
	//BO_ATTRIB_MARKUP_BEGIN
	//BO_ATTRIB_MARKUP_END

};

} // namespace XLS

