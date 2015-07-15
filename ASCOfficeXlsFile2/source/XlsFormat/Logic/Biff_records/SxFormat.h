#pragma once

#include "BiffRecord.h"

namespace XLS
{;


// Logical representation of SxFormat record in BIFF8
class SxFormat: public BiffRecord
{
	BIFF_RECORD_DEFINE_TYPE_INFO(SxFormat)
	BASE_OBJECT_DEFINE_CLASS_NAME(SxFormat)
public:
	SxFormat();
	~SxFormat();

	BaseObjectPtr clone();

	void writeFields(CFRecord& record);
	void readFields(CFRecord& record);

	static const ElementType	type = typeSxFormat;

//-----------------------------
//	BIFF_WORD userName;
public:
	//BO_ATTRIB_MARKUP_BEGIN
//		//BO_ATTRIB_MARKUP_ATTRIB(userName)
	//BO_ATTRIB_MARKUP_END

};

} // namespace XLS

