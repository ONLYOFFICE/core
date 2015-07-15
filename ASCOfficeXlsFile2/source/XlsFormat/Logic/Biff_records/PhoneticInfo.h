#pragma once

#include "BiffRecordContinued.h"
#include <Logic/Biff_structures/Phs.h>
#include <Logic/Biff_structures/SqRef.h>

namespace XLS
{;


// Logical representation of PhoneticInfo record in BIFF8
class PhoneticInfo: public BiffRecordContinued
{
	BIFF_RECORD_DEFINE_TYPE_INFO(PhoneticInfo)
	BASE_OBJECT_DEFINE_CLASS_NAME(PhoneticInfo)
public:
	PhoneticInfo();
	~PhoneticInfo();

	BaseObjectPtr clone();

	void writeFields(CFRecord& record);
	void readFields(CFRecord& record);

	static const ElementType	type = typePhoneticInfo;

//-----------------------------
	Phs phs;
	SqRef sqref;
public:
	//BO_ATTRIB_MARKUP_BEGIN
		//BO_ATTRIB_MARKUP_COMPLEX(phs)
		//BO_ATTRIB_MARKUP_COMPLEX(sqref)
	//BO_ATTRIB_MARKUP_END

};

} // namespace XLS

