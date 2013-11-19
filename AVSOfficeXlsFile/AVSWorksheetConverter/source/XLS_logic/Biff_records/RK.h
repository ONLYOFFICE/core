#pragma once

#include "BiffRecord.h"
#include <XLS_logic/Biff_structures/RkRec.h>
#include <XLS_logic/Biff_structures/Cell.h>

namespace XLS
{;


// Logical representation of RK record in BIFF8
class RK: public BiffRecord
{
	BIFF_RECORD_DEFINE_TYPE_INFO(RK)
	BASE_OBJECT_DEFINE_CLASS_NAME(RK)
public:
	RK();
	~RK();

	BaseObjectPtr clone();

	void writeFields(CFRecord& record);
	void readFields(CFRecord& record);

	const CellRef getLocation() const;

private:
	RkRec rkrec;
	Cell cell;
public:
	BO_ATTRIB_MARKUP_BEGIN
		BO_ATTRIB_MARKUP_COMPLEX(rkrec)
		BO_ATTRIB_MARKUP_COMPLEX(cell)
	BO_ATTRIB_MARKUP_END

};

} // namespace XLS

