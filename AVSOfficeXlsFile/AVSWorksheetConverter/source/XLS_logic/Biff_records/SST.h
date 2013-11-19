#pragma once

#include "BiffRecordContinued.h"
#include <XLS_logic/Biff_structures/XLUnicodeRichExtendedString.h>

namespace XLS
{;


// Logical representation of SST record in BIFF8
class SST: public BiffRecordContinued
{
	BIFF_RECORD_DEFINE_TYPE_INFO(SST)
	BASE_OBJECT_DEFINE_CLASS_NAME(SST)
public:
	SST(const WORD code_page);
	~SST();

	BaseObjectPtr clone();

	void writeFields(CFRecord& record);
	void readFields(CFRecord& record);
private:
	BIFF_LONG cstTotal;
	BIFF_LONG cstUnique;
	BiffStructurePtrVector rgb;

private:
	WORD code_page_;

public:
	BO_ATTRIB_MARKUP_BEGIN
		BO_ATTRIB_MARKUP_ATTRIB(cstTotal)
		BO_ATTRIB_MARKUP_ATTRIB(cstUnique)
		proc.markVector(rgb, XLUnicodeRichExtendedString(continue_records[rt_Continue]));
		//BO_ATTRIB_MARKUP_VECTOR_COMPLEX(rgb, XLUnicodeRichExtendedString)
	BO_ATTRIB_MARKUP_END

};

} // namespace XLS

