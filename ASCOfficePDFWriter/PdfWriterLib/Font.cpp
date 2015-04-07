#include "Font.h"

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


