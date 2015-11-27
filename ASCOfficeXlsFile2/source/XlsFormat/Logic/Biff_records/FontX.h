#pragma once

#include "BiffRecord.h"

namespace XLS
{


// Logical representation of FontX record in BIFF8
class FontX: public BiffRecord
{
	BIFF_RECORD_DEFINE_TYPE_INFO(FontX)
	BASE_OBJECT_DEFINE_CLASS_NAME(FontX)
public:
	FontX();
	~FontX();

	BaseObjectPtr clone();

	void writeFields(CFRecord& record);
	void readFields(CFRecord& record);

	static const ElementType	type = typeFontX;

//-----------------------------
	BIFF_WORD iFont;

};

} // namespace XLS

