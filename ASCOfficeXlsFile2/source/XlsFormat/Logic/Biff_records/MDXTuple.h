#pragma once

#include "BiffRecord.h"
#include <Logic/Biff_structures/MDXStrIndex.h>

namespace XLS
{;


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
private:
	BIFF_LONG istrConnName;
	BIFF_BYTE tfnSrc;
	BIFF_LONG cistr;
	BiffStructurePtrVector rgistr;
public:
	BO_ATTRIB_MARKUP_BEGIN
		BO_ATTRIB_MARKUP_ATTRIB(istrConnName)
		BO_ATTRIB_MARKUP_ATTRIB(tfnSrc)
		BO_ATTRIB_MARKUP_ATTRIB(cistr)
		BO_ATTRIB_MARKUP_VECTOR_COMPLEX(rgistr, MDXStrIndex)
	BO_ATTRIB_MARKUP_END

};

} // namespace XLS

