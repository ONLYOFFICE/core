#pragma once

#include "BiffRecord.h"

namespace XLS
{;


// Logical representation of MTRSettings record in BIFF8
class MTRSettings: public BiffRecord
{
	BIFF_RECORD_DEFINE_TYPE_INFO(MTRSettings)
	BASE_OBJECT_DEFINE_CLASS_NAME(MTRSettings)
public:
	MTRSettings();
	~MTRSettings();

	BaseObjectPtr clone();

	void writeFields(CFRecord& record);
	void readFields(CFRecord& record);
private:
	Boolean<DWORD> fMTREnabled;
	Boolean<DWORD> fUserSetThreadCount;
	BIFF_DWORD cUserThreadCount;
public:
	BO_ATTRIB_MARKUP_BEGIN
		BO_ATTRIB_MARKUP_ATTRIB(fMTREnabled)
		BO_ATTRIB_MARKUP_ATTRIB(fUserSetThreadCount)
		BO_ATTRIB_MARKUP_ATTRIB(cUserThreadCount)
	BO_ATTRIB_MARKUP_END

};

} // namespace XLS

