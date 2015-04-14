#include "Encoder.h"

#include "Utils.h"
#include "Streams.h"
#include "EncoderConstants.h"

//-- Определения для кодировки фонтов ------------------------------------------------------------

unsigned long  EncoderValidate     (EncoderRecPtr pEncoder)
{
	if ( !pEncoder || ENCODER_SIG_BYTES != pEncoder->nSigBytes )
        return false;
    else
        return true;
}
void           EncoderSetParseText (EncoderRecPtr pEncoder, ParseTextRec *pState, const BYTE *pText, unsigned int nLen)
{
	pState->pText     = pText;
	pState->nIndex    = 0;
	pState->nLen      = nLen;
	pState->eByteType = ByteTypeSingle;
}
ByteType       EncoderByteType     (EncoderRecPtr pEncoder, ParseTextRec *pState)
{
    if ( pEncoder->pByteTypeFn )
        return pEncoder->pByteTypeFn( pEncoder, pState );
    else
        return ByteTypeSingle;
}
unsigned short EncoderToUnicode    (EncoderRecPtr pEncoder, unsigned short nCode)
{
	return pEncoder->pToUnicodeFn( pEncoder, nCode );
}
void           EncoderFree         (EncoderRecPtr pEncoder)
{
    if ( !pEncoder )
        return;

    if ( pEncoder->pFreeFn )
        pEncoder->pFreeFn( pEncoder );
    FreeMem( pEncoder->oMMgr, pEncoder );
}
unsigned short EncoderToCode       (EncoderRecPtr pEncoder, unsigned short unUnicode)
{
	// Ищем код для заданного юникодного значения, если такого в данной кодировке нет возвращаем значение 0xFFFF.
	for ( int nCode = 0; nCode < 256; nCode++ )
	{
		unsigned short unCurUnicode = EncoderToUnicode( pEncoder, nCode );
		if ( unCurUnicode == unUnicode )
			return nCode;
	}
	return 0xFFFF;
}

//-- BasicEncoder - Вспомогательные функции ------------------------------------------------------

const BuiltinEncodingData* BasicEncoderFindBuiltinData(const char *sEncodingName )
{
    unsigned int nIndex = 0;

	while ( c_aoBuiltInEncodings[nIndex].sEncodingName ) 
	{
		if ( 0 == UtilsStrCmp( c_aoBuiltInEncodings[nIndex].sEncodingName, sEncodingName ) )
            break;

        nIndex++;
    }

    return &c_aoBuiltInEncodings[nIndex];
}
const char*                UnicodeToGlyphName         (unsigned short nUnicode)
{
    const UnicodeGlyphPair* pMap = c_aoUnicodeGlyphNameMap;

	while ( pMap->nUnicode <= nUnicode) 
	{
		if ( pMap->nUnicode == nUnicode )
			return pMap->sGlyphName;
        pMap++;
    }

	return c_aoUnicodeGlyphNameMap[0].sGlyphName;
}
unsigned short             GlyphNameToUnicode         (const char *sGlyphName)
{
    const UnicodeGlyphPair *pMap = c_aoUnicodeGlyphNameMap;

	while ( 0xFFFF != pMap->nUnicode ) 
	{
		if ( 0 == UtilsStrCmp( sGlyphName, pMap->sGlyphName ) )
			return pMap->nUnicode;
        pMap++;
    }

    return 0x0000;
}
void                       BasicEncoderCopyMap        (EncoderRecPtr pEncoder, const unsigned short *pnMap)
{
	unsigned short *pnDst = ((BasicEncoderAttr)pEncoder->pAttr)->anUnicodeMap + BASIC_ENCODER_FIRST_CHAR;

    for ( unsigned int nIndex = 0; nIndex <= BASIC_ENCODER_LAST_CHAR - BASIC_ENCODER_FIRST_CHAR; nIndex++)
        *pnDst++ = *pnMap++;
}
unsigned long              BasicEncoderOverrideMap    (EncoderRecPtr pEncoder, const unsigned short *pnMap)
{
	BasicEncoderAttr pData = (BasicEncoderAttr)pEncoder->pAttr;
    unsigned short *pnDst = NULL;
    BYTE *pFlags = NULL;

	if ( pData->bHasDifferences )
		return SetError( pEncoder->oError, AVS_OFFICEPDFWRITER_ERROR_INVALID_OPERATION, 0);

	pnDst  = pData->anUnicodeMap  + BASIC_ENCODER_FIRST_CHAR;
	pFlags = pData->anDifferences + BASIC_ENCODER_FIRST_CHAR;

    for ( unsigned int nIndex = 0; nIndex <= BASIC_ENCODER_LAST_CHAR - BASIC_ENCODER_FIRST_CHAR; nIndex++) 
	{
        if ( *pnMap != *pnDst ) 
		{
            *pnDst = *pnMap;
            *pFlags = 1;
        }
        pnMap++;
        pnDst++;
        pFlags++;
    }
    pData->bHasDifferences = true;

    return OK;
}

void           BasicEncoderFree     (EncoderRecPtr pEncoder)
{
	FreeMem( pEncoder->oMMgr, pEncoder->pAttr );
	pEncoder->pAttr = NULL;
}
unsigned long  BasicEncoderWrite    (EncoderRecPtr pEncoder, StreamRecPtr pOut)
{
    unsigned long nRet = OK;
    BasicEncoderAttr pAttr = (BasicEncoderAttr)pEncoder->pAttr;

    //  Если записан ENCODING_FONT_SPECIFIC, тогда у объекта объект Encoding не пишется BaseEncoding
    bool bFontSpecific = false;
	if ( 0 == UtilsStrCmp( pAttr->sBaseEncoding, ENCODING_FONT_SPECIFIC ) )
        bFontSpecific = true;

    // Если EncoderRecPtr имеет различные данные, тогда объект Еncoding пишется как 
    // Dictionary-object, в противном случаем мы его пишем как name-object.
    
    if ( true == pAttr->bHasDifferences )
	{
		if ( !bFontSpecific )
			nRet = StreamWriteStr( pOut, "/Encoding <<\012/Type /Encoding\012/BaseEncoding ");
		else
			nRet = StreamWriteStr( pOut, "/Encoding <<\012/Type /Encoding");
        if ( OK != nRet )
            return nRet;
    } 
	else 
	{
		if ( !bFontSpecific )
			nRet = StreamWriteStr( pOut, "/Encoding ");
		else
			return nRet;

        if ( OK != nRet )
            return nRet;
    }

	if ( !bFontSpecific )
	{
		nRet = StreamWriteEscapeName( pOut, pAttr->sBaseEncoding );
		if ( OK != nRet )
			return nRet;
	}

    nRet = StreamWriteStr( pOut, "\012" );
    if ( OK != nRet )
		return nRet;

    /* write differences data */
    if ( true == pAttr->bHasDifferences )
	{
        nRet = StreamWriteStr( pOut, "/Differences [" );
        if ( OK != nRet )
            return nRet;

		for ( int nIndex = pAttr->nFirstChar; nIndex <= pAttr->nLastChar; nIndex++)
		{
			if ( 1 == pAttr->anDifferences[nIndex] ) 
			{
                char sTemp[TEXT_DEFAULT_LEN];
                char* pTemp = sTemp;
				const char* sCharName = ( '\0' == pAttr->asGlyphNameMap[nIndex][0] ? UnicodeToGlyphName( pAttr->anUnicodeMap[nIndex] ) : pAttr->asGlyphNameMap[nIndex] );

                pTemp = UtilsIToA( pTemp, nIndex, sTemp + TEMP_BUFFER_SIZE - 1);
                *pTemp++ = ' ';
                *pTemp++ = '/';
                pTemp = (char*)UtilsStrCpy( pTemp, sCharName, sTemp + TEMP_BUFFER_SIZE - 1);
                *pTemp++ = ' ';
                *pTemp = 0;

                nRet = StreamWriteStr( pOut, sTemp );
                if ( OK != nRet )
                    return nRet;
            }
        }

        nRet = StreamWriteStr( pOut, "]\012>>\012" );
    }

    return nRet;
}


unsigned short BasicEncoderToUnicode(EncoderRecPtr pEncoder, unsigned short nCode)
{
    BasicEncoderAttr pAttr = (BasicEncoderAttr)pEncoder->pAttr;

    if ( nCode > 255 )
        return 0;

	return pAttr->anUnicodeMap[ nCode ];
}
EncoderRecPtr        BasicEncoderNew      (MMgr oMMgr, const char *sEncodingName)
{
    if ( NULL == oMMgr )
        return NULL;

    const BuiltinEncodingData *pData = BasicEncoderFindBuiltinData( sEncodingName );

	if ( !pData->sEncodingName ) 
	{
		SetError( oMMgr->oError, AVS_OFFICEPDFWRITER_ERROR_INVALID_ENCODING_NAME, 0);
        return NULL;
    }

	EncoderRecPtr pEncoder = (EncoderRecPtr)GetMem( oMMgr, sizeof(EncoderRec) );
    if ( !pEncoder )
        return NULL;

    UtilsMemSet( pEncoder, 0, sizeof(EncoderRec) );

	char *pEndPointer = pEncoder->sName + LIMIT_MAX_NAME_LEN;
	UtilsStrCpy( pEncoder->sName, pData->sEncodingName, pEndPointer);

	pEncoder->oMMgr  = oMMgr;
	pEncoder->oError = oMMgr->oError;
	pEncoder->eType  = EncoderTypeSingleByte;
	pEncoder->pToUnicodeFn = BasicEncoderToUnicode;
	pEncoder->pWriteFn     = BasicEncoderWrite;
	pEncoder->pFreeFn      = BasicEncoderFree;

    BasicEncoderAttr pEncoderAttr = (BasicEncoderAttr)GetMem( oMMgr, sizeof(BasicEncoderAttrRec) );
    if ( !pEncoderAttr ) 
	{
		FreeMem( pEncoder->oMMgr, pEncoder );
        return NULL;
    }

	pEncoder->nSigBytes = ENCODER_SIG_BYTES;
	pEncoder->pAttr     = pEncoderAttr;
	UtilsMemSet( pEncoderAttr, 0, sizeof(BasicEncoderAttrRec) );

	pEncoderAttr->nFirstChar = BASIC_ENCODER_FIRST_CHAR;
	pEncoderAttr->nLastChar  = BASIC_ENCODER_LAST_CHAR;
    pEncoderAttr->bHasDifferences = false;

	pEndPointer = pEncoderAttr->sBaseEncoding + LIMIT_MAX_NAME_LEN;

	switch ( pData->eBaseEncoding ) 
	{
        case BaseEncodingStandard:
			UtilsStrCpy( pEncoderAttr->sBaseEncoding, ENCODING_STANDARD, pEndPointer);
            BasicEncoderCopyMap( pEncoder, c_anUnicodeMapStandard);
            break;
        case BaseEncodingWinAnsi:
            UtilsStrCpy( pEncoderAttr->sBaseEncoding, ENCODING_WIN_ANSI, pEndPointer);
            BasicEncoderCopyMap( pEncoder, c_anUnicodeMapWinAnsi);
            break;
        case BaseEncodingMacRoman:
            UtilsStrCpy( pEncoderAttr->sBaseEncoding, ENCODING_MAC_ROMAN, pEndPointer);
            BasicEncoderCopyMap( pEncoder, c_anUnicodeMapMacRoman);
            break;
		case BaseEncodingMacExpert:
			UtilsStrCpy( pEncoderAttr->sBaseEncoding, ENCODING_MAC_EXPERT, pEndPointer);
			BasicEncoderCopyMap( pEncoder, c_anUnicodeMapMacExpert );
			break;
        default:
			UtilsStrCpy( pEncoderAttr->sBaseEncoding, ENCODING_FONT_SPECIFIC, pEndPointer);
            BasicEncoderCopyMap( pEncoder, c_anUnicodeMapFontSpecific);
    }

	if ( pData->pnOverrideMap )
		BasicEncoderOverrideMap( pEncoder, pData->pnOverrideMap);

    return pEncoder;
}

void           CMapEncoderStreamFreeVector   (EncoderRecPtr pEncoder, CMapVectorEntry *pVector)
{
	if ( NULL == pVector )
		return;

	for ( int nIndex = 0; nIndex < 256; nIndex++ )
	{
		if ( pVector[nIndex].bIsVector && NULL != pVector[nIndex].pVector )
		{
			CMapEncoderStreamFreeVector( pEncoder, pVector[nIndex].pVector );
			FreeMem( pEncoder->oMMgr, pVector[nIndex].pVector );
		}
	}
}
unsigned long  CMapEncoderStreamWrite        (EncoderRecPtr pEncoder, StreamRecPtr pOut)
{
	if ( EncoderTypeDoubleByteStream != pEncoder->eType )
		return OK;

	CMapEncoderStreamAttr pAttr = (CMapEncoderStreamAttr)pEncoder->pAttr;

	if ( 1 == pAttr->nType && StreamValidate( pAttr->pStream ) )
	{
		return StreamWriteToStream( pAttr->pStream, pOut, 0, NULL );
	}

    return OK;
}
void           CMapEncoderStreamFree         (EncoderRecPtr pEncoder)
{
	CMapEncoderStreamAttr pAttr = (CMapEncoderStreamAttr)pEncoder->pAttr;

	StreamFree( pAttr->uUseCMap.pStream );
	StreamFree( pAttr->pStream );

	CMapEncoderStreamFreeVector( pEncoder, pAttr->pVector );
	FreeMem( pEncoder->oMMgr, pAttr->pVector );


	FreeMem( pEncoder->oMMgr, pEncoder->pAttr );
	pEncoder->pAttr = NULL;
}

//-- CMapEncoderStream - Основные функции --------------------------------------------------------
EncoderRecPtr        CMapEncoderStreamNew          (MMgr oMMgr, int nType, char *sName, StreamRecPtr pStream)
{
    if ( NULL == oMMgr )
        return NULL;

    EncoderRecPtr pEncoder = (EncoderRecPtr)GetMem( oMMgr, sizeof(EncoderRec) );
    if ( !pEncoder )
        return NULL;

    UtilsMemSet( pEncoder, 0, sizeof(EncoderRec) );

	UtilsStrCpy( pEncoder->sName, sName, pEncoder->sName + LIMIT_MAX_NAME_LEN );

	pEncoder->oMMgr        = oMMgr;
	pEncoder->oError       = oMMgr->oError;
	pEncoder->eType        = EncoderTypeDoubleByteStream;
	pEncoder->pByteTypeFn  = NULL;
	pEncoder->pToUnicodeFn = NULL;
	pEncoder->pWriteFn     = CMapEncoderStreamWrite;
	pEncoder->pFreeFn      = CMapEncoderStreamFree;
	pEncoder->pInitFn      = NULL;
	pEncoder->nSigBytes    = ENCODER_SIG_BYTES;

	CMapEncoderStreamAttr pAttr = (CMapEncoderStreamAttr)GetMem( oMMgr, sizeof(CMapEncoderStreamAttrRec) );
	if ( !pAttr )
	{
		FreeMem( oMMgr, pEncoder );
		return NULL;
	}

	UtilsStrCpy( pAttr->sName, sName, pAttr->sName + LIMIT_MAX_NAME_LEN );
	pAttr->nType   = nType;
	pAttr->pStream = pStream;

	pAttr->pVector = (CMapVectorEntry *)GetMem( oMMgr, 256 * sizeof(CMapVectorEntry) );
	if ( !pAttr->pVector )
	{
		FreeMem( oMMgr, pEncoder );
		FreeMem( oMMgr, pAttr );
		return NULL;
	}
	for ( int nIndex = 0; nIndex < 256; nIndex++ )
	{
        pAttr->pVector[nIndex].bIsVector = false;
		pAttr->pVector[nIndex].pVector   = NULL;
		pAttr->pVector[nIndex].nCID      = 0;
	}

	pEncoder->pAttr = pAttr;

	for ( int nIndex = 0; nIndex < 65536; nIndex++ )
	{
		pAttr->anCIDtoCode[nIndex].sCode[0] = 0;
		pAttr->anCIDtoCode[nIndex].sCode[1] = 0;
		pAttr->anCIDtoCode[nIndex].sCode[2] = 0;
		pAttr->anCIDtoCode[nIndex].sCode[3] = 0;
		pAttr->anCIDtoCode[nIndex].nLen     = 0;
	}

    return pEncoder;
}
unsigned long  CMapEncoderStreamSetAttrs     (EncoderRecPtr pEncoder, char *sCMapName, char *sRegistry, char *sOrdering, int nSupplement, WritingMode eMode, char *sUseCMap, StreamRecPtr pUseCMap )
{
	if ( EncoderTypeDoubleByteStream != pEncoder->eType )
		return OK;

	CMapEncoderStreamAttr pAttr = (CMapEncoderStreamAttr)pEncoder->pAttr;

	UtilsStrCpy( pAttr->sCMapName, sCMapName, pAttr->sCMapName + LIMIT_MAX_NAME_LEN );
	UtilsStrCpy( pAttr->sRegistry, sRegistry, pAttr->sRegistry + LIMIT_MAX_NAME_LEN );
	UtilsStrCpy( pAttr->sOrdering, sOrdering, pAttr->sOrdering + LIMIT_MAX_NAME_LEN );
	pAttr->nSupplement  = nSupplement;
	pAttr->eWritingMode = eMode;
	UtilsStrCpy( pAttr->uUseCMap.sName, sUseCMap, pAttr->uUseCMap.sName + LIMIT_MAX_NAME_LEN );
	pAttr->uUseCMap.pStream = pUseCMap;

	return OK;
}


unsigned long  CMapEncoderStreamLoadVector2  (EncoderRecPtr pEncoder, XmlUtils::CXmlNode oNode, CMapVectorEntry **ppVector)
{
	unsigned long nRet = OK;

	if ( NULL == ppVector || NULL == *ppVector )
		return OK;

	CMapVectorEntry *pVector = *ppVector;

	XmlUtils::CXmlNodes oNodes;
	oNode.GetNodes( _T("Vector"), oNodes );

	for ( int nIndex = 0; nIndex < oNodes.GetCount(); nIndex++ )
	{
		XmlUtils::CXmlNode oVector;
		
		if ( oNodes.GetAt( nIndex, oVector ) )
		{
			CString sValue;
			sValue = oVector.GetAttribute( _T("index") );
			int nVectorIndex = XmlUtils::GetInteger( sValue );
			sValue = oVector.GetAttribute( _T("isvector") );
            bool bIsVector = ( 1 == XmlUtils::GetInteger( sValue ) ? true : false ) ;
			sValue = oVector.GetAttribute( _T("cid") );
			int nCID = XmlUtils::GetInteger( sValue );

			if ( bIsVector )
			{
                pVector[nVectorIndex].bIsVector = true;
				pVector[nVectorIndex].pVector   = (CMapVectorEntry *)GetMem( pEncoder->oMMgr, 256 * sizeof(CMapVectorEntry) );
				if ( !pVector[nVectorIndex].pVector )
					return AVS_OFFICEPDFWRITER_ERROR_NOT_ENOUGH_MEMORY;

				for ( int nVIndex = 0; nVIndex < 256; nVIndex++ )
				{
					pVector[nVectorIndex].pVector[nVIndex].pVector   = NULL;
					pVector[nVectorIndex].pVector[nVIndex].nCID      = 0;
                    pVector[nVectorIndex].pVector[nVIndex].bIsVector = false;
				}

				if ( OK != ( nRet = CMapEncoderStreamLoadVector2( pEncoder, oVector, &(pVector[nVectorIndex].pVector) ) ) )
					return nRet;
			}
			else
			{
				pVector[nVectorIndex].nCID      = nCID;
			}
		}
	}

	return OK;
}
unsigned long  CMapEncoderStreamLoadVector   (EncoderRecPtr pEncoder, CString sCMapPath)
{
	unsigned long nRet = OK;
	CMapEncoderStreamAttr pAttr = (CMapEncoderStreamAttr)pEncoder->pAttr;

	XmlUtils::CXmlNode oMainNode;
	oMainNode.FromXmlFile( sCMapPath );
	if ( _T("PDF-CMap") == oMainNode.GetName() )
	{
		CString sValue = oMainNode.GetAttribute( _T("name") );

		if ( _T("Identity") != sValue && _T("Identity-H") != sValue && _T("Identity-V") != sValue )
		{
			if ( OK != ( nRet = CMapEncoderStreamLoadVector2( pEncoder, oMainNode, &(pAttr->pVector) ) ) )
				return nRet;
		}
		else
		{
			// Если pVector = NULL, это будет признак того, что CMap = Identity
			CMapEncoderStreamFreeVector( pEncoder, pAttr->pVector );
			FreeMem( pEncoder->oMMgr, pAttr->pVector );
			pAttr->pVector = NULL;
		}
	}

	return OK;
}
int            CMapEncoderStreamGetCID       (EncoderRecPtr pEncoder, char *sChar, int nLen, int *pnUsed) 
{
	unsigned long nRet = OK;
	CMapEncoderStreamAttr pAttr = (CMapEncoderStreamAttr)pEncoder->pAttr;

	CMapVectorEntry *pVector = NULL;
	
	if ( NULL == ( pVector = pAttr->pVector ) ) 
	{
		// Identity CMap
		*pnUsed = 2;
		if ( nLen < 2 ) 
		{
			return 0;
		}
		return ((sChar[0] & 0xff) << 8) + (sChar[1] & 0xff);
	}
	int nUsedCount = 0;
	while (1) 
	{
		if ( nUsedCount >= nLen ) 
		{
			*pnUsed = nUsedCount;
			return 0;
		}
		int nIndex = sChar[nUsedCount++] & 0xff;
		if ( !pVector[nIndex].bIsVector ) 
		{
			*pnUsed = nUsedCount;
			return pVector[nIndex].nCID;
		}
		pVector = pVector[nIndex].pVector;
	}
}

void           CMapEncoderStreamGetCodeByCID (EncoderRecPtr pEncoder, int nCID, char **ppBuffer, int *pnLen)
{
	unsigned long nRet = OK;
	CMapEncoderStreamAttr pAttr = (CMapEncoderStreamAttr)pEncoder->pAttr;

    nCID = (std::min)( 65536, (std::max)( 0, nCID ) );

	char *pBuffer = *ppBuffer;
	pBuffer[0] = pAttr->anCIDtoCode[nCID].sCode[0];
	pBuffer[1] = pAttr->anCIDtoCode[nCID].sCode[1];
	pBuffer[2] = pAttr->anCIDtoCode[nCID].sCode[2];
	pBuffer[3] = pAttr->anCIDtoCode[nCID].sCode[3];
	*(pnLen)   = pAttr->anCIDtoCode[nCID].nLen;
}
void           CMapEncoderStreamAddCIDtoCode (EncoderRecPtr pEncoder, int nCID, char sBuffer[4], int nLen)
{
	unsigned long nRet = OK;
	CMapEncoderStreamAttr pAttr = (CMapEncoderStreamAttr)pEncoder->pAttr;

    nCID = (std::min)( 65536, (std::max)( 0, nCID ) );

	pAttr->anCIDtoCode[nCID].sCode[0] = sBuffer[0];
	pAttr->anCIDtoCode[nCID].sCode[1] = sBuffer[1];
	pAttr->anCIDtoCode[nCID].sCode[2] = sBuffer[2];
	pAttr->anCIDtoCode[nCID].sCode[3] = sBuffer[3];
	pAttr->anCIDtoCode[nCID].nLen     = nLen;
}


ByteType       CMapEncoderByteType           (EncoderRecPtr pEncoder, ParseTextRec *pState)
{
	CMapEncoderAttr pAttr = (CMapEncoderAttr)pEncoder->pAttr;

	if ( pState->nIndex >= pState->nLen )
        return ByteTypeUnknown;

	if ( ByteTypeLead == pState->eByteType ) 
	{
		if ( pAttr->pIsTrialByteFn( pEncoder, pState->pText[ pState->nIndex ] ) )
			pState->eByteType = ByteTypeTrial;
        else
            pState->eByteType = ByteTypeUnknown;
	} 
	else 
	{
		if ( pAttr->pIsLeadByteFn( pEncoder, pState->pText[ pState->nIndex ] ) )
            pState->eByteType = ByteTypeLead;
        else
            pState->eByteType = ByteTypeSingle;
	}

	pState->nIndex++;

	return pState->eByteType;
}
unsigned short CMapEncoderToUnicode          (EncoderRecPtr pEncoder, unsigned short nCode)
{
    BYTE nLen = nCode;
    BYTE nHei = nCode >> 8;
    CMapEncoderAttr pAttr = (CMapEncoderAttr)pEncoder->pAttr;

	return pAttr->anUnicodeMap[nLen][nHei];
}
unsigned short CMapEncoderToCID              (EncoderRecPtr pEncoder, unsigned short nCode)
{
    BYTE nLen = nCode;
    BYTE nHei = nCode >> 8;
    CMapEncoderAttr pAttr = (CMapEncoderAttr)pEncoder->pAttr;

    return pAttr->anCIDMap[nLen][nHei];
}



unsigned long  CMapEncoderWriteHeader        (StreamRecPtr pOut)
{
	unsigned long nRet = OK;

	const char* sCMapHeader = "/CIDInit /ProcSet findresource begin\n"
							  "12 dict begin\n"
							  "begincmap\n";

	nRet = StreamWriteStr( pOut, sCMapHeader );
	return nRet;
}
unsigned long  CMapEncoderWriteFooter        (StreamRecPtr pOut)
{
	unsigned long nRet = OK;

	const char* sCMapFooter = "endcmap\n"
							  "CMapName currentdict /CMap defineresource pop\n"
							  "end\n"
							  "end\n";

	nRet = StreamWriteStr( pOut, sCMapFooter );
	return nRet;
}
unsigned long  CMapEncoderWriteCodeSpaceRange(EncoderRecPtr pEncoder, StreamRecPtr pOut)
{
	// Пока мы тут будем писать всегда: <00> <FF>
	// TO DO: В будущем надо сделать запись из поля pAttr->pCodeSpaceRange
	unsigned long nRet = OK;

	CMapEncoderAttr pAttr = (CMapEncoderAttr)pEncoder->pAttr;

	const char* sBegin = "begincodespacerange";
	const char* sEnd   = "endcodespacerange";
	int nLinesCount = 1;

	if ( ( nRet = StreamWriteInt( pOut, nLinesCount ) ) != OK )
		return nRet;
	if ( ( nRet = StreamWriteChar( pOut, ' ' ) ) != OK )
		return nRet;
	if ( ( nRet = StreamWriteStr( pOut, sBegin ) ) )
		return nRet;
	if ( ( nRet = StreamWriteChar( pOut, '\n' ) ) != OK )
		return nRet;

	// Это значит, что мы собираемся закодировать символы номерами от 0 до 255.
	if ( ( nRet = StreamWriteStr( pOut, "<00> <FF>" ) ) != OK )
		return nRet;
	if ( ( nRet = StreamWriteChar( pOut, '\n' ) ) != OK )
		return nRet;
	if ( ( nRet = StreamWriteStr( pOut, sEnd ) ) )
		return nRet;
	if ( ( nRet = StreamWriteChar( pOut, '\n' ) ) != OK )
		return nRet;

	return nRet;
}
unsigned long  CMapEncoderWriteBFChar        (EncoderRecPtr pEncoder, StreamRecPtr pOut, int nCode, int nUnicode )
{
	unsigned long nRet = OK;

	CMapEncoderAttr pAttr = (CMapEncoderAttr)pEncoder->pAttr;

	if ( ( nRet = StreamWriteChar( pOut, '<' ) ) != OK )
		return nRet;
	
	// Поскольку мы в codespacerange пишем коды от 0 до 255, тогда
	// здесь коды должны быть двусимвольными(в шестнадцатиричной форме)
	if ( ( nRet = StreamWriteHex( pOut, nCode, 2 ) ) )
		return nRet;
	if ( ( nRet = StreamWriteStr( pOut, "> <" ) ) != OK )
		return nRet;
	if ( ( nRet = StreamWriteHex( pOut, nUnicode, 4 ) ) != OK )
		return nRet;
	if ( ( nRet = StreamWriteStr( pOut, ">\n" ) ) != OK )
		return nRet;

	return nRet;
}
unsigned long  CMapEncoderWriteBFChars       (EncoderRecPtr pEncoder, StreamRecPtr pOut, int nStartCode, int nLastCode)
{
	unsigned long nRet = OK;

	CMapEncoderAttr pAttr = (CMapEncoderAttr)pEncoder->pAttr;

	int nCodesCount = nLastCode - nStartCode + 1;

	if ( ( nRet = StreamWriteInt( pOut, nCodesCount ) ) != OK )
		return nRet;
	if ( ( nRet = StreamWriteStr( pOut, " beginbfchar\n") ) != OK )
		return nRet;

	for ( int nCode = nStartCode; nCode <= nLastCode; nCode++ )
	{
		unsigned short unUnicode = pEncoder->pToUnicodeFn( pEncoder, nCode );
		CMapEncoderWriteBFChar( pEncoder, pOut, nCode, unUnicode );
	}

	if ( ( nRet = StreamWriteStr( pOut, "endbfchar\n") ) != OK )
		return nRet;

	return nRet;
}
unsigned long  CMapEncoderWriteBFChars       (EncoderRecPtr pEncoder, StreamRecPtr pOut)
{
	unsigned long nRet = OK;

	CMapEncoderAttr pAttr = (CMapEncoderAttr)pEncoder->pAttr;

	if ( pAttr->nLastChar < 0 || pAttr->nFirstChar < 0 || EncoderTypeToUnicode != pEncoder->eType )
		return AVS_OFFICEPDFWRITER_ERROR_INVALID_ENCODER_TYPE;

	// На самом деле, предполагаем, что nStartCode = 1, т.е. pAttr->nFirstChar = 0
	int nStartCode  = pAttr->nFirstChar + 1; // 0 код не используется
	int nLastCode   = pAttr->nLastChar;

	int nCodesCount = nLastCode - nStartCode + 1;

	// Почему-то во многих CMap файлах идет разбивка по 100 элементов, поэтому сделаем также
	if ( nCodesCount <= 100 )
	{
		nRet += CMapEncoderWriteBFChars( pEncoder, pOut, nStartCode, nLastCode );
	}
	else 
	{
		nRet += CMapEncoderWriteBFChars( pEncoder, pOut, nStartCode, 100 );
		if ( nCodesCount <= 200 )
		{
			nRet += CMapEncoderWriteBFChars( pEncoder, pOut, 101, nLastCode );
		}
		else
		{
			nRet += CMapEncoderWriteBFChars( pEncoder, pOut, 101, 200 );
			nRet += CMapEncoderWriteBFChars( pEncoder, pOut, 201, nLastCode );
		}
	}

	return nRet;
}
unsigned long  CMapEncoderWriteInfo          (EncoderRecPtr pEncoder, StreamRecPtr pOut)
{
	unsigned long nRet = OK;
	CMapEncoderAttr pAttr = (CMapEncoderAttr)pEncoder->pAttr;

	// На самом деле, пока мы будем предполагать, что у нас sRegistry = Adobe, sOrdering = UCS, nSupplement = 0
	//if ( ( nRet = StreamWriteStr( pOut, "/CIDSystemInfo\n<< /Registry (" ) ) != OK )
	//	return nRet;
	//if ( ( nRet = StreamWriteStr( pOut, pAttr->sRegistry ) ) != OK )
	//	return nRet;
	//if ( ( nRet = StreamWriteStr( pOut, ")\n /Ordering (") ) != OK )
	//	return nRet;
	//if ( ( nRet = StreamWriteStr( pOut, pAttr->sOrdering ) ) != OK )
	//	return nRet;
	//if ( ( nRet = StreamWriteStr( pOut, ")\n /Supplement ") ) != OK )
	//	return nRet;
	//if ( ( nRet = StreamWriteInt( pOut, pAttr->nSuppliment ) ) != OK )
	//	return nRet;
	//if ( ( nRet = StreamWriteStr( pOut, "\n>> def\n/CMapName /Adobe-Identity-UCS def\n/CMapType 2 def\n") ) != OK )
	//	return nRet;
	if ( ( nRet = StreamWriteStr( pOut, "/CIDSystemInfo\n<< /Registry (Adobe)\n /Ordering (UCS)\n /Supplement 0\n >> def\n/CMapName /Adobe-Identity-UCS def\n/CMapType 2 def\n" ) ) != OK )
		return nRet;

	return nRet;
}
unsigned long  CMapEncoderWrite              (EncoderRecPtr pEncoder, StreamRecPtr pOut)
{
    unsigned long nRet = OK;
	// ToDo: сделать запись

	// Пока сделаем простую запись, которая необходима для поддержки ToUnicode в шрифтах

	if ( ( nRet = CMapEncoderWriteHeader( pOut ) ) != OK )
		return nRet;
	if ( ( nRet = CMapEncoderWriteInfo( pEncoder, pOut ) ) != OK )
		return nRet;
	if ( ( nRet = CMapEncoderWriteCodeSpaceRange( pEncoder, pOut ) ) != OK )
		return nRet;
	if ( ( nRet = CMapEncoderWriteBFChars( pEncoder, pOut ) ) != OK )
		return nRet;
	if ( ( nRet = CMapEncoderWriteFooter( pOut ) ) != OK )
		return nRet;

    return nRet;
}
void           CMapEncoderFree               (EncoderRecPtr pEncoder)
{    
	CIDRangeRec *pData = NULL;

    CMapEncoderAttr pAttr = (CMapEncoderAttr)pEncoder->pAttr;

	if ( pAttr && pAttr->pCMAPRange ) 
	{
		for ( unsigned int nIndex = 0; nIndex < pAttr->pCMAPRange->nCount; nIndex++) 
		{
			pData = (CIDRangeRec*)ListItemAt( pAttr->pCMAPRange, nIndex );
			FreeMem( pEncoder->oMMgr, pData);
        }

		ListFree( pAttr->pCMAPRange );
    }

	if ( pAttr && pAttr->pNotDefRange ) 
	{
		for ( unsigned int nIndex = 0; nIndex < pAttr->pNotDefRange->nCount; nIndex++)  
		{
			pData = (CIDRangeRec*)ListItemAt( pAttr->pNotDefRange, nIndex );
			FreeMem( pEncoder->oMMgr, pData );
        }
		ListFree( pAttr->pNotDefRange );
    }

	if ( pAttr && pAttr->pCodeSpaceRange ) 
	{
		for ( unsigned int nIndex = 0; nIndex < pAttr->pCodeSpaceRange->nCount; nIndex++) 
		{
			pData = (CIDRangeRec*)ListItemAt( pAttr->pCodeSpaceRange, nIndex );
			FreeMem( pEncoder->oMMgr, pData );
        }
		ListFree( pAttr->pCodeSpaceRange );
    }
	FreeMem( pEncoder->oMMgr, pEncoder->pAttr );
	pEncoder->pAttr = NULL;
}

static unsigned long AddCIDRange    (MMgr oMMgr, CIDRangeRec oRange, List pTarget)
{
    unsigned long nRet = OK;

    CIDRangeRec *pRange = (CIDRangeRec*)GetMem( oMMgr, sizeof(CIDRangeRec) );
    if ( !pRange )
		return oMMgr->oError->nErrorNo;

	pRange->nFrom = oRange.nFrom;
	pRange->nTo   = oRange.nTo;
	pRange->nCID  = oRange.nCID;

    if ( OK != ( nRet = ListAdd( pTarget, pRange ) ) ) 
	{
        FreeMem( oMMgr, pRange );
        return nRet;
    }

    return OK;
}

EncoderRecPtr       CMapEncoderNew               (MMgr oMMgr, char *sName, EncoderInitFunc pInitFn)
{
    if ( NULL == oMMgr )
        return NULL;

    EncoderRecPtr pEncoder = (EncoderRecPtr)GetMem( oMMgr, sizeof(EncoderRec) );
    if ( !pEncoder )
        return NULL;

    UtilsMemSet( pEncoder, 0, sizeof(EncoderRec) );

	UtilsStrCpy( pEncoder->sName, sName, pEncoder->sName + LIMIT_MAX_NAME_LEN );
	pEncoder->oMMgr        = oMMgr;
	pEncoder->oError       = oMMgr->oError;
	pEncoder->eType        = EncoderTypeUninitialized;
	pEncoder->pByteTypeFn  = CMapEncoderByteType;
	pEncoder->pToUnicodeFn = CMapEncoderToUnicode;
	pEncoder->pWriteFn     = CMapEncoderWrite;
	pEncoder->pFreeFn      = CMapEncoderFree;
	pEncoder->pInitFn      = pInitFn;
	pEncoder->nSigBytes    = ENCODER_SIG_BYTES;

    return pEncoder;
}
unsigned long CMapEncoderInitAttr          (EncoderRecPtr pEncoder)
{
    if ( pEncoder->pAttr )
		return AVS_OFFICEPDFWRITER_ERROR_INVALID_ENCODER;

	CMapEncoderAttr pEncoderAttr = (CMapEncoderAttr)GetMem( pEncoder->oMMgr, sizeof(CMapEncoderAttrRec) );

    if ( !pEncoderAttr)
		return pEncoder->oError->nErrorNo;

    UtilsMemSet( pEncoderAttr, 0, sizeof(CMapEncoderAttrRec) );
	pEncoder->pAttr = pEncoderAttr;

	pEncoderAttr->eWritingMode = WModeHorizontal;

    for ( unsigned int nI = 0; nI <= 255; nI++) 
	{
        for ( unsigned int nJ = 0; nJ <= 255; nJ++) 
		{
            // неопределенные символы заменяются квадратами
			pEncoderAttr->anUnicodeMap[nI][nJ] = 0x25A1;
        }
    }

	pEncoderAttr->pCMAPRange = ListNew( pEncoder->oMMgr, DEF_RANGE_TBL_NUM);
    if ( !pEncoderAttr->pCMAPRange )
		return pEncoder->oError->nErrorNo;

	pEncoderAttr->pNotDefRange = ListNew( pEncoder->oMMgr, DEF_ITEMS_PER_BLOCK);
    if ( !pEncoderAttr->pNotDefRange )
        return pEncoder->oError->nErrorNo;

	pEncoderAttr->pCodeSpaceRange = ListNew( pEncoder->oMMgr, DEF_ITEMS_PER_BLOCK);
    if ( !pEncoderAttr->pCodeSpaceRange )
        return pEncoder->oError->nErrorNo;

	pEncoderAttr->nFirstChar = -1;
	pEncoderAttr->nLastChar  = -1;

    return OK;
}

unsigned long CMapEncoderSetParseText      (EncoderRecPtr pEncoder, ParseTextRec *pState, const BYTE *sText, unsigned int nLen)
{
	pState->pText     = sText;
	pState->nIndex    = 0;
	pState->nLen      = nLen;
	pState->eByteType = ByteTypeSingle;

	return OK;
}
unsigned long CMapEncoderAddCMap           (EncoderRecPtr pEncoder, const CIDRangeRec *pRange)
{
    CMapEncoderAttr pAttr = (CMapEncoderAttr)pEncoder->pAttr;

    // Копируем определенный pdf_cid_range массив в fRangeArray.

    while ( 0xffff != pRange->nFrom && 0xffff != pRange->nTo ) 
	{
        unsigned short nCode = pRange->nFrom;
        unsigned short nCID  = pRange->nCID;
        unsigned long nRet = OK;

        while ( nCode <= pRange->nTo ) 
		{
			BYTE nLen = nCode;
			BYTE nHei = nCode >> 8;

            pAttr->anCIDMap[nLen][nHei] = nCID;
            nCode++;
            nCID++;
        }

        CIDRangeRec *pTempRange = (CIDRangeRec *)GetMem( pEncoder->oMMgr, sizeof(CIDRangeRec) );
        if ( !pTempRange )
            return pEncoder->oError->nErrorNo;

        pTempRange->nFrom = pRange->nFrom;
        pTempRange->nTo   = pRange->nTo;
        pTempRange->nCID  = pRange->nCID;

        if ( OK != ( nRet = ListAdd( pAttr->pCMAPRange, pTempRange ) ) ) 
		{
            FreeMem( pEncoder->oMMgr, pTempRange );
            return nRet;
        }

        pRange++;
    }

    return OK;
}
unsigned long CMapEncoderAddNotDefRange    (EncoderRecPtr pEncoder, CIDRangeRec oRange)
{
	CMapEncoderAttr pAttr = (CMapEncoderAttr)pEncoder->pAttr;

	return AddCIDRange( pEncoder->oMMgr, oRange, pAttr->pNotDefRange );
}
unsigned long CMapEncoderAddCodeSpaceRange (EncoderRecPtr pEncoder, CIDRangeRec oRange)
{
	CMapEncoderAttr pAttr = (CMapEncoderAttr)pEncoder->pAttr;

	return AddCIDRange( pEncoder->oMMgr, oRange, pAttr->pCodeSpaceRange );
}
void          CMapEncoderSetUnicodeArray   (EncoderRecPtr pEncoder, const UnicodeMapRec *pArray)
{
	CMapEncoderAttr pAttr = (CMapEncoderAttr)pEncoder->pAttr;

    if ( NULL  != pArray )
	{
		while ( 0xffff != pArray->nUnicode ) 
		{
			BYTE nLen = pArray->nCode;
			BYTE nHei = pArray->nCode >> 8;
			pAttr->anUnicodeMap[nLen][nHei] = pArray->nUnicode;
            pArray++;
        }
		BYTE nLen = pArray->nCode;
		BYTE nHei = pArray->nCode >> 8;
		pAttr->anUnicodeMap[nLen][nHei] = pArray->nUnicode;
	}
}
unsigned long CMapEncoderAddJWWLineHead    (EncoderRecPtr pEncoder, const unsigned short *pnCode)
{
    CMapEncoderAttr pAttr = (CMapEncoderAttr)pEncoder->pAttr;

    for (unsigned int nI = 0; nI < MAX_JWW_NUM; nI++ ) 
	{
        if ( 0 == *pnCode )
            break;

        for ( unsigned int nJ = 0; nJ < MAX_JWW_NUM; nJ++) 
		{
			if ( *pnCode == pAttr->anJWWLineHead[nJ] )
                break;

			if ( 0 == pAttr->anJWWLineHead[nJ] ) 
			{
				pAttr->anJWWLineHead[nJ] = *pnCode;
                break;
            }

            if ( MAX_JWW_NUM - 1 == nJ )
				return SetError( pEncoder->oError, AVS_OFFICEPDFWRITER_ERROR_EXCEED_JWW_CODE_NUM_LIMIT, nI );
        }

        pnCode++;
    }

    return OK;
}
bool          EncoderCheckJWWLineHead      (EncoderRecPtr pEncoder, const unsigned short nCode)
{
    if ( !EncoderValidate( pEncoder ) )
        return false;

	if ( EncoderTypeDoubleByteBuilt != pEncoder->eType )
        return false;

	CMapEncoderAttr pAttr = (CMapEncoderAttr)pEncoder->pAttr;

    for ( unsigned int nIndex = 0; nIndex < MAX_JWW_NUM; nIndex++) 
	{
		if ( nCode == pAttr->anJWWLineHead[nIndex] )
            return true;

		if ( 0 == pAttr->anJWWLineHead[nIndex] )
            return false;
    }

    return false;
}
bool          CMapEncoderUpdateUnicodeArray(EncoderRecPtr pEncoder, unsigned short *pArray)
{
	// Используется только для кодировок типа EncoderTypeToUnicode
	if ( EncoderTypeToUnicode != pEncoder->eType )
        return false;

	CMapEncoderAttr pAttr = (CMapEncoderAttr)pEncoder->pAttr;
	UnicodeMapRec* pResultArray = new UnicodeMapRec[257]; // 0-ой символ + 255 символов кодировки + 1 символ 0xFFFF
	if ( !pResultArray )
        return false;

	int nIndex = 0;
	pResultArray[0].nCode    = 0;
	pResultArray[0].nUnicode = 0x0000;
	for ( nIndex = pAttr->nFirstChar + 1; nIndex <= pAttr->nLastChar; nIndex++ )
	{
		pResultArray[nIndex].nCode    = nIndex;
		pResultArray[nIndex].nUnicode = EncoderToUnicode( pEncoder, nIndex );
	}

	if ( NULL != pArray )
	{
		while( 0xFFFF != *pArray && nIndex < 256 )
		{
			if ( 0xFFFF == EncoderToCode( pEncoder, *pArray ) )
			{
				pResultArray[nIndex].nCode    = nIndex;
				pResultArray[nIndex].nUnicode = *pArray;
				nIndex++;
			}
			pArray++;
		}
	}

	if ( 255 < nIndex )
	{
		delete []pResultArray;
        return false;
	}
	else
	{
		pResultArray[nIndex].nCode    = nIndex;
		pResultArray[nIndex].nUnicode = 0xFFFF;
		CMapEncoderSetUnicodeArray( pEncoder, pResultArray );
		pAttr->nLastChar = nIndex - 1;
		delete []pResultArray;
        return true;
	}

}

unsigned short EncoderGetUnicode    (EncoderRecPtr pEncoder, unsigned short nCode)
{
    if ( !EncoderValidate( pEncoder ) )
        return 0;

    return EncoderToUnicode( pEncoder, nCode );
}
ByteType       EncoderGetByteType   (EncoderRecPtr pEncoder, const char *sText, unsigned int nIndex)
{
    ParseTextRec oParseState;
    ByteType eType;

    if ( !EncoderValidate( pEncoder ) )
        return ByteTypeUnknown;

	if ( EncoderTypeDoubleByteBuilt != pEncoder->eType )
        return ByteTypeSingle;

    EncoderSetParseText( pEncoder, &oParseState, (BYTE*)sText, nIndex + 1 );

    for (;;) 
	{
        eType = CMapEncoderByteType( pEncoder, &oParseState);

        if ( 0 == nIndex )
            break;

        sText++;
        if ( !(*sText) )
            return ByteTypeUnknown;
        nIndex--;
    }

    return eType;
}
EncoderType    EncoderGetType       (EncoderRecPtr pEncoder)
{
    if ( !EncoderValidate( pEncoder ) )
        return EncoderTypeUnknown;

	return pEncoder->eType;
}
WritingMode    EncoderGetWritingMode(EncoderRecPtr pEncoder)
{
    if ( !EncoderValidate( pEncoder ) )
        return WModeHorizontal;

	if ( EncoderTypeDoubleByteBuilt == pEncoder->eType ) 
	{
		CMapEncoderAttr pAttr = (CMapEncoderAttr)pEncoder->pAttr;
        return pAttr->eWritingMode;
    }

    return WModeHorizontal;
}
bool           EncoderWCharToString (EncoderRecPtr pEncoder, CString bsSrc, CString *psDst)
{
	CStringW sSrc( bsSrc );
	// Данная функция проверяет все ли символы из usSrc присутствуют в данной кодировке;
	// если присутствуют все, тогда в psDst записывается строка usSrc, в соответствии с данной кодировкой.
	CString sResult = _T("");
	int nLen = sSrc.GetLength();
	for ( int nIndex = 0; nIndex < nLen; nIndex++ )
	{
		unsigned short unUnicodeChar = (unsigned short)sSrc.GetAt( nIndex );
		unsigned short unCode = EncoderToCode( pEncoder, unUnicodeChar );
		if ( 0xFFFF == unCode )
            return false;
		else
			sResult += wchar_t(unCode);
	}

	*psDst = sResult;

    return true;
}


