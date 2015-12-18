#pragma once

#include "BiffRecord.h"

namespace XLS
{


// Logical representation of SerParent record in BIFF8
class SerParent: public BiffRecord
{
	BIFF_RECORD_DEFINE_TYPE_INFO(SerParent)
	BASE_OBJECT_DEFINE_CLASS_NAME(SerParent)
public:
	SerParent();
	~SerParent();

	BaseObjectPtr clone();

	void writeFields(CFRecord& record);
	void readFields(CFRecord& record);

	static const ElementType	type = typeSerParent;

//-----------------------------
	BIFF_WORD series;

	BaseObjectPtr m_SerAuxTrend;
	BaseObjectPtr m_SerAuxErrBar;
};

} // namespace XLS

