#pragma once

#include "BiffRecord.h"

namespace XLS
{


// Logical representation of DSF record in BIFF8
class DSF: public BiffRecord
{
	BIFF_RECORD_DEFINE_TYPE_INFO(DSF)
	BASE_OBJECT_DEFINE_CLASS_NAME(DSF)
public:
	DSF();
	~DSF();

	BaseObjectPtr clone();

	void writeFields(CFRecord& record);
	void readFields(CFRecord& record);

	static const ElementType	type = typeDSF;

//-----------------------------
//	XLUnicodeString userName;
public:
	//BO_ATTRIB_MARKUP_BEGIN
//	//BO_ATTRIB_MARKUP_ATTRIB(userName)
	//BO_ATTRIB_MARKUP_END

};

} // namespace XLS

