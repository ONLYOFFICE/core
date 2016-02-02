#pragma once

#include "BiffRecord.h"
#include <Logic/Biff_structures/LongRGB.h>

namespace XLS
{


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

	int serialize(std::wostream & _stream);

	BIFF_BYTE	at;
	BIFF_BYTE	vat;
	BIFF_WORD	wBkgMode;
	LongRGB		rgbText;
	BIFF_LONG	x;
	BIFF_LONG	y;
	BIFF_LONG	dx;
	BIFF_LONG	dy;

	bool fAutoColor;
	bool fShowKey;
	bool fShowValue;
	bool fAutoText;
	bool fGenerated;
	bool fDeleted;
	bool fAutoMode;
	bool fShowLabelAndPerc;
	bool fShowPercent;
	bool fShowBubbleSizes;
	bool fShowLabel;

	Icv			icvText;
	BIFF_BYTE	dlp;
	BIFF_BYTE	iReadingOrder;
	_UINT16		trot;

//-----------------------------
	bool is_area;

	int rot;
};

} // namespace XLS

