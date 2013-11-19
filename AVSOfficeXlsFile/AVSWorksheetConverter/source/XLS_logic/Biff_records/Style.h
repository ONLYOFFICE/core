#pragma once

#include "BiffRecord.h"
#include <XLS_logic/Biff_structures/BiffString.h>
#include <XLS_logic/Biff_structures/BuiltInStyle.h>

namespace XLS
{;


// Logical representation of Style record in BIFF8
class Style: public BiffRecord
{
	BIFF_RECORD_DEFINE_TYPE_INFO(Style)
	BASE_OBJECT_DEFINE_CLASS_NAME(Style)
public:
	Style();
	~Style();

	BaseObjectPtr clone();

	void writeFields(CFRecord& record);
	void readFields(CFRecord& record);

private:
	BIFF_WORD ixfe;
	BIFF_BOOL fBuiltIn;
	BuiltInStyle builtInData;
	XLUnicodeString user;

public:
	BO_ATTRIB_MARKUP_BEGIN
		BO_ATTRIB_MARKUP_ATTRIB(ixfe)
		BO_ATTRIB_MARKUP_ATTRIB(fBuiltIn)
		if(fBuiltIn)
		{
			BO_ATTRIB_MARKUP_COMPLEX(builtInData)
		}
		else
		{
			BO_ATTRIB_MARKUP_ATTRIB(user)
		}

	BO_ATTRIB_MARKUP_END

};

} // namespace XLS

