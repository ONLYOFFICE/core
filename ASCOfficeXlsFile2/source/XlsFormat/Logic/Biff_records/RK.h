#pragma once

#include "BiffRecord.h"
#include <Logic/Biff_structures/RkRec.h>
#include <Logic/Biff_structures/Cell.h>

namespace XLS
{


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

	static const ElementType	type = typeRK;

	int serialize(std::wostream & stream);

	const CellRef getLocation() const;

//-----------------------------
	RkRec rkrec;
	Cell cell;

};

} // namespace XLS

