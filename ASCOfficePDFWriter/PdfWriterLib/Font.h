#ifndef _FONT_H
#define _FONT_H

#pragma once 

#include "FontTypes.h"
#include "Utils.h"

//----- Writing Mode --------------------------------------------------------------------------------

typedef enum EFontType 
{
    FontType1 = 0,
    FontTrueType,
    FontType3,
    FontType0CID,
    FontType0TT,
    FontCIDType0,
    FontCIDType2,
    FontMMType1
} FontType;

typedef Dict FontDict;

typedef TextWidth   (*FontTextWidthsFunc)  (FontDict pFont, const BYTE *sText, unsigned int nLen, unsigned int *pCIDs, unsigned int nLenCID);
typedef unsigned int(*FontMeasureTextFunc) (FontDict pFont, const BYTE *sText, unsigned int nLen, unsigned int *pCIDs, unsigned int nLenCID, float fWidth, float fFontSize, float fCharSpace, float fWordSpace, BOOL bWordWrap, float *fRealWidth);

typedef struct TFontAttrRec  *FontAttr;

typedef struct TFontAttrRec 
{
    FontType               eType;
    WritingMode            eWritingMode;
    FontTextWidthsFunc     pTextWidthFn;
    FontMeasureTextFunc    pMeasureTextFn;
    FontDef                pFontDef;

	EncoderRecPtr          pEncoder;
	BOOL                   bFreeEncoder; // TRUE только для OTF, потому что эти кодировки строятся перед записью и не регистрируются в файле
	PToUnicode             pToUnicode;

	// Если тип кодировки EncoderTypeSingleByte, тогда ширина каждого 
	// символа записывается в массив anWidths. Если же тип кодировки
	// EncoderTypeDoubleByte, тогда ширина вычисляется каждый раз.
    short                 *anWidths;
    BYTE                  *pUsed;

	struct
	{
		bool               bUse;       //
		int                nFirstChar; // Если эти параметры заданы, значит таблица ширин задана
		int                nLastChar;  // внешним образом
		short             *anWidths;   //
	} 
        oExternWidths;
    Xref                   pXref;
    FontDict               pDescendantFont;
    Dict                   pMapStream;
    Dict                   pCMapStream;
} FontAttrRec;

//---------------------------------------------------------------------------------------------------

#include "FontTrueType.h"
#include "FontBase14.h"
#include "FontType1.h"
#include "FontCID.h"
#include "FontBase.h"

//---------------------------------------------------------------------------------------------------

BOOL          FontValidate        (FontDict pFont);
TextWidth     FontTextWidth       (FontDict pFont, const BYTE *sText, unsigned int nLen, unsigned int *pCIDs, unsigned int nLenCID);
unsigned int  FontMeasureText     (FontDict pFont, const BYTE *sText, unsigned int nLen, unsigned int *pCIDs, unsigned int nLenCID, float fWidth, float fFontSize, float fCharSpace, float fWordSpace, BOOL bWordWrap, float *pfRealWidth);
const char*   FontGetFontName     (FontDict pFont);
const char*   FontGetEncodingName (FontDict pFont);
const char*   FontGetToUnicodeName(FontDict pFont);
int           FontGetUnicodeWidth (FontDict pFont, unsigned short nCode);
Box           FontGetBBox         (FontDict pFont);
int           FontGetAscent       (FontDict pFont);
int           FontGetDescent      (FontDict pFont);
unsigned int  FontGetXHeight      (FontDict pFont);
unsigned int  FontGetCapHeight    (FontDict pFont);


#endif /* _FONT_H */

