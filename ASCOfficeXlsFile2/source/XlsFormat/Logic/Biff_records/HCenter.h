#pragma once

#include "BiffRecord.h"

namespace XLS
{;


// Logical representation of HCenter record in BIFF8
class HCenter: public BiffRecord
{
	BIFF_RECORD_DEFINE_TYPE_INFO(HCenter)
	BASE_OBJECT_DEFINE_CLASS_NAME(HCenter)
public:
	HCenter();
	~HCenter();

	BaseObjectPtr clone();

	void writeFields(CFRecord& record);
	void readFields(CFRecord& record);

	static const ElementType	type = typeHCenter;

//-----------------------------
	Boolean<unsigned short> hcenter;
public:
	//BO_ATTRIB_MARKUP_BEGIN
		//BO_ATTRIB_MARKUP_ATTRIB(hcenter)
	//BO_ATTRIB_MARKUP_END

};

} // namespace XLS

