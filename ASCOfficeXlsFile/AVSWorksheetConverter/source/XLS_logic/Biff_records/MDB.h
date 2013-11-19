#pragma once

#include "BiffRecord.h"
#include <XLS_logic/Biff_structures/MDir.h>

namespace XLS
{;


// Logical representation of MDB record in BIFF8
class MDB: public BiffRecord
{
	BIFF_RECORD_DEFINE_TYPE_INFO(MDB)
	BASE_OBJECT_DEFINE_CLASS_NAME(MDB)
public:
	MDB();
	~MDB();

	BaseObjectPtr clone();

	void writeFields(CFRecord& record);
	void readFields(CFRecord& record);
private:
	BiffStructurePtrVector rgmdir;
public:
	BO_ATTRIB_MARKUP_BEGIN
		BO_ATTRIB_MARKUP_VECTOR_COMPLEX(rgmdir, MDir)
	BO_ATTRIB_MARKUP_END

};

} // namespace XLS

