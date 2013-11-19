#ifndef _FONT_H
#define _FONT_H

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

	Encoder                pEncoder;
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
short         Type1FontDefGetWidth      (FontDef pFontDef, unsigned short nUnicode);
short         TTFontDefGetCharWidth     (FontDef pFontDef, unsigned short nUnicode);
short         CIDFontDefGetCIDWidth     (FontDef pFontDef, unsigned short nCid);
//---------------------------------------------------------------------------------------------------

BOOL          FontValidate        (FontDict pFont)
{
    if ( !pFont || !pFont->pAttr || pFont->pHeader.nObjClass != (OSUBCLASS_FONT | OCLASS_DICT) )
        return FALSE;

    return TRUE;
}
TextWidth     FontTextWidth       (FontDict pFont, const BYTE *sText, unsigned int nLen, unsigned int *pCIDs, unsigned int nLenCID)
{
	TextWidth oTextWidth = {0, 0, 0, 0};

    if ( !FontValidate( pFont ) )
        return oTextWidth;

    if ( nLen > LIMIT_MAX_STRING_LEN )  
	{
		RaiseError( pFont->oError, AVS_OFFICEPDFWRITER_ERROR_STRING_OUT_OF_RANGE, 0 );
        return oTextWidth;
    }

	FontAttr pAttr = (FontAttr)pFont->pAttr;

	if ( !pAttr->pTextWidthFn ) 
	{
		SetError( pFont->oError, AVS_OFFICEPDFWRITER_ERROR_INVALID_OBJECT, 0);
        return oTextWidth;
    }

	oTextWidth = pAttr->pTextWidthFn( pFont, sText, nLen, pCIDs, nLenCID);

    return oTextWidth;
}
unsigned int  FontMeasureText     (FontDict pFont, const BYTE *sText, unsigned int nLen, unsigned int *pCIDs, unsigned int nLenCID, float fWidth, float fFontSize, float fCharSpace, float fWordSpace, BOOL bWordWrap, float *pfRealWidth)
{
    if ( !FontValidate( pFont ) )
        return 0;

    if ( nLen > LIMIT_MAX_STRING_LEN ) 
	{
		RaiseError( pFont->oError, AVS_OFFICEPDFWRITER_ERROR_STRING_OUT_OF_RANGE, 0 );
        return 0;
    }

	FontAttr pAttr = (FontAttr)pFont->pAttr;

	if ( !pAttr->pMeasureTextFn ) 
	{
		SetError( pFont->oError, AVS_OFFICEPDFWRITER_ERROR_INVALID_OBJECT, 0);
        return 0;
    }

	return pAttr->pMeasureTextFn( pFont, sText, nLen, pCIDs, nLenCID, fWidth, fFontSize, fCharSpace, fWordSpace, bWordWrap, pfRealWidth);
}


const char*   FontGetFontName     (FontDict pFont)
{
    if ( !FontValidate( pFont ) )
        return NULL;

	FontAttr pAttr = (FontAttr)pFont->pAttr;
	
	return pAttr->pFontDef->sBaseFont;
}
const char*   FontGetEncodingName (FontDict pFont)
{
    if ( !FontValidate( pFont ) )
        return NULL;

	FontAttr pAttr = (FontAttr)pFont->pAttr;

	return pAttr->pEncoder->sName;
}
const char*   FontGetToUnicodeName(FontDict pFont)
{
	if ( !FontValidate( pFont ) )
		return NULL;

	FontAttr pAttr = (FontAttr)pFont->pAttr;

	return pAttr->pToUnicode->sName;
}
int           FontGetUnicodeWidth (FontDict pFont, unsigned short nCode)
{
    if ( !FontValidate( pFont ) )
        return 0;

	FontAttr pAttr = (FontAttr)pFont->pAttr;
	FontDef pFontDef = pAttr->pFontDef;

	if ( FontDefType1 == pFontDef->eType ) 
        return Type1FontDefGetWidth( pFontDef, nCode );
	else if ( FontDefTrueType == pFontDef->eType ) 
        return TTFontDefGetCharWidth( pFontDef, nCode);
	else if ( FontDefCID == pFontDef->eType ) 
	{
        CMapEncoderAttr pEncoderAttr = (CMapEncoderAttr)pAttr->pEncoder->pAttr;

        for ( int nIndexL = 0; nIndexL <= 255; nIndexL++) 
		{
            for ( int nIndexH = 0; nIndexH < 255; nIndexH++) 
			{
				if ( nCode == pEncoderAttr->anUnicodeMap[nIndexL][nIndexH] ) 
				{
					unsigned short nCID = pEncoderAttr->anCIDMap[nIndexL][nIndexH];

                    return CIDFontDefGetCIDWidth( pFontDef, nCID );
                }
            }
        }
    }

    return 0;
}
Box           FontGetBBox         (FontDict pFont)
{
    Box oBox = {0, 0, 0, 0};

    if ( FontValidate( pFont ) )
        return ((FontAttr)pFont->pAttr)->pFontDef->oFontBox;

    return oBox;
}
int           FontGetAscent       (FontDict pFont)
{
    if ( FontValidate( pFont ) )
        return ((FontAttr)pFont->pAttr)->pFontDef->nAscent;

    return 0;
}

int           FontGetDescent      (FontDict pFont)
{
    if ( FontValidate( pFont ) )
        return ((FontAttr)pFont->pAttr)->pFontDef->nDescent;

    return 0;
}
unsigned int  FontGetXHeight      (FontDict pFont)
{
    if ( FontValidate( pFont ) )
        return ((FontAttr)pFont->pAttr)->pFontDef->nXHeight;

    return 0;
}
unsigned int  FontGetCapHeight    (FontDict pFont)
{
    if ( FontValidate( pFont ) )
        return ((FontAttr)pFont->pAttr)->pFontDef->nCapHeight;

    return 0;
}



//---------------------------------------------------------------------------------------------------

#include "FontTrueType.h"
#include "FontBase14.h"
#include "FontType1.h"
#include "FontCID.h"
#include "FontBase.h"

#endif /* _FONT_H */

