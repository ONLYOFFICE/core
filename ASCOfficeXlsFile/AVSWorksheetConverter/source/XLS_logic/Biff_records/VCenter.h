#pragma once

#include "BiffRecord.h"

namespace XLS
{;


// Logical representation of VCenter record in BIFF8
class VCenter: public BiffRecord
{
	BIFF_RECORD_DEFINE_TYPE_INFO(VCenter)
	BASE_OBJECT_DEFINE_CLASS_NAME(VCenter)
public:
	VCenter();
	~VCenter();

	BaseObjectPtr clone();

	void writeFields(CFRecord& record);
	void readFields(CFRecord& record);
private:
	Boolean<WORD> vcenter;
public:
	BO_ATTRIB_MARKUP_BEGIN
		BO_ATTRIB_MARKUP_ATTRIB(vcenter)
	BO_ATTRIB_MARKUP_END

};

} // namespace XLS

