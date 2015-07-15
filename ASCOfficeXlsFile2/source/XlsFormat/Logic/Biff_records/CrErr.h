#pragma once

#include "BiffRecord.h"

namespace XLS
{;


// Logical representation of CrErr record in BIFF8
class CrErr: public BiffRecord
{
	BIFF_RECORD_DEFINE_TYPE_INFO(CrErr)
	BASE_OBJECT_DEFINE_CLASS_NAME(CrErr)
public:
	CrErr();
	~CrErr();

	BaseObjectPtr clone();

	void writeFields(CFRecord& record);
	void readFields(CFRecord& record);

	static const ElementType	type = typeCrErr;

//-----------------------------
//	BIFF_WORD userName;
public:
	//BO_ATTRIB_MARKUP_BEGIN
//		//BO_ATTRIB_MARKUP_ATTRIB(userName)
	//BO_ATTRIB_MARKUP_END

};

} // namespace XLS

