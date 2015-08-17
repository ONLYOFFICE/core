#pragma once

#include "BiffRecord.h"

namespace XLS
{


// Logical representation of SXString record in BIFF8
class SXString: public BiffRecord
{
	BIFF_RECORD_DEFINE_TYPE_INFO(SXString)
	BASE_OBJECT_DEFINE_CLASS_NAME(SXString)
public:
	SXString();
	~SXString();

	BaseObjectPtr clone();

	void writeFields(CFRecord& record);
	void readFields(CFRecord& record);

	static const ElementType	type = typeSXString;

//-----------------------------
//	BIFF_WORD userName;
public:
	//BO_ATTRIB_MARKUP_BEGIN
//		//BO_ATTRIB_MARKUP_ATTRIB(userName)
	//BO_ATTRIB_MARKUP_END

};

} // namespace XLS

