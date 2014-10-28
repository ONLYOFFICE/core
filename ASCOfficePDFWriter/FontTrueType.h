#ifndef _FONT_TRUE_TYPE_H
#define _FONT_TRUE_TYPE_H

#include FT_TRUETYPE_TABLES_H

#include "Utils.h"
#include "Font.h"
#include "FontTrueTypeWriter.h"

static char          *GetName(FT_Face pFace, unsigned short unGid)
{
	int nCharIndex = 0;

	if ( !pFace || !FT_HAS_GLYPH_NAMES( pFace ) )
		return NULL;

	char *sName = (char *)malloc( sizeof(char) * (LIMIT_MAX_NAME_LEN + 1) );
	FT_Get_Glyph_Name( pFace, unGid, sName, LIMIT_MAX_NAME_LEN );
	sName[LIMIT_MAX_NAME_LEN] = '\0';

	return (char*)sName;
}
static unsigned short GetGID(FT_Face pFace, unsigned short unUnicode)
{
	int nCharIndex = 0;

	if ( !pFace )
		return nCharIndex;

	for ( int nIndex = 0; nIndex < pFace->num_charmaps; nIndex++ )
	{
		FT_CharMap pCharMap = pFace->charmaps[nIndex];

		if ( FT_Set_Charmap( pFace, pCharMap ) )
			continue;

		FT_Encoding pEncoding = pCharMap->encoding;

		if ( FT_ENCODING_UNICODE == pEncoding )
		{
			if ( nCharIndex = FT_Get_Char_Index( pFace, unUnicode ) )
				return nCharIndex;
		}

		if ( FT_ENCODING_NONE == pEncoding || FT_ENCODING_MS_SYMBOL == pEncoding || FT_ENCODING_APPLE_ROMAN == pEncoding )
		{
			nCharIndex = FT_Get_Char_Index( pFace, unUnicode );
		}
		/*else if ( FT_ENCODING_ADOBE_STANDARD == pEncoding )
		{
			nCharIndex = FT_Get_Char_Index( pFace, unUnicode );
		}
		else if ( FT_ENCODING_ADOBE_CUSTOM == pEncoding )
		{
			nCharIndex = FT_Get_Char_Index( pFace, unUnicode );
		}
		else if ( FT_ENCODING_ADOBE_EXPERT == pEncoding )
		{
			nCharIndex = FT_Get_Char_Index( pFace, unUnicode );
		}*/
	}

	return nCharIndex;
}

static int            GetSymbolicCmapIndex(FT_Face pFace)
{
	TT_OS2 *pOs2 = (TT_OS2 *)FT_Get_Sfnt_Table( pFace, ft_sfnt_os2 );
	if ( NULL == pOs2 || 0xFFFF == pOs2->version )
		return -1;

	// Проверяем установлен ли 31 бит
	if ( !( pOs2->ulCodePageRange1 & 0x80000000 ) && !( pOs2->ulCodePageRange1 == 0 && pOs2->ulCodePageRange2 == 0 ) )
		return -1;


	for( int nIndex = 0; nIndex < pFace->num_charmaps; nIndex++ )
	{
		// Symbol
		if ( 0 == pFace->charmaps[nIndex]->encoding_id && 3 == pFace->charmaps[nIndex]->platform_id )
			return nIndex;
	}

	return -1;
}
static void FileWrite(Stream pStream, char *sData, int nLen) 
{
	StreamWrite( pStream, (BYTE *)sData, nLen );
}

//----- TrueType Fonts ------------------------------------------------------------------------------

static unsigned long TTFOnWrite      (Dict pObj, Stream pStream);
static unsigned long TTFBeforeWrite  (Dict pObj);
static void          TTFOnFree       (Dict pObj);
static unsigned int  CharWidth       (FontDict pFont, BYTE nCode);
static TextWidth     GetTextWidth    (FontDict pFont, const BYTE *sText, unsigned int nLen, unsigned int *pCIDs, unsigned int nLenCID);
static unsigned long CreateDescriptor(FontDict pFont);
static unsigned int  MeasureText     (FontDict pFont, const BYTE *sText, unsigned int nLen, unsigned int *pCIDs, unsigned int nLenCID, float fWidth, float fFontSize, float fCharSpace, float fWordSpace, BOOL bWordWrap, float *pfRealWidth);

unsigned short TTFontDefGetGlyphID  (FontDef pFontDef, unsigned short nUnicode);

//----- Функции для работы с FontFile ---------------------------------------------------------------

#define TTF_MAX_MEM_SIZ    10000

#define REQUIRED_TAGS_COUNT  13

static const char *c_asRequiredTags[REQUIRED_TAGS_COUNT] = 
{
    "OS/2",
    "cmap",
    "cvt ",
    "fpgm",
    "glyf",
    "head",
    "hhea",
    "hmtx",
    "loca",
    "maxp",
    "name",
    "post",
    "prep"
};

//----- Вспомогательные функции ---------------------------------------------------------------------
static unsigned short ReadUShort        (BYTE *pBuffer)
{
	unsigned short ushValue = 0;

	ushValue = pBuffer[0];
	ushValue = ( ushValue << 8 ) | pBuffer[1];

	return ushValue;
}
static unsigned short ReadULong         (BYTE *pBuffer, unsigned int unLen = 4)
{
	unsigned long ulValue = 0;

	unLen = max( 0, min( 4, unLen ) );
	if ( 4 == unLen )
	{
		ulValue = pBuffer[0];
		ulValue = ( ulValue << 8 ) | pBuffer[1];
		ulValue = ( ulValue << 8 ) | pBuffer[2];
		ulValue = ( ulValue << 8 ) | pBuffer[3];
	}
	if ( 3 == unLen )
	{
		ulValue = pBuffer[0];
		ulValue = ( ulValue << 8 ) | pBuffer[1];
		ulValue = ( ulValue << 8 ) | pBuffer[2];
	}
	if ( 2 == unLen )
	{
		ulValue = pBuffer[0];
		ulValue = ( ulValue << 8 ) | pBuffer[1];
	}
	if ( 1 == unLen )
	{
		ulValue = pBuffer[0];
	}

	return ulValue;
}
static unsigned short ReadShort         (BYTE *pBuffer)
{
	short shValue = 0;

	shValue = pBuffer[0];
	shValue = ( shValue << 8 ) | pBuffer[1];

	return shValue;
}
static void          UINT32Swap         (unsigned int   *pnValue)
{
    BYTE aValue[4];

    UtilsMemCpy( aValue, (BYTE *)pnValue, 4);

    *pnValue = (unsigned int)((unsigned int)aValue[0] << 24 |
               (unsigned int)               aValue[1] << 16 |
               (unsigned int)               aValue[2] <<  8 |
               (unsigned int)               aValue[3]);
}
static void          UINT16Swap         (unsigned short *pnValue)
{
    BYTE aValue[2];

    UtilsMemCpy( aValue, (BYTE *)pnValue, 2);
    *pnValue = (unsigned short)((unsigned short)aValue[0] << 8 | (unsigned short)aValue[1]);
}
static void          INT16Swap          (short          *pnValue)
{
    BYTE aValue[2];

    UtilsMemCpy( aValue, (BYTE *)pnValue, 2);
    *pnValue = (short)((short)aValue[0] << 8 | (short)aValue[1]);
}
static unsigned long GetUINT32          (Stream  pStream, unsigned int   *pnValue)
{
    unsigned int nSize = sizeof (unsigned int);

    unsigned long nRet = StreamRead( pStream, (BYTE *)pnValue, &nSize);
    if ( OK != nRet ) 
	{
        *pnValue = 0;
        return nRet;
    }

    UINT32Swap( pnValue );

    return OK;
}
static unsigned long GetUINT16          (Stream  pStream, unsigned short *pnValue)
{
    unsigned int nSize = sizeof (unsigned short);

    unsigned long nRet = StreamRead( pStream, (BYTE *)pnValue, &nSize);
    if ( OK != nRet ) 
	{
        *pnValue = 0;
        return nRet;
    }

    UINT16Swap( pnValue );

    return OK;
}

static unsigned long GetINT16           (Stream  pStream, short          *pnValue)
{
    unsigned int nSize = sizeof (short);

    unsigned long nRet = StreamRead( pStream, (BYTE *)pnValue, &nSize);
    if ( OK != nRet ) 
	{
        *pnValue = 0;
        return nRet;
    }

    INT16Swap( pnValue );
	return OK;
}
static unsigned long WriteUINT32        (Stream  pStream, unsigned int   nValue)
{
    unsigned int nTemp = nValue;

    UINT32Swap( &nTemp );

    unsigned long nRet = StreamWrite( pStream, (BYTE *)&nTemp, sizeof(nTemp) );
    if ( OK != nRet )
        return nRet;

    return OK;
}
static unsigned long WriteUINT16        (Stream  pStream, unsigned short nValue)
{
    unsigned short nTemp = nValue;

    UINT16Swap( &nTemp );

    unsigned long nRet = StreamWrite( pStream, (BYTE *)&nTemp, sizeof(nTemp) );
    if ( OK != nRet )
        return nRet;

    return OK;
}
static unsigned long WriteINT16         (Stream  pStream, short          nValue)
{
    short nTemp = nValue;

    INT16Swap( &nTemp );

    unsigned long nRet = StreamWrite( pStream, (BYTE *)&nTemp, sizeof(nTemp) );
    if ( OK != nRet )
        return nRet;

    return OK;
}
static void          TTFFreeAttr        (FontDef pFontDef)
{
    TTFontDefAttr pAttr = (TTFontDefAttr)pFontDef->pAttr;

    if ( pAttr ) 
	{
		if ( pAttr->pnCharSet )
			FreeMem( pFontDef->oMMgr, pAttr->pnCharSet );

		if ( pAttr->pHMetric )
            FreeMem( pFontDef->oMMgr, pAttr->pHMetric );

		if ( pAttr->oNameTable.pNameRecords )
            FreeMem( pFontDef->oMMgr, pAttr->oNameTable.pNameRecords );

		if ( pAttr->oCMap.pnEndCount )
            FreeMem( pFontDef->oMMgr, pAttr->oCMap.pnEndCount );

		if ( pAttr->oCMap.pnStartCount )
            FreeMem( pFontDef->oMMgr, pAttr->oCMap.pnStartCount );

		if ( pAttr->oCMap.pnIDDelta )
            FreeMem( pFontDef->oMMgr, pAttr->oCMap.pnIDDelta );

		if ( pAttr->oCMap.pnIDRangeOffset )
            FreeMem( pFontDef->oMMgr, pAttr->oCMap.pnIDRangeOffset );

		if ( pAttr->oCMap.pnGlyphIdArray )
			FreeMem( pFontDef->oMMgr, pAttr->oCMap.pnGlyphIdArray );

		if ( pAttr->oCmap.pPairs )
			FreeMem( pFontDef->oMMgr, pAttr->oCmap.pPairs );

		if ( pAttr->oOffsetTable.pTable )
            FreeMem( pFontDef->oMMgr, pAttr->oOffsetTable.pTable );

		if ( pAttr->oGlyphTable.pFlags )
            FreeMem( pFontDef->oMMgr, pAttr->oGlyphTable.pFlags);

		if ( pAttr->oGlyphTable.pnOffsets )
            FreeMem( pFontDef->oMMgr, pAttr->oGlyphTable.pnOffsets );

		if ( pAttr->pStream )
			StreamFree( pAttr->pStream );
    }
}
static void          TTFFreeFunc        (FontDef pFontDef)
{
    TTFontDefAttr pAttr = (TTFontDefAttr)pFontDef->pAttr;

    if ( pAttr ) 
	{
        TTFFreeAttr( pFontDef );
		FreeMem( pFontDef->oMMgr, pAttr );
    }
}
static unsigned long LoadTTFTable       (FontDef pFontDef)
{
    TTFontDefAttr pAttr = (TTFontDefAttr)pFontDef->pAttr;

	// открываем файл
	HANDLE hFile = CreateFile( pAttr->wsFilePath, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (INVALID_HANDLE_VALUE == hFile)
		return AVS_OFFICEPDFWRITER_ERROR_FILE_OPEN_ERROR;

	// мапим этот файл в память - так быстрее читаются данные из файла
	DWORD  nFileSize = GetFileSize(hFile, NULL);
	HANDLE hMapFile  = CreateFileMapping(hFile, NULL, PAGE_READONLY, 0, nFileSize, NULL);
	if (NULL == hMapFile)
	{
		CloseHandle( hFile );
		return AVS_OFFICEPDFWRITER_ERROR_FILE_OPEN_ERROR;
	}

	// создаем view of file
	DWORD nMaxBytesRead	= nFileSize;
	VOID* pBaseAddress	= MapViewOfFile( hMapFile, FILE_MAP_READ, 0, 0, 0 );
	if ( !pBaseAddress )
	{
		CloseHandle( hMapFile );
		CloseHandle( hFile );
		return AVS_OFFICEPDFWRITER_ERROR_FILE_OPEN_ERROR;
	}

	BYTE* pnFile = (BYTE*)pBaseAddress;

	// Проверим, возможно это коллекция шрифтов (ttfc)
	BYTE *pPointer = pnFile;

	BYTE anTag[4] = { pPointer[0], pPointer[1], pPointer[2], pPointer[3] };
    if ( 0 == UtilsMemCmp( anTag, (BYTE*)"ttcf", 4 ) )
	{
		pPointer += 8;

		unsigned int unNumFonts = ReadULong( pPointer ); pPointer += 4;

		if ( pAttr->unFontIndex >= unNumFonts )
		{
			UnmapViewOfFile(pBaseAddress);
			CloseHandle(hMapFile);
			CloseHandle( hFile );
			return SetError( pFontDef->oError, AVS_OFFICEPDFWRITER_ERROR_INVALID_TTC_INDEX, 0);
		}

		pPointer += pAttr->unFontIndex * 4;

		unsigned long ulOffset = ReadULong( pPointer );
		pnFile += ulOffset;
	}
	//

	pAttr->oOffsetTable.nSFNTVersion   = ReadULong( pnFile );  pnFile += 4;
	pAttr->oOffsetTable.nNumTables     = ReadUShort( pnFile ); pnFile += 2;
	pAttr->oOffsetTable.nSearchRange   = ReadUShort( pnFile ); pnFile += 2;
	pAttr->oOffsetTable.nEntrySelector = ReadUShort( pnFile ); pnFile += 2;
	pAttr->oOffsetTable.nRangeShift    = ReadUShort( pnFile ); pnFile += 2;

	if ( pAttr->oOffsetTable.nNumTables * sizeof(TTFTable) > TTF_MAX_MEM_SIZ )
	{
		UnmapViewOfFile(pBaseAddress);
		CloseHandle(hMapFile);
		CloseHandle( hFile );
		return SetError( pFontDef->oError, AVS_OFFICEPDFWRITER_ERROR_TTF_INVALID_FORMAT, 0);
	}
	pAttr->oOffsetTable.pTable = (TTFTable *)GetMem( pFontDef->oMMgr, sizeof(TTFTable) * pAttr->oOffsetTable.nNumTables );
	if ( !pAttr->oOffsetTable.pTable )
	{
		UnmapViewOfFile(pBaseAddress);
		CloseHandle(hMapFile);
		CloseHandle( hFile );
		return ErrorGetCode( pFontDef->oError );
	}
	TTFTable *pTable = pAttr->oOffsetTable.pTable;

	for ( int nIndex = 0; nIndex < pAttr->oOffsetTable.nNumTables; nIndex++) 
	{
		pTable->anTag[0]  = (char)pnFile[0]; pnFile++;
		pTable->anTag[1]  = (char)pnFile[0]; pnFile++;
		pTable->anTag[2]  = (char)pnFile[0]; pnFile++;
		pTable->anTag[3]  = (char)pnFile[0]; pnFile++;
		pTable->nCheckSum = ReadULong( pnFile ); pnFile += 4;
		pTable->nOffset   = ReadULong( pnFile ); pnFile += 4;
		pTable->nLength   = ReadULong( pnFile ); pnFile += 4;
		pTable++;
	}

	UnmapViewOfFile(pBaseAddress);
	CloseHandle(hMapFile);
	CloseHandle( hFile );
	return OK;
}
static TTFTable*     FindTable          (FontDef pFontDef, const char *sTag)
{
    TTFontDefAttr pAttr = (TTFontDefAttr)pFontDef->pAttr;
    TTFTable* pTable = pAttr->oOffsetTable.pTable;

    for ( unsigned int nIndex = 0; nIndex < pAttr->oOffsetTable.nNumTables; nIndex++, pTable++) 
	{
		if ( 0 == UtilsMemCmp( (BYTE*)pTable->anTag, (BYTE*)sTag, 4) ) 
            return pTable;
    }

    return NULL;
}

static unsigned long GetTTFTable        (FontDef pFontDef, FT_Face pFace, const char *sTag, BYTE **ppPointer, unsigned long *ulLength)
{
    if ( !pFontDef )
		return SetError( pFontDef->oError, AVS_OFFICEPDFWRITER_ERROR_TTF_MISSING_TABLE, 5);

	FT_ULong ulLen = 0;
	if ( FT_Load_Sfnt_Table( pFace, FT_MAKE_TAG(sTag[0], sTag[1], sTag[2], sTag[3]), 0, NULL, &ulLen ) )
		return SetError( pFontDef->oError, AVS_OFFICEPDFWRITER_ERROR_TTF_MISSING_TABLE, 5);

	BYTE *pBuffer = new BYTE[ulLen];
	if ( NULL == pBuffer )
		return SetError( pFontDef->oError, AVS_OFFICEPDFREADER_ERROR_NOT_ENOUGH_MEMORY, 5);

	if ( FT_Load_Sfnt_Table( pFace, FT_MAKE_TAG(sTag[0], sTag[1], sTag[2], sTag[3]), 0, pBuffer, &ulLen ) )
		return SetError( pFontDef->oError, AVS_OFFICEPDFWRITER_ERROR_TTF_MISSING_TABLE, 5);

	*ppPointer = pBuffer;
	*ulLength  = ulLen;

	return OK;
}
static unsigned long ParseHead          (FontDef pFontDef, FT_Face pFace)
{
    if ( !pFontDef )
		return SetError( pFontDef->oError, AVS_OFFICEPDFWRITER_ERROR_TTF_MISSING_TABLE, 5);

	TT_Header *pHeader = (TT_Header *)FT_Get_Sfnt_Table( pFace, ft_sfnt_head );
	if ( NULL == pHeader )
		return SetError( pFontDef->oError, AVS_OFFICEPDFWRITER_ERROR_TTF_MISSING_TABLE, 5);

	TTFontDefAttr pAttr = (TTFontDefAttr)pFontDef->pAttr;

	pAttr->oHeader.anVersionNumber[0]  = (BYTE)(pHeader->Table_Version);
	pAttr->oHeader.anVersionNumber[1]  = (BYTE)(pHeader->Table_Version >> 8);
	pAttr->oHeader.anVersionNumber[2]  = (BYTE)(pHeader->Table_Version >> 16);
	pAttr->oHeader.anVersionNumber[3]  = (BYTE)(pHeader->Table_Version >> 24);
	pAttr->oHeader.nFontRevision       = pHeader->Font_Revision;
	pAttr->oHeader.nCheckSumAdjustment = pHeader->CheckSum_Adjust;
	pAttr->oHeader.nMagicNumber        = pHeader->Magic_Number;
	pAttr->oHeader.nFlags              = pHeader->Flags;
	pAttr->oHeader.nUnitsPerEm         = pHeader->Units_Per_EM;

	pAttr->oHeader.anCreated[0]        = (BYTE)(pHeader->Created[0]);
	pAttr->oHeader.anCreated[1]        = (BYTE)(pHeader->Created[0] >> 8);
	pAttr->oHeader.anCreated[2]        = (BYTE)(pHeader->Created[0] >> 16);
	pAttr->oHeader.anCreated[3]        = (BYTE)(pHeader->Created[0] >> 24);
	pAttr->oHeader.anCreated[4]        = (BYTE)(pHeader->Created[1]);
	pAttr->oHeader.anCreated[5]        = (BYTE)(pHeader->Created[1] >> 8);
	pAttr->oHeader.anCreated[6]        = (BYTE)(pHeader->Created[1] >> 16);
	pAttr->oHeader.anCreated[7]        = (BYTE)(pHeader->Created[1] >> 24);	

	pAttr->oHeader.anModified[0]       = (BYTE)(pHeader->Modified[0]);
	pAttr->oHeader.anModified[1]       = (BYTE)(pHeader->Modified[0] >> 8);
	pAttr->oHeader.anModified[2]       = (BYTE)(pHeader->Modified[0] >> 16);
	pAttr->oHeader.anModified[3]       = (BYTE)(pHeader->Modified[0] >> 24);
	pAttr->oHeader.anModified[4]       = (BYTE)(pHeader->Modified[1]);
	pAttr->oHeader.anModified[5]       = (BYTE)(pHeader->Modified[1] >> 8);
	pAttr->oHeader.anModified[6]       = (BYTE)(pHeader->Modified[1] >> 16);
	pAttr->oHeader.anModified[7]       = (BYTE)(pHeader->Modified[1] >> 24);

	pAttr->oHeader.nXmin               = pHeader->xMin;
	pAttr->oHeader.nYmin               = pHeader->yMin;
	pAttr->oHeader.nXmax               = pHeader->xMax;
	pAttr->oHeader.nYmax               = pHeader->yMax;

	pAttr->oHeader.nMacStyle           = pHeader->Mac_Style;
	pAttr->oHeader.nLowestRecPPEM      = pHeader->Lowest_Rec_PPEM;
	pAttr->oHeader.nFontDirectionHint  = pHeader->Font_Direction;
	pAttr->oHeader.nIndexToLocFormat   = pHeader->Index_To_Loc_Format;
	pAttr->oHeader.nGlyphDataFormat    = pHeader->Glyph_Data_Format;

	if ( pAttr->oHeader.nUnitsPerEm )
	{
		pFontDef->oFontBox.fLeft   = (int)pAttr->oHeader.nXmin * 1000 / pAttr->oHeader.nUnitsPerEm;
		pFontDef->oFontBox.fBottom = (int)pAttr->oHeader.nYmin * 1000 / pAttr->oHeader.nUnitsPerEm;
		pFontDef->oFontBox.fRight  = (int)pAttr->oHeader.nXmax * 1000 / pAttr->oHeader.nUnitsPerEm;
		pFontDef->oFontBox.fTop    = (int)pAttr->oHeader.nYmax * 1000 / pAttr->oHeader.nUnitsPerEm;
	}
	else
	{
		pFontDef->oFontBox.fLeft   = (int)1;
		pFontDef->oFontBox.fBottom = (int)1;
		pFontDef->oFontBox.fRight  = (int)1;
		pFontDef->oFontBox.fTop    = (int)1;
	}

    return OK;
}
static unsigned long ParseMaxp          (FontDef pFontDef, FT_Face pFace)
{
    if ( !pFontDef )
		return SetError( pFontDef->oError, AVS_OFFICEPDFWRITER_ERROR_TTF_MISSING_TABLE, 5);

	TT_MaxProfile *pMaxP = (TT_MaxProfile *)FT_Get_Sfnt_Table( pFace, ft_sfnt_maxp );
	if ( NULL == pMaxP )
		return SetError( pFontDef->oError, AVS_OFFICEPDFWRITER_ERROR_TTF_MISSING_TABLE, 5);

	TTFontDefAttr pAttr = (TTFontDefAttr)pFontDef->pAttr;

	pAttr->nNumGlyphs = pMaxP->numGlyphs;

    return OK;
}
static unsigned long ParseHhea          (FontDef pFontDef, FT_Face pFace)
{
    if ( !pFontDef )
		return SetError( pFontDef->oError, AVS_OFFICEPDFWRITER_ERROR_TTF_MISSING_TABLE, 5);

	TT_HoriHeader *pHorHeader = (TT_HoriHeader *)FT_Get_Sfnt_Table( pFace, ft_sfnt_hhea );
	if ( NULL == pHorHeader )
		return SetError( pFontDef->oError, AVS_OFFICEPDFWRITER_ERROR_TTF_MISSING_TABLE, 5);

	TTFontDefAttr pAttr = (TTFontDefAttr)pFontDef->pAttr;

	pFontDef->nAscent  = pHorHeader->Ascender;
	pFontDef->nDescent = pHorHeader->Descender;

	if ( 0 != pAttr->oHeader.nUnitsPerEm )
	{
		pFontDef->nAscent  = (int)pFontDef->nAscent  * 1000 / pAttr->oHeader.nUnitsPerEm;
		pFontDef->nDescent = (int)pFontDef->nDescent * 1000 / pAttr->oHeader.nUnitsPerEm;
	}

	pAttr->nNumHMetric = pHorHeader->number_Of_HMetrics;

    return OK;
}


static unsigned long ParseCMap          (FontDef pFontDef, FT_Face pFace)
{
    TTFontDefAttr pAttr = (TTFontDefAttr)pFontDef->pAttr;

    unsigned int   nMSUnicodeEncodingOffset = 0;
    unsigned int   nByteEncodingOffset      = 0;

	unsigned int   unCMapIndex = 0;
	int nUnicodeCmap = -1, nMacRomanCmap = -1, nMSSymbolCmap = -1;

	for (unsigned int nIndex = 0; nIndex < pFace->num_charmaps; nIndex++ ) 
	{
        unsigned short nPlatformID = pFace->charmaps[nIndex]->platform_id;
        unsigned short nEncodingID = pFace->charmaps[nIndex]->encoding_id;
		
		if ( ( 3 == nPlatformID && 1 == nEncodingID ) || 0 == nPlatformID ) 
		{
			nUnicodeCmap = nIndex;
		} 
		else if ( 1 == nPlatformID && 0 == nEncodingID )
		{
			nMacRomanCmap = nIndex;
		} 
		else if ( 3 == nPlatformID && 0 == nEncodingID )
		{
			nMSSymbolCmap = nIndex;
		}
    }

	BOOL bUseMacRoman = FALSE;
	BOOL bUseUnicode = FALSE;
	
	if ( pAttr->bEncoding ) 
	{
		if ( pAttr->bUsesMacRomanEncoding && nMacRomanCmap >= 0) 
		{
			unCMapIndex = nMacRomanCmap;
		} 
		else if ( nUnicodeCmap >= 0 ) 
		{
			unCMapIndex = nUnicodeCmap;
		} 
		else if ( pAttr->bSymbolic && nMSSymbolCmap >= 0 ) 
		{
			unCMapIndex = nMSSymbolCmap;
		} 
		else if ( pAttr->bSymbolic && nMacRomanCmap >= 0 ) 
		{
			unCMapIndex = nMacRomanCmap;
		} 
		else if ( nMacRomanCmap >= 0 ) 
		{
			unCMapIndex = nMacRomanCmap;
		}
	} 
	else 
	{
		if ( nMSSymbolCmap >= 0 ) 
		{
			unCMapIndex = nMSSymbolCmap;
		} 
		else if ( nMacRomanCmap >= 0 ) 
		{
			unCMapIndex = nMacRomanCmap;
		}
	}

	FT_Set_Charmap( pFace, pFace->charmaps[unCMapIndex] );

	FT_ULong ulCharCode;                                              
	FT_UInt  unGlyphID;

	ulCharCode = FT_Get_First_Char( pFace, &unGlyphID );

	while ( 0 != unGlyphID )                                            
	{
		pAttr->oCmap.unCount++;
		ulCharCode = FT_Get_Next_Char( pFace, ulCharCode, &unGlyphID );        
	}

	pAttr->oCmap.pPairs = (TTrueTypeFontPair *)GetMem( pFontDef->oMMgr, sizeof(TTrueTypeFontPair) * pAttr->oCmap.unCount);
    if ( !pAttr->oCmap.pPairs )
		return ErrorGetCode( pFontDef->oError );

	ulCharCode = FT_Get_First_Char( pFace, &unGlyphID );
	unsigned int unIndex = 0;

	while ( 0 != unGlyphID )                                            
	{
		pAttr->oCmap.pPairs[unIndex].ushCharCode = ulCharCode;
		pAttr->oCmap.pPairs[unIndex].ushGlyphID  = unGlyphID;

		if ( FT_Load_Glyph( pFace, unGlyphID, FT_LOAD_NO_SCALE ) )
		{
			pAttr->oCmap.pPairs[unIndex].oBBox.fLeft    = 0;
			pAttr->oCmap.pPairs[unIndex].oBBox.fTop     = 0;
			pAttr->oCmap.pPairs[unIndex].oBBox.fRight   = 0;
			pAttr->oCmap.pPairs[unIndex].oBBox.fBottom  = 0;
			pAttr->oCmap.pPairs[unIndex].fAdvance       = 0;
		}
		else
		{
			pAttr->oCmap.pPairs[unIndex].oBBox.fLeft    = pFace->glyph->metrics.horiBearingX;
			pAttr->oCmap.pPairs[unIndex].oBBox.fTop     = pFace->glyph->metrics.horiBearingY;
			pAttr->oCmap.pPairs[unIndex].oBBox.fRight   = pFace->glyph->metrics.horiBearingX + pFace->glyph->metrics.width;
			pAttr->oCmap.pPairs[unIndex].oBBox.fBottom  = pFace->glyph->metrics.horiBearingY - pFace->glyph->metrics.height;
			pAttr->oCmap.pPairs[unIndex].fAdvance       = pFace->glyph->metrics.horiAdvance;

			float fKoef = 1000.0f / pAttr->oHeader.nUnitsPerEm;
			pAttr->oCmap.pPairs[unIndex].oBBox.fLeft    *= fKoef;
			pAttr->oCmap.pPairs[unIndex].oBBox.fTop     *= fKoef;
			pAttr->oCmap.pPairs[unIndex].oBBox.fRight   *= fKoef;
			pAttr->oCmap.pPairs[unIndex].oBBox.fBottom  *= fKoef;
			pAttr->oCmap.pPairs[unIndex].fAdvance       *= fKoef;
		}

		ulCharCode = FT_Get_Next_Char( pFace, ulCharCode, &unGlyphID );        
		unIndex++;
	}

    return OK;
}
static unsigned long ParseHmtx          (FontDef pFontDef, FT_Face pFace)
{
	unsigned long unRet = OK;

	BYTE *pBuffer = NULL;
	unsigned long ulLength = 0;
	if ( OK != ( unRet = GetTTFTable( pFontDef, pFace, "hmtx", &pBuffer, &ulLength ) ) ) 
		return unRet;

	BYTE *pPointer = pBuffer;

	TTFontDefAttr pAttr = (TTFontDefAttr)pFontDef->pAttr;

    // Выделяем память для таблицы горизонтальных матриц. Количество записей
	// такое же как и количество символов(glyphs).
	pAttr->pHMetric = (TTFLongHorMetric*)GetMem( pFontDef->oMMgr, sizeof(TTFLongHorMetric) * pAttr->nNumGlyphs );

    if ( !pAttr->pHMetric )
		return ErrorGetCode( pFontDef->oError );

    TTFLongHorMetric *pMetric = pAttr->pHMetric;

	unsigned int nIndex = 0;
	unsigned short ushSaveAdvWidth = 0;
	for ( nIndex = 0; nIndex < pAttr->nNumHMetric; nIndex++) 
	{
		pMetric->nAdvanceWidth    = ReadUShort( pPointer ); pPointer += 2;
		pMetric->nLeftSideBearing = ReadUShort( pPointer ); pPointer += 2;

		ushSaveAdvWidth = pMetric->nAdvanceWidth;
        pMetric++;
    }

    // заполняем nAdvanceWidth оставшихся метрик соответствующим значением последней метрики
	while ( nIndex < pAttr->nNumGlyphs ) 
	{
		pMetric->nAdvanceWidth = ushSaveAdvWidth;

		pMetric->nLeftSideBearing = ReadUShort( pPointer ); pPointer += 2;

        pMetric++;
        nIndex++;
    }

	delete []pBuffer;

    return OK;
}
static unsigned long ParseLoca          (FontDef pFontDef, FT_Face pFace)
{
	unsigned long unRet = OK;

	BYTE *pBuffer = NULL;
	unsigned long ulLength = 0;
	if ( OK != ( unRet = GetTTFTable( pFontDef, pFace, "loca", &pBuffer, &ulLength ) ) ) 
		return unRet;

	BYTE *pPointer = pBuffer;

	TTFontDefAttr pAttr = (TTFontDefAttr)pFontDef->pAttr;

	pAttr->oGlyphTable.pnOffsets = (unsigned int*)GetMem( pFontDef->oMMgr, sizeof(unsigned int) * (pAttr->nNumGlyphs + 1) );

    if ( !pAttr->oGlyphTable.pnOffsets )
		return ErrorGetCode( pFontDef->oError );

	UtilsMemSet( pAttr->oGlyphTable.pnOffsets, 0, sizeof (unsigned int) * (pAttr->nNumGlyphs + 1) );

    // pFlags используется для определения, какие символы используются
	pAttr->oGlyphTable.pFlags = (BYTE*)GetMem( pFontDef->oMMgr, sizeof (BYTE) * pAttr->nNumGlyphs );

    if ( !pAttr->oGlyphTable.pFlags )
		return ErrorGetCode( pFontDef->oError );

	UtilsMemSet( pAttr->oGlyphTable.pFlags, 0, sizeof (BYTE) * pAttr->nNumGlyphs );
    pAttr->oGlyphTable.pFlags[0] = 1;

	unsigned int *pOffset = pAttr->oGlyphTable.pnOffsets;
	if ( 0 == pAttr->oHeader.nIndexToLocFormat ) 
	{
        // short version 
		for ( unsigned int nIndex = 0; nIndex <= pAttr->nNumGlyphs; nIndex++) 
		{
            unsigned short ushTemp = ReadUShort( pPointer ); pPointer += 2;

			*pOffset = ushTemp;
            pOffset++;
        }
    } 
	else 
	{
        // long version 
        for ( unsigned int nIndex = 0; nIndex <= pAttr->nNumGlyphs; nIndex++) 
		{
			unsigned long ulTemp = ReadULong( pPointer ); pPointer += 4;

			*pOffset = ulTemp;
			pOffset++;
        }
    }

	delete []pBuffer;
    return OK;
}



static unsigned long ParseName          (FontDef pFontDef, FT_Face pFace)
{
	unsigned long unRet = OK;

	BYTE *pBuffer = NULL;
	unsigned long ulLength = 0;
	if ( OK != ( unRet = GetTTFTable( pFontDef, pFace, "name", &pBuffer, &ulLength ) ) ) 
		return unRet;

	BYTE *pPointer = pBuffer;

	TTFontDefAttr pAttr = (TTFontDefAttr)pFontDef->pAttr;

	pAttr->oNameTable.nFormat       = ReadUShort( pPointer ); pPointer += 2;
	pAttr->oNameTable.nCount        = ReadUShort( pPointer ); pPointer += 2;
	pAttr->oNameTable.nStringOffset = ReadUShort( pPointer ); pPointer += 2;

	// pNameRecords
	pAttr->oNameTable.pNameRecords = (TTFNameRecord*)GetMem( pFontDef->oMMgr, sizeof (TTFNameRecord) * pAttr->oNameTable.nCount );
    if ( !pAttr->oNameTable.pNameRecords )
        return ErrorGetCode( pFontDef->oError );

	TTFNameRecord *pNameRec = pAttr->oNameTable.pNameRecords;

	for ( unsigned int nIndex = 0; nIndex < pAttr->oNameTable.nCount; nIndex++) 
	{
		pNameRec->nPlatformID = ReadUShort( pPointer ); pPointer += 2;
		pNameRec->nEncodingID = ReadUShort( pPointer ); pPointer += 2;
		pNameRec->nLanguageID = ReadUShort( pPointer ); pPointer += 2;
		pNameRec->nNameID     = ReadUShort( pPointer ); pPointer += 2;
		pNameRec->nLength     = ReadUShort( pPointer ); pPointer += 2;
		pNameRec->nOffset     = ReadUShort( pPointer ); pPointer += 2;

        pNameRec++;
    }

	UtilsMemSet( pAttr->sBaseFont, 0, LIMIT_MAX_NAME_LEN + 1);

	CStringA sFullName = CStringA( pFace->family_name )+ CStringA(",") + CStringA( pFace->style_name );
	
	CStringA sStyle( pFace->style_name );
	if ( -1 == sStyle.Find( "Regular" ) )
	{
		if ( -1 != sStyle.Find("Bold") )
			pFontDef->nFlags |= FONT_FOURCE_BOLD;
		if ( -1 != sStyle.Find("Italic") || -1 != sStyle.Find("Oblique") )
			pFontDef->nFlags |= FONT_ITALIC;
	}

	UtilsStrCpy( pAttr->sBaseFont, sFullName.GetBuffer(), pAttr->sBaseFont + LIMIT_MAX_NAME_LEN );
	UtilsMemCpy( (BYTE*)pFontDef->sBaseFont, (BYTE*)pAttr->sBaseFont, LIMIT_MAX_NAME_LEN + 1);

	delete []pBuffer;
    return OK;
}
static unsigned long ParseOS2           (FontDef pFontDef, FT_Face pFace)
{
    if ( !pFontDef )
		return SetError( pFontDef->oError, AVS_OFFICEPDFWRITER_ERROR_TTF_MISSING_TABLE, 5);

	TT_OS2 *pOs2 = (TT_OS2 *)FT_Get_Sfnt_Table( pFace, ft_sfnt_os2 );
	if ( NULL == pOs2 )
		return SetError( pFontDef->oError, AVS_OFFICEPDFWRITER_ERROR_TTF_MISSING_TABLE, 5);

	TTFontDefAttr pAttr = (TTFontDefAttr)pFontDef->pAttr;

	// проверяем можно ли включить данный фонт, т.е. нарушается ли лицензия
	//if ( pOs2->fsType  & (0x0002 | 0x0100 | 0x0200) && pAttr->bEmbedding )
	//	return SetError( pFontDef->oError, AVS_OFFICEPDFWRITER_ERROR_TTF_CANNOT_EMBEDDING_FONT, 0);

	if ( 1 == pOs2->panose[0] || 4 == pOs2->panose[0] )
		pFontDef->nFlags = pFontDef->nFlags | FONT_SERIF;

	pFontDef->nCapHeight = pOs2->sCapHeight;
	pFontDef->nXHeight   = pOs2->sxHeight;

	return OK;
}
static unsigned long ParseGlyf          (FontDef pFontDef, FT_Face pFace)
{
	TTFontDefAttr pAttr = (TTFontDefAttr)pFontDef->pAttr;

	TTFTable *pTable = FindTable( pFontDef, "glyf" );
    if ( !pTable )
		return SetError( pFontDef->oError, AVS_OFFICEPDFWRITER_ERROR_TTF_MISSING_TABLE, 4);

	pAttr->oGlyphTable.nBaseOffset = pTable->nOffset;

	return OK;
}
Box                  TTFontDefGetCharBBox(FontDef pFontDef, unsigned short nUnicode);
static unsigned long LoadFontData       (FontDef pFontDef, const wchar_t *wsFilePath, BOOL bEmbedding, const char *sToUnicodeName = NULL, unsigned int unIndex = 0)
{
    TTFontDefAttr pAttr = (TTFontDefAttr)pFontDef->pAttr;
	if ( NULL != sToUnicodeName )
		UtilsMemCpy( (BYTE *)pAttr->sToUnicodeName, (BYTE *)sToUnicodeName, LIMIT_MAX_NAME_LEN + 1 );

	CFontFileTrueType *pTTF = CFontFileTrueType::LoadFromFile( pAttr->wsFilePath );
	if ( pTTF )
	{
		pAttr->bCFF = pTTF->IsOpenTypeCFF();
		delete pTTF;
	}

	FT_Library pLibrary = NULL;
	if ( FT_Init_FreeType( &pLibrary ) )
	{
		return 1;
	}

	USES_CONVERSION;

	FT_Face pFace = NULL;

	char *sPath = W2A( wsFilePath );
	if ( FT_New_Face( pLibrary, sPath, unIndex, &pFace  ) )
	{
		FT_Done_FreeType( pLibrary );
		return 1;
	}


	unsigned long nRet = OK;

	pAttr->bEmbedding = bEmbedding;

	if ( OK != ( nRet = LoadTTFTable( pFontDef ) ) )
	{
		FT_Done_Face( pFace );
		FT_Done_FreeType( pLibrary );
		return nRet;
	}

    ParseHead( pFontDef, pFace );
    ParseMaxp( pFontDef, pFace );
    ParseHhea( pFontDef, pFace );
    ParseCMap( pFontDef, pFace );
    ParseHmtx( pFontDef, pFace );

	//if ( OK != ( nRet = ParseLoca( pFontDef, pFace ) ) )
	//	return nRet;

    ParseName( pFontDef, pFace );
    ParseOS2 ( pFontDef, pFace );

	//if ( OK != ( nRet = ParseGlyf( pFontDef, pFace ) ) )
	//	return nRet;

	if ( 0 == pFontDef->nCapHeight ) pFontDef->nCapHeight = TTFontDefGetCharBBox( pFontDef, (unsigned short)'H').fTop;
	if ( 0 == pFontDef->nXHeight   ) pFontDef->nXHeight   = TTFontDefGetCharBBox( pFontDef, (unsigned short)'x').fTop;
	pFontDef->nMissingWidth = (unsigned int)pAttr->pHMetric[0].nAdvanceWidth * 1000 / pAttr->oHeader.nUnitsPerEm;

	FT_Done_Face( pFace );
	FT_Done_FreeType( pLibrary );

    return OK;
}
static unsigned long LoadFontData2      (FontDef pFontDef, const wchar_t *wsFilePath, unsigned int unIndex, BOOL bEmbedding, const char *sEncoding = NULL)
{
    return LoadFontData( pFontDef, wsFilePath, bEmbedding, sEncoding, unIndex );
}
static unsigned long CheckCompositGlyph (FontDef pFontDef, unsigned short nGid)
{
	TTFontDefAttr pAttr = (TTFontDefAttr)pFontDef->pAttr;
	// открываем файл
	HANDLE hFile = CreateFile( pAttr->wsFilePath, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (INVALID_HANDLE_VALUE == hFile)
		return AVS_OFFICEPDFWRITER_ERROR_FILE_OPEN_ERROR;

	// мапим этот файл в память - так быстрее читаются данные из файла
	DWORD  nFileSize = GetFileSize(hFile, NULL);
	HANDLE hMapFile  = CreateFileMapping(hFile, NULL, PAGE_READONLY, 0, nFileSize, NULL);
	if (NULL == hMapFile)
	{
		CloseHandle( hFile );
		return AVS_OFFICEPDFWRITER_ERROR_FILE_OPEN_ERROR;
	}

	// создаем view of file
	DWORD nMaxBytesRead	= nFileSize;
	VOID* pBaseAddress	= MapViewOfFile( hMapFile, FILE_MAP_READ, 0, 0, 0 );
	if ( !pBaseAddress )
	{
		CloseHandle( hMapFile );
		CloseHandle( hFile );
		return AVS_OFFICEPDFWRITER_ERROR_FILE_OPEN_ERROR;
	}

	BYTE* pnFile = (BYTE*)pBaseAddress;

	unsigned int nOffset = pAttr->oGlyphTable.pnOffsets[nGid];
	unsigned long nRet = OK;

	if ( 0 == pAttr->oHeader.nIndexToLocFormat )
		nOffset *= 2;

	nOffset += pAttr->oGlyphTable.nBaseOffset;

	pnFile += nOffset;

	short nNumOfContorous;
	unsigned short nFlags = 0;
	unsigned short nGlyphIndex = 0;
	const unsigned short ARG_1_AND_2_ARE_WORDS = 1;
	const unsigned short WE_HAVE_A_SCALE  = 8;
	const unsigned short MORE_COMPONENTS = 32;
	const unsigned short WE_HAVE_AN_X_AND_Y_SCALE = 64;
	const unsigned short WE_HAVE_A_TWO_BY_TWO = 128;

	nNumOfContorous = ReadShort( pnFile ); pnFile += 2;

	if ( -1 != nNumOfContorous )
		return OK;

	short nMinX, nMinY, nMaxX, nMaxY;

	nMinX = ReadShort( pnFile ); pnFile += 2;
	nMinY = ReadShort( pnFile ); pnFile += 2;
	nMaxX = ReadShort( pnFile ); pnFile += 2;
	nMaxY = ReadShort( pnFile ); pnFile += 2;

	//pnFile += 8;

	do 
	{
		nFlags      = ReadUShort( pnFile ); pnFile += 2;
		nGlyphIndex = ReadUShort( pnFile ); pnFile += 2;

		if ( nFlags & ARG_1_AND_2_ARE_WORDS ) 
		{
			pnFile += 4;
		}
		else 
		{
			pnFile += 2;
		}

		if ( nFlags & WE_HAVE_A_SCALE ) 
		{
			pnFile += 2;
		} 
		else if ( nFlags & WE_HAVE_AN_X_AND_Y_SCALE) 
		{
			pnFile += 4;
		} 
		else if ( nFlags & WE_HAVE_A_TWO_BY_TWO) 
		{
			pnFile += 8;
		}

		if ( nGlyphIndex > 0 && nGlyphIndex < pAttr->nNumGlyphs )
		{
			int k = 10;
			//pAttr->oGlyphTable.pFlags[nGlyphIndex] = 1;
		}

	} while ( nFlags & MORE_COMPONENTS );

	UnmapViewOfFile(pBaseAddress);
	CloseHandle(hMapFile);
	CloseHandle( hFile );
	return OK;
}
static unsigned long RecreateGLYF       (FontDef pFontDef, unsigned int *pnNewOffsets, Stream pStream)
{
    unsigned int nSaveOffset = 0;
	unsigned int nStartOffset = pStream->nSize;
    TTFontDefAttr pAttr = (TTFontDefAttr)pFontDef->pAttr;
    unsigned long nRet = OK;

	for ( int nIndex = 0; nIndex < pAttr->nNumGlyphs; nIndex++) 
	{
        BYTE buf[STREAM_BUF_SIZ];

		if ( 1 == pAttr->oGlyphTable.pFlags[nIndex] || 0 == pAttr->oGlyphTable.pFlags[nIndex] ) 
		{
			unsigned int nOffset = pAttr->oGlyphTable.pnOffsets[nIndex];
            unsigned int nLen = pAttr->oGlyphTable.pnOffsets[nIndex + 1] - nOffset;

			pnNewOffsets[nIndex] = pStream->nSize - nStartOffset;
			if ( 0 == pAttr->oHeader.nIndexToLocFormat ) 
			{
                pnNewOffsets[nIndex] /= 2;
                nLen *= 2;
				nOffset *= 2;
            }

			nOffset += pAttr->oGlyphTable.nBaseOffset;

			if ( OK != ( nRet = StreamSeek( pAttr->pStream, nOffset, SeekSet ) ) )
                return nRet;

            while ( nLen > 0 ) 
			{
                unsigned int nTempLen = (nLen > STREAM_BUF_SIZ) ? STREAM_BUF_SIZ : nLen;

                UtilsMemSet( buf, 0, nTempLen );

				if ( OK != ( nRet = StreamRead( pAttr->pStream, buf, &nTempLen ) ) )
                    return nRet;

                if ( OK != ( nRet = StreamWrite( pStream, buf, nTempLen ) ) )
                    return nRet;

                nLen -= nTempLen;
            }

			nSaveOffset = pStream->nSize - nStartOffset;
			if ( 0 == pAttr->oHeader.nIndexToLocFormat )
                nSaveOffset /= 2;
        } 
		else 
            pnNewOffsets[nIndex] = nSaveOffset;
    }

	pnNewOffsets[ pAttr->nNumGlyphs ] = nSaveOffset;

    return OK;
}
static unsigned long RecreateName       (FontDef pFontDef, Stream pStream)
{
    TTFontDefAttr pAttr = (TTFontDefAttr)pFontDef->pAttr;
    TTFTable *pTable = FindTable( pFontDef, "name");
    unsigned long nRet = OK;

	Stream pTempStream = MemStreamNew( pFontDef->oMMgr, STREAM_BUF_SIZ);

    if ( !pTempStream )
		return ErrorGetCode( pFontDef->oError );

	nRet += WriteUINT16( pStream, pAttr->oNameTable.nFormat );
	nRet += WriteUINT16( pStream, pAttr->oNameTable.nCount );
	nRet += WriteUINT16( pStream, pAttr->oNameTable.nStringOffset );

    if ( OK != nRet ) 
	{
        StreamFree( pTempStream );
		return ErrorGetCode( pFontDef->oError );
    }

	TTFNameRecord *pNameRec = pAttr->oNameTable.pNameRecords;
	for ( unsigned int nIndex = 0; nIndex < pAttr->oNameTable.nCount; nIndex++) 
	{
		unsigned int nNameLen = pNameRec->nLength;
        BYTE buf[STREAM_BUF_SIZ];
        unsigned int nTempLen = nNameLen;
		unsigned int nOffset  = pTable->nOffset + pAttr->oNameTable.nStringOffset + pNameRec->nOffset;
		unsigned int nRecOffset = pTempStream->nSize;

        // добавляем суффикс к названию фонта
		if ( 1 == pNameRec->nNameID || 4 == pNameRec->nNameID ) 
		{
			if ( 0 == pNameRec->nPlatformID || 3 == pNameRec->nPlatformID ) 
			{
				nRet += StreamWrite( pTempStream, (BYTE*)pAttr->sTagName2, sizeof(pAttr->sTagName2) );
				nNameLen += sizeof(pAttr->sTagName2);
            } 
			else 
			{
				nRet += StreamWrite( pTempStream, (BYTE*)pAttr->sTagName, sizeof(pAttr->sTagName) );
				nNameLen += sizeof(pAttr->sTagName);
            }
        }

		nRet += WriteUINT16( pStream, pNameRec->nPlatformID );
		nRet += WriteUINT16( pStream, pNameRec->nEncodingID );
		nRet += WriteUINT16( pStream, pNameRec->nLanguageID );
		nRet += WriteUINT16( pStream, pNameRec->nNameID );
        nRet += WriteUINT16( pStream, (unsigned short)nNameLen );
        nRet += WriteUINT16( pStream, (unsigned short)nRecOffset );

		nRet += StreamSeek( pAttr->pStream, nOffset, SeekSet );

        if ( OK != nRet ) 
		{
            StreamFree( pTempStream );
			return ErrorGetCode( pFontDef->oError );
        }

        while ( nTempLen > 0) 
		{
            unsigned int nLen = (nTempLen > STREAM_BUF_SIZ) ? STREAM_BUF_SIZ : nTempLen;

			if ( OK != ( nRet = StreamRead( pAttr->pStream, buf, &nLen ) ) ) 
			{
                StreamFree( pTempStream );
                return nRet;
            }

            if ( OK != ( nRet = StreamWrite( pTempStream, buf, nLen ) ) ) 
			{
                StreamFree( pTempStream );
                return nRet;
            }

            nTempLen -= nLen;
        }
        pNameRec++;
    }

    nRet = StreamWriteToStream( pTempStream, pStream, STREAM_FILTER_NONE, NULL);

    StreamFree( pTempStream );

    return nRet;
}
static unsigned long RecreateCMap       (FontDef pFontDef, Stream pStream, PToUnicode pToUnicode)
{
	return OK;
	//TTFontDefAttr pAttr = (TTFontDefAttr)pFontDef->pAttr;
	//TTFTable *pTable = FindTable( pFontDef, "cmap");
	//unsigned long nRet = OK;

	//int nStartCmapPos = StreamTell( pStream );

	//Stream pTempStream = MemStreamNew( pFontDef->oMMgr, STREAM_BUF_SIZ );
	//if ( !pTempStream )
	//	return ErrorGetCode( pFontDef->oError );

	//// Пишем заголовок
	//int nVersion = 0, nCMapCount = 1;
	//nRet += WriteUINT16( pStream, nVersion ); 
	//nRet += WriteUINT16( pStream, nCMapCount );

	//// Записи о кодировках
	//unsigned short nPlatformID = 1;
	//unsigned short nEncodingID = 0; 
	//unsigned short nFormat     = 6;
	//unsigned int   nOffset     = 12;

	//nRet += WriteUINT16( pStream, nPlatformID );
	//nRet += WriteUINT16( pStream, nEncodingID );
	//nRet += WriteUINT32( pStream, nOffset );

	//// Format - 6
	//CMapEncoderAttr pEncoderAttr = (CMapEncoderAttr)pEncoder->pAttr;
	//unsigned short nFirstCode = 1, nEntryCount = ( pEncoderAttr->nLastChar - pEncoderAttr->nFirstChar + 1 ) - 1; // нулевой символ мы не считаем
	//unsigned short nSubTableLen = 10 + 2 * nEntryCount; // 10 + 255 кодов (без нулевого)

	//nRet += WriteUINT16( pStream, nFormat );
	//nRet += WriteUINT16( pStream, nSubTableLen );
	//nRet += WriteUINT16( pStream, pAttr->oCMap.nLanguage );
	//nRet += WriteUINT16( pStream, nFirstCode );
	//nRet += WriteUINT16( pStream, nEntryCount );


	//// Теперь пишем сам anGlyphIdArray[]
	//for ( int nCode = 1; nCode <= nEntryCount; nCode++ )
	//{
	//	unsigned short unUnicode = pEncoder->pToUnicodeFn( pEncoder, nCode );
	//	nRet += WriteUINT16( pStream, nCode );
	//}

	//StreamFree( pTempStream );

	//return nRet;
}
static unsigned long RecreateFPGM       (FontDef pFontDef, Stream pStream) 
{
	// Ничего не делаем
}
static unsigned long WriteHeader        (FontDef pFontDef, Stream pStream, unsigned int *pnCheckSum)
{
	TTFontDefAttr pAttr = (TTFontDefAttr)pFontDef->pAttr;
	unsigned long nRet = OK;

	nRet += StreamWrite( pStream, pAttr->oHeader.anVersionNumber, 4 );
	nRet += WriteUINT32( pStream, pAttr->oHeader.nFontRevision );

	// нужно сохранить адрес nCheckSumAdjustment. Потому что данная величина
	// будет пересчитываться позже.
	*pnCheckSum = pStream->nSize;

	nRet += WriteUINT32( pStream, 0);
	nRet += WriteUINT32( pStream, pAttr->oHeader.nMagicNumber );
	nRet += WriteUINT16( pStream, pAttr->oHeader.nFlags );
	nRet += WriteUINT16( pStream, pAttr->oHeader.nUnitsPerEm );
	nRet += StreamWrite( pStream, pAttr->oHeader.anCreated, 8);
	nRet += StreamWrite( pStream, pAttr->oHeader.anModified, 8);
	nRet += WriteINT16( pStream, pAttr->oHeader.nXmin );
	nRet += WriteINT16( pStream, pAttr->oHeader.nYmin );
	nRet += WriteINT16( pStream, pAttr->oHeader.nXmax );
	nRet += WriteINT16( pStream, pAttr->oHeader.nYmax );
	nRet += WriteUINT16( pStream, pAttr->oHeader.nMacStyle );
	nRet += WriteUINT16( pStream, pAttr->oHeader.nLowestRecPPEM );
	nRet += WriteINT16( pStream, pAttr->oHeader.nFontDirectionHint );
	nRet += WriteINT16( pStream, pAttr->oHeader.nIndexToLocFormat );
	nRet += WriteINT16( pStream, pAttr->oHeader.nGlyphDataFormat );

	if ( OK != nRet )
		return ErrorGetCode( pFontDef->oError );

	return OK;
}
//----- Основные функции ----------------------------------------------------------------------------

FontDef        TTFontDefNew         (MMgr oMMgr)
{
    if ( !oMMgr )
        return NULL;
    
	FontDef pFontDef = (FontDef)GetMem( oMMgr, sizeof(FontDefRec) );

    if ( !pFontDef )
        return NULL;

    UtilsMemSet( pFontDef, 0, sizeof(FontDefRec) );
	pFontDef->nSigBytes = FONTDEF_SIG_BYTES;
	pFontDef->oMMgr     = oMMgr;
	pFontDef->oError    = oMMgr->oError;
    pFontDef->eType     = FontDefTrueType;
	pFontDef->pCleanFn  = NULL;
	pFontDef->pFreeFn   = TTFFreeFunc;

    TTFontDefAttr pFontDefAttr = (TTFontDefAttr)GetMem( oMMgr, sizeof(TTFontDefAttrRec) );
    if ( !pFontDefAttr ) 
	{
		FreeMem( pFontDef->oMMgr, pFontDef );
        return NULL;
    }

	pFontDef->pAttr = pFontDefAttr;
    UtilsMemSet( (BYTE *)pFontDefAttr, 0, sizeof(TTFontDefAttrRec) );
	pFontDef->nFlags = FONT_STD_CHARSET;

    return pFontDef;
}
FontDef        TTFontDefLoad        (MMgr oMMgr, const wchar_t *wsFilePath, BOOL bEmbedding, const char *sEncodingName = NULL, BOOL bEncoding = FALSE, BOOL bSymbolic = FALSE, BOOL bUsesMacRomanEncoding = FALSE)
{
    unsigned long nRet = OK;
    FontDef pFontDef = TTFontDefNew( oMMgr );

	if ( !pFontDef ) 
        return NULL;

	// Специальные настройки для точного определения кодировки cmap
	TTFontDefAttr pAttr = (TTFontDefAttr)pFontDef->pAttr;
	pAttr->bEncoding             = bEncoding;
	pAttr->bSymbolic             = bSymbolic;
	pAttr->bUsesMacRomanEncoding = bUsesMacRomanEncoding;	
	wcscpy( pAttr->wsFilePath, wsFilePath );
	pAttr->unFontIndex           = 0;

    nRet = LoadFontData( pFontDef, wsFilePath, bEmbedding, sEncodingName );
    if ( OK != nRet ) 
	{
        FontDefFree( pFontDef );
        return NULL;
    }

    return pFontDef;
}
FontDef        TTFontDefLoad2       (MMgr oMMgr, const wchar_t *wsFilePath, unsigned int nIndex, BOOL bEmbedding, const char *sEncodingName = NULL, BOOL bEncoding = FALSE, BOOL bSymbolic = FALSE, BOOL bUsesMacRomanEncoding = FALSE)
{
    unsigned long nRet = OK;
    FontDef pFontDef = TTFontDefNew( oMMgr );

    if ( !pFontDef ) 
        return NULL;

	// Специальные настройки для точного определения кодировки cmap
	TTFontDefAttr pAttr = (TTFontDefAttr)pFontDef->pAttr;
	pAttr->bEncoding             = bEncoding;
	pAttr->bSymbolic             = bSymbolic;
	pAttr->bUsesMacRomanEncoding = bUsesMacRomanEncoding;
	wcscpy( pAttr->wsFilePath, wsFilePath );
	pAttr->unFontIndex           = nIndex;

    nRet = LoadFontData2( pFontDef, wsFilePath, nIndex, bEmbedding, sEncodingName );
    if ( OK != nRet ) 
	{
        FontDefFree( pFontDef );
        return NULL;
    }

    return pFontDef;
}
unsigned short TTFontDefGetGlyphID  (FontDef pFontDef, unsigned short nUnicode)
{
	TTFontDefAttr pAttr = (TTFontDefAttr)pFontDef->pAttr;

	FT_Library pLibrary = NULL;
	if ( FT_Init_FreeType( &pLibrary ) )
		return 0;

	USES_CONVERSION;

	FT_Face pFace = NULL;

	char *sPath = W2A( pAttr->wsFilePath );
	if ( FT_New_Face( pLibrary, sPath, pAttr->unFontIndex, &pFace  ) )
	{
		FT_Done_FreeType( pLibrary );
		return 0;
	}

	unsigned short unGID = GetGID( pFace, nUnicode );	

	FT_Done_Face( pFace );
	FT_Done_FreeType( pLibrary );

	return unGID;
}
Box            TTFontDefGetCharBBox (FontDef pFontDef, unsigned short nUnicode)
{
    TTFontDefAttr pAttr = (TTFontDefAttr)pFontDef->pAttr;

	for ( unsigned int unIndex = 0; unIndex < pAttr->oCmap.unCount; unIndex++ )
	{
		if ( pAttr->oCmap.pPairs[unIndex].ushCharCode == nUnicode )
		{
			return ToBox( pAttr->oCmap.pPairs[unIndex].oBBox.fLeft, pAttr->oCmap.pPairs[unIndex].oBBox.fBottom, pAttr->oCmap.pPairs[unIndex].oBBox.fRight, pAttr->oCmap.pPairs[unIndex].oBBox.fTop );
		}
	}

	return ToBox( (short)0, (short)0, (short)0, (short)0);
}
short          TTFontDefGetCharWidth(FontDef pFontDef, unsigned short nUnicode)
{
    TTFontDefAttr pAttr = (TTFontDefAttr)pFontDef->pAttr;
	short shWidth = pFontDef->nAvgWidth;

	FT_Library pLibrary = NULL;
	if ( FT_Init_FreeType( &pLibrary ) )
		return shWidth;

	USES_CONVERSION;

	FT_Face pFace = NULL;

	char *sPath = W2A( pAttr->wsFilePath );
	if ( FT_New_Face( pLibrary, sPath, pAttr->unFontIndex, &pFace  ) )
	{
		FT_Done_FreeType( pLibrary );
		return shWidth;
	}

	unsigned short unGID = GetGID( pFace, nUnicode );
	
	if ( !FT_Load_Glyph( pFace, unGID, FT_LOAD_NO_SCALE ) )
	{
		if ( 0 != pFace->units_per_EM )
			shWidth = (unsigned int)pFace->glyph->metrics.horiAdvance * 1000 / pFace->units_per_EM;
		else
			shWidth = (unsigned int)pFace->glyph->metrics.horiAdvance;
	}

	FT_Done_Face( pFace );
	FT_Done_FreeType( pLibrary );

	return shWidth;
}
short          TTFontDefGetGidWidth (FontDef pFontDef, unsigned short nGid)
{
    TTFontDefAttr pAttr = (TTFontDefAttr)pFontDef->pAttr;
	short shWidth = pFontDef->nAvgWidth;

	FT_Library pLibrary = NULL;
	if ( FT_Init_FreeType( &pLibrary ) )
		return shWidth;

	USES_CONVERSION;

	FT_Face pFace = NULL;

	char *sPath = W2A( pAttr->wsFilePath );
	if ( FT_New_Face( pLibrary, sPath, pAttr->unFontIndex, &pFace  ) )
	{
		FT_Done_FreeType( pLibrary );
		return shWidth;
	}
	
	if ( !FT_Load_Glyph( pFace, nGid, FT_LOAD_NO_SCALE ) )
	{
		if ( 0 != pFace->units_per_EM )
			shWidth = (unsigned int)pFace->glyph->metrics.horiAdvance * 1000 / pFace->units_per_EM;
		else
			shWidth = (unsigned int)pFace->glyph->metrics.horiAdvance;
	}

	FT_Done_Face( pFace );
	FT_Done_FreeType( pLibrary );

	return shWidth;
}
unsigned long  TTFontDefSaveFontData(FontDef pFontDef, Stream pStream, FontAttr *ppFontAttr = NULL, PToUnicode pToUnicode = NULL)
{
	TTFontDefAttr pAttr = (TTFontDefAttr)pFontDef->pAttr;

	CFontFileTrueType *pTTF = CFontFileTrueType::LoadFromFile( pAttr->wsFilePath );
	if ( pTTF )
	{
		if ( NULL != pToUnicode && ToUnicodeBuilt == pToUnicode->eType )
		{
			FT_Library pLibrary = NULL;
			FT_Init_FreeType( &pLibrary );

			USES_CONVERSION;

			FT_Face pFace = NULL;

			char *sPath = W2A( pAttr->wsFilePath );
			FT_New_Face( pLibrary, sPath, pAttr->unFontIndex, &pFace  );

			int nSymbolic = GetSymbolicCmapIndex( pFace );

			unsigned short arrCmap[256];
			
			if ( pAttr->bCFF )
			{
				Encoder pEncoder = BasicEncoderNew( pFontDef->oMMgr, ENCODING_FONT_SPECIFIC );
				BasicEncoderAttr pEncoderAttr = (BasicEncoderAttr)pEncoder->pAttr;
				pEncoderAttr->bHasDifferences = TRUE;
				pEncoderAttr->nFirstChar = ((ToUnicodeBuiltAttr)pToUnicode->pAttr)->nFirstChar;
				pEncoderAttr->nLastChar  = ((ToUnicodeBuiltAttr)pToUnicode->pAttr)->nLastChar;
			
				for ( int nIndex = 0; nIndex < 256; nIndex++ )
				{
					arrCmap[nIndex] = 0;
					unsigned short ushUnicode = pToUnicode->pToUnicodeFn( pToUnicode, nIndex );
					arrCmap[nIndex] = GetGID( pFace, ushUnicode );
					if ( arrCmap[nIndex] <= 0 && -1 != nSymbolic  )
						arrCmap[nIndex] = GetGID( pFace, ushUnicode + 0xF000 );

					char *sName = GetName( pFace, arrCmap[nIndex] );
					pEncoderAttr->anDifferences[nIndex] = 1;
					pEncoderAttr->anUnicodeMap[nIndex]  = GlyphNameToUnicode( sName ); 
					strcpy( (char *)pEncoderAttr->asGlyphNameMap[nIndex], sName );

					free( sName );
				}

				(*ppFontAttr)->pEncoder     = pEncoder;
				(*ppFontAttr)->bFreeEncoder = TRUE;
			}
			else
			{
				for ( int nIndex = 0; nIndex < 256; nIndex++ )
				{
					arrCmap[nIndex] = 0;
					unsigned short ushUnicode = pToUnicode->pToUnicodeFn( pToUnicode, nIndex );
					arrCmap[nIndex] = GetGID( pFace, ushUnicode );
					if ( arrCmap[nIndex] <= 0 && -1 != nSymbolic  )
						arrCmap[nIndex] = GetGID( pFace, ushUnicode + 0xF000 );
				}
			}

			// Создаем список используемых глифов, чтобы вшить шрифт только с данными символами
			long lGlyfsCount = pFace->num_glyphs;
			unsigned char *pUseGlyf = new unsigned char[lGlyfsCount];
			if ( pUseGlyf )
			{
				memset( (void *)pUseGlyf, 0x00, pFace->num_glyphs * sizeof(unsigned char) );
				for ( int nIndex = 0; nIndex < 256; nIndex++ )
				{
					if (arrCmap[nIndex] >= lGlyfsCount)
						continue;

					// Данный символ используется
					pUseGlyf[arrCmap[nIndex]] = 1;

					// Если данный символ составной (CompositeGlyf), тогда мы должны учесть все его дочерные символы (subglyfs)
					if ( 0 == FT_Load_Glyph( pFace, arrCmap[nIndex], FT_LOAD_NO_SCALE | FT_LOAD_NO_RECURSE ) )
					{
						for ( int nSubIndex = 0; nSubIndex < pFace->glyph->num_subglyphs; nSubIndex++ )
						{
							FT_Int       nSubGID;
							FT_UInt      unFlags;
							FT_Int       nArg1;
							FT_Int       nArg2;
							FT_Matrix    oMatrix;
							FT_Get_SubGlyph_Info( pFace->glyph, nSubIndex, &nSubGID, &unFlags, &nArg1, &nArg2, &oMatrix );

							pUseGlyf[nSubGID] = 1;
						}
					}
				}

				FT_Done_Face( pFace );
				FT_Done_FreeType( pLibrary );

				pTTF->WriteTTF( &FileWrite, pStream, NULL, arrCmap, pUseGlyf, lGlyfsCount );

				delete pUseGlyf;
			}
			else
			{
				FT_Done_Face( pFace );
				FT_Done_FreeType( pLibrary );

				pTTF->WriteTTF( &FileWrite, pStream, NULL, arrCmap );
			}
		}
		else
		{
			pTTF->WriteTTF( &FileWrite, pStream );
		}

		pAttr->nLength1 = pStream->nSize;

		delete pTTF;

		return OK;
	}

	return OK;
}
void           TTFontDefSetTagName  (FontDef pFontDef, char *sTag)
{
	TTFontDefAttr pAttr = (TTFontDefAttr)pFontDef->pAttr;
    char sBuf[LIMIT_MAX_NAME_LEN + 1];

    if ( TTF_FONT_TAG_LEN != UtilsStrLen( sTag, LIMIT_MAX_NAME_LEN ) )
        return;

	UtilsMemCpy( (BYTE*)pAttr->sTagName, (BYTE*)sTag, TTF_FONT_TAG_LEN );
	pAttr->sTagName[TTF_FONT_TAG_LEN] = '+';

    for ( unsigned int nIndex = 0; nIndex < TTF_FONT_TAG_LEN + 1; nIndex++) 
	{
		pAttr->sTagName2[nIndex * 2] = 0x00;
		pAttr->sTagName2[nIndex * 2 + 1] = pAttr->sTagName[nIndex];
    }

    UtilsMemSet( sBuf, 0, LIMIT_MAX_NAME_LEN + 1);
	UtilsMemCpy( (BYTE*)sBuf, (BYTE*)pAttr->sTagName, TTF_FONT_TAG_LEN + 1);
	UtilsMemCpy( (BYTE*)sBuf + TTF_FONT_TAG_LEN + 1, (BYTE*)pFontDef->sBaseFont, LIMIT_MAX_NAME_LEN - TTF_FONT_TAG_LEN - 1);
    UtilsMemCpy( (BYTE*)pAttr->sBaseFont, (BYTE*)sBuf, LIMIT_MAX_NAME_LEN + 1);
}

//----- Основные функции ----------------------------------------------------------------------------

FontDict TTFontNew  (MMgr oMMgr, FontDef pFontDef, Encoder pEncoder, Xref pXref, PToUnicode pToUnicode = NULL)
{
	unsigned long nRet = OK;
	Dict pFont = DictNew( oMMgr );
	if ( !pFont )
		return NULL;

	pFont->pHeader.nObjClass |= OSUBCLASS_FONT;

	// проверяем типы объектов pFontDef и pEncoder
	if ( EncoderTypeSingleByte != pEncoder->eType && EncoderTypeToUnicode != pEncoder->eType ) 
	{
		SetError( pFont->oError, AVS_OFFICEPDFWRITER_ERROR_INVALID_ENCODER_TYPE, 0);
		return NULL;
	}

	if ( FontDefTrueType != pFontDef->eType ) 
	{
		SetError( pFont->oError, AVS_OFFICEPDFWRITER_ERROR_INVALID_FONTDEF_TYPE, 0);
		return NULL;
	}

	FontAttr pAttr = (FontAttr)GetMem( oMMgr, sizeof(FontAttrRec) );

	if ( !pAttr ) 
	{
		DictFree( pFont );
		return NULL;
	}

	UtilsMemSet( pAttr, 0, sizeof(FontAttrRec) );

	pFont->pHeader.nObjClass |= OSUBCLASS_FONT;
	pFont->pWriteFn       = TTFOnWrite;
	pFont->pBeforeWriteFn = TTFBeforeWrite;
	pFont->pFreeFn        = TTFOnFree;
	pFont->pAttr          = pAttr;

	pAttr->eType          = FontTrueType;
	pAttr->eWritingMode   = WModeHorizontal;
	pAttr->pTextWidthFn   = GetTextWidth;
	pAttr->pMeasureTextFn = MeasureText;
	pAttr->pFontDef       = pFontDef;
    pAttr->pEncoder       = pEncoder;
	pAttr->pToUnicode     = pToUnicode;
    pAttr->pXref          = pXref;

	pAttr->bFreeEncoder   = FALSE;

	pAttr->oExternWidths.bUse       = false;
	pAttr->oExternWidths.nLastChar  = 0;
	pAttr->oExternWidths.nFirstChar = 0;
	pAttr->oExternWidths.anWidths   = NULL;
	// SingleByte-Font содержит массив ширин, как массив состоящий из 256
	// чисел типа short. 
	// В случает Type1-Font, этот массив ширин для всех букв создается в 
	// конструкторе. А в случае TrueType-Font, массив инициализируется
	// нулем, и заполняетя когда соответствующий символ используется в
	// первый раз.

	pAttr->anWidths = (short*)GetMem( oMMgr, sizeof(short) * 256);

	if ( !pAttr->anWidths ) 
	{
        DictFree( pFont );
        return NULL;
    }

	UtilsMemSet( pAttr->anWidths, 0, sizeof(short) * 256);

	pAttr->pUsed = (BYTE*)GetMem( oMMgr, sizeof(BYTE) * 256);

	if ( !pAttr->pUsed ) 
	{
        DictFree( pFont );
        return NULL;
    }

	UtilsMemSet( pAttr->pUsed, 0, sizeof(BYTE) * 256);

    TTFontDefAttr pFontDefAttr = (TTFontDefAttr)pFontDef->pAttr;

    nRet += DictAddName( pFont, "Type", "Font" );
    nRet += DictAddName( pFont, "BaseFont", pFontDefAttr->sBaseFont );
	if ( !pFontDefAttr->bCFF )
	{
		nRet += DictAddName( pFont, "Subtype", "TrueType" );
	}
	else
	{
		nRet += DictAddName( pFont, "Subtype", "Type1" );
	}

	if ( pToUnicode  )
	{
		UtilsMemCpy( (BYTE *)pFontDefAttr->sToUnicodeName, (BYTE *)pToUnicode->sName, LIMIT_MAX_NAME_LEN + 1 );
	}

	//if ( pToUnicode && ToUnicodeBuilt == pToUnicode->eType )
	//{
	//	ToUnicodeBuiltAttr pToUnicodeAttr = (ToUnicodeBuiltAttr)pToUnicode->pAttr;

	//	nRet += DictAddNumber( pFont, "FirstChar", pToUnicodeAttr->nFirstChar );
	//	nRet += DictAddNumber( pFont, "LastChar",  pToUnicodeAttr->nLastChar  );
	//}
	//else //if ( EncoderTypeSingleByte != pEncoder->eType )
	//{
	//	BasicEncoderAttr pEncoderAttr = (BasicEncoderAttr)pEncoder->pAttr;

	//	nRet += DictAddNumber( pFont, "FirstChar", pEncoderAttr->nFirstChar );
	//	nRet += DictAddNumber( pFont, "LastChar", pEncoderAttr->nLastChar);
	//}
	//else
	//{
	//	CMapEncoderAttr pEncoderAttr = (CMapEncoderAttr)pEncoder->pAttr;

	//	nRet += DictAddNumber( pFont, "FirstChar", pEncoderAttr->nFirstChar );
	//	nRet += DictAddNumber( pFont, "LastChar", pEncoderAttr->nLastChar);

	//}

	if ( 0 != pFontDef->nMissingWidth )
		nRet += DictAddNumber( pFont, "MissingWidth", pFontDef->nMissingWidth );

    if ( OK != nRet ) 
	{
        DictFree( pFont );
        return NULL;
    }

    if ( OK != XrefAdd( pXref, pFont) )
        return NULL;

    return pFont;
}
static unsigned long CreateDescriptor  (FontDict pFont)
{
	FontAttr pFontAttr = (FontAttr)pFont->pAttr;
	FontDef pDef = pFontAttr->pFontDef;
	TTFontDefAttr pDefAttr = (TTFontDefAttr)pDef->pAttr;
	
	if ( !pFontAttr->pFontDef->pDescriptor ) 
	{
		Dict pDescriptor = DictNew( pFont->oMMgr );
        unsigned long nRet = OK;
        
        if ( !pDescriptor )
			return ErrorGetCode( pFont->oError );

		nRet += XrefAdd( pFontAttr->pXref, pDescriptor );
        nRet += DictAddName( pDescriptor, "Type", "FontDescriptor");
		nRet += DictAddNumber( pDescriptor, "Ascent", pDef->nAscent );
		nRet += DictAddNumber( pDescriptor, "Descent", pDef->nDescent );

		// Выставляем бит Symbolic, а бит NonSymbolic убираем
		unsigned int nFlags = pDef->nFlags;
		if ( pFontAttr->pToUnicode && ToUnicodeBuilt == pFontAttr->pToUnicode->eType )
		{
			if ( !( nFlags & 4 ) )
				UIntChangeBit( &nFlags, 2 );
			if ( nFlags & 32 )
				UIntChangeBit( &nFlags, 5 );
		}

		nRet += DictAddNumber( pDescriptor, "Flags", nFlags );

		Array oArray = BoxArrayNew( pFont->oMMgr, pDef->oFontBox );

		nRet += DictAdd( pDescriptor, "FontBBox", oArray );
        nRet += DictAddName( pDescriptor, "FontName", pDefAttr->sBaseFont );
		nRet += DictAddNumber( pDescriptor, "ItalicAngle", pDef->nItalicAngle );
		nRet += DictAddNumber( pDescriptor, "StemV", pDef->nStemV );
		nRet += DictAddNumber( pDescriptor, "XHeight", pDef->nXHeight );

        if ( pDefAttr->pnCharSet )
            nRet += DictAddName( pDescriptor, "CharSet", pDefAttr->pnCharSet);

        if ( OK != nRet )
			return ErrorGetCode( pFont->oError );

        if ( pDefAttr->bEmbedding ) 
		{
			Dict pFontData = DictStreamNew( pFont->oMMgr, pFontAttr->pXref );

            if ( !pFontData )
				return ErrorGetCode( pFont->oError );

			if ( OK != TTFontDefSaveFontData( pFontAttr->pFontDef, pFontData->pStream, &pFontAttr, pFontAttr->pToUnicode ) )
				return ErrorGetCode( pFont->oError );

			if ( !pDefAttr->bCFF )
			{
				nRet += DictAdd( pDescriptor, "FontFile2", pFontData );
				nRet += DictAddNumber( pFontData, "Length1", pDefAttr->nLength1);
				nRet += DictAddNumber( pFontData, "Length2", 0);
				nRet += DictAddNumber( pFontData, "Length3", 0);
			}
			else
			{
				nRet += DictAdd( pDescriptor, "FontFile3", pFontData );
				nRet += DictAddName( pFontData, "Subtype", "OpenType" );
			}

#ifndef _DEBUG
			pFontData->nFilter = STREAM_FILTER_FLATE_DECODE;
#endif
        }

        if ( OK != nRet )
			return ErrorGetCode( pFont->oError );

		pFontAttr->pFontDef->pDescriptor = pDescriptor;
    }

	return DictAdd( pFont, "FontDescriptor", pFontAttr->pFontDef->pDescriptor );
}


static unsigned long CreateToUnicode   (FontDict pFont)
{
	FontAttr pFontAttr = (FontAttr)pFont->pAttr;
	DictRemoveElement( pFont, "MissingWidth" );

	if ( NULL != pFontAttr->pToUnicode )
	{
		Dict pToUnicode = DictStreamNew( pFont->oMMgr, pFontAttr->pXref );

		pFontAttr->pToUnicode->pWriteFn( pFontAttr->pToUnicode, pToUnicode->pStream );

#ifndef _DEBUG
		pToUnicode->nFilter = STREAM_FILTER_FLATE_DECODE;
#endif

		pFontAttr->pCMapStream = pToUnicode;

		return DictAdd( pFont, "ToUnicode", pFontAttr->pCMapStream );
	}
	return OK;
}
static unsigned int  CharWidth         (FontDict pFont, BYTE nCode)
{
	FontAttr pAttr = (FontAttr)pFont->pAttr;

	if ( 0 == pAttr->pUsed[ nCode ] ) 
	{
		short shWidth = 0;
		// Ширины заданы внешним образом
		if ( pAttr->oExternWidths.bUse )
		{
			shWidth = pAttr->oExternWidths.anWidths[nCode - pAttr->oExternWidths.nFirstChar];
			TTFontDefGetCharWidth( pAttr->pFontDef, nCode ); // Отмечаем, что символ используется
		}
		else if ( NULL != pAttr->pToUnicode && ToUnicodeBuilt == pAttr->pToUnicode->eType )
		{
			unsigned short ushUnicode = pAttr->pToUnicode->pToUnicodeFn( pAttr->pToUnicode, nCode );
			shWidth = TTFontDefGetCharWidth( pAttr->pFontDef, ushUnicode );
		}
		else
		{
			unsigned short ushUnicode = EncoderToUnicode( pAttr->pEncoder, nCode );
			shWidth = TTFontDefGetCharWidth( pAttr->pFontDef, ushUnicode );
		}

		pAttr->pUsed[ nCode ]    = 1;
		pAttr->anWidths[ nCode ] = shWidth;
    }

	return pAttr->anWidths[ nCode ];
}
static TextWidth     GetTextWidth      (FontDict pFont, const BYTE *sText, unsigned int nLen, unsigned int *pCIDs, unsigned int nLenCID)
{
	FontAttr pAttr = (FontAttr)pFont->pAttr;
    TextWidth oRet = {0, 0, 0, 0};

	BYTE nByte = 0;

	if ( pAttr->anWidths ) 
	{
        for ( unsigned int nIndex = 0; nIndex < nLen; nIndex++) 
		{
            nByte = sText[nIndex];
            oRet.nNumchars++;
            oRet.nWidth += CharWidth( pFont, nByte );

            if ( IS_WHITE_SPACE(nByte) ) 
			{
                oRet.nNumchars++;
                oRet.nNumwords++;
            }
        }
    } 
	else
		SetError( pFont->oError, AVS_OFFICEPDFWRITER_ERROR_FONT_INVALID_WIDTHS_TABLE, 0);

    if ( IS_WHITE_SPACE( nByte ) )
        ; // ничего не делаем
    else
		oRet.nNumwords++;

    return oRet;
}
static unsigned int  MeasureText       (FontDict pFont, const BYTE *sText, unsigned int nLen, unsigned int *pCIDs, unsigned int nLenCID, float fWidth, float fFontSize, float fCharSpace, float fWordSpace, BOOL bWordWrap, float *pfRealWidth)
{
    double dWidth = 0;
    unsigned int nTempLen = 0;

    for ( unsigned int nIndex = 0; nIndex < nLen; nIndex++) 
	{
        BYTE nByte = sText[nIndex];

        if ( IS_WHITE_SPACE(nByte) ) 
		{
            nTempLen = nIndex + 1;

            if ( pfRealWidth)
                *pfRealWidth = dWidth;

            dWidth += fWordSpace;
        } 
		else if ( !bWordWrap ) 
		{
            nTempLen = nIndex;

            if ( pfRealWidth)
                *pfRealWidth = dWidth;
        }

        dWidth += (double)CharWidth( pFont, nByte ) * fFontSize / 1000;

        if ( dWidth > fWidth ||  0x0A == nByte )
            return nTempLen;

        if ( nIndex > 0 )
            dWidth += fCharSpace;
    }

	if ( pfRealWidth)
		*pfRealWidth = dWidth;
    return nLen;
}
static unsigned long TTFOnWrite        (Dict pObj, Stream pStream)
{
    FontAttr pAttr = (FontAttr)pObj->pAttr;

	if ( true == pAttr->oExternWidths.bUse )
	{
		unsigned long nRet = OK;

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


		char buf[128];
		char *pbuf = buf;
		char *eptr = buf + 127;

		if ( OK != ( nRet = StreamWriteEscapeName( pStream, "Widths" ) ) )
			return nRet;

		if ( OK != ( nRet = StreamWriteStr( pStream, " [\012" ) ) )
			return nRet;

		for ( unsigned int nIndex = 0; nIndex <= pAttr->oExternWidths.nLastChar - pAttr->oExternWidths.nFirstChar; nIndex++) 
		{

			pbuf = UtilsIToA ( pbuf, pAttr->oExternWidths.anWidths[nIndex], eptr);
			*pbuf++ = ' ';

			if ( 0 == (nIndex + 1) % 16 ) 
			{
				UtilsStrCpy(pbuf, "\012", eptr );
				if ( OK != ( nRet = StreamWriteStr( pStream, buf ) ) )
					return nRet;
				pbuf = buf;
			}

		}

		UtilsStrCpy( pbuf, "]\012", eptr );

		if ( OK != ( nRet = StreamWriteStr( pStream, buf ) ) )
			return nRet;

		if ( pAttr->pEncoder )
			nRet += pAttr->pEncoder->pWriteFn( pAttr->pEncoder, pStream );
		return nRet;
	}
	if ( (NULL == pAttr->pToUnicode) || (ToUnicodeBuilt != pAttr->pToUnicode->eType) )
	{
		BasicEncoderAttr pEncoderAttr = (BasicEncoderAttr)pAttr->pEncoder->pAttr;

		unsigned long nRet = OK;

		char sBuffer[TEMP_BUFFER_SIZE];
		char *pEndPointer = sBuffer + TEMP_BUFFER_SIZE - 1;

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

		char buf[128];
		char *pbuf = buf;
		char *eptr = buf + 127;

		if ( OK != ( nRet = StreamWriteEscapeName( pStream, "Widths" ) ) )
			return nRet;

		if ( OK != ( nRet = StreamWriteStr( pStream, " [\012" ) ) )
			return nRet;

		for ( unsigned int nIndex = pEncoderAttr->nFirstChar; nIndex <= pEncoderAttr->nLastChar; nIndex++) 
		{

			pbuf = UtilsIToA ( pbuf, pAttr->anWidths[nIndex], eptr);
			*pbuf++ = ' ';

			if ( 0 == (nIndex + 1) % 16 ) 
			{
				UtilsStrCpy(pbuf, "\012", eptr );
				if ( OK != ( nRet = StreamWriteStr( pStream, buf ) ) )
					return nRet;
				pbuf = buf;
			}

		}

		UtilsStrCpy( pbuf, "]\012", eptr );

		if ( OK != ( nRet = StreamWriteStr( pStream, buf ) ) )
			return nRet;

		return pAttr->pEncoder->pWriteFn( pAttr->pEncoder, pStream );
	}
	else
	{
		unsigned long nRet = OK;

		// Значит кодировка у нас задана в ToUnicode
		FontDef pDef = pAttr->pFontDef;
		ToUnicodeBuiltAttr pToUnicodeAttr = (ToUnicodeBuiltAttr)pAttr->pToUnicode->pAttr;


		char sBuffer[TEMP_BUFFER_SIZE];
		char *pEndPointer = sBuffer + TEMP_BUFFER_SIZE - 1;

		char *pBufPointer;

		pBufPointer = (char*)UtilsStrCpy( sBuffer, "/FirstChar ", pEndPointer);
		pBufPointer = UtilsIToA( pBufPointer, pToUnicodeAttr->nFirstChar, pEndPointer);
		UtilsStrCpy( pBufPointer, "\012", pEndPointer);
		if ( OK != ( nRet = StreamWriteStr( pStream, sBuffer ) ) )
			return nRet;

		pBufPointer = (char*)UtilsStrCpy( sBuffer, "/LastChar ", pEndPointer);
		pBufPointer = UtilsIToA( pBufPointer, pToUnicodeAttr->nLastChar, pEndPointer);
		UtilsStrCpy( pBufPointer, "\012", pEndPointer);
		if ( OK != ( nRet = StreamWriteStr( pStream, sBuffer ) ) )
			return nRet;

		char buf[128];
		char *pbuf = buf;
		char *eptr = buf + 127;

		if ( OK != ( nRet = StreamWriteEscapeName( pStream, "Widths" ) ) )
			return nRet;

		if ( OK != ( nRet = StreamWriteStr( pStream, " [\012" ) ) )
			return nRet;

		int nCodesCount;
		for ( nCodesCount = 0; nCodesCount < 256 && pToUnicodeAttr->anUnicodeMap[nCodesCount][0] != 0xffff; nCodesCount++ );

		for ( unsigned int nIndex = 0; nIndex < nCodesCount; nIndex++ ) 
		{
			unsigned short unUnicode = pAttr->pToUnicode->pToUnicodeFn( pAttr->pToUnicode, nIndex );
			short nCharWidth = 0;

			nCharWidth = TTFontDefGetCharWidth( pDef, unUnicode );

			pbuf = UtilsIToA ( pbuf, nCharWidth, eptr);
			*pbuf++ = ' ';

			if ( 0 == (nIndex + 1) % 16 ) 
			{
				UtilsStrCpy(pbuf, "\012", eptr );
				if ( OK != ( nRet = StreamWriteStr( pStream, buf ) ) )
					return nRet;
				pbuf = buf;
			}

		}

		UtilsStrCpy( pbuf, "]\012", eptr );

		if ( OK != ( nRet = StreamWriteStr( pStream, buf ) ) )
			return nRet;

		if ( NULL != pAttr->pEncoder && NULL != pAttr->pEncoder->pWriteFn )
			pAttr->pEncoder->pWriteFn( pAttr->pEncoder, pStream );
	}
	return OK;
}
static unsigned long TTFBeforeWrite    (Dict pObj)
{
	unsigned long nRet = OK;

	if ( ( nRet = CreateDescriptor( pObj ) ) != OK )
		return nRet;

	if ( ( nRet = CreateToUnicode( pObj ) ) != OK )
		return nRet;

	return OK;
}
static void          TTFOnFree         (Dict pObj)
{
	FontAttr pAttr = (FontAttr)pObj->pAttr;

    if ( pAttr ) 
	{
		if ( pAttr->anWidths ) 
			FreeMem( pObj->oMMgr, pAttr->anWidths );

		if ( pAttr->pUsed ) 
			FreeMem( pObj->oMMgr, pAttr->pUsed );

		if ( pAttr->oExternWidths.anWidths ) 
			FreeMem( pObj->oMMgr, pAttr->oExternWidths.anWidths );

		if ( pAttr->bFreeEncoder )
			EncoderFree( pAttr->pEncoder );

		FreeMem( pObj->oMMgr, pAttr);
    }
}


#endif /* _FONT_TRUE_TYPE_H */
