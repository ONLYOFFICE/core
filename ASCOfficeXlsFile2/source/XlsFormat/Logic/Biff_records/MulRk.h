#pragma once

#include "BiffRecord.h"
#include <Logic/Biff_structures/RkRec.h>
#include <Logic/Biff_structures/Cell.h>

namespace XLS
{


// Logical representation of MulRk record in BIFF8
class MulRk: public BiffRecord
{
	BIFF_RECORD_DEFINE_TYPE_INFO(MulRk)
	BASE_OBJECT_DEFINE_CLASS_NAME(MulRk)
public:
	MulRk();
	~MulRk();

	BaseObjectPtr clone();

	void writeFields(CFRecord& record);
	void readFields(CFRecord& record);

	const long GetRow() const;

	int serialize(std::wostream & stream);
//-----------------------------
	Rw rw;
	BiffStructurePtrVector rgrkrec;
	BiffStructurePtrVector cells;

};

} // namespace XLS

