#pragma once

#include "BiffRecord.h"

namespace XLS
{


// Logical representation of SxRule record in BIFF8
class SxRule: public BiffRecord
{
	BIFF_RECORD_DEFINE_TYPE_INFO(SxRule)
	BASE_OBJECT_DEFINE_CLASS_NAME(SxRule)
public:
	SxRule();
	~SxRule();

	BaseObjectPtr clone();

	void writeFields(CFRecord& record);
	void readFields(CFRecord& record);

	static const ElementType	type = typeSxRule;

//-----------------------------
//	BIFF_WORD userName;
public:
	//BO_ATTRIB_MARKUP_BEGIN
//		//BO_ATTRIB_MARKUP_ATTRIB(userName)
	//BO_ATTRIB_MARKUP_END

};

} // namespace XLS

