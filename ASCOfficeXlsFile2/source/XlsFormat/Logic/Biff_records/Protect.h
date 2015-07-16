#pragma once

#include "BiffRecord.h"

namespace XLS
{;


// Logical representation of Protect record in BIFF8
class Protect: public BiffRecord
{
	BIFF_RECORD_DEFINE_TYPE_INFO(Protect)
	BASE_OBJECT_DEFINE_CLASS_NAME(Protect)
public:
	Protect();
	~Protect();

	BaseObjectPtr clone();

	void writeFields(CFRecord& record);
	void readFields(CFRecord& record);

	static const ElementType	type = typeProtect;

//-----------------------------
	Boolean<unsigned short> fLock;
public:
	//BO_ATTRIB_MARKUP_BEGIN
		//BO_ATTRIB_MARKUP_ATTRIB(fLock)
	//BO_ATTRIB_MARKUP_END

};

} // namespace XLS

