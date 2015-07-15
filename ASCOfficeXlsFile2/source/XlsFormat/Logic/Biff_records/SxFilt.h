#pragma once

#include "BiffRecord.h"

namespace XLS
{;


// Logical representation of SxFilt record in BIFF8
class SxFilt: public BiffRecord
{
	BIFF_RECORD_DEFINE_TYPE_INFO(SxFilt)
	BASE_OBJECT_DEFINE_CLASS_NAME(SxFilt)
public:
	SxFilt();
	~SxFilt();

	BaseObjectPtr clone();

	void writeFields(CFRecord& record);
	void readFields(CFRecord& record);

	static const ElementType	type = typeSxFilt;

//-----------------------------
//	BIFF_WORD userName;
public:
	//BO_ATTRIB_MARKUP_BEGIN
//		//BO_ATTRIB_MARKUP_ATTRIB(userName)
	//BO_ATTRIB_MARKUP_END

};

} // namespace XLS

