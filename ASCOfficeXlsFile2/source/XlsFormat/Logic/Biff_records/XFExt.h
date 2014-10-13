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
private:
	BIFF_WORD ixfe;
	BIFF_WORD cexts;
	BiffStructurePtrVector rgExt;
public:
	BO_ATTRIB_MARKUP_BEGIN
		BO_ATTRIB_MARKUP_ATTRIB(ixfe)
		BO_ATTRIB_MARKUP_ATTRIB(cexts)
		BO_ATTRIB_MARKUP_VECTOR_COMPLEX(rgExt, ExtProp)
	BO_ATTRIB_MARKUP_END

};

} // namespace XLS

