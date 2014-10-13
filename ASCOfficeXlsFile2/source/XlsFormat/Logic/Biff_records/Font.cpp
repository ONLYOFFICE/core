#include "precompiled_xls.h"
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
	unsigned __int16 flags = 0;
	SETBIT(flags, 1, fItalic);
	SETBIT(flags, 3, fStrikeOut);
	SETBIT(flags, 4, fOutline);
	SETBIT(flags, 5, fShadow);
	SETBIT(flags, 6, fCondense);
	SETBIT(flags, 7, fExtend);

	record << dyHeight << flags;
	record << icv << bls << sss << uls << bFamily << bCharSet;
	record.reserveNunBytes(1, static_cast<unsigned char>(0x5E));
	record << fontName;
}


void Font::readFields(CFRecord& record)
{
	unsigned __int16 flags;
	record >> dyHeight >> flags;
	fItalic = GETBIT(flags, 1);
	fStrikeOut = GETBIT(flags, 3);
	fOutline = GETBIT(flags, 4);
	fShadow = GETBIT(flags, 5);
	fCondense = GETBIT(flags, 6);
	fExtend = GETBIT(flags, 7);

	record >> icv >> bls >> sss >> uls >> bFamily >> bCharSet;
	record.skipNunBytes(1);
	record >> fontName;
}

} // namespace XLS

