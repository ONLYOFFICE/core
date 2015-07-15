#pragma once

#include "BiffRecord.h"
#include <Logic/Biff_structures/MDXStrIndex.h>

namespace XLS
{;


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
	BIFF_LONG istrConnName;
	BIFF_BYTE tfnSrc;
	BIFF_BYTE sso;
	BIFF_DWORD istrSetDef;
	BIFF_LONG cistr;
	BiffStructurePtrVector rgistr;
public:
	//BO_ATTRIB_MARKUP_BEGIN
		//BO_ATTRIB_MARKUP_ATTRIB(istrConnName)
		//BO_ATTRIB_MARKUP_ATTRIB(tfnSrc)
		//BO_ATTRIB_MARKUP_ATTRIB(sso)
		//BO_ATTRIB_MARKUP_ATTRIB(istrSetDef)
		//BO_ATTRIB_MARKUP_ATTRIB(cistr)
		//BO_ATTRIB_MARKUP_VECTOR_COMPLEX(rgistr, MDXStrIndex)
	//BO_ATTRIB_MARKUP_END

};

} // namespace XLS

