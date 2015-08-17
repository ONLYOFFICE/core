#pragma once

#include "BiffRecord.h"

namespace XLS
{


// Logical representation of SxTbpg record in BIFF8
class SxTbpg: public BiffRecord
{
	BIFF_RECORD_DEFINE_TYPE_INFO(SxTbpg)
	BASE_OBJECT_DEFINE_CLASS_NAME(SxTbpg)
public:
	SxTbpg();
	~SxTbpg();

	BaseObjectPtr clone();

	void writeFields(CFRecord& record);
	void readFields(CFRecord& record);

	static const ElementType	type = typeSxTbpg;

//-----------------------------
//	BIFF_WORD userName;
public:
	//BO_ATTRIB_MARKUP_BEGIN
//		//BO_ATTRIB_MARKUP_ATTRIB(userName)
	//BO_ATTRIB_MARKUP_END

};

} // namespace XLS

