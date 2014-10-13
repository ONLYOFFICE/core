#pragma once

#include "BiffRecord.h"
#include <Logic/Biff_structures/BiffString.h>

namespace XLS
{;


// Logical representation of FileSharing record in BIFF8
class FileSharing: public BiffRecord
{
	BIFF_RECORD_DEFINE_TYPE_INFO(FileSharing)
	BASE_OBJECT_DEFINE_CLASS_NAME(FileSharing)
public:
	FileSharing();
	~FileSharing();

	BaseObjectPtr clone();

	void writeFields(CFRecord& record);
	void readFields(CFRecord& record);
private:
	Boolean<unsigned __int16> fReadOnlyRec;
	unsigned __int16 wResPassNum;
	BIFF_BSTR wResPass;
	BIFF_WORD iNoResPass;
	XLUnicodeString stUNUsername;
public:
	BO_ATTRIB_MARKUP_BEGIN
	BO_ATTRIB_MARKUP_ATTRIB(fReadOnlyRec)
	BO_ATTRIB_MARKUP_ATTRIB(wResPass)
	if(!wResPassNum)
	{
		BO_ATTRIB_MARKUP_ATTRIB(iNoResPass)
	}
	else
	{
		BO_ATTRIB_MARKUP_ATTRIB(stUNUsername)
	}
	BO_ATTRIB_MARKUP_END

};

} // namespace XLS

