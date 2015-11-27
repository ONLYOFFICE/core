#pragma once

#include "BiffRecord.h"

namespace XLS
{


// Logical representation of CrtLine record in BIFF8
class CrtLine: public BiffRecord
{
	BIFF_RECORD_DEFINE_TYPE_INFO(CrtLine)
	BASE_OBJECT_DEFINE_CLASS_NAME(CrtLine)
public:
	CrtLine();
	~CrtLine();

	BaseObjectPtr clone();

	void writeFields(CFRecord& record);
	void readFields(CFRecord& record);

	static const ElementType	type = typeCrtLine;

//-----------------------------
	BIFF_WORD		id;
	BaseObjectPtr	m_LineFormat;
};

} // namespace XLS

