#pragma once

#include "BiffRecord.h"

namespace XLS
{;


// Logical representation of CrtLink record in BIFF8
class CrtLink: public BiffRecord
{
	BIFF_RECORD_DEFINE_TYPE_INFO(CrtLink)
	BASE_OBJECT_DEFINE_CLASS_NAME(CrtLink)
public:
	CrtLink();
	~CrtLink();

	BaseObjectPtr clone();

	void writeFields(CFRecord& record);
	void readFields(CFRecord& record);

	static const ElementType	type = typeCrtLink;

public:
	//BO_ATTRIB_MARKUP_BEGIN
	//BO_ATTRIB_MARKUP_END

};

} // namespace XLS

