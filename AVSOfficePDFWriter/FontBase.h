#ifndef _FONT_BASE_H
#define _FONT_BASE_H

#include "Utils.h"
#include "Font.h"

//----- BaseFontDef основные функции ----------------------------------------------------------------
FontDef       BaseFontDefNew           (MMgr oMMgr, const char *sFontName)
{
    if ( !oMMgr )
        return NULL;

    FontDef pFontDef = (FontDef)GetMem( oMMgr, sizeof(FontDefRec) );
    if ( !pFontDef )
        return NULL;
	UtilsMemSet( (BYTE*)pFontDef, 0, sizeof(FontDefRec) );

	pFontDef->nSigBytes     = FONTDEF_SIG_BYTES;
    pFontDef->sBaseFont[0]  = 0;
	pFontDef->oMMgr         = oMMgr;
	pFontDef->oError        = oMMgr->oError;
    pFontDef->eType         = FontBase;
	pFontDef->pCleanFn      = NULL;
	pFontDef->pFreeFn       = NULL;
	pFontDef->pDescriptor   = NULL;
    pFontDef->bValid        = FALSE;
	pFontDef->nMissingWidth = 0;
	pFontDef->pAttr         = NULL;
	pFontDef->nFlags        = FONT_STD_CHARSET;

	UtilsStrCpy( pFontDef->sBaseFont, sFontName, pFontDef->sBaseFont + LIMIT_MAX_NAME_LEN );

    return pFontDef;
}
unsigned long BaseFontDefSetProps      (FontDef pFontDef, short shAscent, short shDescent, unsigned int unFlags, Box oFontBox, short shItalicAngle, unsigned short ushStemV, short shAvgWidth, short shMaxWidth, short shMissingWidth, unsigned short ushStemH, unsigned short ushXHeight, unsigned short ushCapHeight)
{
	pFontDef->nAscent       = shAscent;
	pFontDef->nAvgWidth     = shAvgWidth;
	pFontDef->nCapHeight    = ushCapHeight;
	pFontDef->nDescent      = shDescent;
	pFontDef->nFlags        = unFlags;
	pFontDef->nItalicAngle  = shItalicAngle;
	pFontDef->nMaxWidth     = shMaxWidth;
	pFontDef->nMissingWidth = shMissingWidth;
	pFontDef->nStemH        = ushStemH;
	pFontDef->nStemV        = ushStemV;
	pFontDef->nXHeight      = ushXHeight;
	pFontDef->oFontBox      = oFontBox;

	return OK;
}
//----- BaseFont вспомогательные функции ------------------------------------------------------------
static unsigned long BaseFontCreateDescriptor(MMgr oMMgr, FontDict pFont, Xref pXref)
{
    FontAttr pFontAttr = (FontAttr)pFont->pAttr;
    FontDef pDef = pFontAttr->pFontDef;

    if ( !pFontAttr->pFontDef->pDescriptor ) 
	{
        Dict pDescriptor = DictNew( oMMgr );
        unsigned long nRet = OK;
        Array oArray;

        if ( !pDescriptor )
            return ErrorGetCode( pFont->oError );

        nRet += XrefAdd( pXref, pDescriptor );
        nRet += DictAddName( pDescriptor, "Type", "FontDescriptor" );

        nRet += DictAddNumber( pDescriptor, "Ascent",      pDef->nAscent  );
        nRet += DictAddNumber( pDescriptor, "Descent",     pDef->nDescent );
        nRet += DictAddNumber( pDescriptor, "Flags",       pDef->nFlags   );

        oArray = BoxArrayNew( oMMgr, pDef->oFontBox );
        nRet += DictAdd(       pDescriptor, "FontBBox",    oArray         );
		nRet += DictAddName(   pDescriptor, "FontName",    pFontAttr->pFontDef->sBaseFont );
		nRet += DictAddNumber( pDescriptor, "ItalicAngle", pDef->nItalicAngle );
		nRet += DictAddNumber( pDescriptor, "StemV",       pDef->nStemV );
		nRet += DictAddNumber( pDescriptor, "StemH",       pDef->nStemH );
		nRet += DictAddNumber( pDescriptor, "XHeight",     pDef->nXHeight );
		nRet += DictAddNumber( pDescriptor, "AvgWidth",    pDef->nAvgWidth );
		nRet += DictAddNumber( pDescriptor, "MaxWidth",    pDef->nMaxWidth );
		nRet += DictAddNumber( pDescriptor, "MissingWidth",pDef->nMissingWidth );
		nRet += DictAddNumber( pDescriptor, "CapHeight",   pDef->nCapHeight );

        if ( OK != nRet )
			return ErrorGetCode( pFont->oError );

		pFontAttr->pFontDef->pDescriptor = pDescriptor;
    }

    return DictAdd( pFont, "FontDescriptor", pFontAttr->pFontDef->pDescriptor );
}
static unsigned long BaseFontCreateToUnicode (MMgr oMMgr, FontDict pFont, Xref pXref)
{
	FontAttr pFontAttr = (FontAttr)pFont->pAttr;
	
	if ( NULL != pFontAttr->pToUnicode )
	{
		Dict pToUnicodeDict = DictStreamNew( oMMgr, pXref );
        if ( !pToUnicodeDict )
            return ErrorGetCode( pFont->oError );

		pFontAttr->pToUnicode->pWriteFn( pFontAttr->pToUnicode, pToUnicodeDict->pStream );

#ifdef _DEBUG
		pToUnicodeDict->nFilter = STREAM_FILTER_FLATE_DECODE;
#endif /* _DEBUG */

		pFontAttr->pCMapStream = pToUnicodeDict;

		return DictAdd( pFont, "ToUnicode", pFontAttr->pCMapStream );
	}

	return OK;
}
static unsigned long BaseFontBeforeWrite     (Dict pObj)
{
	unsigned long nRet = OK;

	FontDict pFont = (FontDict)pObj;
	FontAttr pFontAttr = (FontAttr)pFont->pAttr;

	if ( OK != ( nRet = BaseFontCreateDescriptor( pFont->oMMgr, pFont, pFontAttr->pXref ) ) )
		return nRet;

	if ( OK != ( nRet = BaseFontCreateToUnicode( pFont->oMMgr, pFont, pFontAttr->pXref ) ) )
		return nRet;

	return OK;
}
static unsigned long BaseFontOnWrite         (Dict pObj, Stream pStream)
{
	unsigned long nRet = OK;

	FontAttr pAttr = (FontAttr)pObj->pAttr;

	if ( true == pAttr->oExternWidths.bUse )
	{
		char sBuffer[TEMP_BUFFER_SIZE];
		char *pEndPointer = sBuffer + TEMP_BUFFER_SIZE - 1;

		char *pBufPointer;

		pBufPointer = (char*)UtilsStrCpy( sBuffer, "/FirstChar ", pEndPointer);
		pBufPointer = UtilsIToA( pBufPointer, pAttr->oExternWidths.nFirstChar, pEndPointer);
		UtilsStrCpy( pBufPointer, "\012", pEndPointer);
		if ( OK != ( nRet = StreamWriteStr( pStream, sBuffer ) ) )
			return nRet;

		pBufPointer = (char*)UtilsStrCpy( sBuffer, "/LastChar ", pEndPointer);
		pBufPointer = UtilsIToA( pBufPointer, pAttr->oExternWidths.nLastChar, pEndPointer);
		UtilsStrCpy( pBufPointer, "\012", pEndPointer);
		if ( OK != ( nRet = StreamWriteStr( pStream, sBuffer ) ) )
			return nRet;

		if ( OK != ( nRet = StreamWriteEscapeName( pStream, "Widths" ) ) )
			return nRet;

		if ( OK != ( nRet = StreamWriteStr( pStream, " [\012" ) ) )
			return nRet;

		pBufPointer = sBuffer;
		for ( unsigned int nIndex = 0; nIndex <= pAttr->oExternWidths.nLastChar - pAttr->oExternWidths.nFirstChar; nIndex++) 
		{
			pBufPointer = UtilsIToA( pBufPointer, pAttr->oExternWidths.anWidths[nIndex], pEndPointer);
			*pBufPointer++ = ' ';

			if ( 0 == (nIndex + 1) % 16 ) 
			{
				UtilsStrCpy( pBufPointer, "\012", pEndPointer);
				if ( OK != ( nRet = StreamWriteStr( pStream, sBuffer ) ) )
					return nRet;
				pBufPointer = sBuffer;
			}
		}

		UtilsStrCpy( pBufPointer, "]\012", pEndPointer);

		if ( OK != ( nRet = StreamWriteStr( pStream, sBuffer ) ) )
			return nRet;
	}

	if ( pAttr->pEncoder )
		pAttr->pEncoder->pWriteFn( pAttr->pEncoder, pStream );

	return OK;
}
static TextWidth     BaseFontTextWidth       (FontDict pFont, const BYTE *pText, unsigned int nLen, unsigned int *pCIDs, unsigned int nLenCID)
{
    FontAttr pAttr = (FontAttr)pFont->pAttr;
    TextWidth oRet = {0, 0, 0, 0};
    BYTE nChar = 0;

	if ( pAttr->oExternWidths.bUse )
	{
		if ( pAttr->oExternWidths.anWidths ) 
		{
			for ( unsigned int nIndex = 0; nIndex < nLen; nIndex++ ) 
			{
				nChar = pText[nIndex];
				oRet.nNumchars++;
				BYTE nCode = nChar - pAttr->oExternWidths.nFirstChar;
				oRet.nWidth += pAttr->oExternWidths.anWidths[nCode];

				if ( IS_WHITE_SPACE(nChar) ) 
				{
					oRet.nNumspace++;
					oRet.nNumwords++;
				}
			}
		} 
		else
			SetError( pFont->oError, AVS_OFFICEPDFWRITER_ERROR_FONT_INVALID_WIDTHS_TABLE, 0);

		if ( IS_WHITE_SPACE(nChar) )
			; // ничего не делаем
		else
			oRet.nNumwords++;
	}

	return oRet;
}
static unsigned int  BaseFontMeasureText     (FontDict pFont, const BYTE *pText, unsigned int nLen, unsigned int *pCIDs, unsigned int nLenCID, float fWidth, float fFontSize, float fCharSpace, float fWordSpace, BOOL bWordWrap, float *pfRealWidth)
{
    float fTempWidth = 0;
    unsigned int nTempLen = 0;
    FontAttr pAttr = (FontAttr)pFont->pAttr;

    for (unsigned int nIndex = 0; nIndex < nLen; nIndex++) 
	{
        BYTE nChar = pText[nIndex];

        if ( IS_WHITE_SPACE(nChar) ) 
		{
            nTempLen = nIndex + 1;

            if ( pfRealWidth )
                *pfRealWidth = fTempWidth;

            fTempWidth += fWordSpace;
        } 
		else if ( !bWordWrap ) 
		{
            nTempLen = nIndex;

            if ( pfRealWidth )
                *pfRealWidth = fTempWidth;
        }

		if ( pAttr->oExternWidths.bUse )
			fTempWidth += pAttr->oExternWidths.anWidths[nChar] * fFontSize / 1000;
		else
			fTempWidth += 0;

        /* 2006.08.04 break when it encountered  line feed */
        if ( fTempWidth > fWidth || 0x0A == nChar )
            return nTempLen;

        if ( nIndex > 0 )
            fTempWidth += fCharSpace;
    }

    /* all of text can be put in the specified width */
    if ( pfRealWidth )
        *pfRealWidth = fTempWidth;

    return nLen;
}
//----- BaseFont основные функции -------------------------------------------------------------------
FontDict BaseFontNew  (MMgr oMMgr, FontDef pFontDef, Encoder pEncoder, Xref pXref, PToUnicode pToUnicode)
{
    unsigned long nRet = OK;

    Dict pFont = DictNew( oMMgr );
    if ( !pFont )
        return NULL;

	pFont->pHeader.nObjClass |= OSUBCLASS_FONT;

    FontAttr pAttr = (FontAttr)GetMem( oMMgr, sizeof(FontAttrRec) );
    if ( !pAttr ) 
	{
        DictFree( pFont );
        return NULL;
    }

	pFont->pBeforeWriteFn = BaseFontBeforeWrite;
	pFont->pWriteFn       = BaseFontOnWrite;
	pFont->pFreeFn        = NULL;

    UtilsMemSet( pAttr, 0, sizeof(FontAttrRec) );

    pFont->pAttr          = pAttr;
	pAttr->eType          = FontType1;
    pAttr->eWritingMode   = WModeHorizontal;
	pAttr->pTextWidthFn   = BaseFontTextWidth;
    pAttr->pMeasureTextFn = BaseFontMeasureText;
	pAttr->pFontDef       = pFontDef;
    pAttr->pEncoder       = pEncoder;
	pAttr->pToUnicode     = pToUnicode;
    pAttr->pXref          = pXref;

	pAttr->oExternWidths.bUse       = true; // В данном шрифте ширины всегда должны задаваться внешним образом
	pAttr->oExternWidths.nLastChar  = 0;
	pAttr->oExternWidths.nFirstChar = 0;
	pAttr->oExternWidths.anWidths   = NULL;

    nRet += DictAddName( pFont, "Type", "Font");
	nRet += DictAddName( pFont, "BaseFont", pFontDef->sBaseFont );
    nRet += DictAddName( pFont, "Subtype", "Type1");

    if ( OK != nRet ) 
	{
        DictFree( pFont );
        return NULL;
    }

    if ( OK != XrefAdd( pXref, pFont ) )
        return NULL;

    return pFont;
}

#endif /* _FONT_BASE_H */