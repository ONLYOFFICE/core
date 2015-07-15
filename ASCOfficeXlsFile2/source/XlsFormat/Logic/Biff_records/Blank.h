#pragma once

#include "BiffRecord.h"
#include <Logic/Biff_structures/CellOffsetResender.h>
#include <Logic/Biff_structures/Cell.h>

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

	static const ElementType	type = typeBlank;


	const CellRef getLocation() const;

//-----------------------------
	CellOffsetResender resender;
	Cell cell;

};

} // namespace XLS

