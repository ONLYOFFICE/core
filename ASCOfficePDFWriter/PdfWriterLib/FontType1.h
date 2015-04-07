#ifndef _FONT_TYPE_1_H
#define _FONT_TYPE_1_H


#include "Utils.h"
#include "Font.h"

//----- Type1FontDef основные функции ---------------------------------------------------------------
FontDef       Type1FontDefNew            (MMgr oMMgr);
FontDef       Type1FontDefLoad           (MMgr oMMgr, StreamRecPtr pAFM, StreamRecPtr pFontData, short nType);
FontDef       Type1FontDefDuplicate      (MMgr oMMgr, FontDef pSrc);
unsigned long Type1FontDefSetWidths      (FontDef pFontDef, const CharData* poWidths);
short         Type1FontDefGetWidth       (FontDef pFontDef, unsigned short nUnicode);
short         Type1FontDefGetWidthByName (FontDef pFontDef, const char* sGlyphName);
FontDict	  Type1FontNew  (MMgr oMMgr, FontDef pFontDef, EncoderRecPtr pEncoder, Xref pXref, PToUnicode pToUnicode);
#endif /* _FONT_TYPE_1_H */
