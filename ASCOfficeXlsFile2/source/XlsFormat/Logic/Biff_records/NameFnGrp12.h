#pragma once

#include "BiffRecord.h"
#include <Logic/Biff_structures/BiffString.h>

namespace XLS
{


// Logical representation of NameFnGrp12 record in BIFF8
class NameFnGrp12: public BiffRecord
{
	BIFF_RECORD_DEFINE_TYPE_INFO(NameFnGrp12)
	BASE_OBJECT_DEFINE_CLASS_NAME(NameFnGrp12)
public:
	NameFnGrp12();
	~NameFnGrp12();

	BaseObjectPtr clone();

	void writeFields(CFRecord& record);
	void readFields(CFRecord& record);
//-----------------------------
	BIFF_WORD cachName;
	BIFF_WORD fgrp;
	XLNameUnicodeString rgach;
public:
	//BO_ATTRIB_MARKUP_BEGIN
		//BO_ATTRIB_MARKUP_ATTRIB(cachName)
		//BO_ATTRIB_MARKUP_ATTRIB(fgrp)
		//BO_ATTRIB_MARKUP_ATTRIB(rgach)
	//BO_ATTRIB_MARKUP_END

};

} // namespace XLS

