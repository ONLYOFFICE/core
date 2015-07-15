#pragma once

#include "BiffRecord.h"

namespace XLS
{;


// Logical representation of EntExU2 record in BIFF8
class EntExU2: public BiffRecord
{
	BIFF_RECORD_DEFINE_TYPE_INFO(EntExU2)
	BASE_OBJECT_DEFINE_CLASS_NAME(EntExU2)
public:
	EntExU2();
	~EntExU2();

	BaseObjectPtr clone();

	void writeFields(CFRecord& record);
	void readFields(CFRecord& record);

	static const ElementType	type = typeEntExU2;

public:
	//BO_ATTRIB_MARKUP_BEGIN
	//BO_ATTRIB_MARKUP_END

};

} // namespace XLS

