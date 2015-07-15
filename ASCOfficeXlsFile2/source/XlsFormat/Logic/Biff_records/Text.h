#pragma once

#include "BiffRecord.h"
#include <Logic/Biff_structures/LongRGB.h>

namespace XLS
{;


// Logical representation of Text record in BIFF8
class Text: public BiffRecord
{
	BIFF_RECORD_DEFINE_TYPE_INFO(Text)
	BASE_OBJECT_DEFINE_CLASS_NAME(Text)
public:
	Text();
	~Text();

	BaseObjectPtr clone();

	void writeFields(CFRecord& record);
	void readFields(CFRecord& record);

	static const ElementType	type = typeText;

//-----------------------------
	BIFF_BYTE at;
	BIFF_BYTE vat;
	BIFF_WORD wBkgMode;
	LongRGB rgbText;
	BIFF_LONG x;
	BIFF_LONG y;
	BIFF_LONG dx;
	BIFF_LONG dy;

	BIFF_BOOL fAutoColor;
	BIFF_BOOL fShowKey;
	BIFF_BOOL fShowValue;
	BIFF_BOOL fAutoText;
	BIFF_BOOL fGenerated;
	BIFF_BOOL fDeleted;
	BIFF_BOOL fAutoMode;
	BIFF_BOOL fShowLabelAndPerc;
	BIFF_BOOL fShowPercent;
	BIFF_BOOL fShowBubbleSizes;
	BIFF_BOOL fShowLabel;

	Icv icvText;
	BIFF_BYTE dlp;
	BIFF_BYTE iReadingOrder;
	BIFF_WORD trot;
};

} // namespace XLS

