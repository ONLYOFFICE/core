#pragma once

#include "BiffRecord.h"

namespace XLS
{


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

	static const ElementType	type = typeTheme;

//-----------------------------
	BIFF_DWORD dwThemeVersion;
	//std::wstring rgb;

};

} // namespace XLS

