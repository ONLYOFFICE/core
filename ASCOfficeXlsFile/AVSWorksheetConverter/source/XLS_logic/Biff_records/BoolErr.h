#pragma once

#include "BiffRecord.h"
#include <XLS_logic/Biff_structures/CellOffsetResender.h>
#include <XLS_logic/Biff_structures/Cell.h>
#include <XLS_logic/Biff_structures/Bes.h>

namespace XLS
{;


// Logical representation of BoolErr record in BIFF8
class BoolErr: public BiffRecord
{
	BIFF_RECORD_DEFINE_TYPE_INFO(BoolErr)
	BASE_OBJECT_DEFINE_CLASS_NAME(BoolErr)
public:
	BoolErr();
	~BoolErr();

	BaseObjectPtr clone();

	void writeFields(CFRecord& record);
	void readFields(CFRecord& record);

	const CellRef getLocation() const;

private:
	CellOffsetResender resender;
	Cell cell;
	Bes bes;
public:
	BO_ATTRIB_MARKUP_BEGIN
		BO_ATTRIB_MARKUP_COMPLEX(resender)
		BO_ATTRIB_MARKUP_COMPLEX(cell)
		BO_ATTRIB_MARKUP_COMPLEX(bes)
	BO_ATTRIB_MARKUP_END

};

} // namespace XLS

