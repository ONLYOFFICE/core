#pragma once

#include "BiffRecord.h"

namespace XLS
{


// Logical representation of PicF record in BIFF8
class PicF: public BiffRecord
{
	BIFF_RECORD_DEFINE_TYPE_INFO(PicF)
	BASE_OBJECT_DEFINE_CLASS_NAME(PicF)
public:
	PicF();
	~PicF();

	BaseObjectPtr clone();

	void writeFields(CFRecord& record);
	void readFields(CFRecord& record);

	static const ElementType	type = typePicF;
};

} // namespace XLS

