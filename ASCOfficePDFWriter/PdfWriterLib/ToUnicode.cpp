#include "ToUnicode.h"

unsigned long  ToUnicodeValidate     (PToUnicode pToUnicode)
{
	if ( !pToUnicode || TO_UNICODE_SIG_BYTES != pToUnicode->nSigBytes )
        return false;
    else
        return true;
}
unsigned short ToUnicodeToUnicode    (PToUnicode pToUnicode, unsigned short nCode)
{
	if ( !pToUnicode || !pToUnicode->pToUnicodeFn )
		return 0;

	return pToUnicode->pToUnicodeFn( pToUnicode, nCode );
}
void           ToUnicodeFree         (PToUnicode pToUnicode)
{
    if ( !pToUnicode )
        return;

    if ( pToUnicode->pFreeFn )
        pToUnicode->pFreeFn( pToUnicode );

    FreeMem( pToUnicode->oMMgr, pToUnicode );
}
unsigned short ToUnicodeToCode       (PToUnicode pToUnicode, unsigned short unUnicode)
{
	// Ищем код для заданного юникодного значения, если такого в данной кодировке нет возвращаем значение 0xFFFF.
	for ( int nCode = 0; nCode < 256; nCode++ )
	{
		unsigned short unCurUnicode = ToUnicodeToUnicode( pToUnicode, nCode );
		if ( unCurUnicode == unUnicode )
			return nCode;
	}
	return 0xFFFF;
}


//-- ToUnicodeStream - Вспомогательные функции ---------------------------------------------------

//-- ToUnicodeStream - Основные функции ----------------------------------------------------------
void           ToUnicodeStreamFree     (PToUnicode pToUnicode)
{
	ToUnicodeStreamAttr pAttr = (ToUnicodeStreamAttr)pToUnicode->pAttr;
	StreamFree( pAttr->pStream );
	FreeMem( pToUnicode->oMMgr, pToUnicode->pAttr );
	pToUnicode->pAttr = NULL;
}
unsigned long  ToUnicodeStreamWrite    (PToUnicode pToUnicode, StreamRecPtr pOut)
{
    ToUnicodeStreamAttr pAttr = (ToUnicodeStreamAttr)pToUnicode->pAttr;

	return StreamWriteToStream( pAttr->pStream, pOut, 0, NULL );
}
PToUnicode     ToUnicodeStreamNew      (MMgr oMMgr, BYTE *pBuffer, unsigned int unSize, const char *sName)
{
    if ( NULL == oMMgr )
        return NULL;

	if ( NULL == pBuffer || 0 == unSize ) 
	{
		SetError( oMMgr->oError, AVS_OFFICEPDFWRITER_ERROR_INVALID_ENCODER, 0);
        return NULL;
    }

	PToUnicode pToUnicode = (PToUnicode)GetMem( oMMgr, sizeof(ToUnicodeRec) );
    if ( !pToUnicode )
        return NULL;

    UtilsMemSet( pToUnicode, 0, sizeof(ToUnicodeRec) );

	char *pEndPointer = pToUnicode->sName + LIMIT_MAX_NAME_LEN;
	UtilsStrCpy( pToUnicode->sName, sName, pEndPointer);

	pToUnicode->oMMgr        = oMMgr;
	pToUnicode->oError       = oMMgr->oError;
	pToUnicode->eType        = ToUnicodeStream;

	pToUnicode->pToUnicodeFn = NULL;
	pToUnicode->pWriteFn     = ToUnicodeStreamWrite;
	pToUnicode->pFreeFn      = ToUnicodeStreamFree;
	pToUnicode->pInitFn      = NULL;

    ToUnicodeStreamAttr pAttr = (ToUnicodeStreamAttr)GetMem( oMMgr, sizeof(ToUnicodeStreamAttrRec) );
    if ( !pAttr ) 
	{
		FreeMem( pToUnicode->oMMgr, pToUnicode );
        return NULL;
    }

	pToUnicode->nSigBytes    = TO_UNICODE_SIG_BYTES;
	pToUnicode->pAttr        = pAttr;

	UtilsMemSet( pAttr, 0, sizeof(ToUnicodeStreamAttrRec) );

    pAttr->pStream = TempFileStreamNew( oMMgr);
	StreamSeek( pAttr->pStream, 0, SeekSet );
	StreamWrite( pAttr->pStream, pBuffer, unSize, true );

    return pToUnicode;
}

unsigned short ToUnicodeBuiltToUnicode          (PToUnicode pToUnicode, unsigned short nCode)
{
    BYTE nLen = nCode;
    BYTE nHei = nCode >> 8;

    ToUnicodeBuiltAttr pAttr = (ToUnicodeBuiltAttr)pToUnicode->pAttr;

	return pAttr->anUnicodeMap[nLen][nHei];
}
unsigned long  ToUnicodeBuiltWriteHeader        (StreamRecPtr pOut)
{
	unsigned long nRet = OK;

	const char* sToUnicodeHeader = "/CIDInit /ProcSet findresource begin\n"
							  "12 dict begin\n"
							  "begincmap\n";

	nRet = StreamWriteStr( pOut, sToUnicodeHeader );
	return nRet;
}
unsigned long  ToUnicodeBuiltWriteFooter        (StreamRecPtr pOut)
{
	unsigned long nRet = OK;

	const char* sCMapFooter = "endcmap\n"
							  "CMapName currentdict /CMap defineresource pop\n"
							  "end\n"
							  "end\n";

	nRet = StreamWriteStr( pOut, sCMapFooter );
	return nRet;
}
unsigned long  ToUnicodeBuiltWriteCodeSpaceRange(PToUnicode pToUnicode, StreamRecPtr pOut)
{
	// Пока мы тут будем писать всегда: <00> <FF>
	// TO DO: В будущем надо сделать запись из поля pAttr->pCodeSpaceRange
	unsigned long nRet = OK;

	ToUnicodeBuiltAttr pAttr = (ToUnicodeBuiltAttr)pToUnicode->pAttr;

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

	if ( 8 == pAttr->nBitCount )
	{
		// Это значит, что мы собираемся закодировать символы номерами от 0 до 255.
		if ( ( nRet = StreamWriteStr( pOut, "<00> <FF>" ) ) != OK )
			return nRet;
	}
	else // if ( 16 == pAttr->nBitCount )
	{
		// Значит символы закодированы двубайтными значениями
		if ( ( nRet = StreamWriteStr( pOut, "<0000> <FFFF>" ) ) != OK )
			return nRet;
	}

	if ( ( nRet = StreamWriteChar( pOut, '\n' ) ) != OK )
		return nRet;
	if ( ( nRet = StreamWriteStr( pOut, sEnd ) ) )
		return nRet;
	if ( ( nRet = StreamWriteChar( pOut, '\n' ) ) != OK )
		return nRet;

	return nRet;
}
unsigned long  ToUnicodeBuiltWriteBFChar        (PToUnicode pToUnicode, StreamRecPtr pOut, int nCode, int nUnicode )
{
	unsigned long nRet = OK;

	ToUnicodeBuiltAttr pAttr = (ToUnicodeBuiltAttr)pToUnicode->pAttr;

	if ( 8 == pAttr->nBitCount )
	{
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
	}
	return nRet;
}
unsigned long  ToUnicodeBuiltWriteBFChars       (PToUnicode pToUnicode, StreamRecPtr pOut, int nStartCode, int nLastCode)
{
	unsigned long nRet = OK;

	ToUnicodeBuiltAttr pAttr = (ToUnicodeBuiltAttr)pToUnicode->pAttr;

	int nCodesCount = nLastCode - nStartCode + 1;

	if ( ( nRet = StreamWriteInt( pOut, nCodesCount ) ) != OK )
		return nRet;
	if ( ( nRet = StreamWriteStr( pOut, " beginbfchar\n") ) != OK )
		return nRet;

	for ( int nCode = nStartCode; nCode <= nLastCode; nCode++ )
	{
		unsigned short unUnicode = pToUnicode->pToUnicodeFn( pToUnicode, nCode );
		ToUnicodeBuiltWriteBFChar( pToUnicode, pOut, nCode, unUnicode );
	}

	if ( ( nRet = StreamWriteStr( pOut, "endbfchar\n") ) != OK )
		return nRet;

	return nRet;
}
unsigned long  ToUnicodeBuiltWriteBFChars       (PToUnicode pToUnicode, StreamRecPtr pOut)
{
	unsigned long nRet = OK;

	ToUnicodeBuiltAttr pAttr = (ToUnicodeBuiltAttr)pToUnicode->pAttr;

	if ( pAttr->nLastChar < 0 || pAttr->nFirstChar < 0 )
		return AVS_OFFICEPDFWRITER_ERROR_INVALID_ENCODER_TYPE;

	// На самом деле, предполагаем, что nStartCode = 1, т.е. pAttr->nFirstChar = 0
	int nStartCode  = pAttr->nFirstChar + 1; // 0 код не используем
	int nLastCode   = pAttr->nLastChar;

	int nCodesCount = nLastCode - nStartCode + 1;

	if ( 8 == pAttr->nBitCount )
	{
		// Почему-то во многих CMap файлах идет разбивка по 100 элементов, поэтому сделаем также
		if ( nCodesCount <= 100 )
		{
			nRet += ToUnicodeBuiltWriteBFChars( pToUnicode, pOut, nStartCode, nLastCode );
		}
		else 
		{
			nRet += ToUnicodeBuiltWriteBFChars( pToUnicode, pOut, nStartCode, 100 );
			if ( nCodesCount <= 200 )
			{
				nRet += ToUnicodeBuiltWriteBFChars( pToUnicode, pOut, 101, nLastCode );
			}
			else
			{
				nRet += ToUnicodeBuiltWriteBFChars( pToUnicode, pOut, 101, 200 );
				nRet += ToUnicodeBuiltWriteBFChars( pToUnicode, pOut, 201, nLastCode );
			}
		}
	}

	return nRet;
}
unsigned long  ToUnicodeBuiltWriteInfo          (PToUnicode pToUnicode, StreamRecPtr pOut)
{
	unsigned long nRet = OK;

	if ( ( nRet = StreamWriteStr( pOut, "/CIDSystemInfo\n<< /Registry (Adobe)\n /Ordering (UCS)\n /Supplement 0\n >> def\n/CMapName /Adobe-Identity-UCS def\n/CMapType 2 def\n" ) ) != OK )
		return nRet;

	return nRet;
}
unsigned long  ToUnicodeBuiltWrite              (PToUnicode pToUnicode, StreamRecPtr pOut)
{
    unsigned long nRet = OK;

	// Пока сделаем простую запись, которая необходима для поддержки ToUnicode в шрифтах

	if ( ( nRet = ToUnicodeBuiltWriteHeader( pOut ) ) != OK )
		return nRet;
	if ( ( nRet = ToUnicodeBuiltWriteInfo( pToUnicode, pOut ) ) != OK )
		return nRet;
	if ( ( nRet = ToUnicodeBuiltWriteCodeSpaceRange( pToUnicode, pOut ) ) != OK )
		return nRet;
	if ( ( nRet = ToUnicodeBuiltWriteBFChars( pToUnicode, pOut ) ) != OK )
		return nRet;
	if ( ( nRet = ToUnicodeBuiltWriteFooter( pOut ) ) != OK )
		return nRet;

    return nRet;
}
void           ToUnicodeBuiltFree               (PToUnicode pToUnicode)
{
	FreeMem( pToUnicode->oMMgr, pToUnicode->pAttr );
	pToUnicode->pAttr = NULL;
}
//-- ToUnicodeBuilt - Основные функции -----------------------------------------------------------

PToUnicode    ToUnicodeBuiltNew               (MMgr oMMgr, char *sName, unsigned int unBitCount /*= 8*/)
{
    if ( NULL == oMMgr )
        return NULL;

    PToUnicode pToUnicode = (PToUnicode)GetMem( oMMgr, sizeof(ToUnicodeRec) );
    if ( !pToUnicode )
        return NULL;

    UtilsMemSet( pToUnicode, 0, sizeof(ToUnicodeRec) );
	UtilsStrCpy( pToUnicode->sName, sName, pToUnicode->sName + LIMIT_MAX_NAME_LEN );

	pToUnicode->oMMgr        = oMMgr;
	pToUnicode->oError       = oMMgr->oError;
	pToUnicode->eType        = ToUnicodeBuilt;
	pToUnicode->pToUnicodeFn = ToUnicodeBuiltToUnicode;
	pToUnicode->pWriteFn     = ToUnicodeBuiltWrite;
	pToUnicode->pFreeFn      = ToUnicodeBuiltFree;
	pToUnicode->pInitFn      = NULL;
	pToUnicode->nSigBytes    = TO_UNICODE_SIG_BYTES;

	ToUnicodeBuiltAttr pAttr = (ToUnicodeBuiltAttr)GetMem( pToUnicode->oMMgr, sizeof(ToUnicodeBuiltAttrRec) );
	UtilsMemSet( pAttr, 0, sizeof(ToUnicodeBuiltAttrRec) );

	if ( !pAttr )
	{
		FreeMem( oMMgr, pToUnicode );
		return NULL;
	}

	pToUnicode->pAttr = pAttr;
    for ( unsigned int nI = 0; nI <= 255; nI++) 
	{
        for ( unsigned int nJ = 0; nJ <= 255; nJ++) 
		{
            // неопределенные символы заменяются квадратами
			pAttr->anUnicodeMap[nI][nJ] = 0x25A1;
        }
    }

	pAttr->nFirstChar = -1;
	pAttr->nLastChar  = -1;
	pAttr->nBitCount  = unBitCount;

    return pToUnicode;
}
void          ToUnicodeBuiltSetUnicodeArray   (PToUnicode pToUnicode, const UnicodeMapRec *pArray)
{
	if ( ToUnicodeBuilt != pToUnicode->eType )
		return;

	ToUnicodeBuiltAttr pAttr = (ToUnicodeBuiltAttr)pToUnicode->pAttr;

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
bool          ToUnicodeUpdateUnicodeArray     (PToUnicode pToUnicode, unsigned short *pArray, unsigned int unSize/* = 0*/)
{
	if ( ToUnicodeBuilt != pToUnicode->eType )
        return false;

	ToUnicodeBuiltAttr pAttr = (ToUnicodeBuiltAttr)pToUnicode->pAttr;

	if ( 8 != pAttr->nBitCount )
        return false;

	UnicodeMapRec* pResultArray = new UnicodeMapRec[257]; // 0-ой символ + 255 символов кодировки + 1 символ 0xFFFF
	if ( !pResultArray )
        return false;

	int nIndex = 0;

	pResultArray[0].nCode    = 0;
	pResultArray[0].nUnicode = 0x0000;

	// Выставляем прежние значения 
	for ( nIndex = pAttr->nFirstChar + 1; nIndex <= pAttr->nLastChar; nIndex++ )
	{
		pResultArray[nIndex].nCode    = nIndex;
		pResultArray[nIndex].nUnicode = ToUnicodeToUnicode( pToUnicode, nIndex );
	}

	// Берем значения из нового массива
	if ( NULL != pArray )
	{
		if ( 0 == unSize )
		{
			while( 0xFFFF != *pArray && nIndex < 256 )
			{
				if ( 0xFFFF == ToUnicodeToCode( pToUnicode, *pArray ) )
				{
					pResultArray[nIndex].nCode    = nIndex;
					pResultArray[nIndex].nUnicode = *pArray;
					nIndex++;
				}
				pArray++;
			}
		}
		else
		{
			for ( unsigned int unCurIndex = 0; unCurIndex < unSize && nIndex < 256; unCurIndex++ )
			{
				if ( 0xFFFF == ToUnicodeToCode( pToUnicode, *pArray ) )
				{
					pResultArray[nIndex].nCode    = nIndex;
					pResultArray[nIndex].nUnicode = *pArray;
					nIndex++;
				}
				pArray++;
			}
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
		ToUnicodeBuiltSetUnicodeArray( pToUnicode, pResultArray );
		pAttr->nLastChar = nIndex - 1;
		delete []pResultArray;
        return true;
	}

}

bool    ToUnicodeWCharToString  (PToUnicode pToUnicide, std::wstring sSrc, std::wstring & sResult)
{
	// Данная функция проверяет все ли символы из usSrc присутствуют в данной кодировке;
	// если присутствуют все, тогда в psDst записывается строка usSrc, в соответствии с данной кодировкой.
    sResult = _T("");
    int nLen = sSrc.length();
	for ( int nIndex = 0; nIndex < nLen; nIndex++ )
	{
		//todooo КОНВЕРТАЦИЯ wchar32 -> wchar16 
		unsigned short unUnicodeChar = (unsigned short)sSrc.at( nIndex );
		unsigned short unCode = ToUnicodeToCode( pToUnicide, unUnicodeChar );
		if ( 0xFFFF == unCode )
            return false;
		else
			sResult += wchar_t(unCode);
	}

    return true;
}
