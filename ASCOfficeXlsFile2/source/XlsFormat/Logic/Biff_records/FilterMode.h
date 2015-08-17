#pragma once

#include "BiffRecord.h"

namespace XLS
{


// Logical representation of FilterMode record in BIFF8
class FilterMode: public BiffRecord
{
	BIFF_RECORD_DEFINE_TYPE_INFO(FilterMode)
	BASE_OBJECT_DEFINE_CLASS_NAME(FilterMode)
public:
	FilterMode();
	~FilterMode();

	BaseObjectPtr clone();

	void writeFields(CFRecord& record);
	void readFields(CFRecord& record);

	static const ElementType	type = typeFilterMode;

public:
	//BO_ATTRIB_MARKUP_BEGIN
	//BO_ATTRIB_MARKUP_END
};

} // namespace XLS

