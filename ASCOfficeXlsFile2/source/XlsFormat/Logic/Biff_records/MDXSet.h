#pragma once

#include "BiffRecord.h"
#include <Logic/Biff_structures/MDXStrIndex.h>

namespace XLS
{


// Logical representation of MDXSet record in BIFF8
class MDXSet: public BiffRecord
{
	BIFF_RECORD_DEFINE_TYPE_INFO(MDXSet)
	BASE_OBJECT_DEFINE_CLASS_NAME(MDXSet)
public:
	MDXSet();
	~MDXSet();

	BaseObjectPtr clone();

	void writeFields(CFRecord& record);
	void readFields(CFRecord& record);
//-----------------------------
	_INT32	istrConnName;
	unsigned char	tfnSrc;
	unsigned char	sso;
	_UINT32	istrSetDef;
	_INT32	cistr;
	BiffStructurePtrVector rgistr;
};

} // namespace XLS

