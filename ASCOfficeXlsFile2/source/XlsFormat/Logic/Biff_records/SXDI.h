#pragma once

#include "BiffRecord.h"

namespace XLS
{;


// Logical representation of SXDI record in BIFF8
class SXDI: public BiffRecord
{
	BIFF_RECORD_DEFINE_TYPE_INFO(SXDI)
	BASE_OBJECT_DEFINE_CLASS_NAME(SXDI)
public:
	SXDI();
	~SXDI();

	BaseObjectPtr clone();

	void writeFields(CFRecord& record);
	void readFields(CFRecord& record);

	static const ElementType	type = typeSXDI;

//-----------------------------
//	BIFF_WORD userName;
public:
	//BO_ATTRIB_MARKUP_BEGIN
//		//BO_ATTRIB_MARKUP_ATTRIB(userName)
	//BO_ATTRIB_MARKUP_END

};

} // namespace XLS

