#pragma once

#include "BiffRecord.h"
#include <Logic/Biff_structures/BiffString.h>

namespace XLS
{;


// Logical representation of NamePublish record in BIFF8
class NamePublish: public BiffRecord
{
	BIFF_RECORD_DEFINE_TYPE_INFO(NamePublish)
	BASE_OBJECT_DEFINE_CLASS_NAME(NamePublish)
public:
	NamePublish();
	~NamePublish();

	BaseObjectPtr clone();

	void writeFields(CFRecord& record);
	void readFields(CFRecord& record);
private:
	BIFF_BOOL fPublished;
	BIFF_BOOL fWorkbookParam;
	XLNameUnicodeString strName;
public:
	BO_ATTRIB_MARKUP_BEGIN
		BO_ATTRIB_MARKUP_ATTRIB(fPublished)
		BO_ATTRIB_MARKUP_ATTRIB(fWorkbookParam)
		BO_ATTRIB_MARKUP_ATTRIB(strName)
	BO_ATTRIB_MARKUP_END

};

} // namespace XLS

