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

	unsigned char	at;
	unsigned char	vat;

	_UINT16			wBkgMode;
	LongRGB			rgbText;

	_INT32			x;
	_INT32			y;
	_INT32			dx;
	_INT32			dy;

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

	Icv				icvText;
	unsigned char	dlp;
	unsigned char	iReadingOrder;
	_UINT16			trot;

//-----------------------------
	bool is_area;
};

} // namespace XLS

