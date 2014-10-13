#pragma once

#include "BiffRecord.h"
#include <Logic/Biff_structures/BiffString.h>
#include <Logic/Biff_structures/Cell.h>

namespace XLS
{;


// Logical representation of Label record in BIFF8
class Label: public BiffRecord
{
	BIFF_RECORD_DEFINE_TYPE_INFO(Label)
	BASE_OBJECT_DEFINE_CLASS_NAME(Label)
public:
	Label();
	~Label();

	BaseObjectPtr clone();

	void writeFields(CFRecord& record);
	void readFields(CFRecord& record);

private:
	Cell cell;
	XLUnicodeString st;

public:
	BO_ATTRIB_MARKUP_BEGIN
		BO_ATTRIB_MARKUP_COMPLEX(cell)
		BO_ATTRIB_MARKUP_ATTRIB(st)
	BO_ATTRIB_MARKUP_END

};

} // namespace XLS

