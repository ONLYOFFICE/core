#pragma once

#include "BiffRecord.h"
#include <Logic/Biff_structures/BiffString.h>

namespace XLS
{


// Logical representation of Lel record in BIFF8
class Lel: public BiffRecord
{
	BIFF_RECORD_DEFINE_TYPE_INFO(Lel)
	BASE_OBJECT_DEFINE_CLASS_NAME(Lel)
public:
	Lel();
	~Lel();

	BaseObjectPtr clone();

	void writeFields(CFRecord& record);
	void readFields(CFRecord& record);
//-----------------------------
	XLUnicodeString stFormulaName;
public:
	//BO_ATTRIB_MARKUP_BEGIN
	//BO_ATTRIB_MARKUP_ATTRIB(stFormulaName)
	//BO_ATTRIB_MARKUP_END

};

} // namespace XLS

