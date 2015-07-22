#pragma once

#include "BiffRecordContinued.h"
#include <Logic/Biff_structures/XLUnicodeRichExtendedString.h>

namespace XLS
{;


// Logical representation of SST record in BIFF8
class SST: public BiffRecordContinued
{
	BIFF_RECORD_DEFINE_TYPE_INFO(SST)
	BASE_OBJECT_DEFINE_CLASS_NAME(SST)
public:
	SST(const unsigned short code_page);
	~SST();

	BaseObjectPtr clone();

	void writeFields(CFRecord& record);
	void readFields(CFRecord& record);

	static const ElementType	type = typeSST;

	int serialize(std::wostream & stream);

//-----------------------------
	BIFF_LONG cstTotal;
	BIFF_LONG cstUnique;
	BiffStructurePtrVector rgb;

//-----------------------------
	unsigned short code_page_;

};

} // namespace XLS

