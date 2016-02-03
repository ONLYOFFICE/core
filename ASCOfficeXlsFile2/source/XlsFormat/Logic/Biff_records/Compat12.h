#pragma once

#include "BiffRecord.h"

namespace XLS
{


// Logical representation of Compat12 record in BIFF8
class Compat12: public BiffRecord
{
	BIFF_RECORD_DEFINE_TYPE_INFO(Compat12)
	BASE_OBJECT_DEFINE_CLASS_NAME(Compat12)
public:
	Compat12();
	~Compat12();

	BaseObjectPtr clone();

	void writeFields(CFRecord& record);
	void readFields(CFRecord& record);

	static const ElementType	type = typeCompat12;

//-----------------------------
	bool fNoCompatChk;
};

} // namespace XLS

