#pragma once

#include "BiffRecord.h"

namespace XLS
{;


// Logical representation of MDXProp record in BIFF8
class MDXProp: public BiffRecord
{
	BIFF_RECORD_DEFINE_TYPE_INFO(MDXProp)
	BASE_OBJECT_DEFINE_CLASS_NAME(MDXProp)
public:
	MDXProp();
	~MDXProp();

	BaseObjectPtr clone();

	void writeFields(CFRecord& record);
	void readFields(CFRecord& record);
//-----------------------------
	BIFF_LONG istrConnName;
	BIFF_BYTE tfnSrc;
	BIFF_DWORD istrMbr;
	BIFF_DWORD istrProp;
public:
	//BO_ATTRIB_MARKUP_BEGIN
		//BO_ATTRIB_MARKUP_ATTRIB(istrConnName)
		//BO_ATTRIB_MARKUP_ATTRIB(tfnSrc)
		//BO_ATTRIB_MARKUP_ATTRIB(istrMbr)
		//BO_ATTRIB_MARKUP_ATTRIB(istrProp)
	//BO_ATTRIB_MARKUP_END

};

} // namespace XLS

