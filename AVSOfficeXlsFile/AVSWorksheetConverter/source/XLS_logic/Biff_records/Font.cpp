#include "stdafx.h"
#include "Font.h"

namespace XLS
{;

Font::Font()
{
}


Font::~Font()
{
}


BaseObjectPtr Font::clone()
{
	return BaseObjectPtr(new Font(*this));
}


void Font::writeFields(CFRecord& record)
{
	WORD flags = 0;
	SETBIT(flags, 1, fItalic);
	SETBIT(flags, 3, fStrikeOut);
	SETBIT(flags, 4, fOutline);
	SETBIT(flags, 5, fShadow);
	SETBIT(flags, 6, fCondense);
	SETBIT(flags, 7, fExtend);

	record << dyHeight << flags;
	record << icv << bls << sss << uls << bFamily << bCharSet;
	record.reserveNBytes(1, static_cast<BYTE>(0x5E));
	record << fontName;
}


void Font::readFields(CFRecord& record)
{
	WORD flags;
	record >> dyHeight >> flags;
	fItalic = GETBIT(flags, 1);
	fStrikeOut = GETBIT(flags, 3);
	fOutline = GETBIT(flags, 4);
	fShadow = GETBIT(flags, 5);
	fCondense = GETBIT(flags, 6);
	fExtend = GETBIT(flags, 7);

	record >> icv >> bls >> sss >> uls >> bFamily >> bCharSet;
	record.skipNBytes(1);
	record >> fontName;
}

} // namespace XLS

