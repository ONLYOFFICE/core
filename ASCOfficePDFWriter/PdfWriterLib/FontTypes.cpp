#include "FontType1.h"

#include FT_TYPE1_TABLES_H
#include FT_ADVANCES_H
//----- Type1FontDef вспомогательные функции --------------------------------------------------------
static void          Type1FreeWidth   (FontDef pFontDef)
{
    Type1FontDefAttr pAttr = (Type1FontDefAttr)pFontDef->pAttr;

	FreeMem( pFontDef->oMMgr, pAttr->aoWidths );
    pAttr->aoWidths = NULL;

    pFontDef->bValid = false;
}
static void          Type1FreeFunc    (FontDef pFontDef)
{
    Type1FontDefAttr pAttr = (Type1FontDefAttr)pFontDef->pAttr;
	if ( pAttr->sCharSet )
        FreeMem( pFontDef->oMMgr, pAttr->sCharSet );

    if ( pAttr->pFontData )
        StreamFree( pAttr->pFontData );

	FreeMem( pFontDef->oMMgr, pAttr->aoWidths );
    FreeMem( pFontDef->oMMgr, pAttr );
}
static const char*   GetKeyword  (const char *sSrc, char *sKeyword, unsigned int nLen)
{
    unsigned int nSrcLen = UtilsStrLen( sSrc, -1 );

    if ( !sKeyword || 0 == nSrcLen || 0 == nLen )
        return NULL;

    *sKeyword = 0;

    while ( nLen > 0 ) 
	{
        if ( IS_WHITE_SPACE(*sSrc) ) 
		{
            *sKeyword = 0;

            while ( IS_WHITE_SPACE(*sSrc) )
                sSrc++;
            return sSrc;
        }

        *sKeyword++ = *sSrc++;
        nLen--;
    }

    *sKeyword = 0;
    return NULL;
}

static unsigned long LoadType1   (FontDef pFontDef, CStringW wsFilePath/*, StreamRecPtr pStream*/)
{
	unsigned long nRet = OK;
	

	Type1FontDefAttr pAttr = (Type1FontDefAttr)pFontDef->pAttr;

	// Загружаем Free Type
	FT_Library pLibrary = NULL;
	if ( FT_Init_FreeType( &pLibrary ) || NULL == pLibrary )
		return 1;

	// Загружаем шрифт
	char *sPath		= NULL;
#if defined(_WIN32) || defined (_WIN64)
	USES_CONVERSION;
	sPath = W2A( wsFilePath );
#else
    std::string ansiStr(wsFilePath.begin(), wsFilePath.end());
    sPath = (char*)ansiStr.c_str();
#endif
	FT_Face pFace = NULL;
	if ( FT_New_Face( pLibrary, sPath, 0, &pFace ) || NULL == pFace ) 
	{
		FT_Done_FreeType( pLibrary );
		return 1;
	}

	T1_FontInfo oInfo;
	if ( FT_Get_PS_Font_Info( pFace, &oInfo ) )
	{
		FT_Done_Face( pFace );
		FT_Done_FreeType( pLibrary );		
		return 1;
	}

	// Название шрифта
	UtilsStrCpy( pFontDef->sBaseFont, oInfo.full_name, pFontDef->sBaseFont + LIMIT_MAX_NAME_LEN );

	// Weight
	if ( 0 == UtilsStrCmp( oInfo.weight, "Bold" ) )
		pFontDef->nFlags |= FONT_FOURCE_BOLD;

	//	FixedPitch
	if ( oInfo.is_fixed_pitch )
		pFontDef->nFlags |= FONT_FIXED_WIDTH;

	//	Italic 
	if ( 0 != oInfo.italic_angle )
		pFontDef->nFlags |= FONT_ITALIC;

	//	if ( 0 == UtilsStrCmp( sKeyword, "CharacterSet" ) ) 
	//	{
	//		unsigned int nLen = UtilsStrLen( sTemp, LIMIT_MAX_STRING_LEN );
	//		if ( nLen > 0 ) 
	//		{
	//			pAttr->sCharSet = (char*)GetMem( pFontDef->oMMgr, nLen + 1 );
	//			if ( !pAttr->sCharSet )
	//				return ErrorGetCode( pFontDef->oError );
	//			UtilsStrCpy( pAttr->sCharSet, sTemp, pAttr->sCharSet + nLen);
	//		}
	//	} 
	

	// Bbox
	pFontDef->oFontBox.fLeft   = pFace->bbox.xMin;
	pFontDef->oFontBox.fRight  = pFace->bbox.xMax;
	pFontDef->oFontBox.fBottom = pFace->bbox.yMin;
	pFontDef->oFontBox.fTop    = pFace->bbox.yMax;

	//	else if ( 0 == UtilsStrCmp( sKeyword, "EncodingScheme" ) ) 
	//	{
	//		UtilsStrCpy( pAttr->sEncodingScheme, sTemp, pAttr->sEncodingScheme + LIMIT_MAX_NAME_LEN);
	//	} 

	// CapHeight
	pFontDef->nCapHeight = pFace->max_advance_height;

	//	Ascender
	pFontDef->nAscent    = pFace->ascender;
	//	Descender
	pFontDef->nDescent   = pFace->descender;

	// Кодировка и ширины
	CharData arrChars[256];

	int nCount = 0;
	FT_UInt unGID = 0;
	FT_UInt unCharCode = FT_Get_First_Char( pFace, &unGID );

	for ( nCount = 0; nCount < 256 && 0 != unGID; nCount++ )
	{
		arrChars[nCount].nCharCode = unCharCode;
		if ( FT_HAS_GLYPH_NAMES( pFace ) )
		{
			char sBuffer[LIMIT_MAX_NAME_LEN + 1];

			FT_Get_Glyph_Name( pFace, unGID, sBuffer, LIMIT_MAX_NAME_LEN + 1 );
			arrChars[nCount].nUnicode = GlyphNameToUnicode( sBuffer );
		}
		else
		{
			arrChars[nCount].nUnicode = unCharCode;
		}

		FT_Load_Glyph( pFace, unGID, FT_LOAD_DEFAULT );
		FT_Pos nAdvance = pFace->glyph->metrics.horiAdvance;

		//FT_Load_Glyph( FT_Get_Advance( pFace, unGID, FT_LOAD_DEFAULT | FT_ADVANCE_FLAG_FAST_ONLY, &nAdvance )
		//FT_Fixed nAdvance;
		//if ( FT_Get_Advance( pFace, unGID, FT_LOAD_DEFAULT | FT_ADVANCE_FLAG_FAST_ONLY, &nAdvance ) )
		//	FT_Get_Advance( pFace, unGID, FT_LOAD_DEFAULT, &nAdvance );
		arrChars[nCount].nWidth = nAdvance;

		unCharCode = FT_Get_Next_Char( pFace, unCharCode, &unGID );
	}

	pAttr->nWidthsCount = nCount;

	CharData *pCharData = (CharData*)GetMem( pFontDef->oMMgr, sizeof(CharData) * pAttr->nWidthsCount );
    if ( NULL == pCharData )
	{
		FT_Done_Face( pFace );
		FT_Done_FreeType( pLibrary );	
		return ErrorGetCode( pFontDef->oError );
	}

	UtilsMemSet( pCharData, 0, sizeof(CharData) * pAttr->nWidthsCount );
	pAttr->aoWidths = pCharData;

	for ( unsigned int unIndex = 0; unIndex < pAttr->nWidthsCount; unIndex++, pCharData++ )
	{
		pCharData[unIndex].nCharCode = arrChars[unIndex].nCharCode;
		pCharData[unIndex].nUnicode  = arrChars[unIndex].nUnicode;
		pCharData[unIndex].nWidth    = arrChars[unIndex].nWidth;
	}

	FT_Done_Face( pFace );
	FT_Done_FreeType( pLibrary );


	//// Записываем внедренный шрифт

 //   char sBuffer[STREAM_BUF_SIZ];
 //   char* pBufPointer = sBuffer;
 //   unsigned int nLen = 0;
 //   bool bEndFlag = false;

	//pAttr->pFontData = MemStreamNew( pFontDef->oMMgr, STREAM_BUF_SIZ );

 //   if ( !pAttr->pFontData )
	//	return ErrorGetCode( pFontDef->oError );

 //   nLen = 11;
 //   nRet = StreamRead( pStream, (BYTE*)pBufPointer, &nLen );
 //   if ( OK != nRet )
 //       return nRet;
 //   pBufPointer += 11;

 //   for (;;) 
	//{
 //       nLen = STREAM_BUF_SIZ - 11;
 //       nRet = StreamRead( pStream, (BYTE*)pBufPointer, &nLen );
 //       if ( AVS_OFFICEPDFWRITER_ERROR_STREAM_EOF == nRet ) 
	//	{
 //           bEndFlag = true;
 //       } 
	//	else if ( OK != nRet )
 //           return nRet;

 //       if ( nLen > 0 ) 
	//	{
	//		if ( 0 == pAttr->nLength1 ) 
	//		{
 //              const char *sTemp1 = UtilsStrStr( sBuffer, "eexec", nLen + 11 );
 //              // значение nLength1 обозначает размер ascii-data в font-file.
 //              if ( sTemp1 )
	//			   pAttr->nLength1 = pAttr->pFontData->nSize + ( sTemp1 - sBuffer ) + 6;
 //           }

	//		if ( pAttr->nLength1 > 0 && 0 == pAttr->nLength2 ) 
	//		{
 //               const char *sTemp2 = UtilsStrStr( sBuffer, "cleartomark", nLen + 11 );

 //               if ( sTemp2 )
	//				pAttr->nLength2 = pAttr->pFontData->nSize + - 520 - pAttr->nLength1 + ( sTemp2 - sBuffer );
 //               //  nLength2 обозначает размер бинарный данных. Для большинства фонтов эти данные
	//			//  умещаются в 520 байт, но нужно будет это поправить, т.к. это не полностью
	//			//  соответствует специкикации Type1-fonts.
 //           }
 //       }

 //       if ( bEndFlag ) 
	//	{
	//		if ( OK != ( nRet = StreamWrite( pAttr->pFontData, (BYTE*)sBuffer, nLen + 11 ) ) )
 //               return nRet;

 //           break;
 //       } 
	//	else 
	//	{
 //           if ( OK != ( nRet = StreamWrite( pAttr->pFontData, (BYTE*)sBuffer, nLen ) ) )
 //               return nRet;
 //           UtilsMemCpy( (BYTE*)sBuffer, (BYTE*)sBuffer + nLen, 11 );
 //           pBufPointer = sBuffer + 11;
 //       }
 //   }

	//if ( 0 == pAttr->nLength1 || 0 == pAttr->nLength2 )
	//	return SetError( pFontDef->oError, AVS_OFFICEPDFWRITER_ERROR_UNSUPPORTED_TYPE1_FONT, 0);

	//pAttr->nLength3 = pAttr->pFontData->nSize - pAttr->nLength1 - pAttr->nLength2;


	return nRet;
}
static unsigned long LoadAfm     (FontDef pFontDef, StreamRecPtr pStream)
{
    Type1FontDefAttr pAttr = (Type1FontDefAttr)pFontDef->pAttr;
    char sBuffer[TEMP_BUFFER_SIZE];
    unsigned long nRet = OK;
    char sKeyword[LIMIT_MAX_NAME_LEN + 1];

    unsigned int nLen = TEMP_BUFFER_SIZE;

    // проверяем заголовок Afm-файла
    if ( OK != ( nRet = StreamReadLn( pStream, sBuffer, &nLen ) ) )
         return nRet;

    GetKeyword( sBuffer, sKeyword, LIMIT_MAX_NAME_LEN + 1 );

    if ( 0 != UtilsStrCmp( sKeyword, "StartFontMetrics" ) )
        return AVS_OFFICEPDFWRITER_ERROR_INVALID_AFM_HEADER;

    /* Global Font Information */

    for (;;) 
	{
        const char *sTemp;

        nLen = TEMP_BUFFER_SIZE;
        if ( OK != ( nRet = StreamReadLn( pStream, sBuffer, &nLen ) ) )
            return nRet;

        sTemp = GetKeyword( sBuffer, sKeyword, LIMIT_MAX_NAME_LEN + 1);

        if ( 0 == UtilsStrCmp( sKeyword, "FontName" ) ) 
		{
            UtilsStrCpy( pFontDef->sBaseFont, sTemp, pFontDef->sBaseFont + LIMIT_MAX_NAME_LEN );
        } 
		else if ( 0 == UtilsStrCmp( sKeyword, "Weight" ) ) 
		{
			if ( 0 == UtilsStrCmp( sTemp, "Bold" ) )
				pFontDef->nFlags |= FONT_FOURCE_BOLD;
		} 
		else if ( 0 == UtilsStrCmp( sKeyword, "IsFixedPitch" ) ) 
		{
			if ( 0 == UtilsStrCmp( sTemp, "true" ) )
				pFontDef->nFlags |= FONT_FIXED_WIDTH;
		} 
		else if ( 0 == UtilsStrCmp( sKeyword, "ItalicAngle" ) ) 
		{
			pFontDef->nItalicAngle = UtilsAToI( sTemp );
			if ( 0 != pFontDef->nItalicAngle )
				pFontDef->nFlags |= FONT_ITALIC;
		} 
		else if ( 0 == UtilsStrCmp( sKeyword, "CharacterSet" ) ) 
		{
			unsigned int nLen = UtilsStrLen( sTemp, LIMIT_MAX_STRING_LEN );
			if ( nLen > 0 ) 
			{
				pAttr->sCharSet = (char*)GetMem( pFontDef->oMMgr, nLen + 1 );
				if ( !pAttr->sCharSet )
					return ErrorGetCode( pFontDef->oError );
				UtilsStrCpy( pAttr->sCharSet, sTemp, pAttr->sCharSet + nLen);
			}
		} 
		else if ( 0 == UtilsStrCmp( sKeyword, "FontBBox" ) ) 
		{
			char buf[INT_LEN + 1];

			sTemp = GetKeyword( sTemp, buf, INT_LEN + 1);
			pFontDef->oFontBox.fLeft = UtilsAToI( buf );

			sTemp = GetKeyword( sTemp, buf, INT_LEN + 1);
			pFontDef->oFontBox.fBottom = UtilsAToI( buf );

			sTemp = GetKeyword( sTemp, buf, INT_LEN + 1);
			pFontDef->oFontBox.fRight = UtilsAToI( buf );

			GetKeyword( sTemp, buf, INT_LEN + 1);
			pFontDef->oFontBox.fTop = UtilsAToI( buf );
		} 
		else if ( 0 == UtilsStrCmp( sKeyword, "EncodingScheme" ) ) 
		{
			UtilsStrCpy( pAttr->sEncodingScheme, sTemp, pAttr->sEncodingScheme + LIMIT_MAX_NAME_LEN);
		} 
		else if ( 0 == UtilsStrCmp( sKeyword, "CapHeight" ) ) 
		{
			pFontDef->nCapHeight = UtilsAToI( sTemp );
        } 
		else if ( 0 == UtilsStrCmp( sKeyword, "Ascender" ) ) 
		{
			pFontDef->nAscent = UtilsAToI( sTemp );
        } 
		else if ( 0 == UtilsStrCmp( sKeyword, "Descender" ) ) 
		{
			pFontDef->nDescent = UtilsAToI( sTemp );
        } 
		else if ( 0 == UtilsStrCmp( sKeyword, "StdHW" ) ) 
		{
			pFontDef->nStemH = UtilsAToI( sTemp );
        } 
		else if ( 0 == UtilsStrCmp( sKeyword, "StdHV" ) ) 
		{
			pFontDef->nStemV = UtilsAToI( sTemp );
        } 
		else if ( 0 == UtilsStrCmp( sKeyword, "StartCharMetrics" ) ) 
		{
			pAttr->nWidthsCount = UtilsAToI( sTemp );
            break;
        }
		else if ( 0 == UtilsStrCmp( sKeyword, "XHeight" ) )
		{
			pFontDef->nXHeight = UtilsAToI( sTemp );
		}
    }

	CharData *pCharData = (CharData*)GetMem( pFontDef->oMMgr, sizeof(CharData) * pAttr->nWidthsCount );
    if ( NULL == pCharData )
		return ErrorGetCode( pFontDef->oError );

    UtilsMemSet( pCharData, 0, sizeof(CharData) * pAttr->nWidthsCount );
	pAttr->aoWidths = pCharData;

    /* load CharMetrics */
	for ( unsigned int nIndex = 0; nIndex < pAttr->nWidthsCount; nIndex++, pCharData++) 
	{
        const char *sTemp;
        char sBuffer2[LIMIT_MAX_NAME_LEN + 1];

        nLen = TEMP_BUFFER_SIZE;
        if ( OK != ( nRet = StreamReadLn( pStream, sBuffer, &nLen ) ) )
            return nRet;

        /* C default character code. */
        sTemp = GetKeyword( sBuffer, sBuffer2, LIMIT_MAX_NAME_LEN + 1);
        if ( 0 == UtilsStrCmp( sBuffer2, "CX") ) 
		{
            /* not suppoted yet. */
			return SetError( pFontDef->oError, AVS_OFFICEPDFWRITER_ERROR_INVALID_CHAR_METRICS_DATA, 0);
        } 
		else if ( 0 == UtilsStrCmp( sBuffer2, "C") ) 
		{
            //sTemp += 2;

            sTemp = GetKeyword( sTemp, sBuffer2, LIMIT_MAX_NAME_LEN + 1);

			pCharData->nCharCode = UtilsAToI( sBuffer2 );

        } 
		else
			return SetError( pFontDef->oError, AVS_OFFICEPDFWRITER_ERROR_INVALID_CHAR_METRICS_DATA, 0);

        /* WX Character width */
        sTemp = UtilsStrStr( sTemp, "WX ", 0);
        if ( !sTemp )
			return SetError( pFontDef->oError, AVS_OFFICEPDFWRITER_ERROR_INVALID_WX_DATA, 0);

        sTemp += 3;

        sTemp = GetKeyword( sTemp, sBuffer2, LIMIT_MAX_NAME_LEN + 1);
        if ( 0 == sBuffer2[0] )
			return SetError( pFontDef->oError, AVS_OFFICEPDFWRITER_ERROR_INVALID_WX_DATA, 0);

		pCharData->nWidth = UtilsAToI( sBuffer2 );

        /* N PostScript language character name */
        sTemp = UtilsStrStr( sTemp, "N ", 0);
        if ( !sTemp )
		{
			pCharData->nUnicode = pCharData->nCharCode;
			continue;
            //return SetError( pFontDef->oError, AVS_OFFICEPDFWRITER_ERROR_INVALID_N_DATA, 0);
		}

        sTemp += 2;

        GetKeyword( sTemp, sBuffer2, LIMIT_MAX_NAME_LEN + 1);

		pCharData->nUnicode = GlyphNameToUnicode( sBuffer2 );

    }

    return OK;
}


static unsigned long LoadFontData(FontDef pFontDef, StreamRecPtr pStream, short nType)
{
    Type1FontDefAttr pAttr = (Type1FontDefAttr)pFontDef->pAttr;

    pAttr->nType = (std::max)( (short)0, (std::min)( (short)2, nType ) );

	if ( 0 == nType )
	{
		char sBuffer[STREAM_BUF_SIZ];
		char* pBufPointer = sBuffer;
		unsigned int nLen = 0;
		unsigned long nRet = OK;
        bool bEndFlag = false;

		pAttr->pFontData = MemStreamNew( pFontDef->oMMgr, STREAM_BUF_SIZ );

		if ( !pAttr->pFontData )
			return ErrorGetCode( pFontDef->oError );

		nLen = 11;
		nRet = StreamRead( pStream, (BYTE*)pBufPointer, &nLen );
		if ( OK != nRet )
			return nRet;
		pBufPointer += 11;

		for (;;) 
		{
			nLen = STREAM_BUF_SIZ - 11;
			nRet = StreamRead( pStream, (BYTE*)pBufPointer, &nLen );
			if ( AVS_OFFICEPDFWRITER_ERROR_STREAM_EOF == nRet ) 
			{
                bEndFlag = true;
			} 
			else if ( OK != nRet )
				return nRet;

			if ( nLen > 0 ) 
			{
				if ( 0 == pAttr->nLength1 ) 
				{
					const char *sTemp1 = UtilsStrStr( sBuffer, "eexec", nLen + 11 );
					// значение nLength1 обозначает размер ascii-data в font-file.
					if ( sTemp1 )
						pAttr->nLength1 = pAttr->pFontData->nSize + ( sTemp1 - sBuffer ) + 6;
				}

				if ( pAttr->nLength1 > 0 && 0 == pAttr->nLength2 ) 
				{
					const char *sTemp2 = UtilsStrStr( sBuffer, "cleartomark", nLen + 11 );

					if ( sTemp2 )
						pAttr->nLength2 = pAttr->pFontData->nSize + - 520 - pAttr->nLength1 + ( sTemp2 - sBuffer );
					//  nLength2 обозначает размер бинарный данных. Для большинства фонтов эти данные
					//  умещаются в 520 байт, но нужно будет это поправить, т.к. это не полностью
					//  соответствует специкикации Type1-fonts.
				}
			}

			if ( bEndFlag ) 
			{
				if ( OK != ( nRet = StreamWrite( pAttr->pFontData, (BYTE*)sBuffer, nLen + 11 ) ) )
					return nRet;

				break;
			} 
			else 
			{
				if ( OK != ( nRet = StreamWrite( pAttr->pFontData, (BYTE*)sBuffer, nLen ) ) )
					return nRet;
				UtilsMemCpy( (BYTE*)sBuffer, (BYTE*)sBuffer + nLen, 11 );
				pBufPointer = sBuffer + 11;
			}
		}

		//if ( 0 == pAttr->nLength1 || 0 == pAttr->nLength2 )
		//	return SetError( pFontDef->oError, AVS_OFFICEPDFWRITER_ERROR_UNSUPPORTED_TYPE1_FONT, 0);

		pAttr->nLength3 = pAttr->pFontData->nSize - pAttr->nLength1 - pAttr->nLength2;
	}
	else if ( 1 == nType || 2 == nType )
	{
		pAttr->pFontData = MemStreamNew( pFontDef->oMMgr, STREAM_BUF_SIZ );

		if ( !pAttr->pFontData )
			return ErrorGetCode( pFontDef->oError );

		StreamWriteToStream( pStream, pAttr->pFontData, 0, NULL );

		pAttr->nLength1 = pAttr->pFontData->nSize;
		pAttr->nLength2 = 0;
		pAttr->nLength3 = 0;
	}


    return OK;
}
//----- Type1FontDef основные функции ---------------------------------------------------------------
FontDef       Type1FontDefNew            (MMgr oMMgr)
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
    pFontDef->eType         = FontDefType1;
	pFontDef->pCleanFn      = NULL;
	pFontDef->pFreeFn       = Type1FreeFunc;
	pFontDef->pDescriptor   = NULL;
    pFontDef->bValid        = false;
	//pFontDef->nMissingWidth = 0;

    Type1FontDefAttr pFontDefAttr = (Type1FontDefAttr)GetMem( oMMgr, sizeof(Type1FontDefAttrRec) );
    if ( !pFontDefAttr ) 
	{
		FreeMem( pFontDef->oMMgr, pFontDef);
        return NULL;
    }

	pFontDef->pAttr = pFontDefAttr;
    UtilsMemSet( (BYTE *)pFontDefAttr, 0, sizeof(Type1FontDefAttrRec) );
	pFontDef->nFlags = FONT_STD_CHARSET;

    return pFontDef;
}
FontDef       Type1FontDefLoad           (MMgr oMMgr, StreamRecPtr pAFM, StreamRecPtr pFontData, short nType)
{
    unsigned long nRet = OK;

    if ( !pAFM )
        return NULL;

    FontDef pFontDef = Type1FontDefNew( oMMgr );

    if ( !pFontDef )
        return NULL;

    nRet = LoadAfm( pFontDef, pAFM );
	//nRet = LoadType1( pFontDef, wsFilePath/*, pFontData*/ );
    if ( OK != nRet ) 
	{
        FontDefFree( pFontDef );
        return NULL;
    }

	// если pFontData определена, тогда эти данные включаем в файл
    if ( pFontData ) 
	{
        nRet = LoadFontData( pFontDef, pFontData, nType );
        if ( OK != nRet ) 
		{
            FontDefFree( pFontDef );
            return NULL;
        }
    }

    return pFontDef;
}
FontDef       Type1FontDefDuplicate      (MMgr oMMgr, FontDef pSrc)
{
    FontDef pFontDef = Type1FontDefNew( oMMgr );

    pFontDef->eType = pSrc->eType;
    pFontDef->bValid = pSrc->bValid;

    // copy data of attr,widths
    // attention to charset
    return NULL;
}
unsigned long Type1FontDefSetWidths      (FontDef pFontDef, const CharData* poWidths)
{
    const CharData* poSrc = poWidths;
    Type1FontDefAttr pAttr = (Type1FontDefAttr)pFontDef->pAttr;
    CharData* poDst = NULL;
    unsigned nIndex = 0;

    Type1FreeWidth( pFontDef );

    while ( 0xFFFF != poSrc->nUnicode ) 
	{
        poSrc++;
        nIndex++;
    }

    pAttr->nWidthsCount = nIndex;

    poDst = (CharData*)GetMem( pFontDef->oMMgr, sizeof(CharData) * pAttr->nWidthsCount );
    if ( NULL == poDst )
        return ErrorGetCode( pFontDef->oError );

    UtilsMemSet( poDst, 0, sizeof(CharData) * pAttr->nWidthsCount );
	pAttr->aoWidths = poDst;

    poSrc = poWidths;
	for ( nIndex = 0; nIndex < pAttr->nWidthsCount; nIndex++) 
	{
		poDst->nCharCode = poSrc->nCharCode;
		poDst->nUnicode  = poSrc->nUnicode;
		poDst->nWidth    = poSrc->nWidth;
		if ( 0x0020 == poDst->nUnicode ) 
		{
			pFontDef->nMissingWidth = poSrc->nWidth;
        }

        poSrc++;
        poDst++;
    }

    return OK;
}
short         Type1FontDefGetWidth       (FontDef pFontDef, unsigned short nUnicode)
{
    Type1FontDefAttr pAttr = (Type1FontDefAttr)pFontDef->pAttr;
	CharData *pCharData = pAttr->aoWidths;

	for ( unsigned int nIndex = 0; nIndex < pAttr->nWidthsCount; nIndex++) 
	{
		if ( nUnicode == pCharData->nUnicode )
			return pCharData->nWidth;
        pCharData++;
    }

	return pFontDef->nMissingWidth;
}
short         Type1FontDefGetWidthByName (FontDef pFontDef, const char* sGlyphName)
{
    unsigned short nUnicode = GlyphNameToUnicode( sGlyphName );
    return Type1FontDefGetWidth( pFontDef, nUnicode);
}
//----- Type1Font вспомогательные функции -----------------------------------------------------------
static unsigned long Type1FontCreateDescriptor(MMgr oMMgr, FontDict pFont, Xref pXref)
{
    FontAttr pFontAttr = (FontAttr)pFont->pAttr;
    FontDef pDef = pFontAttr->pFontDef;
    Type1FontDefAttr pDefAttr = (Type1FontDefAttr)pDef->pAttr;

    if ( !pFontAttr->pFontDef->pDescriptor ) 
	{
        Dict pDescriptor = DictNew( oMMgr );
        unsigned long nRet = OK;
        Array oArray;

        if ( !pDescriptor )
            return ErrorGetCode( pFont->oError );

        nRet += XrefAdd( pXref, pDescriptor );
        nRet += DictAddName( pDescriptor, "Type", "FontDescriptor" );
        nRet += DictAddNumber( pDescriptor, "Ascent", pDef->nAscent );
        nRet += DictAddNumber( pDescriptor, "Descent", pDef->nDescent );
        nRet += DictAddNumber( pDescriptor, "Flags", pDef->nFlags );

        oArray = BoxArrayNew( oMMgr, pDef->oFontBox );
        nRet += DictAdd( pDescriptor, "FontBBox", oArray );
		nRet += DictAddName( pDescriptor, "FontName", pFontAttr->pFontDef->sBaseFont );
		nRet += DictAddNumber( pDescriptor, "ItalicAngle", pDef->nItalicAngle );
		nRet += DictAddNumber( pDescriptor, "StemV", pDef->nStemV );
		nRet += DictAddNumber( pDescriptor, "XHeight", pDef->nXHeight );

		if ( pDefAttr->sCharSet )
            nRet += DictAddName( pDescriptor, "CharSet", pDefAttr->sCharSet );

        if ( OK != nRet )
			return ErrorGetCode( pFont->oError );

        if ( pDefAttr->pFontData ) 
		{
            Dict pFontData = DictStreamNew( oMMgr, pXref );

            if ( !pFontData )
				return ErrorGetCode( pFont->oError );

			if ( OK != StreamWriteToStream( pDefAttr->pFontData, pFontData->pStream, STREAM_FILTER_NONE, NULL ) )
				return ErrorGetCode( pFont->oError );

			if ( 0 == pDefAttr->nType )
				nRet += DictAdd( pDescriptor, "FontFile", pFontData );
			else if ( 1 == pDefAttr->nType )
			{
				nRet += DictAdd( pDescriptor, "FontFile2", pFontData );
				nRet += DictAddName( pFontData, "Subtype", "Type1C" );
			}
			else if ( 2 == pDefAttr->nType )
			{
				nRet += DictAdd( pDescriptor, "FontFile3", pFontData );
				nRet += DictAddName( pFontData, "Subtype", "OpenType" );
			}

			nRet += DictAddNumber( pFontData, "Length1", pDefAttr->nLength1 );
			nRet += DictAddNumber( pFontData, "Length2", pDefAttr->nLength2 );
			nRet += DictAddNumber( pFontData, "Length3", pDefAttr->nLength3 );

            pFontData->nFilter = pFont->nFilter;
        }

        if ( OK != nRet )
			return ErrorGetCode( pFont->oError );

		pFontAttr->pFontDef->pDescriptor = pDescriptor;
    }

    return DictAdd( pFont, "FontDescriptor", pFontAttr->pFontDef->pDescriptor );
}
static unsigned long Type1FontCreateToUnicode (MMgr oMMgr, FontDict pFont, Xref pXref)
{
	FontAttr pFontAttr = (FontAttr)pFont->pAttr;
	
	if ( NULL != pFontAttr->pToUnicode )
	{
		Dict pToUnicodeDict = DictStreamNew( oMMgr, pXref );
        if ( !pToUnicodeDict )
            return ErrorGetCode( pFont->oError );

		pFontAttr->pToUnicode->pWriteFn( pFontAttr->pToUnicode, pToUnicodeDict->pStream );

#ifdef FILTER_FLATE_DECODE_ENABLED//???
		pToUnicodeDict->nFilter |= STREAM_FILTER_FLATE_DECODE;
#endif 

		pFontAttr->pCMapStream = pToUnicodeDict;

		return DictAdd( pFont, "ToUnicode", pFontAttr->pCMapStream );
	}

	return OK;
}
static unsigned long Type1FontBeforeWrite     (Dict pObj)
{
	unsigned long nRet = OK;

	FontDict pFont = (FontDict)pObj;
	FontAttr pFontAttr = (FontAttr)pFont->pAttr;
	
	if ( ((Type1FontDefAttr)pFontAttr->pFontDef->pAttr)->bIsBase14Font )
		return nRet;

	if ( OK != ( nRet = Type1FontCreateDescriptor( pFont->oMMgr, pFont, pFontAttr->pXref ) ) )
		return nRet;

	if ( OK != ( nRet = Type1FontCreateToUnicode( pFont->oMMgr, pFont, pFontAttr->pXref ) ) )
		return nRet;

	return OK;
}
static unsigned long Type1FontOnWrite         (Dict pObj, StreamRecPtr pStream)
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

		return pAttr->pEncoder->pWriteFn( pAttr->pEncoder, pStream);
	}
	else if ( EncoderTypeToUnicode != pAttr->pEncoder->eType )
	{
		Type1FontDefAttr pFontDefAttr = (Type1FontDefAttr)pAttr->pFontDef->pAttr;
		BasicEncoderAttr pEncoderAttr = (BasicEncoderAttr)pAttr->pEncoder->pAttr;

		char sBuffer[TEMP_BUFFER_SIZE];
		char *pEndPointer = sBuffer + TEMP_BUFFER_SIZE - 1;

		// для основных 14 фонтов следующие поля не обязательны
		if ( !pFontDefAttr->bIsBase14Font || pEncoderAttr->bHasDifferences ) 
		{
			char *pBufPointer;

			pBufPointer = (char*)UtilsStrCpy( sBuffer, "/FirstChar ", pEndPointer);
			pBufPointer = UtilsIToA( pBufPointer, pEncoderAttr->nFirstChar, pEndPointer);
			UtilsStrCpy( pBufPointer, "\012", pEndPointer);
			if ( OK != ( nRet = StreamWriteStr( pStream, sBuffer ) ) )
				return nRet;

			pBufPointer = (char*)UtilsStrCpy( sBuffer, "/LastChar ", pEndPointer);
			pBufPointer = UtilsIToA( pBufPointer, pEncoderAttr->nLastChar, pEndPointer);
			UtilsStrCpy( pBufPointer, "\012", pEndPointer);
			if ( OK != ( nRet = StreamWriteStr( pStream, sBuffer ) ) )
				return nRet;

			if ( OK != ( nRet = StreamWriteEscapeName( pStream, "Widths" ) ) )
				return nRet;

			if ( OK != ( nRet = StreamWriteStr( pStream, " [\012" ) ) )
				return nRet;

			pBufPointer = sBuffer;
			for ( unsigned int nIndex = pEncoderAttr->nFirstChar; nIndex <= pEncoderAttr->nLastChar; nIndex++) 
			{
				pBufPointer = UtilsIToA( pBufPointer, pAttr->anWidths[nIndex], pEndPointer);
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

		return pAttr->pEncoder->pWriteFn( pAttr->pEncoder, pStream);
	}
	else
	{
		Type1FontDefAttr pFontDefAttr = (Type1FontDefAttr)pAttr->pFontDef->pAttr;
		CMapEncoderAttr pEncoderAttr = (CMapEncoderAttr)pAttr->pEncoder->pAttr;

		char sBuffer[TEMP_BUFFER_SIZE];
		char *pEndPointer = sBuffer + TEMP_BUFFER_SIZE - 1;

		// для основных 14 фонтов следующие поля не обязательны
		if ( !pFontDefAttr->bIsBase14Font ) 
		{
			char *pBufPointer;

			pBufPointer = (char*)UtilsStrCpy( sBuffer, "/FirstChar ", pEndPointer);
			pBufPointer = UtilsIToA( pBufPointer, pEncoderAttr->nFirstChar, pEndPointer);
			UtilsStrCpy( pBufPointer, "\012", pEndPointer);
			if ( OK != ( nRet = StreamWriteStr( pStream, sBuffer ) ) )
				return nRet;

			pBufPointer = (char*)UtilsStrCpy( sBuffer, "/LastChar ", pEndPointer);
			pBufPointer = UtilsIToA( pBufPointer, pEncoderAttr->nLastChar, pEndPointer);
			UtilsStrCpy( pBufPointer, "\012", pEndPointer);
			if ( OK != ( nRet = StreamWriteStr( pStream, sBuffer ) ) )
				return nRet;

			if ( OK != ( nRet = StreamWriteEscapeName( pStream, "Widths" ) ) )
				return nRet;

			if ( OK != ( nRet = StreamWriteStr( pStream, " [\012" ) ) )
				return nRet;

			pBufPointer = sBuffer;
			for ( unsigned int nIndex = pEncoderAttr->nFirstChar; nIndex <= pEncoderAttr->nLastChar; nIndex++) 
			{
				pBufPointer = UtilsIToA( pBufPointer, pFontDefAttr->aoWidths[nIndex].nWidth, pEndPointer);
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

		return OK;
	}
}
static void          Type1FontOnFree          (Dict pObj)
{
    FontAttr pAttr = (FontAttr)pObj->pAttr;

    if ( pAttr ) 
	{
		if ( pAttr->anWidths ) 
			FreeMem( pObj->oMMgr, pAttr->anWidths );
		if ( pAttr->oExternWidths.anWidths )
			FreeMem( pObj->oMMgr, pAttr->oExternWidths.anWidths );
		FreeMem( pObj->oMMgr, pAttr );
    }
}
static TextWidth     Type1FontTextWidth       (FontDict pFont, const BYTE *pText, unsigned int nLen, unsigned int *pCIDs, unsigned int nLenCID)
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
	else
	{
		if ( pAttr->anWidths ) 
		{
			for ( unsigned int nIndex = 0; nIndex < nLen; nIndex++ ) 
			{
				nChar = pText[nIndex];
				oRet.nNumchars++;
				oRet.nWidth += pAttr->anWidths[nChar];

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
static unsigned int  Type1FontMeasureText     (FontDict pFont, const BYTE *pText, unsigned int nLen, unsigned int *pCIDs, unsigned int nLenCID, float fWidth, float fFontSize, float fCharSpace, float fWordSpace, bool bWordWrap, float *pfRealWidth)
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
			fTempWidth += pAttr->anWidths[nChar] * fFontSize / 1000;

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
//----- Type1Font основные функции ------------------------------------------------------------------
FontDict Type1FontNew  (MMgr oMMgr, FontDef pFontDef, EncoderRecPtr pEncoder, Xref pXref, PToUnicode pToUnicode)
{
    unsigned long nRet = OK;

    // Проверяем типы кодировки и фонта
	if ( EncoderTypeSingleByte != pEncoder->eType && EncoderTypeToUnicode != pEncoder->eType ) 
	{
        SetError( oMMgr->oError, AVS_OFFICEPDFWRITER_ERROR_INVALID_ENCODER_TYPE, 0);
        return NULL;
    }

	if ( FontDefType1 != pFontDef->eType ) 
	{
        SetError( oMMgr->oError, AVS_OFFICEPDFWRITER_ERROR_INVALID_FONTDEF_TYPE, 0);
        return NULL;
    }

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

	//pFont->pHeader.nObjClass |= OSUBCLASS_FONT;
	pFont->pBeforeWriteFn = Type1FontBeforeWrite;
	pFont->pWriteFn       = Type1FontOnWrite;
	pFont->pFreeFn        = Type1FontOnFree;

    UtilsMemSet( pAttr, 0, sizeof(FontAttrRec) );

    pFont->pAttr          = pAttr;
	pAttr->eType          = FontType1;
    pAttr->eWritingMode   = WModeHorizontal;
	pAttr->pTextWidthFn   = Type1FontTextWidth;
    pAttr->pMeasureTextFn = Type1FontMeasureText;
	pAttr->pFontDef       = pFontDef;
    pAttr->pEncoder       = pEncoder;
	pAttr->pToUnicode     = pToUnicode;
    pAttr->pXref          = pXref;

	pAttr->oExternWidths.bUse       = false;
	pAttr->oExternWidths.nLastChar  = 0;
	pAttr->oExternWidths.nFirstChar = 0;
	pAttr->oExternWidths.anWidths   = NULL;

    // однобайтные фонты содержат в себе массив ширин, который является массивом из 256 чисел тип short
	pAttr->anWidths = (short*)GetMem( oMMgr, sizeof(short) * 256);
    if ( !pAttr->anWidths ) 
	{
        DictFree( pFont );
        return NULL;
    }

	BasicEncoderAttr pEncoderAttr = (BasicEncoderAttr)pEncoder->pAttr;

	UtilsMemSet( pAttr->anWidths, 0, sizeof(short) * 256);
	for ( unsigned int nIndex = pEncoderAttr->nFirstChar; nIndex <= pEncoderAttr->nLastChar; nIndex++) 
	{
		unsigned short nUnicode = pEncoderAttr->anUnicodeMap[nIndex];
        unsigned short nWidth = Type1FontDefGetWidth( pFontDef, nUnicode );
		pAttr->anWidths[nIndex] = nWidth;
    }

    Type1FontDefAttr pFontDefAttr = (Type1FontDefAttr)pFontDef->pAttr;

    nRet += DictAddName( pFont, "Type", "Font");
    nRet += DictAddName( pFont, "BaseFont", pFontDef->sBaseFont );
    nRet += DictAddName( pFont, "Subtype", "Type1");

	if ( !pFontDefAttr->bIsBase14Font ) 
	{
		if ( 0 != pFontDef->nMissingWidth )
			nRet += DictAddNumber( pFont, "MissingWidth", pFontDef->nMissingWidth );

        //nRet += Type1FontCreateDescriptor( oMMgr, pFont, pXref );
    }

    if ( OK != nRet ) 
	{
        DictFree( pFont );
        return NULL;
    }

    if ( OK != XrefAdd( pXref, pFont ) )
        return NULL;

    return pFont;
}


//------ Общие функции ---FontTypes-----------------------------------------------------------

void FontDefFree    (FontDef pFontDef)
{
    if ( !pFontDef )
        return;

    if ( pFontDef->pFreeFn )
        pFontDef->pFreeFn( pFontDef );
    FreeMem( pFontDef->oMMgr, pFontDef);
}
void FontDefCleanup (FontDef pFontDef)
{
    if ( !pFontDef )
        return;

    if ( pFontDef->pCleanFn )
        pFontDef->pCleanFn( pFontDef );

	pFontDef->pDescriptor = NULL;
}
bool FontDefValidate(FontDef pFontDef)
{
    if ( !pFontDef || pFontDef->nSigBytes != FONTDEF_SIG_BYTES )
        return false;
    else
        return true;
}
