#pragma once

#include "BiffRecord.h"

namespace XLS
{;


// Logical representation of Theme record in BIFF8
class Theme: public BiffRecord
{
	BIFF_RECORD_DEFINE_TYPE_INFO(Theme)
	BASE_OBJECT_DEFINE_CLASS_NAME(Theme)
public:
	Theme();
	~Theme();

	BaseObjectPtr clone();

	void writeFields(CFRecord& record);
	void readFields(CFRecord& record);
private:
	BIFF_DWORD dwThemeVersion;
	//std::wstring rgb;
public:
	BO_ATTRIB_MARKUP_BEGIN
		BO_ATTRIB_MARKUP_ATTRIB(dwThemeVersion)
/*
		if(!dwThemeVersion)
		{
			xml_tag->Puttext(rgb.c_str());
		}
*/
	BO_ATTRIB_MARKUP_END

};

} // namespace XLS

