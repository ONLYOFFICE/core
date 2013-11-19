#ifndef _DOC_H
#define _DOC_H

#define SIG_BYTES 0x41504446L

#include "Catalog.h"
#include "Image.h"
#include "Pages.h"
#include "Outline.h"
#include "GraphicState.h"
#include "Utils.h"
#include "EncryptDictionary.h"
#include "Destination.h"
#include "MetaData.h"
#include "Version.h"
#include "LZWEncoder.h"
#include "Pattern.h"

static const char *VERSION_STR[5] = 
{
	"%PDF-1.2\012%ASC\012",
	"%PDF-1.3\012%ASC\012",
	"%PDF-1.4\012%ASC\012",
	"%PDF-1.5\012%ASC\012",
	"%PDF-1.6\012%ASC\012"
};


#define VER_DEFAULT  PDF_1_2

typedef struct TDocRec 
{
    unsigned int    nSigBytes;
    PDFVer          ePdfVersion;

    MMgr            oMMgr;
    Catalog         pCatalog;
    Outline         pOutlines;
    Xref            pXref;
    Pages           pRootPages;
    Pages           pCurPages;
    Page            pCurPage;
    List            pPageList;
    ErrorRec        oError;
    Dict            pInfo;
    Dict            pTrailer;

	Dict			resources;

    List            pFontManager;
    BYTE            aTTFontTag[6];

    // список для загрузки fontdefs
    List            pFontDefList;

    // список для загрузки кодировок
    List            pEncoderList;
    Encoder         pCurEncoder;

	// список для загрузки ToUnicode
	List            pToUnicodeList;         // Весь список отображений ToUnicode
	unsigned int    nToUnicodeStreamCount;  // Количество ToUnicode типа Stream
	unsigned int    nToUnicodeBuiltCount;   // Количество ToUnicode типа Built
	PToUnicode      pCurToUnicode;          // Текущий объект ToUnicode
	PToUnicode      pLastToUnicodeBuilt;    // Последний объект ToUnicode типа Built

	// Список ExtGState
	List            pExtGStateList;
	ExtGState       pCurExtGState;

    // default compression mode 
    unsigned short  nCompressionMode;

    BOOL            bEncryptOn;
    EncryptDict     pEncryptDict;

    Encoder         pDefEncoder;

    unsigned int    nPagePerPages;
    unsigned int    nCurPageNum;
	
	// Счетчик CMap кодировок
	unsigned int    nCMapCount;
	Encoder         pLastCMapEncoder;

	// Темповая папка, куда скидываются все временный файлы
	wchar_t         wsTempDir[MAX_PATH];

    // буфер для сохранения в потоке
    Stream          pStream;

} DocRec;

typedef struct TDocRec *Doc;

#include "PageLabel.h"

//----------------- определения вспомогательных функций ------------------------------------------

static unsigned long WriteHeader            (Doc pPDF, Stream pStream)
{
	unsigned int nIndex = (int)pPDF->ePdfVersion;

    if ( OK != StreamWriteStr( pStream, VERSION_STR[ nIndex ] ) )
		return pPDF->oError.nErrorNo;

    return OK;
}

static unsigned long PrepareTrailer         (Doc pPDF)
{
	if ( OK != DictAdd( pPDF->pTrailer, "Root", pPDF->pCatalog ) )
		return pPDF->oError.nErrorNo;

	if ( OK != DictAdd( pPDF->pTrailer, "Info", pPDF->pInfo ) )
        return pPDF->oError.nErrorNo;

    return OK;
}
static void          FreeEncoderList        (Doc pPDF);
static void          FreeFontDefList        (Doc pPDF);
static void          CleanupFontDefList     (Doc pPDF);
static Dict          GetInfo                (Doc pPDF);
unsigned long        DocPrepareEncryption   (Doc pPDF);
static unsigned long InternalSaveToStream   (Doc pPDF, Stream pStream)
{
    unsigned long nRet = OK;

	// пишем заголовок
    if ( OK != ( nRet = WriteHeader( pPDF, pStream ) ) )
        return nRet;

    // подготовка trailer
    if ( OK != ( nRet = PrepareTrailer( pPDF ) ) )
        return nRet;

    // подготовка шифрования
    if ( pPDF->bEncryptOn ) 
	{
		Encrypt pEncrypt = EncryptDictGetAttr( pPDF->pEncryptDict );

        if ( OK != ( nRet = DocPrepareEncryption( pPDF ) ) )
            return nRet;

        if ( OK != ( nRet = XrefWriteToStream( pPDF->pXref, pStream, pEncrypt ) ) )
            return nRet;
    } 
	else 
	{
		if ( OK != ( nRet = XrefWriteToStream( pPDF->pXref, pStream, NULL ) ) )
            return nRet;
    }

    return OK;
}

static const char*   LoadType1FontFromStream(Doc pPDF, Stream pAfmData, Stream pPfmData, short nType = 0);
static const char*   LoadTTFontFromStream   (Doc pPDF, BOOL bEmbedding, const wchar_t *wsFileName, const char *sEncodingName = NULL, const char *sPrefix = NULL);
static const char*   LoadTTFontFromStream2  (Doc pPDF, unsigned int nIndex, BOOL bEmbedding, const wchar_t *wsFileName);
static unsigned int  DocGetTempFile         (Doc pPDF, CString *pwsTempFileName)
{
	unsigned int unRet = 0;
	// Создаем временный файл
	wchar_t wsTempFilePath[MAX_PATH];
	if ( '\0' == pPDF->wsTempDir[0] )
	{
		wchar_t wsTempPath[MAX_PATH];
		if ( 0 == GetTempPath( MAX_PATH, wsTempPath ) )
			return 0;

		unRet = GetTempFileName( wsTempPath, _T("PDF"), 0, wsTempFilePath );
	}
	else
	{
		unRet = GetTempFileName( pPDF->wsTempDir, _T("PDF"), 0, wsTempFilePath );
	}

	*pwsTempFileName = CString( wsTempFilePath );

	return unRet;
}
static unsigned long InitCMapEncoding       (Encoder pEncoder)
{
	//CMapEncoderAttr pAttr = (CMapEncoderAttr)pEncoder->pAttr;
	pEncoder->eType = EncoderTypeToUnicode;
	
	return OK;
}


//----------------- определения основных функций --------------------------------------------------
BOOL          HasDoc                 (Doc pPDF);
Pages         DocAddPagesTo          (Doc pPDF, Pages pParent);
BOOL          DocValidate            (Doc pPDF);
unsigned long GetError               (Doc pPDF);
//----------------- Функции для работы с ToUnicode -------------------------------------------------
PToUnicode    DocFindToUnicode      (Doc pPDF, const char *sToUnicodeName)
{
	List pList = pPDF->pToUnicodeList;

	for ( unsigned int nIndex = 0; nIndex < pList->nCount; nIndex++) 
	{
		PToUnicode pToUnicode = (PToUnicode)ListItemAt( pList, nIndex );

		if ( 0 == UtilsStrCmp( sToUnicodeName, pToUnicode->sName ) ) 
		{
			return pToUnicode;
		}
	}

	return NULL;
}
PToUnicode    DocFindToUnicode      (Doc pPDF, unsigned short *pUnicode, unsigned int unSize)
{
	List pList = pPDF->pToUnicodeList;

	for ( unsigned int nIndex = 0; nIndex < pList->nCount; nIndex++) 
	{
		PToUnicode pToUnicode = (PToUnicode)ListItemAt( pList, nIndex );

		if ( ToUnicodeBuilt == pToUnicode->eType )
		{
			// Проверяем все ли значения из pUnicode есть в данной кодировке ToUnicode
			bool bGood = true;
			for ( unsigned int unCharIndex = 0; unCharIndex < unSize; unCharIndex++ )
			{
				if ( 0xFFFF == ToUnicodeToCode( pToUnicode, pUnicode[unCharIndex] ) )
				{
					bGood = false;
					break;
				}
			}

			if ( bGood )
				return pToUnicode;
		}
	}

	return NULL;
}
PToUnicode    DocFindToUnicode      (Doc pPDF, BYTE *pBuffer, unsigned int unSize)
{
	List pList = pPDF->pToUnicodeList;

	for ( unsigned int nIndex = 0; nIndex < pList->nCount; nIndex++) 
	{
		PToUnicode pToUnicode = (PToUnicode)ListItemAt( pList, nIndex );

		if ( ToUnicodeStream == pToUnicode->eType )
		{
			// Проверяем совпадение по чексумме буффера
			CRC32 oCRC;
			oCRC.ProcessCRC( (void*)pBuffer, unSize );

			ToUnicodeStreamAttr pAttr = (ToUnicodeStreamAttr)pToUnicode->pAttr;
			if ( pAttr->pStream->nCheckSum == oCRC.m_nCRC32 )
				return pToUnicode;
		}
	}

	return NULL;
}
PToUnicode    GetToUnicode          (Doc pPDF, const char *sToUnicodeName)
{
	if ( !HasDoc( pPDF ) )
		return NULL;

	PToUnicode pToUnicode = DocFindToUnicode( pPDF, sToUnicodeName );

	return pToUnicode;
}
PToUnicode    GetToUnicode          (Doc pPDF, unsigned short *pUnicode, unsigned int unSize)
{
	if ( !HasDoc( pPDF) || NULL == pUnicode || 0 == unSize )
		return NULL;

	// возможно кодировка, в которой есть все данные символы уже присутствует
	PToUnicode pToUnicode = DocFindToUnicode( pPDF, pUnicode, unSize );
	unsigned long nRet = OK;

	if ( NULL == pToUnicode )
	{
		// Попробуем дополнить последнюю кодировку
		pToUnicode = pPDF->pLastToUnicodeBuilt;
		if ( NULL != pToUnicode && ToUnicodeUpdateUnicodeArray( pToUnicode, pUnicode, unSize ) )
			return pToUnicode;

		// Дополнить не получилось, значит состовляем новую кодировку
		pPDF->nToUnicodeBuiltCount++;

		char sToUnicodeBuiltName[LIMIT_MAX_NAME_LEN + 1];
		sprintf( sToUnicodeBuiltName, "CMap%d", pPDF->nToUnicodeBuiltCount );

		pToUnicode = ToUnicodeBuiltNew( pPDF->oMMgr, sToUnicodeBuiltName, 8 );
		if ( !pToUnicode )
		{
			CheckError( &pPDF->oError );
			return NULL;
		}

		// Обычные шрифты в PDF поддерживают до 256 символов. ( 0 символ зарезервирован )
		unSize = ( unSize >= 256 ? 255 : unSize );

		ToUnicodeBuiltAttr pAttr = (ToUnicodeBuiltAttr)pToUnicode->pAttr;
		pAttr->nFirstChar = 0;
		pAttr->nLastChar  = 0;
		// Внутри данной функции сразу идет проверка на дублирование символов
		ToUnicodeUpdateUnicodeArray( pToUnicode, pUnicode, unSize );

		if ( OK != ( nRet = ListAdd( pPDF->pToUnicodeList, pToUnicode ) ) ) 
		{
			ToUnicodeFree( pToUnicode );
			RaiseError( &pPDF->oError, nRet, 0);
			return NULL;
		}

		pPDF->pLastToUnicodeBuilt = pToUnicode;
		pPDF->pCurToUnicode       = pToUnicode;
	}
	return pToUnicode;
}
PToUnicode    GetToUnicode          (Doc pPDF, BYTE *pBuffer, unsigned int unSize)
{
	if ( !HasDoc( pPDF) || NULL == pBuffer || 0 == unSize )
		return NULL;

	// возможно кодировка, в которой есть все данные символы уже присутствует
	PToUnicode pToUnicode = DocFindToUnicode( pPDF, pBuffer, unSize );
	unsigned long nRet = OK;

	if ( NULL == pToUnicode )
	{
		pPDF->nToUnicodeStreamCount++;

		char sToUnicodeBuiltName[LIMIT_MAX_NAME_LEN + 1];
		sprintf( sToUnicodeBuiltName, "Stream%d", pPDF->nToUnicodeStreamCount );

		pToUnicode = ToUnicodeStreamNew( pPDF->oMMgr, pBuffer, unSize, sToUnicodeBuiltName );
		if ( !pToUnicode )
		{
			CheckError( &pPDF->oError );
			return NULL;
		}

		if ( OK != ( nRet = ListAdd( pPDF->pToUnicodeList, pToUnicode ) ) ) 
		{
			ToUnicodeFree( pToUnicode );
			RaiseError( &pPDF->oError, nRet, 0);
			return NULL;
		}

		pPDF->pCurToUnicode       = pToUnicode;
	}
	return pToUnicode;
}
PToUnicode    DocGetCurrentToUnicode(Doc pPDF)
{
	if ( !HasDoc( pPDF ) )
		return NULL;

	return pPDF->pCurToUnicode;
}

static void   FreeToUnicodeList     (Doc pPDF)
{
    List pList = pPDF->pToUnicodeList;

	for ( unsigned int nIndex = 0; nIndex < pList->nCount; nIndex++) 
	{
        PToUnicode pToUnicode = (PToUnicode)ListItemAt( pList, nIndex );

		ToUnicodeFree( pToUnicode );
    }

    ListFree( pList );

    pPDF->pToUnicodeList = NULL;
}
PToUnicode    FindToUnicodeForString(Doc pPDF, BSTR bsSrc)
{
	CStringW sSrc( bsSrc );
	unsigned short *aUnicodeMap = new unsigned short[sSrc.GetAllocLength() + 1];
	for ( int nIndex = 0; nIndex < sSrc.GetLength(); nIndex++ )
	{
		aUnicodeMap[nIndex] = (unsigned short)sSrc.GetAt(nIndex);
	}
	aUnicodeMap[sSrc.GetLength()] = 0xFFFF;

	unsigned int unSize = sSrc.GetLength();

	PToUnicode pToUnicode = GetToUnicode( pPDF, aUnicodeMap, unSize );

	delete []aUnicodeMap;

	return pToUnicode;
}
//----------------- Функции для работы с кодировками -----------------------------------------------
Encoder       DocFindEncoder      (Doc pPDF, const char *sEncodingName, unsigned short *pUnicode = NULL, int nCount = 0)
{
	if ( NULL == pUnicode )
	{
		List pList = pPDF->pEncoderList;

		for ( unsigned int nIndex = 0; nIndex < pList->nCount; nIndex++) 
		{
			Encoder pEncoder = (Encoder)ListItemAt( pList, nIndex );

			if ( 0 == UtilsStrCmp( sEncodingName, pEncoder->sName ) ) 
			{
				// если кодировка не инициализирована, тогда вызываем функцию InitFn

				if ( EncoderTypeUninitialized == pEncoder->eType ) 
				{
					if ( !pEncoder->pInitFn || OK != pEncoder->pInitFn( pEncoder ) )
						return NULL;
				}

				return pEncoder;
			}
		}

		return NULL;
	}
	else
	{
		// Тут надо сделать поиск кодировки с данным pUnicode
		return NULL;
	}
}
unsigned long DocRegisterEncoder  (Doc pPDF, Encoder pEncoder)
{
    unsigned long nRet = OK;

    if ( !pEncoder )
		return SetError( &pPDF->oError, AVS_OFFICEPDFWRITER_ERROR_INVALID_OBJECT, 0);

    if ( NULL != DocFindEncoder( pPDF, pEncoder->sName ) ) 
	{
        EncoderFree( pEncoder );
		return SetError( &pPDF->oError, AVS_OFFICEPDFWRITER_ERROR_DUPLICATE_REGISTRATION, 0 );
    }

	if ( OK != ( nRet = ListAdd( pPDF->pEncoderList, pEncoder ) ) ) 
	{
        EncoderFree( pEncoder );
		return SetError( &pPDF->oError, nRet, 0 );
    }

    return OK;
}
Encoder       GetEncoder          (Doc pPDF, const char *sEncodingName, unsigned short *pUnicode = NULL, int nCount = 0)
{
	if ( NULL == pUnicode )
	{
		if ( !HasDoc( pPDF ) )
			return NULL;

		Encoder pEncoder = DocFindEncoder( pPDF, sEncodingName );

		unsigned long nRet = OK;

		if ( !pEncoder ) 
		{
			pEncoder = BasicEncoderNew( pPDF->oMMgr, sEncodingName );

			if ( !pEncoder ) 
			{
				CheckError( &pPDF->oError );
				return NULL;
			}

			if ( OK != ( nRet = ListAdd( pPDF->pEncoderList, pEncoder ) ) ) 
			{
				EncoderFree( pEncoder );
				RaiseError( &pPDF->oError, nRet, 0);
				return NULL;
			}
		}

		return pEncoder;
	}
	else
	{
		if ( !HasDoc( pPDF) )
			return NULL;

		// возможно кодировка, в которой есть все данные символы уже присутствует
		Encoder pEncoder = DocFindEncoder( pPDF, sEncodingName, pUnicode, nCount );
		unsigned long nRet = OK;

		if ( !pEncoder )
		{
			pPDF->nCMapCount++;
			char sEncodingName[100];
			sprintf( sEncodingName, "CMap%d", pPDF->nCMapCount );

			pEncoder = CMapEncoderNew( pPDF->oMMgr, sEncodingName, &InitCMapEncoding  );
			if ( !pEncoder )
			{
				CheckError( &pPDF->oError );
				return NULL;
			}
			pEncoder->eType = EncoderTypeToUnicode;
			CMapEncoderInitAttr( pEncoder );

			// Обычные шрифты в PDF поддерживают до 256 символов. ( 0 символ зарезервирован )
			nCount = ( nCount >= 256 ? 255 : nCount ); 

			UtilsUIntArrayClearRepeat( &pUnicode, &nCount );

			UnicodeMapRec *aUnicodeMap = new UnicodeMapRec[nCount + 2]; // первый и последний символы зарезервированы
			aUnicodeMap[0].nCode    = 0;
			aUnicodeMap[0].nUnicode = 0x0000;
			for ( int nIndex = 1; nIndex <= nCount; nIndex++ )
			{
				aUnicodeMap[nIndex].nCode    = nIndex;
				aUnicodeMap[nIndex].nUnicode = pUnicode[nIndex - 1];
			}
			aUnicodeMap[nCount + 1].nCode    = nCount + 1;
			aUnicodeMap[nCount + 1].nUnicode = 0xffff; // Таким образом мы обозначаем конец массива

			CMapEncoderSetUnicodeArray( pEncoder, aUnicodeMap );
			delete []aUnicodeMap;

			CMapEncoderAttr pAttr = (CMapEncoderAttr)pEncoder->pAttr;
			pAttr->nFirstChar = 0;
			pAttr->nLastChar  = nCount;

			if ( OK != ( nRet = ListAdd( pPDF->pEncoderList, pEncoder ) ) ) 
			{
				EncoderFree( pEncoder );
				RaiseError( &pPDF->oError, nRet, 0);
				return NULL;
			}

			pPDF->pLastCMapEncoder = pEncoder;

		}
		return pEncoder;
	}
}
Encoder       DocGetCurrentEncoder(Doc pPDF)
{
    if ( !HasDoc( pPDF ) )
        return NULL;

	return pPDF->pCurEncoder;
}
unsigned long DocSetCurrentEncoder(Doc pPDF, const char *sEncodingName)
{
    if ( !HasDoc( pPDF ) )
        return GetError( pPDF );

    Encoder pEncoder = GetEncoder( pPDF, sEncodingName );
    pPDF->pCurEncoder = pEncoder;

    if ( !pPDF )
        return GetError( pPDF );

    return OK;
}
static void   FreeEncoderList     (Doc pPDF)
{
    List pList = pPDF->pEncoderList;

	for ( unsigned int nIndex = 0; nIndex < pList->nCount; nIndex++) 
	{
        Encoder pEncoder = (Encoder)ListItemAt( pList, nIndex );

        EncoderFree( pEncoder );
    }

    ListFree( pList );

    pPDF->pEncoderList = NULL;
}
Encoder       FindEncoderForString(Doc pPDF, BSTR bsSrc, BOOL *pbNew)
{
	*pbNew = FALSE;
	// Среди всех кодировок, имеющихся в документе, ищем ту, в которой можно написать заданную строку
	List pEncoderList = pPDF->pEncoderList;
	
	for ( int nIndex = 0; nIndex < pEncoderList->nCount; nIndex++ )
	{
		Encoder pCurEncoder = (Encoder)ListItemAt( pEncoderList, nIndex );
		if ( EncoderTypeToUnicode != pCurEncoder->eType )
			continue;
		BOOL bResult = FALSE;
		CString sDst;
		if ( NULL != pCurEncoder )
			bResult = EncoderWCharToString( pCurEncoder, bsSrc, &sDst );

		if ( bResult )
		{
			return pCurEncoder;
		}
	}
	// Среди имеющихся кодировок нужной кодировки нет, поэтому попробуем дополнить pLastCMapEncoder
	CStringW sSrc( bsSrc );
	unsigned short *aUnicodeMap = new unsigned short[sSrc.GetAllocLength() + 1];
	for ( int nIndex = 0; nIndex < sSrc.GetLength(); nIndex++ )
	{
		aUnicodeMap[nIndex] = (unsigned short)sSrc.GetAt(nIndex);
	}
	aUnicodeMap[sSrc.GetLength()] = 0xFFFF;

	int nCount = sSrc.GetLength();
	UtilsUIntArrayClearRepeat( &aUnicodeMap, &nCount );
	if ( pPDF->pLastCMapEncoder && CMapEncoderUpdateUnicodeArray( pPDF->pLastCMapEncoder, aUnicodeMap ) )
	{
		delete []aUnicodeMap;
		return pPDF->pLastCMapEncoder;
	}
	else
	{
		Encoder pNewEncoder = GetEncoder( pPDF, "", aUnicodeMap, nCount );
		delete []aUnicodeMap;
		*pbNew = TRUE;
		return pNewEncoder;
	}
}
//----------------- Функции для работы с фонтами ---------------------------------------------------
//----------------- FontDef ------------------------------------------------------------------------
static void FreeFontDefList   (Doc pPDF)
{
	List pList = pPDF->pFontDefList;

	for ( unsigned int nIndex = 0; nIndex < pList->nCount; nIndex++) 
	{
        FontDef pDef = (FontDef)ListItemAt( pList, nIndex );
        FontDefFree( pDef );
    }

    ListFree( pList );
    pPDF->pFontDefList = NULL;
}
static void CleanupFontDefList(Doc pPDF)
{
    List pList = pPDF->pFontDefList;
	for ( unsigned int nIndex = 0; nIndex < pList->nCount; nIndex++) 
	{
        FontDef pDef = (FontDef)ListItemAt( pList, nIndex );
        FontDefCleanup( pDef );
    }
}
FontDef       DocFindFontDef     (Doc pPDF, const char *sFontName, const char *sToUnicodeName = NULL)
{
	List pList = pPDF->pFontDefList;
	for ( unsigned int nIndex = 0; nIndex < pList->nCount; nIndex++) 
	{
		FontDef       pDef  = (FontDef)ListItemAt( pList, nIndex );

		if ( FontDefTrueType == pDef->eType && NULL != sToUnicodeName )
		{
			TTFontDefAttr pAttr = (TTFontDefAttr)pDef->pAttr;

			if ( 0 == UtilsStrCmp( sFontName, pDef->sBaseFont) && 0 == UtilsStrCmp( sToUnicodeName, pAttr->sToUnicodeName ) ) 
			{
				if ( FontDefUninitialized == pDef->eType ) 
				{
					if ( !pDef->pInitFn || OK != pDef->pInitFn( pDef ) )
						return NULL;
				}
				return pDef;
			}
		}
		else
		{
			if ( 0 == UtilsStrCmp( sFontName, pDef->sBaseFont ) ) 
			{
				if ( FontDefUninitialized == pDef->eType ) 
				{
					if ( !pDef->pInitFn || OK != pDef->pInitFn( pDef ) )
						return NULL;
				}
				return pDef;
			}

		}
	}
	return NULL;
}
unsigned long DocRegisterFontDef (Doc pPDF, FontDef pFontDef)
{
    unsigned long nRet = OK;

    if ( !pFontDef )
        return SetError( &pPDF->oError, AVS_OFFICEPDFWRITER_ERROR_INVALID_OBJECT, 0);

    if ( NULL != DocFindFontDef( pPDF, pFontDef->sBaseFont ) ) 
	{
        FontDefFree( pFontDef );
        return SetError( &pPDF->oError, AVS_OFFICEPDFWRITER_ERROR_DUPLICATE_REGISTRATION, 0 );
    }

	if ( OK != ( nRet = ListAdd( pPDF->pFontDefList, pFontDef ) ) ) 
	{
        FontDefFree( pFontDef );
        return SetError( &pPDF->oError, nRet, 0);
    }

    return OK;
}
FontDef       GetFontDef         (Doc pPDF, const char *sFontName, const char *sToUnicode = NULL)
{
    unsigned long nRet = OK;
   
    if ( !HasDoc( pPDF ) )
        return NULL;

    FontDef pDef = DocFindFontDef( pPDF, sFontName, sToUnicode );

    if ( !pDef ) 
	{
        pDef = Base14FontDefNew( pPDF->oMMgr, sFontName);

        if ( !pDef )
		{
			pDef = BaseFontDefNew( pPDF->oMMgr, sFontName );
			if ( !pDef )
				return NULL;
		}

		if ( OK != ( nRet = ListAdd( pPDF->pFontDefList, pDef ) ) ) 
		{
            FontDefFree( pDef );
			RaiseError( &pPDF->oError, nRet, 0 );
            pDef = NULL;
        }
    }
    return pDef;
}
//----------------- Font ----------------------------------------------------------------------------
FontDict    DocFindFont           (Doc pPDF, const char *sFontName, const char *sEncodingName, const char *sToUnicodeName = NULL)
{
    FontDict pFont = NULL;

	for ( unsigned int nIndex = 0; nIndex < pPDF->pFontManager->nCount; nIndex++) 
	{
		pFont = (FontDict)ListItemAt( pPDF->pFontManager, nIndex);
		FontAttr pAttr = (FontAttr)pFont->pAttr;

		// Строка sToUnicodeName может быть NULL. Функция UtilsStrCmp безопасно работает в данном случае
		if ( NULL != pAttr->pToUnicode )
		{
			if ( 0 == UtilsStrCmp( pAttr->pFontDef->sBaseFont, sFontName ) && 0 == UtilsStrCmp( pAttr->pEncoder->sName, sEncodingName ) && 0 == UtilsStrCmp( pAttr->pToUnicode->sName, sToUnicodeName ) )
				return pFont;
		}
		else if ( NULL == pAttr->pToUnicode && NULL == sToUnicodeName )
		{
			if ( 0 == UtilsStrCmp( pAttr->pFontDef->sBaseFont, sFontName ) && 0 == UtilsStrCmp( pAttr->pEncoder->sName, sEncodingName ) )
				return pFont;
		}
	}

	return NULL;
}
FontDict    GetFont               (Doc pPDF, const char *sFontName, const char *sEncodingName, const char *sToUnicodeName = NULL, unsigned short *pUnicode = NULL, int nCount = 0)
{
    if ( !HasDoc( pPDF ) )
        return NULL;

	if ( !sFontName ) 
	{
		RaiseError( &pPDF->oError, AVS_OFFICEPDFWRITER_ERROR_INVALID_FONT_NAME, 0 );
        return NULL;
    }

    FontDef    pFontDef   = NULL;
    Encoder    pEncoder   = NULL;
    FontDict   pFont      = NULL;
	PToUnicode pToUnicode = NULL;

	// если кодировка не определена, тогда ищем стандартную кодировку для данного фонта
	if ( !sEncodingName ) 
	{
		pFontDef = GetFontDef( pPDF, sFontName, sToUnicodeName );

		if ( pFontDef ) 
		{
			if ( FontDefTrueType == pFontDef->eType )
				pEncoder = GetEncoder( pPDF, ENCODING_FONT_SPECIFIC );
			else if ( FontDefType1 == pFontDef->eType )
			{
				Type1FontDefAttr pAttr = (Type1FontDefAttr)pFontDef->pAttr;

				if ( FontDefType1 == pFontDef->eType && 0 == UtilsStrCmp( pAttr->sEncodingScheme, ENCODING_FONT_SPECIFIC ) )
					pEncoder = GetEncoder( pPDF, ENCODING_FONT_SPECIFIC );
				else
					pEncoder = GetEncoder( pPDF, ENCODING_WIN_ANSI/*ENCODING_STANDARD*/, pUnicode, nCount );
			}
		} 
		else 
		{
			CheckError( &pPDF->oError );
			return NULL;
		}

		pFont = DocFindFont( pPDF, sFontName, pEncoder->sName, sToUnicodeName );
	} 
	else 
        pFont = DocFindFont( pPDF, sFontName, sEncodingName, sToUnicodeName );

    if ( pFont )
        return pFont;

    if ( !pFontDef ) 
	{
        pFontDef = GetFontDef( pPDF, sFontName, sToUnicodeName );

        if ( !pFontDef ) 
		{
            CheckError( &pPDF->oError );
            return NULL;
        }
    }

    if ( !pEncoder ) 
	{
        pEncoder = GetEncoder( pPDF, sEncodingName, pUnicode, nCount );
        if ( !pEncoder )
            return NULL;
    }

	if ( !pToUnicode )
	{
		pToUnicode = GetToUnicode( pPDF, sToUnicodeName );
	}

	switch ( pFontDef->eType ) 
	{
        case FontDefType1:
			pFont = Type1FontNew( pPDF->oMMgr, pFontDef, pEncoder, pPDF->pXref, pToUnicode );
            if ( pFont )
				ListAdd( pPDF->pFontManager, pFont); 
            break;
        case FontDefTrueType:
            if ( EncoderTypeDoubleByteBuilt == pEncoder->eType )
				pFont = Type0FontNew( pPDF->oMMgr, pFontDef, pEncoder, pPDF->pXref, pToUnicode );
            else
				pFont = TTFontNew( pPDF->oMMgr, pFontDef, pEncoder, pPDF->pXref, pToUnicode);
            if ( pFont )
                ListAdd( pPDF->pFontManager, pFont); 
            break;
        case FontDefCID:
            pFont = Type0FontNew( pPDF->oMMgr, pFontDef, pEncoder, pPDF->pXref, pToUnicode );
            if ( pFont )
                ListAdd( pPDF->pFontManager, pFont); 
            break;
		case FontBase:
			pFont = BaseFontNew( pPDF->oMMgr, pFontDef, pEncoder, pPDF->pXref, pToUnicode );
			if ( pFont )
				ListAdd( pPDF->pFontManager, pFont );
			break;
        default:
			RaiseError( &pPDF->oError, AVS_OFFICEPDFWRITER_ERROR_UNSUPPORTED_FONT_TYPE, 0);
            return NULL;
    }

    if ( !pFont )
		CheckError( &pPDF->oError );

#ifndef _DEBUG
	if ( pFont && ( pPDF->nCompressionMode & COMP_METADATA ) )
        pFont->nFilter = STREAM_FILTER_FLATE_DECODE;
#endif

    return pFont;
}
const char* LoadType1FontFromFile (Doc pPDF, const wchar_t *wsAFMFileName, const wchar_t *wsDataFileName, short nType = 0)
{
    Stream pAFM = NULL;
    Stream pData = NULL;
    const char *sRet;

    if ( !HasDoc( pPDF ) )
        return NULL;

    // создаем поток
	pAFM = FileReaderNew( pPDF->oMMgr, wsAFMFileName );

    if ( wsDataFileName )
		pData = FileReaderNew( pPDF->oMMgr, wsDataFileName );

    if ( StreamValidate( pAFM ) && ( !wsDataFileName || StreamValidate( pData ) ) ) 
        sRet = LoadType1FontFromStream( pPDF, pAFM, pData, nType );
    else
        sRet = NULL;

    // удаляем поток
    if ( pAFM )
        StreamFree( pAFM );

    if ( pData )
        StreamFree( pData );

    if ( !sRet )
		CheckError( &pPDF->oError );

    return sRet;
}
static const char* LoadType1FontFromStream  (Doc pPDF, Stream pAFMData, Stream pPFMData, short nType)
{
    if ( !HasDoc( pPDF ) )
        return NULL;

	FontDef pDef = Type1FontDefLoad( pPDF->oMMgr, pAFMData, pPFMData, nType );
    if ( pDef ) 
	{
        FontDef pTempDef = DocFindFontDef( pPDF, pDef->sBaseFont );
        if ( pTempDef ) 
		{
            FontDefFree( pDef );
			SetError( &pPDF->oError, AVS_OFFICEPDFWRITER_ERROR_FONT_EXISTS, 0);
			return pTempDef->sBaseFont;
        }
		if ( OK != ListAdd( pPDF->pFontDefList, pDef ) ) 
		{
            FontDefFree( pDef );
            return NULL;
        }
    }
    return pDef->sBaseFont;
}
const char* LoadTTFontFromFile    (Doc pPDF, const wchar_t *wsFileName, BOOL bEmbedding, const char* sEncodingName = NULL, const char *sPrefix = NULL)
{
    if ( !HasDoc( pPDF ) )
		return NULL;

	const char *sRet = LoadTTFontFromStream( pPDF, bEmbedding, wsFileName, sEncodingName, sPrefix );

    if ( !sRet )
        CheckError( &pPDF->oError );

    return sRet;
}
static const char* LoadTTFontFromStream     (Doc pPDF, BOOL bEmbedding, const wchar_t *wsFileName, const char *sEncodingName, const char *sPrefix)
{
	FontDef pDef = TTFontDefLoad( pPDF->oMMgr, wsFileName, bEmbedding, sEncodingName );
    if ( pDef ) 
	{
		if ( NULL != sPrefix )
			strcat( pDef->sBaseFont, sPrefix );

		FontDef pTempDef = DocFindFontDef( pPDF, pDef->sBaseFont, sEncodingName );
		if ( pTempDef ) 
		{
			FontDefFree( pDef );
			SetError( &pPDF->oError, AVS_OFFICEPDFWRITER_ERROR_FONT_EXISTS, 0);
			return pTempDef->sBaseFont;
		}

		if ( OK != ListAdd( pPDF->pFontDefList, pDef ) ) 
		{
            FontDefFree( pDef );
            return NULL;
        }
    } 
	else
        return NULL;

    if ( bEmbedding ) 
	{
		if ( 0 == pPDF->aTTFontTag[0] ) 
		{
			UtilsMemCpy( pPDF->aTTFontTag, (BYTE*)"BAAAAA", 6);
        } 
		else 
		{
            for ( unsigned int nIndex = 0; nIndex <= 5; nIndex++) 
			{
				pPDF->aTTFontTag[nIndex] += 1;
                if ( pPDF->aTTFontTag[nIndex] > 'Z')
					pPDF->aTTFontTag[nIndex] = 'A';
                else
                    break;
            }
        }
        TTFontDefSetTagName( pDef, (char*)pPDF->aTTFontTag );
    }

	return pDef->sBaseFont;
}
const char* LoadTTFontFromFile2   (Doc pPDF, const wchar_t *wsFileName, unsigned int nIndex, BOOL bEmbedding)
{
    if ( !HasDoc( pPDF ) )
        return NULL;

    const char *sRet = LoadTTFontFromStream2( pPDF, nIndex, bEmbedding, wsFileName );

    if ( !sRet )
		CheckError( &pPDF->oError );

    return sRet;
}
static const char* LoadTTFontFromStream2    (Doc pPDF, unsigned int nIndex, BOOL bEmbedding, const wchar_t *wsFileName)
{
	FontDef pDef = TTFontDefLoad2( pPDF->oMMgr, wsFileName, nIndex, bEmbedding );
    if ( pDef ) 
	{
		FontDef pTempDef = DocFindFontDef( pPDF, pDef->sBaseFont );
        if ( pTempDef ) 
		{
            FontDefFree( pDef );
			SetError( &pPDF->oError, AVS_OFFICEPDFWRITER_ERROR_FONT_EXISTS, 0);
            return NULL;
        }

		if ( OK != ListAdd( pPDF->pFontDefList, pDef ) ) 
		{
            FontDefFree( pDef );
            return NULL;
        }
    } 
	else
        return NULL;

    if ( bEmbedding ) 
	{
		if ( 0 == pPDF->aTTFontTag[0] ) 
			UtilsMemCpy( pPDF->aTTFontTag, (BYTE*)"AVSAAA", 6);
        else 
		{
           for ( unsigned int nIndex = 5; nIndex >= 0; nIndex--) 
		   {
                pPDF->aTTFontTag[nIndex] += 1;
                if ( pPDF->aTTFontTag[nIndex] > 'Z' )
                    pPDF->aTTFontTag[nIndex] = 'A';
                else
                    break;
            }
        }
		TTFontDefSetTagName( pDef, (char*)pPDF->aTTFontTag );
    }

    return pDef->sBaseFont;
}


//----------------- Images --------------------------------------------------------------------------
ImageDict LoadRawImageFromFile (Doc pPDF, const wchar_t *wsFileName, unsigned int nWidth, unsigned int nHeight, ColorSpace eColorSpace)
{
    ImageDict pImage = NULL;

    if ( !HasDoc( pPDF ) )
        return NULL;

    // создаем файловый поток
	Stream pImageData = FileReaderNew( pPDF->oMMgr, wsFileName );

    if ( StreamValidate( pImageData ) )
        pImage = ImageLoadRawImage( pPDF->oMMgr, pImageData, pPDF->pXref, nWidth, nHeight, eColorSpace);
    else
        pImage = NULL;

    // удаялем поток
    StreamFree( pImageData );

    if ( !pImage )
        CheckError( &pPDF->oError );

#ifndef _DEBUG
    if ( pPDF->nCompressionMode & COMP_IMAGE )
        pImage->nFilter = STREAM_FILTER_FLATE_DECODE;
#endif

    return pImage;
}

ImageDict LoadRawImageFromMem  (Doc pPDF, const BYTE *pBuffer,   unsigned int nWidth, unsigned int nHeight, ColorSpace eColorSpace, unsigned int nBitsPerComponent, BOOL bAlpha = FALSE, const BYTE *pAlphaBuffer = NULL)
{
    if ( !HasDoc( pPDF ))
        return NULL;

    ImageDict pImage = ImageLoadRawImageFromMem( pPDF->oMMgr, pBuffer, pPDF->pXref, nWidth, nHeight, eColorSpace, nBitsPerComponent, bAlpha, pAlphaBuffer );

    if ( !pImage )
        CheckError( &pPDF->oError );

#ifndef _DEBUG
    if ( pPDF->nCompressionMode & COMP_IMAGE )
        pImage->nFilter = STREAM_FILTER_FLATE_DECODE;
#endif

    return pImage;
}
ImageDict LoadJbig2ImageFromInt(Doc pPDF, IUnknown **pInterface, unsigned int nWidth, unsigned int nHeight, unsigned int unImageCheckSum, BOOL bAlpha = FALSE, const BYTE *pAlphaBuffer = NULL, unsigned int unAlphaCheckSum = 0)
{
	ImageDict pImage = NULL;

	if ( !(HasDoc(pPDF)) )
		return NULL;

	// Создаем временный файл
	CString wsTempFile;
	if ( 0 == DocGetTempFile( pPDF, &wsTempFile ) )
	{
		SetError( pPDF->oMMgr->oError, AVS_OFFICEPDFWRITER_ERROR_FILE_WRITE_ERROR, errno );
		return NULL;
	}

	// Записываем во временный файл картинку в формате JBig2
	JBig2::IJBig2File *pJBig2 = NULL;
	::CoCreateInstance( __uuidof( JBig2::CJBig2File ), NULL, CLSCTX_ALL, __uuidof( JBig2::IJBig2File ), (void**)&pJBig2 );

	if ( !pJBig2 )
		return NULL;

	BSTR bsTempFile = ::SysAllocString( wsTempFile );
	BSTR bsOptions  = ::SysAllocString( _T("<JBig2-LoadOptions><PDFMode/></JBig2-LoadOptions>") );
	pJBig2->InterfaceToJBig2( pInterface, bsTempFile, bsOptions );

	::SysFreeString( bsTempFile );
	::SysFreeString( bsOptions );

	RELEASEINTERFACE( pJBig2 );

	pImage = ImageLoadJBig2Image( pPDF->oMMgr, wsTempFile, pPDF->pXref, nWidth, nHeight, unImageCheckSum );

	// Декодирование во Flate картинок в формате JBig2 отключено, из-за того что Acrobat
	// может плохо прочитать такие картинки.
#ifndef _DEBUG
	//if ( pPDF->nCompressionMode & COMP_IMAGE )
	//	pImage->nFilter |= STREAM_FILTER_FLATE_DECODE;
#endif

	return pImage;
}
ImageDict LoadJpegImageFromFile(Doc pPDF, const wchar_t *wsFileName)
{
    ImageDict pImage = NULL;

    if ( !HasDoc( pPDF ) )
        return NULL;

    // Создаем файловый поток
	Stream pImageData = FileReaderNew( pPDF->oMMgr, wsFileName );

    if ( StreamValidate(pImageData) )
        pImage = ImageLoadJpegImage( pPDF->oMMgr, pImageData, pPDF->pXref );
    else
        pImage = NULL;

    // освобождаем файловый поток
    StreamFree( pImageData );

    if ( !pImage )
        CheckError( &pPDF->oError );

    return pImage;
}
ImageDict LoadJpxImageFromFile (Doc pPDF, const wchar_t *wsFileName, long nOpacityFlag)
{
	ImageDict pImage = NULL;

	if ( !(HasDoc(pPDF) ) )
		return NULL;

	// Создаем файловый поток 
	Stream pImageData = FileReaderNew( pPDF->oMMgr, wsFileName );
	if ( StreamValidate( pImageData ) )
		pImage = ImageLoadJpxImage( pPDF->oMMgr, pImageData, pPDF->pXref, nOpacityFlag );
	else
		pImage = NULL;

	// Освобождаем файлоый поток
	StreamFree( pImageData );

	if ( !pImage )
		CheckError( &pPDF->oError );

	return pImage;
}
ImageDict LoadJpxImageFromMem  (Doc pPDF, const BYTE *pBuffer,   unsigned int nWidth, unsigned int nHeight, unsigned int unImageCheckSum, BOOL bAlpha = FALSE, const BYTE *pAlphaBuffer = NULL, unsigned int unAlphaCheckSum = 0)
{
    if ( !HasDoc( pPDF ))
        return NULL;

	// Переводим Image -> IAVSUncompressedVideoFrame
	MediaCore::IAVSUncompressedVideoFrame *pInterface = NULL;
	HRESULT hRes = CoCreateInstance( MediaCore::CLSID_CAVSUncompressedVideoFrame, NULL, CLSCTX_ALL, MediaCore::IID_IAVSUncompressedVideoFrame, (void**)&pInterface );
	if ( hRes != S_OK )
		return NULL;

	int nBufferSize = 4 * nWidth * nHeight;
	if ( nBufferSize < 1 )
		return NULL;

	pInterface->put_ColorSpace( 1 << 6 ); // CPS_BGRA
	pInterface->put_Width( nWidth );
	pInterface->put_Height( nHeight );
	pInterface->put_AspectRatioX( nWidth );
	pInterface->put_AspectRatioY( nHeight );
	pInterface->put_Interlaced( VARIANT_FALSE );
	pInterface->put_Stride( 0, 4 * nWidth );
	pInterface->AllocateBuffer( nBufferSize );

	unsigned char *pBufferPtr = NULL;
	long nCreatedBufferSize = 0;

	pInterface->get_Buffer( &pBufferPtr );
	pInterface->get_BufferSize( &nCreatedBufferSize );
	pInterface->put_Plane( 0, pBufferPtr );

	if ( !pBufferPtr || nCreatedBufferSize != nBufferSize )
		return NULL;

	// Пишем данные в pBufferPtr

	for ( int nIndex = 0; nIndex < nWidth * nHeight; nIndex++, pBufferPtr += 4, pBuffer += 3 ) 
	{
		pBufferPtr[0] = pBuffer[ 2 ];
		pBufferPtr[1] = pBuffer[ 1 ];
		pBufferPtr[2] = pBuffer[ 0 ];
		pBufferPtr[3] = 255;
	}

	// Создаем темповый файл для записи туда Jpx
	CString wsTempFile, wsTempAlpha;
	if ( 0 == DocGetTempFile( pPDF, &wsTempFile ) )
	{
		SetError( pPDF->oMMgr->oError, AVS_OFFICEPDFWRITER_ERROR_FILE_WRITE_ERROR, errno );
		return NULL;
	}

	// Записываем во временный файл картинку в формате Jpx
	Jpx::IJ2kFile *pJpx = NULL;
	::CoCreateInstance( __uuidof( Jpx::CJ2kFile ), NULL, CLSCTX_ALL, __uuidof( Jpx::IJ2kFile ), (void**)&pJpx );

	if ( !pJpx )
		return NULL;

	CString sOptions;
	sOptions.Format( _T("<Jpeg2000-Options><SaveOptions><ComponentsCount value='3'/><Format value='1' /></SaveOptions></Jpeg2000-Options>" ), nWidth, nHeight );

	BSTR bsTempFile = ::SysAllocString( wsTempFile );
	BSTR bsOptions  = sOptions.AllocSysString();
	pJpx->InterfaceToJ2k( (IUnknown **)&pInterface, bsTempFile, bsOptions );

	::SysFreeString( bsTempFile );
	::SysFreeString( bsOptions );

	RELEASEINTERFACE( pInterface );
	RELEASEINTERFACE( pJpx );

	if ( bAlpha )
	{
		// Переводим Image -> IAVSUncompressedVideoFrame
		pInterface = NULL;
		hRes = CoCreateInstance( MediaCore::CLSID_CAVSUncompressedVideoFrame, NULL, CLSCTX_ALL, MediaCore::IID_IAVSUncompressedVideoFrame, (void**)&pInterface );
		if ( hRes != S_OK )
			return NULL;

		pInterface->put_ColorSpace( ( 1 << 6 ) ); // CPS_BGRA
		pInterface->put_Width( nWidth );
		pInterface->put_Height( nHeight );
		pInterface->put_AspectRatioX( nWidth );
		pInterface->put_AspectRatioY( nHeight );
		pInterface->put_Interlaced( VARIANT_FALSE );
		pInterface->put_Stride( 0, 4 * nWidth );
		pInterface->AllocateBuffer( nBufferSize );

		pBufferPtr = NULL;
		nCreatedBufferSize = 0;

		pInterface->get_Buffer( &pBufferPtr );
		pInterface->get_BufferSize( &nCreatedBufferSize );
		pInterface->put_Plane( 0, pBufferPtr );

		if ( !pBufferPtr || nCreatedBufferSize != nBufferSize )
			return NULL;

		// Пишем данные в pBufferPtr

		for ( int nIndex = 0; nIndex < nWidth * nHeight; nIndex++, pBufferPtr += 4 ) 
		{
			pBufferPtr[0] = pAlphaBuffer[ nIndex ];
			pBufferPtr[1] = pAlphaBuffer[ nIndex ];
			pBufferPtr[2] = pAlphaBuffer[ nIndex ];
			pBufferPtr[3] = 255;
		}

		if ( 0 == DocGetTempFile( pPDF, &wsTempAlpha ) )
		{
			SetError( pPDF->oMMgr->oError, AVS_OFFICEPDFWRITER_ERROR_FILE_WRITE_ERROR, errno );
			return NULL;
		}


		// Записываем во временный файл картинку в формате Jpx
		Jpx::IJ2kFile *pJpx = NULL;
		::CoCreateInstance( __uuidof( Jpx::CJ2kFile ), NULL, CLSCTX_ALL, __uuidof( Jpx::IJ2kFile ), (void**)&pJpx );

		if ( !pJpx )
			return NULL;

		CString sOptions;
		sOptions.Format( _T("<Jpeg2000-Options><SaveOptions><ComponentsCount value='1'/><Format value='0' /><Tile width='%d' height='%d' /></SaveOptions></Jpeg2000-Options>" ), nWidth, nHeight );

		BSTR bsTempFile = ::SysAllocString( wsTempAlpha );
		BSTR bsOptions  = sOptions.AllocSysString();
		pJpx->InterfaceToJ2k( (IUnknown **)&pInterface, bsTempFile, bsOptions );

		::SysFreeString( bsTempFile );
		::SysFreeString( bsOptions );

		RELEASEINTERFACE( pInterface );
		RELEASEINTERFACE( pJpx );
	}

	ImageDict pImage = ImageLoadJpxImage ( pPDF->oMMgr, wsTempFile.GetBuffer(), pPDF->pXref, nWidth, nHeight, unImageCheckSum, bAlpha, wsTempAlpha.GetBuffer(), unAlphaCheckSum );

    if ( !pImage )
        CheckError( &pPDF->oError );

#ifndef _DEBUG
    if ( pPDF->nCompressionMode & COMP_IMAGE )
        pImage->nFilter |= STREAM_FILTER_FLATE_DECODE;
#endif

    return pImage;
}
ImageDict LoadJpegImageFromMem (Doc pPDF, const BYTE *pBuffer,   unsigned int nWidth, unsigned int nHeight, unsigned int unImageCheckSum, BOOL bAlpha = FALSE, const BYTE *pAlphaBuffer = NULL, unsigned int unAlphaCheckSum = 0)
{
    if ( !HasDoc( pPDF ))
        return NULL;

	// Переводим Image -> IAVSUncompressedVideoFrame
	MediaCore::IAVSUncompressedVideoFrame *pInterface = NULL;
	HRESULT hRes = CoCreateInstance( MediaCore::CLSID_CAVSUncompressedVideoFrame, NULL, CLSCTX_ALL, MediaCore::IID_IAVSUncompressedVideoFrame, (void**)&pInterface );
	if ( hRes != S_OK )
		return NULL;

	int nBufferSize = 4 * nWidth * nHeight;
	if ( nBufferSize < 1 )
		return NULL;

	pInterface->put_ColorSpace( 1 << 6 ); // CPS_BGRA
	pInterface->put_Width( nWidth );
	pInterface->put_Height( nHeight );
	pInterface->put_AspectRatioX( nWidth );
	pInterface->put_AspectRatioY( nHeight );
	pInterface->put_Interlaced( VARIANT_FALSE );
	pInterface->put_Stride( 0, 4 * nWidth );
	pInterface->AllocateBuffer( nBufferSize );

	unsigned char *pBufferPtr = NULL;
	long nCreatedBufferSize = 0;

	pInterface->get_Buffer( &pBufferPtr );
	pInterface->get_BufferSize( &nCreatedBufferSize );
	pInterface->put_Plane( 0, pBufferPtr );

	if ( !pBufferPtr || nCreatedBufferSize != nBufferSize )
		return NULL;

	// Пишем данные в pBufferPtr

	for ( int nIndex = 0; nIndex < nWidth * nHeight; nIndex++, pBufferPtr += 4, pBuffer += 3 ) 
	{
		pBufferPtr[0] = pBuffer[ 2 ];
		pBufferPtr[1] = pBuffer[ 1 ];
		pBufferPtr[2] = pBuffer[ 0 ];
		pBufferPtr[3] = 255;
	}

	// Создаем темповый файл для записи туда Jpeg
	CString wsTempFile, wsTempAlpha;
	if ( 0 == DocGetTempFile( pPDF, &wsTempFile ) )
	{
		SetError( pPDF->oMMgr->oError, AVS_OFFICEPDFWRITER_ERROR_FILE_WRITE_ERROR, errno );
		return NULL;
	}

	// Записываем во временный файл картинку в формате Jpx
	ImageFile::IImageFile3 *pImageFile = NULL;
	::CoCreateInstance( __uuidof( ImageFile::ImageFile3 ), NULL, CLSCTX_ALL, __uuidof( ImageFile::IImageFile3 ), (void**)&pImageFile );

	if ( !pImageFile )
		return NULL;

	BSTR bsTempFile = ::SysAllocString( wsTempFile );
	pImageFile->put_JpegQuality( 100 );
	pImageFile->SaveImage2( (IUnknown**)&pInterface, 3, bsTempFile  );

	::SysFreeString( bsTempFile );

	RELEASEINTERFACE( pInterface );
	RELEASEINTERFACE( pImageFile );

	if ( bAlpha )
	{
		// Создаем темповый файл для записи туда альфа-канала
		if ( 0 == DocGetTempFile( pPDF, &wsTempAlpha ) )
		{
			SetError( pPDF->oMMgr->oError, AVS_OFFICEPDFWRITER_ERROR_FILE_WRITE_ERROR, errno );
			return NULL;
		}

		CEncoderLZW oLZW( pAlphaBuffer, nWidth * nHeight, FALSE );
		oLZW.Encode( wsTempAlpha.GetBuffer() );
	}

	ImageDict pImage = ImageLoadJpegImage ( pPDF->oMMgr, wsTempFile.GetBuffer(), pPDF->pXref, nWidth, nHeight, unImageCheckSum, bAlpha, wsTempAlpha.GetBuffer(), unAlphaCheckSum );

    if ( !pImage )
        CheckError( &pPDF->oError );

#ifndef _DEBUG
    if ( pPDF->nCompressionMode & COMP_IMAGE )
        pImage->nFilter |= STREAM_FILTER_FLATE_DECODE;
#endif

    return pImage;
}
//----------------- Catalog -------------------------------------------------------------------------
PageLayout    GetPageLayout        (Doc pPDF)
{
    if ( !HasDoc( pPDF ) )
        return PLSingle;

	return CatalogGetPageLayout( pPDF->pCatalog );
}


unsigned long SetPageLayout        (Doc pPDF, PageLayout eLayout)
{
    unsigned long nRet = OK;

    if ( !HasDoc( pPDF ) )
        return AVS_OFFICEPDFWRITER_ERROR_INVALID_DOCUMENT;

    if ( eLayout < 0 || eLayout >= PLEOF )
        return RaiseError( &pPDF->oError, AVS_OFFICEPDFWRITER_ERROR_PAGE_LAYOUT_OUT_OF_RANGE, (unsigned long)eLayout);

    nRet = CatalogSetPageLayout( pPDF->pCatalog, eLayout);
    if ( OK != nRet )
        CheckError( &pPDF->oError );

    return OK;
}

PageMode      GetPageMode          (Doc pPDF)
{
    if ( !HasDoc( pPDF ) )
        return PageModeUseNone;

    return CatalogGetPageMode( pPDF->pCatalog );
}


unsigned long SetPageMode          (Doc pPDF, PageMode eMode)
{
    unsigned long nRet = OK;

    if ( !HasDoc( pPDF ) )
        return AVS_OFFICEPDFWRITER_ERROR_INVALID_DOCUMENT;

    if ( eMode < 0 || eMode >= PageModeEOF )
        return RaiseError( &pPDF->oError, AVS_OFFICEPDFWRITER_ERROR_PAGE_MODE_OUT_OF_RANGE, (unsigned long)eMode);
 
	nRet = CatalogSetPageMode( pPDF->pCatalog, eMode);

	if ( OK != nRet )
        return CheckError( &pPDF->oError);

    return OK;
}


unsigned long SetOpenAction        (Doc pPDF, Destination pOpenAction)
{
    unsigned long nRet = OK;

    if ( !HasDoc( pPDF ) )
        return AVS_OFFICEPDFWRITER_ERROR_INVALID_DOCUMENT;

    if ( pOpenAction && !DestinationValidate( pOpenAction ) )
		return RaiseError( &pPDF->oError, AVS_OFFICEPDFWRITER_ERROR_INVALID_DESTINATION, 0);

    nRet = CatalogSetOpenAction( pPDF->pCatalog, pOpenAction );

	if ( OK != nRet )
        return CheckError( &pPDF->oError);

    return OK;
}


unsigned int  GetViewerPreference  (Doc pPDF)
{
    if ( !HasDoc( pPDF ) )
        return 0;

    return CatalogGetViewerPreference( pPDF->pCatalog );
}


unsigned long SetViewerPreference  (Doc pPDF, unsigned int nValue)
{
    unsigned long nRet = OK;

    if ( !HasDoc( pPDF ) )
        return AVS_OFFICEPDFWRITER_ERROR_INVALID_DOCUMENT;

    nRet = CatalogSetViewerPreference( pPDF->pCatalog, nValue );

	if ( OK != nRet )
        return CheckError( &pPDF->oError);

    return OK;
}


unsigned long AddPageLabel         (Doc pPDF, unsigned int nPageNum, PageNumStyle eStyle, unsigned int nFirstPage, const char *sPrefix)
{
    Dict pPageLabel = NULL;
    unsigned long nRet = OK;

    if ( !HasDoc( pPDF ) )
        return AVS_OFFICEPDFWRITER_ERROR_INVALID_DOCUMENT;

    pPageLabel = PageLabelNew( pPDF, eStyle, nFirstPage, sPrefix );

    if ( !pPageLabel )
        return CheckError( &pPDF->oError);

    if ( eStyle < 0 || eStyle >= PageNumStyleEOF )
		return RaiseError( &pPDF->oError, AVS_OFFICEPDFWRITER_ERROR_PAGE_NUM_STYLE_OUT_OF_RANGE, (unsigned long)eStyle );

	nRet = CatalogAddPageLabel( pPDF->pCatalog, nPageNum, pPageLabel);

	if ( OK != nRet )
        return CheckError( &pPDF->oError);

    return OK;
}


//----------------- Info -----------------------------------------------------------------------------
static Dict   GetInfo            (Doc pPDF)
{
    if ( !HasDoc( pPDF ) )
        return NULL;

	if ( !pPDF->pInfo ) 
	{
		pPDF->pInfo = DictNew( pPDF->oMMgr );

        if ( !pPDF->pInfo || XrefAdd( pPDF->pXref, pPDF->pInfo ) != OK )
            pPDF->pInfo = NULL;
    }

    return pPDF->pInfo;
}


unsigned long SetInfoAttr        (Doc pPDF, InfoType eType, const char *sValue)
{
    unsigned long nRet = OK;
    
	Dict pInfo = GetInfo( pPDF );

    if ( !pInfo )
		return CheckError( &pPDF->oError );

	nRet = InfoSetInfoAttr( pInfo, eType, sValue, pPDF->pCurEncoder );
    if ( OK != nRet )
		return CheckError( &pPDF->oError );

    return nRet;
}

const char*   GetInfoAttr        (Doc pPDF, InfoType eType)
{
    const char *sRes = NULL;
    
	Dict pInfo = GetInfo( pPDF );

    if ( pInfo )
        sRes = InfoGetInfoAttr( pInfo, eType);
    else
        CheckError( &pPDF->oError  );

    return sRes;
}
unsigned long SetInfoDateAttr    (Doc pPDF, InfoType eType, TDate oDate)
{
    unsigned long nRet = OK;
    
	Dict pInfo = GetInfo( pPDF );

    if ( !pInfo )
		return CheckError( &pPDF->oError );

    nRet = InfoSetInfoDateAttr( pInfo, eType, oDate );
    if ( OK != nRet )
        return CheckError( &pPDF->oError );

    return nRet;
}


Outline       CreateOutline      (Doc pPDF, Outline pParent, const char *sTitle, Encoder pEncoder)
{
    Outline pOutline = NULL;

    if ( !HasDoc( pPDF ) )
        return NULL;

    if ( !pParent ) 
	{
        if ( pPDF->pOutlines ) 
		{
            pParent = pPDF->pOutlines;
        } 
		else 
		{
            pPDF->pOutlines = OutlineRootNew( pPDF->oMMgr, pPDF->pXref );

            if ( pPDF->pOutlines ) 
			{
				unsigned long nRet = DictAdd( pPDF->pCatalog, "Outlines", pPDF->pOutlines );

                if ( OK != nRet ) 
				{
					CheckError( &pPDF->oError );
					pPDF->pOutlines = NULL;
                    return NULL;
                }

				pParent = pPDF->pOutlines;
            } 
			else 
			{
				CheckError( &pPDF->oError );
                return NULL;
            }
        }
    }

	if ( !OutlineValidate( pParent ) || pPDF->oMMgr != pParent->oMMgr ) 
	{
		RaiseError( &pPDF->oError, AVS_OFFICEPDFWRITER_ERROR_INVALID_OUTLINE, 0);
        return NULL;
    }

	pOutline = OutlineNew( pPDF->oMMgr, pParent, sTitle, pEncoder, pPDF->pXref );
    if ( !pOutline )
        CheckError( &pPDF->oError );

    return pOutline;
}


ExtGState     CreateExtGState    (Doc pPDF)
{
    ExtGState pExtGState = NULL;

    if ( !HasDoc( pPDF ) )
        return NULL;

	pPDF->ePdfVersion = PDF_1_4;

	pExtGState = ExtGStateNew( pPDF->oMMgr, pPDF->pXref );
    if (!pExtGState)
		CheckError( &pPDF->oError );

    return pExtGState;
}
ExtGState     FindExtGState      (Doc pPDF, float fAlphaStroke = -1, float fAlphaFill = -1, BlendMode eMode = BMEOF, int nStrokeAdjustment = -1)
{
    ExtGState pExtGState = NULL;

	for ( unsigned int nIndex = 0; nIndex < pPDF->pExtGStateList->nCount; nIndex++) 
	{
		pExtGState = (ExtGState)ListItemAt( pPDF->pExtGStateList, nIndex);

		// Сравниваем CA
		Real pAlphaStroke = (Real)DictGetItem( (Dict)pExtGState, "CA", OCLASS_REAL );
		if ( NULL == pAlphaStroke )
		{
			if ( -1 != fAlphaStroke )
				continue;
		}
		else
		{
			if ( -1 == fAlphaStroke || pAlphaStroke->fValue != fAlphaStroke )
				continue;
		}

		// Сравниваем ca
		Real pAlphaFill = (Real)DictGetItem( (Dict)pExtGState, "ca", OCLASS_REAL );
		if ( NULL == pAlphaFill )
		{
			if ( -1 != fAlphaFill )
				continue;
		}
		else
		{
			if ( -1 == fAlphaFill || pAlphaFill->fValue != fAlphaFill )
				continue;
		}

		// Сравниваем BlendMode
		Name pBlendMode = (Name)DictGetItem( (Dict)pExtGState, "BM", OCLASS_NAME );
		if ( NULL == pBlendMode )
		{
			if ( BMEOF != eMode )
				continue;
		}
		else
		{
			if ( BMEOF == eMode || 0 != UtilsStrCmp( pBlendMode->sValue, BM_NAMES[ (int)eMode ] ) )
				continue;
		}

		// Сравниваем StrokeAdjustment
		Boolean pSA = (Boolean)DictGetItem( (Dict)pExtGState, "SA", OCLASS_BOOLEAN );
		if ( NULL == pSA )
		{
			if ( -1 != nStrokeAdjustment )
				continue;
		}
		else
		{
			if ( -1 == nStrokeAdjustment || ( 0 == nStrokeAdjustment && TRUE == pSA->bValue ) || ( 1 == nStrokeAdjustment && FALSE == pSA->bValue ) )
				continue;
		}

		return pExtGState;
	}

	return NULL;

}
ExtGState     GetExtGState       (Doc pPDF, float fAlphaStroke = -1, float fAlphaFill = -1, BlendMode eMode = BMEOF, int nStrokeAdjustment = -1)
{
	ExtGState pExtGState = FindExtGState( pPDF, fAlphaStroke, fAlphaFill, eMode, nStrokeAdjustment );

	if ( NULL == pExtGState )
	{
		pExtGState = CreateExtGState( pPDF );
		if ( NULL == pExtGState )
			return NULL;

		if ( -1 != fAlphaStroke )
			ExtGStateSetAlphaStroke( pExtGState, fAlphaStroke );
		if ( -1 != fAlphaFill )
			ExtGStateSetAlphaFill( pExtGState, fAlphaFill );
		if ( BMEOF != eMode )
			ExtGStateSetBlendMode( pExtGState, eMode );
		if ( -1 != nStrokeAdjustment )
			ExtGStateSetStrokeAdjustment( pExtGState, (BOOL)nStrokeAdjustment );

		ListAdd( pPDF->pExtGStateList, pExtGState );
	}

	return pExtGState;
}
unsigned long SetCompressionMode (Doc pPDF, unsigned int nMode)
{
    if ( !DocValidate( pPDF ) )
        return AVS_OFFICEPDFWRITER_ERROR_INVALID_DOCUMENT;

    if ( nMode != ( nMode & COMP_MASK ) )
        return RaiseError( &pPDF->oError, AVS_OFFICEPDFWRITER_ERROR_INVALID_COMPRESSION_MODE, 0);

#ifndef NOZLIB
    pPDF->nCompressionMode = nMode;

    return OK;

#else /* NOZLIB */

    return AVS_OFFICEPDFWRITER_ERROR_INVALID_COMPRESSION_MODE;

#endif /* NOZLIB */
}

unsigned long GetError           (Doc pPDF)
{
    if ( !DocValidate( pPDF ) )
        return AVS_OFFICEPDFWRITER_ERROR_INVALID_DOCUMENT;

    return ErrorGetCode( &pPDF->oError );
}

unsigned long GetErrorDetail     (Doc pPDF)
{
    if ( !DocValidate( pPDF ) )
        return AVS_OFFICEPDFWRITER_ERROR_INVALID_DOCUMENT;

    return ErrorGetDetailCode( &pPDF->oError );
}


void          ResetError         (Doc pPDF)
{
    if ( !DocValidate( pPDF ) )
        return;

    ErrorReset( &pPDF->oError );
}


//----------------- Основные функции -----------------------------------------------------------------
BOOL          DocValidate            (Doc pPDF)
{
	if ( !pPDF || pPDF->nSigBytes != SIG_BYTES )
        return FALSE;
    else
        return TRUE;
}


BOOL          HasDoc                 (Doc pPDF)
{
	if ( !pPDF || pPDF->nSigBytes != SIG_BYTES )
        return FALSE;

	if ( !pPDF->pCatalog || pPDF->oError.nErrorNo != NOERROR) 
	{
		RaiseError( &pPDF->oError, AVS_OFFICEPDFWRITER_ERROR_INVALID_DOCUMENT, 0 );
        return FALSE;
    } 
	else
        return TRUE;
}


void          FreeDoc                (Doc pPDF)
{
    if ( DocValidate( pPDF ) ) 
	{
		if ( pPDF->pXref ) 
		{
			XrefFree( pPDF->pXref );
			pPDF->pXref = NULL;
        }

		if ( pPDF->pFontManager ) 
		{
			ListFree( pPDF->pFontManager );
			pPDF->pFontManager = NULL;
        }

		if ( pPDF->pFontDefList )
            CleanupFontDefList( pPDF );

		UtilsMemSet( pPDF->aTTFontTag, 0, 6);

		pPDF->ePdfVersion = PDF_1_3;
		pPDF->pOutlines   = NULL;
		pPDF->pCatalog    = NULL;
		pPDF->pRootPages  = NULL;
		pPDF->pCurPages   = NULL;
		pPDF->pCurPage    = NULL;
        pPDF->bEncryptOn  = FALSE;
		pPDF->nCurPageNum = 0;
		pPDF->pCurEncoder = NULL;
		pPDF->pDefEncoder = NULL;
		pPDF->nPagePerPages = 0;

		if ( pPDF->pPageList ) 
		{
			ListFree( pPDF->pPageList );
			pPDF->pPageList = NULL;
        }

		if ( pPDF->pExtGStateList )
		{
			ListFree( pPDF->pExtGStateList );
			pPDF->pExtGStateList = NULL;
		}

		pPDF->pEncryptDict = NULL;
		pPDF->pInfo = NULL;

		ErrorReset( &pPDF->oError );

		if ( pPDF->pStream ) 
		{
			StreamFree( pPDF->pStream );
            pPDF->pStream = NULL;
        }

		::memset( pPDF->wsTempDir, 0x00, MAX_PATH );
    }
}


void          FreeDocAll             (Doc pPDF)
{
    if ( DocValidate( pPDF ) ) 
	{
        FreeDoc( pPDF );

		if ( pPDF->pFontDefList )
            FreeFontDefList( pPDF );

		if ( pPDF->pEncoderList )
			FreeEncoderList( pPDF );

		if ( pPDF->pToUnicodeList )
			FreeToUnicodeList( pPDF );

		pPDF->nCompressionMode = COMP_NONE;

		ErrorReset( &pPDF->oError );
    }
}


void          Free                   (Doc pPDF)
{
    if ( pPDF ) 
	{
		MMgr oMMgr = pPDF->oMMgr;

        FreeDocAll( pPDF );

		pPDF->nSigBytes = 0;

        FreeMem( oMMgr, pPDF );
        MMgrFree( oMMgr );
    }
}


unsigned long NewDoc                 (Doc pPDF)
{
    if ( !DocValidate( pPDF ) )
        return AVS_OFFICEPDFWRITER_ERROR_DOC_INVALID_OBJECT;

    FreeDoc( pPDF );

	pPDF->pXref = XrefNew( pPDF->oMMgr, 0);
    if ( !pPDF->pXref )
		return CheckError( &pPDF->oError );

	pPDF->pTrailer = pPDF->pXref->oTrailer;

	pPDF->pFontManager = ListNew( pPDF->oMMgr, DEF_ITEMS_PER_BLOCK );
	if ( !pPDF->pFontManager )
		return CheckError( &pPDF->oError );

	if ( !pPDF->pFontDefList ) 
	{
		pPDF->pFontDefList = ListNew( pPDF->oMMgr, DEF_ITEMS_PER_BLOCK);
		if ( !pPDF->pFontDefList )
			return CheckError( &pPDF->oError );
    }

	if ( !pPDF->pEncoderList ) 
	{
		pPDF->pEncoderList = ListNew( pPDF->oMMgr, DEF_ITEMS_PER_BLOCK );
        if ( !pPDF->pEncoderList )
			return CheckError( &pPDF->oError );
    }

	if ( !pPDF->pToUnicodeList )
	{
		pPDF->pToUnicodeList = ListNew( pPDF->oMMgr, DEF_ITEMS_PER_BLOCK );
		if ( !pPDF->pToUnicodeList )
			return CheckError( &pPDF->oError );
	}

	if ( !pPDF->pExtGStateList )
	{
		pPDF->pExtGStateList = ListNew( pPDF->oMMgr, DEF_ITEMS_PER_BLOCK );
		if ( !pPDF->pExtGStateList )
			return CheckError( &pPDF->oError );
	}

	pPDF->pCatalog = CatalogNew( pPDF->oMMgr, pPDF->pXref );
    if ( !pPDF->pCatalog)
		return CheckError( &pPDF->oError );

	pPDF->pRootPages = CatalogGetRoot( pPDF->pCatalog );
    if ( !pPDF->pRootPages )
		return CheckError( &pPDF->oError );

	pPDF->pPageList = ListNew( pPDF->oMMgr, DEF_PAGE_LIST_NUM );
    if ( !pPDF->pPageList )
		return CheckError( &pPDF->oError );

	pPDF->pCurPages = pPDF->pRootPages;

	char sBuf[TEMP_BUFFER_SIZE];
    char *pPointer = sBuf;
    char *pEndPointer = sBuf + TEMP_BUFFER_SIZE - 1;
    const char *sVersion;

	pPointer = (char *)UtilsStrCpy( pPointer, "Ascensio System SIA Copyright (c) 2013", pEndPointer);
	//sVersion = STRVER;
 //   UtilsStrCpy( pPointer, sVersion, pEndPointer);

    if ( OK != SetInfoAttr( pPDF, InfoProducer, sBuf ) )
		return CheckError( &pPDF->oError );

	//pPDF->resources	=	CreateExtGState(pPDF);	// список ресурсов ( в атрибут Resources если записывать данные то Adobe просмотрщик не видит их )

    return OK;
}


Doc           NewEx                  (ErrorHandler pUserErrorFn, AllocFunc pUserAllocFn, FreeFunc pUserFreeFn, unsigned int nMemPoolBufSize, void *pUserData)
{
    ErrorRec oTempError;

    // инициализируем временный объект описания ошибок
    ErrorInit( &oTempError, pUserData );

    // создаем объект для управления памятью
    MMgr oMMgr = MMgrNew( &oTempError, nMemPoolBufSize, pUserAllocFn, pUserFreeFn );
    if ( !oMMgr ) 
	{
        CheckError( &oTempError );
        return NULL;
    }

    // создаем объект - документ
    Doc pPDF = (Doc)GetMem( oMMgr, sizeof (DocRec) );
    if ( !pPDF ) 
	{
        MMgrFree( oMMgr );
        CheckError( &oTempError );
        return NULL;
    }

	oMMgr->pDoc = pPDF;

    UtilsMemSet( pPDF, 0, sizeof( DocRec ) );
	pPDF->nSigBytes        = SIG_BYTES;
	pPDF->oMMgr            = oMMgr;
	pPDF->ePdfVersion      = PDF_1_3;
	pPDF->nCompressionMode = COMP_NONE;

    // копируем данные из oTempError в объект для ошибок, содержащийся в структуре Doc
	pPDF->oError = oTempError;

    // меням указатель на объект Error для MemoryManager
	oMMgr->oError = &pPDF->oError;

    if ( OK != NewDoc( pPDF ) ) 
	{
        Free( pPDF );
        CheckError( &oTempError );
        return NULL;
    }

	pPDF->oError.ErrorFn = pUserErrorFn;

	pPDF->nCMapCount = 0;

    return pPDF;
}


Doc           New                    (ErrorHandler pUserErrorFn, void *pUserData)
{
    return NewEx( pUserErrorFn, NULL, NULL, 0, pUserData);
}
Doc           New                    (void)
{
	return New( NULL, NULL);
}


unsigned long SetPagesConfiguration  (Doc pPDF, unsigned int nPagePerPages)
{
    if ( !HasDoc( pPDF ) )
        return AVS_OFFICEPDFWRITER_ERROR_INVALID_DOCUMENT;

	if ( pPDF->pCurPage )
		return RaiseError( &pPDF->oError, AVS_OFFICEPDFWRITER_ERROR_INVALID_DOCUMENT_STATE, 0);

    if ( nPagePerPages > LIMIT_MAX_ARRAY )
        return RaiseError( &pPDF->oError, AVS_OFFICEPDFWRITER_ERROR_INVALID_PARAMETER, 0);

	if ( pPDF->pCurPages == pPDF->pRootPages ) 
	{
        pPDF->pCurPages = DocAddPagesTo( pPDF, pPDF->pRootPages );
        if ( !pPDF->pCurPages )
			return pPDF->oError.nErrorNo;
		pPDF->nCurPageNum = 0;
    }

	pPDF->nPagePerPages = nPagePerPages;

    return OK;
}
unsigned long DocSetEncryptOn        (Doc pPDF)
{
    if ( pPDF->bEncryptOn )
        return OK;

	if ( !pPDF->pEncryptDict )
		return SetError( &pPDF->oError, AVS_OFFICEPDFWRITER_ERROR_DOC_ENCRYPTDICT_NOT_FOUND, 0);

	if ( OTYPE_NONE == pPDF->pEncryptDict->pHeader.nObjClass )
		if ( OK != XrefAdd( pPDF->pXref, pPDF->pEncryptDict ) )
			return pPDF->oError.nErrorNo;

	if ( OK != DictAdd( pPDF->pTrailer, "Encrypt", pPDF->pEncryptDict ) )
		return pPDF->oError.nErrorNo;

    pPDF->bEncryptOn = TRUE;

    return OK;
}
unsigned long SetPassword            (Doc pPDF, const char *sOwnerPassword, const char *sUserPassword)
{
    if ( !HasDoc( pPDF ) )
        return AVS_OFFICEPDFWRITER_ERROR_DOC_INVALID_OBJECT;

	if ( !pPDF->pEncryptDict ) 
	{
		pPDF->pEncryptDict = EncryptDictNew( pPDF->oMMgr, pPDF->pXref );

        if ( !pPDF->pEncryptDict )
			return CheckError( &pPDF->oError );
    }

	if ( OK != EncryptDictSetPassword( pPDF->pEncryptDict, sOwnerPassword, sUserPassword ) )
        return CheckError( &pPDF->oError );

    return DocSetEncryptOn( pPDF );
}
unsigned long SetPermission          (Doc pPDF, unsigned int nPermission)
{
    if ( !HasDoc( pPDF ) )
        return AVS_OFFICEPDFWRITER_ERROR_DOC_INVALID_OBJECT;

	Encrypt pEncrypt = EncryptDictGetAttr( pPDF->pEncryptDict );

    if ( !pEncrypt )
        return RaiseError( &pPDF->oError, AVS_OFFICEPDFWRITER_ERROR_DOC_ENCRYPTDICT_NOT_FOUND, 0);
    else
        pEncrypt->nPermission = nPermission;

    return OK;
}
unsigned long SetEncryptionMode      (Doc pPDF, EncryptMode eMode, unsigned int nKeyLen)
{
    if ( !HasDoc( pPDF ) )
        return AVS_OFFICEPDFWRITER_ERROR_DOC_INVALID_OBJECT;

	Encrypt pEncrypt = EncryptDictGetAttr( pPDF->pEncryptDict );

    if ( !pEncrypt )
        return RaiseError( &pPDF->oError, AVS_OFFICEPDFWRITER_ERROR_DOC_ENCRYPTDICT_NOT_FOUND, 0);
    else
	{
        if ( EncryptR2 == eMode )
			pEncrypt->nKeyLen = 5;
        else 
		{
            pPDF->ePdfVersion = PDF_1_4;

            if ( nKeyLen >= 5 && nKeyLen <= 16 )
                pEncrypt->nKeyLen = nKeyLen;
            else if ( 0 == nKeyLen )
                pEncrypt->nKeyLen = 16;
            else
				return RaiseError( &pPDF->oError, AVS_OFFICEPDFWRITER_ERROR_INVALID_ENCRYPT_KEY_LEN, 0 );
        }
		pEncrypt->eMode = eMode;
    }

    return OK;
}
unsigned long DocSetEncryptOff       (Doc pPDF)
{
    if ( !pPDF->bEncryptOn )
        return OK;

    // Если EncryptDict зарегистрирован в cross-reference-table, заменяем его нулевым объектом.
    // Кроме этого, удаляем EncryptDict из трайлера.
	if ( pPDF->pEncryptDict ) 
	{
		unsigned int nObjId = pPDF->pEncryptDict->pHeader.nObjId;

		if ( nObjId & OTYPE_INDIRECT ) 
		{
			DictRemoveElement( pPDF->pTrailer, "Encrypt" );

			XrefEntry pEntry = XrefGetEntryByObjectId( pPDF->pXref, nObjId & 0x00FFFFFF );

            if ( !pEntry ) 
				return SetError( &pPDF->oError, AVS_OFFICEPDFWRITER_ERROR_DOC_ENCRYPTDICT_NOT_FOUND, 0);

			Null pNull = NullNew( pPDF->oMMgr );
            if ( !pNull )
				return pPDF->oError.nErrorNo;

			pEntry->pObj = pNull;
			pNull->pHeader.nObjId = nObjId | OTYPE_INDIRECT;
			pPDF->pEncryptDict->pHeader.nObjId = OTYPE_NONE;
        }
    }

    pPDF->bEncryptOn = FALSE;
    return OK;
}
unsigned long DocPrepareEncryption   (Doc pPDF)
{
    Encrypt pEncrypt = EncryptDictGetAttr( pPDF->pEncryptDict );
    Dict pInfo = GetInfo( pPDF );

    if ( !pEncrypt )
        return AVS_OFFICEPDFWRITER_ERROR_DOC_ENCRYPTDICT_NOT_FOUND;

    if ( !pInfo )
		return pPDF->oError.nErrorNo;

	if ( OK != EncryptDictPrepare( pPDF->pEncryptDict, pInfo, pPDF->pXref ) )
		return pPDF->oError.nErrorNo;

	Array pID = (Array)DictGetItem( pPDF->pTrailer, "ID", OCLASS_ARRAY );
    if ( !pID ) 
	{
		pID = ArrayNew( pPDF->oMMgr );

		if ( !pID || OK != DictAdd( pPDF->pTrailer, "ID", pID ) )
			return pPDF->oError.nErrorNo;
    } 
	else
        ArrayClear( pID );

	if ( OK != ArrayAdd( pID, BinaryNew( pPDF->oMMgr, pEncrypt->anEncryptID, ID_LEN ) ) )
        return pPDF->oError.nErrorNo;

	if ( OK != ArrayAdd( pID, BinaryNew( pPDF->oMMgr, pEncrypt->anEncryptID, ID_LEN ) ) )
        return pPDF->oError.nErrorNo;

    return OK;
}
unsigned long SaveToStream           (Doc pPDF)
{
    if ( !HasDoc( pPDF ) )
        return AVS_OFFICEPDFWRITER_ERROR_INVALID_DOCUMENT;

    if ( !pPDF->pStream )
		pPDF->pStream = MemStreamNew( pPDF->oMMgr, STREAM_BUF_SIZ );

	if ( !StreamValidate( pPDF->pStream ) )
		return RaiseError( &pPDF->oError, AVS_OFFICEPDFWRITER_ERROR_INVALID_STREAM, 0);

    MemStreamFreeData( pPDF->pStream );

	if ( OK != InternalSaveToStream( pPDF, pPDF->pStream ) )
		return CheckError( &pPDF->oError );

    return OK;
}


unsigned int  GetStreamSize          (Doc pPDF)
{
    if ( !HasDoc( pPDF ) )
        return AVS_OFFICEPDFWRITER_ERROR_INVALID_DOCUMENT;

	if ( !StreamValidate( pPDF->pStream ) )
        return 0;

	return StreamSize( pPDF->pStream );
}


unsigned long ReadFromStream         (Doc pPDF, BYTE *pBuf, unsigned int *pnSize)
{
    unsigned int nSize = *pnSize;
    unsigned long nRet = OK;

    if ( !HasDoc( pPDF ) )
        return AVS_OFFICEPDFWRITER_ERROR_INVALID_DOCUMENT;

	if ( !StreamValidate( pPDF->pStream ) )
		return RaiseError( &pPDF->oError, AVS_OFFICEPDFWRITER_ERROR_INVALID_OPERATION, 0);

    if ( *pnSize == 0 )
        return RaiseError( &pPDF->oError, AVS_OFFICEPDFWRITER_ERROR_INVALID_PARAMETER, 0);

    nRet = StreamRead( pPDF->pStream, pBuf, &nSize );

    *pnSize = nSize;

    if ( OK != nRet )
		CheckError( &pPDF->oError );

    return nRet;
}


unsigned long ResetStream            (Doc pPDF)
{
    if ( !HasDoc( pPDF ) )
        return AVS_OFFICEPDFWRITER_ERROR_INVALID_DOCUMENT;

	if ( !StreamValidate( pPDF->pStream ) )
		return RaiseError( &pPDF->oError, AVS_OFFICEPDFWRITER_ERROR_INVALID_OPERATION, 0);

	return StreamSeek( pPDF->pStream, 0, SeekSet );
}


unsigned long SaveDocToFile          (Doc pPDF, const wchar_t *wsFileName)
{
    if ( !HasDoc( pPDF ) )
        return AVS_OFFICEPDFWRITER_ERROR_INVALID_DOCUMENT;

    Stream pStream = FileWriterNew( pPDF->oMMgr, wsFileName );
    if ( !pStream )
        return CheckError( &pPDF->oError );

    InternalSaveToStream( pPDF, pStream );

    StreamFree( pStream );

	return CheckError( &pPDF->oError );
}


Page          GetCurrentPage         (Doc pPDF)
{
    if ( !HasDoc( pPDF ) )
        return NULL;

    return pPDF->pCurPage;
}


Page          GetPageByIndex         (Doc pPDF, unsigned int nIndex)
{
    Page pRes = NULL;

    if ( !HasDoc( pPDF ) )
        return NULL;

    pRes = (Page)ListItemAt( pPDF->pPageList, nIndex );
    
	if ( !pRes ) 
	{
		RaiseError( &pPDF->oError, AVS_OFFICEPDFWRITER_ERROR_INVALID_PAGE_INDEX, 0 );
        return NULL;
    }

    return pRes;
}


Pages         DocGetCurrentPages     (Doc pPDF)
{
    if ( !HasDoc( pPDF ) )
        return NULL;

    return pPDF->pCurPages;
}


unsigned long DocSetCurrentPages     (Doc pPDF, Pages pPages)
{
    if ( !HasDoc( pPDF ) )
        return AVS_OFFICEPDFWRITER_ERROR_INVALID_DOCUMENT;

    if ( !PagesValidate( pPages ) )
		return SetError( &pPDF->oError, AVS_OFFICEPDFWRITER_ERROR_INVALID_PAGES, 0 );

    // проверяем принадлежит ли объект pPages нашему pdf - документу
	if ( pPDF->oMMgr != pPages->oMMgr )
		return SetError( &pPDF->oError, AVS_OFFICEPDFWRITER_ERROR_INVALID_PAGES, 0 );

	pPDF->pCurPages = pPages;

    return OK;
}


unsigned long DocSetCurrentPage      (Doc pPDF, Page pPage)
{
    if ( !HasDoc( pPDF ) )
        return AVS_OFFICEPDFWRITER_ERROR_INVALID_DOCUMENT;

    if ( !PageValidate( pPage ) )
		return SetError( &pPDF->oError, AVS_OFFICEPDFWRITER_ERROR_INVALID_PAGE, 0);

    // проверяем принадлежит ли объект pPage нашему pdf - документу
	if ( pPDF->oMMgr != pPage->oMMgr )
		return SetError( &pPDF->oError, AVS_OFFICEPDFWRITER_ERROR_INVALID_PAGE, 0 );

	pPDF->pCurPage = pPage;

    return OK;
}


unsigned long DocSetTempDir          (Doc pPDF, wchar_t *wsTempDir)
{
	if ( !HasDoc( pPDF ) )
		return AVS_OFFICEPDFWRITER_ERROR_INVALID_DOCUMENT;

	wcscpy( pPDF->wsTempDir, wsTempDir );

	return OK;
}
Page          AddPage                (Doc pPDF)
{
    if ( !HasDoc( pPDF ) )
		return NULL;

    Page pPage;
    unsigned long nRet = OK;

	if ( pPDF->nPagePerPages ) 
	{
		if ( pPDF->nPagePerPages <= pPDF->nCurPageNum ) 
		{
			pPDF->pCurPages = DocAddPagesTo( pPDF, pPDF->pRootPages );

            if ( !pPDF->pCurPages )
                return NULL;
            pPDF->nCurPageNum = 0;
        }
    }

	pPage = PageNew( pPDF->oMMgr, pPDF->pXref );
    if ( !pPage ) 
	{
		CheckError( &pPDF->oError );
        return NULL;
    }

	if ( OK != ( nRet = PagesAddKids( pPDF->pCurPages, pPage ) ) ) 
	{
		RaiseError( &pPDF->oError, nRet, 0);
        return NULL;
    }

	if ( OK != ( nRet = ListAdd( pPDF->pPageList, pPage ) ) ) 
	{
		RaiseError( &pPDF->oError, nRet, 0);
        return NULL;
    }

	pPDF->pCurPage = pPage;

#ifndef _DEBUG
	if ( pPDF->nCompressionMode & COMP_TEXT )
        PageSetFilter( pPage, STREAM_FILTER_FLATE_DECODE );
#endif

	pPDF->nCurPageNum++;

    return pPage;
}


Pages         DocAddPagesTo          (Doc pPDF, Pages pParent)
{
    Pages pPages = NULL;

    if ( !HasDoc( pPDF ) )
        return NULL;

    if ( !PagesValidate( pParent ) ) 
	{
		RaiseError( &pPDF->oError, AVS_OFFICEPDFWRITER_ERROR_INVALID_PAGES, 0);
        return NULL;
    }

    // проверяем принадлежит ли страница нашему PDF документу
	if ( pPDF->oMMgr != pParent->oMMgr ) 
	{
        RaiseError( &pPDF->oError, AVS_OFFICEPDFWRITER_ERROR_INVALID_PAGES, 0);
        return NULL;
    }

	pPages = PagesNew( pPDF->oMMgr, pParent, pPDF->pXref );
    if ( pPages )
		pPDF->pCurPages = pPages;
    else
		CheckError( &pPDF->oError );

    return pPages;
}


Page          InsertPage             (Doc pPDF, Page pTarget)
{
    Page pPage = NULL;
    unsigned long nRet = OK;

    if ( !HasDoc( pPDF ) )
        return NULL;

    if ( !PageValidate( pTarget ) ) 
	{
		RaiseError( &pPDF->oError, AVS_OFFICEPDFWRITER_ERROR_INVALID_PAGE, 0);
        return NULL;
    }

    // проверяем принадлежит ли объект pTarget нашему pdf - документу
	if ( pPDF->oMMgr != pTarget->oMMgr ) 
	{
		RaiseError( &pPDF->oError, AVS_OFFICEPDFWRITER_ERROR_INVALID_PAGE, 0);
        return NULL;
    }

	pPage = PageNew( pPDF->oMMgr, pPDF->pXref );
    if ( !pPage ) 
	{
		CheckError( &pPDF->oError );
        return NULL;
    }

    if ( OK != ( nRet = PageInsertBefore( pPage, pTarget ) ) ) 
	{
		RaiseError( &pPDF->oError, nRet, 0 );
        return NULL;
    }

	if ( OK != ( nRet = ListInsert( pPDF->pPageList, pTarget, pPage ) ) ) 
	{
		RaiseError( &pPDF->oError, nRet, 0 );
        return NULL;
    }

#ifndef _DEBUG
	if ( pPDF->nCompressionMode & COMP_TEXT )
        PageSetFilter( pPage, STREAM_FILTER_FLATE_DECODE );
#endif

    return pPage;
}


unsigned long SetErrorHandler        (Doc pPDF, ErrorHandler pUserErrorFn)
{
    if ( !DocValidate( pPDF ) )
		return AVS_OFFICEPDFWRITER_ERROR_INVALID_DOCUMENT;

	pPDF->oError.ErrorFn = pUserErrorFn;

    return OK;
}

#endif /* _DOC_H */

