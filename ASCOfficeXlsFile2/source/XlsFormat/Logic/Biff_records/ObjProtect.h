#pragma once

#include "BiffRecord.h"

namespace XLS
{;


// Logical representation of ObjProtect record in BIFF8
class ObjProtect: public BiffRecord
{
	BIFF_RECORD_DEFINE_TYPE_INFO(ObjProtect)
	BASE_OBJECT_DEFINE_CLASS_NAME(ObjProtect)
public:
	ObjProtect();
	~ObjProtect();

	BaseObjectPtr clone();

	void writeFields(CFRecord& record);
	void readFields(CFRecord& record);

	static const ElementType	type = typeObjProtect;

//-----------------------------
	Boolean<unsigned short> fLockObj;
public:
	//BO_ATTRIB_MARKUP_BEGIN
		//BO_ATTRIB_MARKUP_ATTRIB(fLockObj)
	//BO_ATTRIB_MARKUP_END

};

} // namespace XLS

