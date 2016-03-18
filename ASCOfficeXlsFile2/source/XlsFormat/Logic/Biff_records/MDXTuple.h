#pragma once

#include "BiffRecord.h"
#include <Logic/Biff_structures/MDXStrIndex.h>

namespace XLS
{


// Logical representation of MDXTuple record in BIFF8
class MDXTuple: public BiffRecord
{
	BIFF_RECORD_DEFINE_TYPE_INFO(MDXTuple)
	BASE_OBJECT_DEFINE_CLASS_NAME(MDXTuple)
public:
	MDXTuple();
	~MDXTuple();

	BaseObjectPtr clone();

	void writeFields(CFRecord& record);
	void readFields(CFRecord& record);

	static const ElementType	type = typeMDXTuple;

//-----------------------------
	BIFF_LONG istrConnName;
	BIFF_BYTE tfnSrc;
	BIFF_LONG cistr;
	BiffStructurePtrVector rgistr;

};

} // namespace XLS

