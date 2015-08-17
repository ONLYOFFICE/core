#pragma once

#include "BiffRecord.h"
#include <Logic/Biff_structures/CellOffsetResender.h>
#include <Logic/Biff_structures/Cell.h>
#include <Logic/Biff_structures/Bes.h>

namespace XLS
{


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

	static const ElementType	type = typeBoolErr;

	int serialize(std::wostream & stream);

	const CellRef getLocation() const;

//-----------------------------
	CellOffsetResender resender;
	Cell cell;
	Bes bes;

};

} // namespace XLS

