#pragma once

#include "BiffRecord.h"
#include <XLS_logic/Biff_structures/FrtRefHeaderU.h>
#include <XLS_logic/Biff_structures/XFProps.h>

namespace XLS
{;


// Logical representation of DXF record in BIFF8
class DXF: public BiffRecord
{
	BIFF_RECORD_DEFINE_TYPE_INFO(DXF)
	BASE_OBJECT_DEFINE_CLASS_NAME(DXF)
public:
	DXF();
	~DXF();

	BaseObjectPtr clone();

	void writeFields(CFRecord& record);
	void readFields(CFRecord& record);
private:
	FrtRefHeaderU frtRefHeaderU;
	BIFF_BOOL fNewBorder;
	XFProps xfprops;
public:
	BO_ATTRIB_MARKUP_BEGIN
		BO_ATTRIB_MARKUP_COMPLEX(frtRefHeaderU)
		BO_ATTRIB_MARKUP_ATTRIB(fNewBorder)
		BO_ATTRIB_MARKUP_COMPLEX(xfprops)
	BO_ATTRIB_MARKUP_END

};

} // namespace XLS

