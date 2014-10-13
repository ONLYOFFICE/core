#pragma once

#include "BiffRecord.h"
#include <Logic/Biff_structures/BiffString.h>

namespace XLS
{;


// Logical representation of NameCmt record in BIFF8
class NameCmt: public BiffRecord
{
	BIFF_RECORD_DEFINE_TYPE_INFO(NameCmt)
	BASE_OBJECT_DEFINE_CLASS_NAME(NameCmt)
public:
	NameCmt();
	NameCmt(const XLUnicodeStringNoCch lbl_name);
	~NameCmt();

	BaseObjectPtr clone();

	void writeFields(CFRecord& record);
	void readFields(CFRecord& record);
private:
	XLUnicodeStringNoCch name;
	XLUnicodeStringNoCch comment;
public:
	BO_ATTRIB_MARKUP_BEGIN
		BO_ATTRIB_MARKUP_ATTRIB(name)
		BO_ATTRIB_MARKUP_ATTRIB(comment)
	BO_ATTRIB_MARKUP_END

};

} // namespace XLS

