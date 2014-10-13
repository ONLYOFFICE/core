#include "precompiled_xls.h"
#include "Text.h"

namespace XLS
{;

Text::Text()
{
}


Text::~Text()
{
}


BaseObjectPtr Text::clone()
{
	return BaseObjectPtr(new Text(*this));
}


void Text::writeFields(CFRecord& record)
{
	unsigned __int16 flags1 = 0;
	unsigned __int16 flags2 = 0;

	SETBIT(flags1, 0, fAutoColor);
	SETBIT(flags1, 1, fShowKey);
	SETBIT(flags1, 2, fShowValue);
	SETBIT(flags1, 4, fAutoText);
	SETBIT(flags1, 5, fGenerated);
	SETBIT(flags1, 6, fDeleted);
	SETBIT(flags1, 7, fAutoMode);
	SETBIT(flags1, 11, fShowLabelAndPerc);
	SETBIT(flags1, 12, fShowPercent);
	SETBIT(flags1, 13, fShowBubbleSizes);
	SETBIT(flags1, 14, fShowLabel);

	SETBITS(flags2, 0, 3, dlp);
	SETBITS(flags2, 14, 15, iReadingOrder);

	record << at  << vat << wBkgMode << rgbText << x << y << dx << dy << flags1 << icvText << flags2 << trot;
}


void Text::readFields(CFRecord& record)
{
	unsigned __int16 flags1;
	unsigned __int16 flags2;
	record >> at  >> vat >> wBkgMode >> rgbText >> x >> y >> dx >> dy >> flags1 >> icvText >> flags2 >> trot;
	
	fAutoColor = GETBIT(flags1, 0);
	fShowKey = GETBIT(flags1, 1);
	fShowValue = GETBIT(flags1, 2);
	fAutoText = GETBIT(flags1, 4);
	fGenerated = GETBIT(flags1, 5);
	fDeleted = GETBIT(flags1, 6);
	fAutoMode = GETBIT(flags1, 7);
	fShowLabelAndPerc = GETBIT(flags1, 11);
	fShowPercent = GETBIT(flags1, 12);
	fShowBubbleSizes = GETBIT(flags1, 13);
	fShowLabel = GETBIT(flags1, 14);

	dlp = static_cast<unsigned char>(GETBITS(flags2, 0, 3));
	iReadingOrder = static_cast<unsigned char>(GETBITS(flags2, 14, 15));
}

} // namespace XLS

