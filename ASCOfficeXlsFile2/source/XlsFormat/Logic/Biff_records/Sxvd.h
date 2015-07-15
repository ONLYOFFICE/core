#pragma once

#include "BiffRecord.h"

namespace XLS
{;


// Logical representation of Sxvd record in BIFF8
class Sxvd: public BiffRecord
{
	BIFF_RECORD_DEFINE_TYPE_INFO(Sxvd)
	BASE_OBJECT_DEFINE_CLASS_NAME(Sxvd)
public:
	Sxvd();
	~Sxvd();

	BaseObjectPtr clone();

	void writeFields(CFRecord& record);
	void readFields(CFRecord& record);

	static const ElementType	type = typeSxvd;

//-----------------------------
//	BIFF_WORD userName;
public:
	//BO_ATTRIB_MARKUP_BEGIN
//		//BO_ATTRIB_MARKUP_ATTRIB(userName)
	//BO_ATTRIB_MARKUP_END

};

} // namespace XLS

