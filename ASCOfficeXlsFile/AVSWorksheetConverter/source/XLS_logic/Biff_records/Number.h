#pragma once

#include "BiffRecord.h"
#include <XLS_logic/Biff_structures/CellOffsetResender.h>
#include <XLS_logic/Biff_structures/Cell.h>

namespace XLS
{;


// Logical representation of Number record in BIFF8
class Number: public BiffRecord
{
	BIFF_RECORD_DEFINE_TYPE_INFO(Number)
	BASE_OBJECT_DEFINE_CLASS_NAME(Number)
public:
	Number();
	~Number();

	BaseObjectPtr clone();

	void writeFields(CFRecord& record);
	void readFields(CFRecord& record);

	const CellRef getLocation() const;

private:
	CellOffsetResender resender;
	Cell cell;
	BIFF_DOUBLE num;
public:
	BO_ATTRIB_MARKUP_BEGIN
		BO_ATTRIB_MARKUP_COMPLEX(resender)
		BO_ATTRIB_MARKUP_COMPLEX(cell)
		BO_ATTRIB_MARKUP_ATTRIB(num)
	BO_ATTRIB_MARKUP_END

};

} // namespace XLS

