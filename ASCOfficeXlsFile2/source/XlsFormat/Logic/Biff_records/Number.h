#pragma once

#include "BiffRecord.h"
#include <Logic/Biff_structures/CellOffsetResender.h>
#include <Logic/Biff_structures/Cell.h>

namespace XLS
{


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

	static const ElementType	type = typeNumber;

	int serialize(std::wostream & stream);

	const CellRef getLocation() const;

//-----------------------------
	CellOffsetResender resender;
	Cell cell;
	BIFF_DOUBLE num;


};

} // namespace XLS

