#pragma once

#include "BiffRecord.h"

namespace XLS
{


// Logical representation of SxView record in BIFF8
class SxView: public BiffRecord
{
	BIFF_RECORD_DEFINE_TYPE_INFO(SxView)
	BASE_OBJECT_DEFINE_CLASS_NAME(SxView)
public:
	SxView();
	~SxView();

	BaseObjectPtr clone();

	void writeFields(CFRecord& record);
	void readFields(CFRecord& record);

	static const ElementType	type = typeSxView;

};

} // namespace XLS

