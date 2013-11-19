#pragma once

#include "BiffRecord.h"
#include <XLS_logic/Biff_structures/BiffString.h>

namespace XLS
{;


// Logical representation of TableStyle record in BIFF8
class TableStyle: public BiffRecord
{
	BIFF_RECORD_DEFINE_TYPE_INFO(TableStyle)
	BASE_OBJECT_DEFINE_CLASS_NAME(TableStyle)
public:
	TableStyle();
	~TableStyle();

	BaseObjectPtr clone();

	void writeFields(CFRecord& record);
	void readFields(CFRecord& record);
private:
	BIFF_BOOL fIsPivot;
	BIFF_BOOL fIsTable;
	BIFF_DWORD ctse;
	LPWideStringNoCch rgchName;

public:
	BO_ATTRIB_MARKUP_BEGIN
		BO_ATTRIB_MARKUP_ATTRIB(fIsPivot)
		BO_ATTRIB_MARKUP_ATTRIB(fIsTable)
		BO_ATTRIB_MARKUP_ATTRIB(ctse)
		BO_ATTRIB_MARKUP_ATTRIB(rgchName)
	BO_ATTRIB_MARKUP_END

};

} // namespace XLS

