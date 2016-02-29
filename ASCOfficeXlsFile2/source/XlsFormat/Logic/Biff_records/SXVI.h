#pragma once

#include "BiffRecord.h"

namespace XLS
{


// Logical representation of SXVI record in BIFF8
class SXVI: public BiffRecord
{
	BIFF_RECORD_DEFINE_TYPE_INFO(SXVI)
	BASE_OBJECT_DEFINE_CLASS_NAME(SXVI)
public:
	SXVI();
	~SXVI();

	BaseObjectPtr clone();

	void writeFields(CFRecord& record);
	void readFields(CFRecord& record);

	static const ElementType	type = typeSXVI;


};

} // namespace XLS

