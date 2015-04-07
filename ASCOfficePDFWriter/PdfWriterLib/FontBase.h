#ifndef _FONT_BASE_H
#define _FONT_BASE_H

#include "Utils.h"
#include "Font.h"

//----- BaseFontDef основные функции ----------------------------------------------------------------
FontDef       BaseFontDefNew           (MMgr oMMgr, const char *sFontName);
unsigned long BaseFontDefSetProps      (FontDef pFontDef, short shAscent, short shDescent, unsigned int unFlags, Box oFontBox, short shItalicAngle, unsigned short ushStemV, short shAvgWidth, short shMaxWidth, short shMissingWidth, unsigned short ushStemH, unsigned short ushXHeight, unsigned short ushCapHeight);
FontDict BaseFontNew  (MMgr oMMgr, FontDef pFontDef, EncoderRecPtr pEncoder, Xref pXref, PToUnicode pToUnicode);
#endif /* _FONT_BASE_H */