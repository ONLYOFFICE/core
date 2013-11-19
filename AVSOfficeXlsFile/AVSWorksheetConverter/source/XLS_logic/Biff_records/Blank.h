#pragma once

#include "BiffRecord.h"
#include <XLS_logic/Biff_structures/CellOffsetResender.h>
#include <XLS_logic/Biff_structures/Cell.h>

namespace XLS
{;


// Logical representation of Blank record in BIFF8
class Blank: public BiffRecord
{
	BIFF_RECORD_DEFINE_TYPE_INFO(Blank)
	BASE_OBJECT_DEFINE_CLASS_NAME(Blank)
public:
	Blank();
	~Blank();

	BaseObjectPtr clone();

	void writeFields(CFRecord& record);
	void readFields(CFRecord& record);

	const CellRef getLocation() const;

private:
	CellOffsetResender resender;
	Cell cell;
public:
	BO_ATTRIB_MARKUP_BEGIN
		BO_ATTRIB_MARKUP_COMPLEX(resender)
		BO_ATTRIB_MARKUP_COMPLEX(cell)
	BO_ATTRIB_MARKUP_END

};

} // namespace XLS

