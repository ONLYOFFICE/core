#pragma once

#include "BiffRecord.h"
#include <Logic/Biff_structures/VertBrk.h>

namespace XLS
{;


// Logical representation of VerticalPageBreaks record in BIFF8
class VerticalPageBreaks: public BiffRecord
{
	BIFF_RECORD_DEFINE_TYPE_INFO(VerticalPageBreaks)
	BASE_OBJECT_DEFINE_CLASS_NAME(VerticalPageBreaks)
public:
	VerticalPageBreaks();
	~VerticalPageBreaks();

	BaseObjectPtr clone();

	void writeFields(CFRecord& record);
	void readFields(CFRecord& record);
private:
	BIFF_WORD cbrk;
	BiffStructurePtrVector rgbrk;
public:
	BO_ATTRIB_MARKUP_BEGIN
		BO_ATTRIB_MARKUP_ATTRIB(cbrk)
		BO_ATTRIB_MARKUP_VECTOR_COMPLEX(rgbrk, VertBrk)
	BO_ATTRIB_MARKUP_END

};

} // namespace XLS

