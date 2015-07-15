#pragma once

#include "BiffRecord.h"
#include <Logic/Biff_structures/ExtProp.h>

namespace XLS
{;


// Logical representation of XFExt record in BIFF8
class XFExt: public BiffRecord
{
	BIFF_RECORD_DEFINE_TYPE_INFO(XFExt)
	BASE_OBJECT_DEFINE_CLASS_NAME(XFExt)
public:
	XFExt();
	~XFExt();

	BaseObjectPtr clone();

	void writeFields(CFRecord& record);
	void readFields(CFRecord& record);
	
	static const ElementType	type = typeXFExt;

//-----------------------------
	BIFF_WORD ixfe;
	BIFF_WORD cexts;
	BiffStructurePtrVector rgExt;

};

} // namespace XLS

