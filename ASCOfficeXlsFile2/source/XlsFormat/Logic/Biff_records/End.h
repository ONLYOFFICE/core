#pragma once

#include "BiffRecord.h"

namespace XLS
{;


// Logical representation of End record in BIFF8
class End: public BiffRecord
{
	BIFF_RECORD_DEFINE_TYPE_INFO(End)
	BASE_OBJECT_DEFINE_CLASS_NAME(End)
public:
	End();
	~End();

	BaseObjectPtr clone();

	void writeFields(CFRecord& record);
	void readFields(CFRecord& record);

	static const ElementType	type = typeEnd;

//-----------------------------
public:
	//BO_ATTRIB_MARKUP_BEGIN
	//BO_ATTRIB_MARKUP_END

};

} // namespace XLS

