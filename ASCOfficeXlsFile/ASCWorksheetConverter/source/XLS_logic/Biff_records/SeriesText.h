#pragma once

#include "BiffRecord.h"
#include <XLS_logic/Biff_structures/BiffString.h>

namespace XLS
{;


// Logical representation of SeriesText record in BIFF8
class SeriesText: public BiffRecord
{
	BIFF_RECORD_DEFINE_TYPE_INFO(SeriesText)
	BASE_OBJECT_DEFINE_CLASS_NAME(SeriesText)
public:
	SeriesText();
	~SeriesText();

	BaseObjectPtr clone();

	void writeFields(CFRecord& record);
	void readFields(CFRecord& record);
private:
	ShortXLUnicodeString stText;
public:
	BO_ATTRIB_MARKUP_BEGIN
		BO_ATTRIB_MARKUP_ATTRIB(stText)
	BO_ATTRIB_MARKUP_END

};

} // namespace XLS

