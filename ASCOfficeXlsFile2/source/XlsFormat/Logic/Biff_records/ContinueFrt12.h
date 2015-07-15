#pragma once

#include "BiffRecord.h"

namespace XLS
{;


// Logical representation of ContinueFrt12 record in BIFF8
class ContinueFrt12: public BiffRecord
{
	BIFF_RECORD_DEFINE_TYPE_INFO(ContinueFrt12)
	BASE_OBJECT_DEFINE_CLASS_NAME(ContinueFrt12)
public:
	ContinueFrt12();
	~ContinueFrt12();

	BaseObjectPtr clone();

	void writeFields(CFRecord& record);
	void readFields(CFRecord& record);

	static const ElementType	type = typeContinueFrt12;

public:
	//BO_ATTRIB_MARKUP_BEGIN		
	//BO_ATTRIB_MARKUP_END
};

typedef boost::shared_ptr<ContinueFrt12> ContinueFrt12Ptr;

} // namespace XLS

