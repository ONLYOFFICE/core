#pragma once

#include "BiffRecord.h"

namespace XLS
{


// Logical representation of DConBin record in BIFF8
class DConBin: public BiffRecord
{
	BIFF_RECORD_DEFINE_TYPE_INFO(DConBin)
	BASE_OBJECT_DEFINE_CLASS_NAME(DConBin)
public:
	DConBin();
	~DConBin();

	BaseObjectPtr clone();

	void writeFields(CFRecord& record);
	void readFields(CFRecord& record);

	static const ElementType	type = typeDConBin;


};

} // namespace XLS

