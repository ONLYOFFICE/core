#pragma once

#include "BiffRecord.h"

namespace XLS
{


// Logical representation of SXViewEx record in BIFF8
class SXViewEx: public BiffRecord
{
	BIFF_RECORD_DEFINE_TYPE_INFO(SXViewEx)
	BASE_OBJECT_DEFINE_CLASS_NAME(SXViewEx)
public:
	SXViewEx();
	~SXViewEx();

	BaseObjectPtr clone();

	void writeFields(CFRecord& record);
	void readFields(CFRecord& record);

	static const ElementType	type = typeSXViewEx;

};

} // namespace XLS

