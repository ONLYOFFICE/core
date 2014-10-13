#pragma once

#include "BiffRecord.h"
#include <Logic/Biff_structures/BuiltInStyle.h>
#include <Logic/Biff_structures/BiffString.h>
#include <Logic/Biff_structures/XFProps.h>

namespace XLS
{;


// Logical representation of StyleExt record in BIFF8
class StyleExt: public BiffRecord
{
	BIFF_RECORD_DEFINE_TYPE_INFO(StyleExt)
	BASE_OBJECT_DEFINE_CLASS_NAME(StyleExt)
public:
	StyleExt();
	~StyleExt();

	BaseObjectPtr clone();

	void writeFields(CFRecord& record);
	void readFields(CFRecord& record);

private:
	BIFF_BOOL fBuiltIn;
	BIFF_BOOL fHidden;
	BIFF_BOOL fCustom;
	BIFF_BYTE iCategory;
	BuiltInStyle builtInData;
	LPWideString stName;
	XFProps xfProps;
public:
	BO_ATTRIB_MARKUP_BEGIN
		BO_ATTRIB_MARKUP_ATTRIB(fBuiltIn)
		BO_ATTRIB_MARKUP_ATTRIB(fHidden)
		BO_ATTRIB_MARKUP_ATTRIB(fCustom)
		BO_ATTRIB_MARKUP_ATTRIB(iCategory)
		BO_ATTRIB_MARKUP_ATTRIB(stName)
		if(fBuiltIn)
		{
			BO_ATTRIB_MARKUP_COMPLEX(builtInData)
		}
		BO_ATTRIB_MARKUP_COMPLEX(xfProps)

	BO_ATTRIB_MARKUP_END

};

} // namespace XLS

