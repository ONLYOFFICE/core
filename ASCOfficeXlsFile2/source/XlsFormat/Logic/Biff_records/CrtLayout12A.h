#pragma once

#include "BiffRecord.h"

namespace XLS
{;


// Logical representation of CrtLayout12A record in BIFF8
class CrtLayout12A: public BiffRecord
{
	BIFF_RECORD_DEFINE_TYPE_INFO(CrtLayout12A)
	BASE_OBJECT_DEFINE_CLASS_NAME(CrtLayout12A)
public:
	CrtLayout12A();
	~CrtLayout12A();

	BaseObjectPtr clone();

	void writeFields(CFRecord& record);
	void readFields(CFRecord& record);

	static const ElementType	type = typeCrtLayout12A;

//-----------------------------
//	BIFF_WORD userName;
public:
	//BO_ATTRIB_MARKUP_BEGIN
//		//BO_ATTRIB_MARKUP_ATTRIB(userName)
	//BO_ATTRIB_MARKUP_END

};

} // namespace XLS

