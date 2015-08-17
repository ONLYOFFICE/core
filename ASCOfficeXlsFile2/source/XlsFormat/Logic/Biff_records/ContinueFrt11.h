#pragma once

#include "BiffRecord.h"

namespace XLS
{


// Logical representation of ContinueFrt11 record in BIFF8
class ContinueFrt11: public BiffRecord
{
	BIFF_RECORD_DEFINE_TYPE_INFO(ContinueFrt11)
	BASE_OBJECT_DEFINE_CLASS_NAME(ContinueFrt11)
public:
	ContinueFrt11();
	~ContinueFrt11();

	BaseObjectPtr clone();

	void writeFields(CFRecord& record);
	void readFields(CFRecord& record);

	static const ElementType	type = typeContinueFrt11;

public:
	//BO_ATTRIB_MARKUP_BEGIN
	//BO_ATTRIB_MARKUP_END

};

} // namespace XLS

