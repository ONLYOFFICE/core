#pragma once

#include "BiffRecord.h"
#include <XLS_logic/Biff_structures/BiffString.h>

namespace XLS
{;


// Logical representation of Footer record in BIFF8
class Footer: public BiffRecord
{
	BIFF_RECORD_DEFINE_TYPE_INFO(Footer)
	BASE_OBJECT_DEFINE_CLASS_NAME(Footer)
public:
	Footer();
	~Footer();

	BaseObjectPtr clone();

	void writeFields(CFRecord& record);
	void readFields(CFRecord& record);
private:
	XLUnicodeString ast;
public:
	BO_ATTRIB_MARKUP_BEGIN
		BO_ATTRIB_MARKUP_ATTRIB(ast)
	BO_ATTRIB_MARKUP_END

};

} // namespace XLS

