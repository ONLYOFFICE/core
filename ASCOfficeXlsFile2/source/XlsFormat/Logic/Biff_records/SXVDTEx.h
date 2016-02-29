#pragma once

#include "BiffRecord.h"

namespace XLS
{


// Logical representation of SXVDTEx record in BIFF8
class SXVDTEx: public BiffRecord
{
	BIFF_RECORD_DEFINE_TYPE_INFO(SXVDTEx)
	BASE_OBJECT_DEFINE_CLASS_NAME(SXVDTEx)
public:
	SXVDTEx();
	~SXVDTEx();

	BaseObjectPtr clone();

	void writeFields(CFRecord& record);
	void readFields(CFRecord& record);

	static const ElementType	type = typeSXVDTEx;


};

} // namespace XLS

