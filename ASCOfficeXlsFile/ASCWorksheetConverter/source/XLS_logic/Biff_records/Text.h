#pragma once

#include "BiffRecord.h"
#include <XLS_logic/Biff_structures/LongRGB.h>

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
private:
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

public:
	BO_ATTRIB_MARKUP_BEGIN
		BO_ATTRIB_MARKUP_ATTRIB(at)
		BO_ATTRIB_MARKUP_ATTRIB(vat)
		BO_ATTRIB_MARKUP_ATTRIB(wBkgMode)
		BO_ATTRIB_MARKUP_COMPLEX(rgbText)
		BO_ATTRIB_MARKUP_ATTRIB(x)
		BO_ATTRIB_MARKUP_ATTRIB(y)
		BO_ATTRIB_MARKUP_ATTRIB(dx)
		BO_ATTRIB_MARKUP_ATTRIB(dy)

		BO_ATTRIB_MARKUP_ATTRIB(fAutoColor)
		BO_ATTRIB_MARKUP_ATTRIB(fShowKey)
		BO_ATTRIB_MARKUP_ATTRIB(fShowValue)
		BO_ATTRIB_MARKUP_ATTRIB(fAutoText)
		BO_ATTRIB_MARKUP_ATTRIB(fGenerated)
		BO_ATTRIB_MARKUP_ATTRIB(fDeleted)
		BO_ATTRIB_MARKUP_ATTRIB(fAutoMode)
		BO_ATTRIB_MARKUP_ATTRIB(fShowLabelAndPerc)
		BO_ATTRIB_MARKUP_ATTRIB(fShowPercent)
		BO_ATTRIB_MARKUP_ATTRIB(fShowBubbleSizes)
		BO_ATTRIB_MARKUP_ATTRIB(fShowLabel)

		BO_ATTRIB_MARKUP_ATTRIB(icvText)
		BO_ATTRIB_MARKUP_ATTRIB(dlp)
		BO_ATTRIB_MARKUP_ATTRIB(iReadingOrder)
		BO_ATTRIB_MARKUP_ATTRIB(trot)
	BO_ATTRIB_MARKUP_END

};

} // namespace XLS

