#ifndef _OBJECTS_H
#define _OBJECTS_H

#include "Encoder.h"
#include "ToUnicode.h"
#include "Utils.h"
#include "Streams.h"
#include "Pattern.h"


// Если установлен бит OTYPE_DIRECT, значит данный объект принадлежит другому
// объекту. Если установлен бит OTYPE_INDIRECT, значит объект управляется таблицей xref.

#define  OTYPE_NONE              0x00000000
#define  OTYPE_DIRECT            0x80000000
#define  OTYPE_INDIRECT          0x40000000
#define  OTYPE_ANY               (OTYPE_DIRECT | OTYPE_INDIRECT)
#define  OTYPE_HIDDEN            0x10000000

#define  OCLASS_UNKNOWN          0x0001
#define  OCLASS_NULL             0x0002
#define  OCLASS_BOOLEAN          0x0003
#define  OCLASS_NUMBER           0x0004
#define  OCLASS_REAL             0x0005
#define  OCLASS_NAME             0x0006
#define  OCLASS_STRING           0x0007
#define  OCLASS_BINARY           0x0008
#define  OCLASS_ARRAY            0x0010
#define  OCLASS_DICT             0x0011
#define  OCLASS_PROXY            0x0012


#define	 OCLASS_PATTERN_REF		 0x00FA
#define	 OCLASS_FUNC_REF		 0x00F2
#define	 OCLASS_MASK_FORM		 0x00F3
#define	 OCLASS_MASK_REF		 0x00F4
#define	 OCLASS_XFORM			 0x00F5

#define  OCLASS_ANY              0x00FF

#define  OSUBCLASS_FONT          0x0100
#define  OSUBCLASS_CATALOG       0x0200
#define  OSUBCLASS_PAGES         0x0300
#define  OSUBCLASS_PAGE          0x0400
#define  OSUBCLASS_XOBJECT       0x0500
#define  OSUBCLASS_OUTLINE       0x0600
#define  OSUBCLASS_DESTINATION   0x0700
#define  OSUBCLASS_ANNOTATION    0x0800
#define  OSUBCLASS_ENCRYPT       0x0900
#define  OSUBCLASS_EXT_GSTATE    0x0A00
#define  OSUBCLASS_EXT_GSTATE_R  0x0B00  /* read only object */



//------ Значения относящиеся к объекту xref ------------------------------------

#define FREE_ENTRY             'f'
#define IN_USE_ENTRY           'n'


/*
 *  structure of Object-ID
 *
 *  1       direct-object
 *  2       indirect-object
 *  3       reserved
 *  4       shadow-object
 *  5-8     reserved
 *  9-32    object-idЃi0-8388607Ѓj
 *
 *  the real Object-ID is described "obj_id & 0x00FFFFFF"
 */

typedef struct TObjHeader 
{
    unsigned int nObjId;
    unsigned int nGenNo;
    unsigned int nObjClass;
} ObjHeader;

// Определения основных функций для работы с объектами

unsigned long ObjWriteValue (void *pObj, Stream pStream, Encrypt pEncrypt);
unsigned long ObjWrite      (void *pObj, Stream pStream, Encrypt pEncrypt);
void          ObjForceFree  (MMgr oMMgr, void *pObj);
void          ObjFree       (MMgr oMMgr, void *pObj);
//----- NullObject -------------------------------------------------------------

typedef struct TNullRec  *Null;

typedef struct TNullRec 
{
    ObjHeader pHeader;
} NullRec;

Null NullNew  (MMgr oMMgr)
{
    Null pObj = (Null)GetMem( oMMgr, sizeof(NullRec) );

    if ( pObj ) 
	{
		UtilsMemSet( &pObj->pHeader, 0, sizeof(ObjHeader) );
		pObj->pHeader.nObjClass = OCLASS_NULL;
    }

    return pObj;
}



//----- BooleanObject --------------------------------------------------------

typedef struct TBooleanRec  *Boolean;

typedef struct TBooleanRec {
    ObjHeader pHeader;
    BOOL      bValue;
} BooleanRec;

Boolean       BooleanNew   (MMgr oMMgr, BOOL bValue)
{
    Boolean pObj = (Boolean)GetMem( oMMgr, sizeof(BooleanRec) );

    if ( pObj ) 
	{
		UtilsMemSet( &pObj->pHeader, 0, sizeof(ObjHeader) );
		pObj->pHeader.nObjClass = OCLASS_BOOLEAN;
		pObj->bValue = bValue;
    }

    return pObj;
}


unsigned long BooleanWrite (Boolean pObj, Stream pStream)
{
    unsigned long nRet = 0;

    if ( pObj->bValue )
        nRet = StreamWriteStr( pStream, "true" );
    else
        nRet = StreamWriteStr( pStream, "false");

    return nRet;
}

//----- NumberObject (int value) ---------------------------------------------

typedef struct TNumberRec  *Number;

typedef struct TNumberRec 
{
    ObjHeader pHeader;
    int       nValue;
} NumberRec;

Number        NumberNew     (MMgr oMMgr, int nValue)
{
    Number pObj = (Number)GetMem( oMMgr, sizeof(NumberRec) );

    if ( pObj ) 
	{
		UtilsMemSet(&pObj->pHeader, 0, sizeof(ObjHeader) );
		pObj->pHeader.nObjClass = OCLASS_NUMBER;
		pObj->nValue = nValue;
    }

    return pObj;
}


unsigned long NumberWrite   (Number pObj, Stream pStream)
{
	return StreamWriteInt( pStream, pObj->nValue );
}


void          NumberSetValue(Number pObj, int nValue)
{
	pObj->nValue = nValue;
}


//----- Real ----------------------------------------------------------------

typedef struct TRealRec  *Real;

typedef struct TRealRec 
{
    ObjHeader pHeader;
    Error     oError;
    float     fValue;
} RealRec;

unsigned long RealSetValue (Real pObj, float fValue)
{
    unsigned long nRet = OK;

    if ( fValue > LIMIT_MAX_REAL )
		return SetError( pObj->oError, AVS_OFFICEPDFWRITER_ERROR_REAL_OUT_OF_RANGE, 0);

    if ( fValue < LIMIT_MIN_REAL )
        return SetError( pObj->oError, AVS_OFFICEPDFWRITER_ERROR_REAL_OUT_OF_RANGE, 0);

	pObj->fValue = fValue;

    return nRet;
}

Real          RealNew      (MMgr oMMgr, float fValue)
{
    Real pObj = (Real)GetMem( oMMgr, sizeof(RealRec) );

    if ( pObj ) 
	{
		UtilsMemSet (&pObj->pHeader, 0, sizeof(ObjHeader) );
		pObj->pHeader.nObjClass = OCLASS_REAL;
		pObj->oError = oMMgr->oError;
        RealSetValue( pObj, fValue);
    }

    return pObj;
}


unsigned long RealWrite    (Real pObj, Stream pStream)
{
    return StreamWriteReal( pStream, pObj->fValue);
}

//----- NameObjects -----------------------------------------------------------

typedef struct TNameRec *Name;

typedef struct TNameRec 
{
    ObjHeader pHeader;
    Error     oError;
    char      sValue[LIMIT_MAX_NAME_LEN + 1];
} NameRec;

unsigned long NameSetValue(Name pObj, const char *sValue)
{
    if ( !sValue || 0 == sValue[0] )
        return SetError( pObj->oError, AVS_OFFICEPDFWRITER_ERROR_NAME_INVALID_VALUE, 0);

    if ( UtilsStrLen(sValue, LIMIT_MAX_NAME_LEN + 1) > LIMIT_MAX_NAME_LEN )
        return SetError( pObj->oError, AVS_OFFICEPDFWRITER_ERROR_NAME_OUT_OF_RANGE, 0);

    UtilsStrCpy( pObj->sValue, sValue, pObj->sValue + LIMIT_MAX_NAME_LEN );

    return OK;
}

Name          NameNew     (MMgr oMMgr, const char *sValue)
{
    Name pObj = (Name)GetMem( oMMgr, sizeof(NameRec) );

    if ( pObj ) 
	{
        UtilsMemSet( &pObj->pHeader, 0, sizeof(ObjHeader) );
        pObj->pHeader.nObjClass = OCLASS_NAME;
        pObj->oError = oMMgr->oError;

        if ( AVS_OFFICEPDFWRITER_ERROR_NAME_INVALID_VALUE == NameSetValue( pObj, sValue ) ) 
		{
            FreeMem( oMMgr, pObj );
            return NULL;
        }
    }

    return pObj;
}


unsigned long NameWrite   (Name pObj, Stream pStream)
{
    return StreamWriteEscapeName( pStream, pObj->sValue );
}

const char*   NameGetValue(Name pObj)
{
    return (const char *)pObj->sValue;
}


//----- StringObject ---------------------------------------------------------

typedef struct TStringRec  *String;

typedef struct TStringRec 
{
    ObjHeader    pHeader;
    MMgr         oMMgr;
    Error        oError;
    Encoder      pEncoder;
    BYTE        *pValue;
    unsigned int nLen;
} StringRec;

static const BYTE UNICODE_HEADER[] = { 0xFE, 0xFF };

unsigned long StringSetValue(String  pObj, const char *sValue)
{
    unsigned int nLen = 0;
    unsigned long nRet = OK;

    if ( pObj->pValue ) 
	{
		FreeMem( pObj->oMMgr, pObj->pValue );
		pObj->nLen = 0;
    }

    nLen = UtilsStrLen( sValue, LIMIT_MAX_STRING_LEN + 1 );

    if ( nLen > LIMIT_MAX_STRING_LEN )
		return SetError( pObj->oError, AVS_OFFICEPDFWRITER_ERROR_STRING_OUT_OF_RANGE, 0);

	pObj->pValue = (BYTE *)GetMem( pObj->oMMgr, nLen + 1 );

	if ( !pObj->pValue )
		return ErrorGetCode( pObj->oError );

	UtilsStrCpy( (char *)pObj->pValue, (char *)sValue, (char *)(pObj->pValue + nLen) );
	pObj->nLen = nLen;

    return nRet;
}

String        StringNew     (MMgr oMMgr, const char *sValue, Encoder pEncoder)
{
    String pObj = (String)GetMem( oMMgr, sizeof(StringRec) );

    if ( pObj ) 
	{
		UtilsMemSet( &pObj->pHeader, 0, sizeof(ObjHeader) );
		pObj->pHeader.nObjClass = OCLASS_STRING;

        pObj->oMMgr    = oMMgr;
		pObj->oError   = oMMgr->oError;
        pObj->pEncoder = pEncoder;
		pObj->pValue   = NULL;
		pObj->nLen     = 0;

        if ( OK != StringSetValue( pObj, sValue) ) 
		{
			FreeMem( pObj->oMMgr, pObj );
            return NULL;
        }
    }

    return pObj;
}


void          StringFree    (String  pObj)
{
    if ( !pObj )
        return;

	FreeMem( pObj->oMMgr, pObj->pValue );
	FreeMem( pObj->oMMgr, pObj);
}


unsigned long StringWrite   (String  pObj, Stream pStream, Encrypt pEncrypt)
{
    unsigned long nRet = 0;

	//Когда encoder не NULL, тект меняется на unicode, используя 
	//соответсвующую кодировку(encoder) и выводится с помощью
	//функции StreamsWriteBinary

    if ( pEncrypt )
        EncryptReset( pEncrypt );

	if ( NULL == pObj->pEncoder ) 
	{
        if ( pEncrypt ) 
		{
            if ( OK != ( nRet = StreamWriteChar( pStream, '<' ) ) )
                return nRet;

			if ( OK != ( nRet = StreamWriteBinary( pStream, pObj->pValue, UtilsStrLen( (const char*)pObj->pValue, -1), pEncrypt ) ) )
                return nRet;

            return StreamWriteChar( pStream, '>' );
        } 
		else 
		{
			return StreamWriteEscapeText( pStream, pObj->pValue, pObj->nLen );
        }
    } 
	else 
	{
		BYTE* pSource = pObj->pValue;
        BYTE  sBuffer[TEXT_DEFAULT_LEN * 2];
        unsigned int nTempLen = 0;
        BYTE* pBuffer = sBuffer;
		int nLen = pObj->nLen;
        ParseTextRec oParseState;

        if ( OK != ( nRet = StreamWriteChar( pStream, '<' ) ) )
			return nRet;

        if ( OK != ( nRet = StreamWriteBinary( pStream, UNICODE_HEADER, 2, pEncrypt ) ) )
			return nRet;

		EncoderSetParseText( pObj->pEncoder, &oParseState, pSource, nLen);

        for ( unsigned int nIndex = 0; nIndex < nLen; nIndex++ ) 
		{
            BYTE nChar = pSource[nIndex];
            unsigned short nTempUnicode;
			ByteType eType = EncoderByteType( pObj->pEncoder, &oParseState);

            if ( nTempLen >= TEXT_DEFAULT_LEN - 1 )
			{
                if ( OK != ( nRet = StreamWriteBinary( pStream, sBuffer, nTempLen * 2, pEncrypt ) ) )
                    return nRet;

                nTempLen = 0;
                pBuffer = sBuffer;
            }

            if ( ByteTypeTrial != eType ) 
			{
                if ( ByteTypeLead == eType ) 
				{
                    BYTE nChar2 = pSource[nIndex + 1];
                    unsigned short nCharCode = (unsigned int) nChar * 256 + nChar2;
					nTempUnicode = EncoderToUnicode( pObj->pEncoder, nCharCode);
                } 
				else 
				{
					nTempUnicode = EncoderToUnicode( pObj->pEncoder, nChar);
                }

                UInt16Swap( &nTempUnicode );
                UtilsMemCpy( pBuffer, (BYTE*)&nTempUnicode, 2);
                pBuffer += 2;
                nTempLen++;
            }
        }

        if ( nTempLen > 0 ) 
		{
            if ( OK != ( nRet = StreamWriteBinary( pStream, sBuffer, nTempLen * 2, pEncrypt ) ) )
                return nRet;
        }

        if ( OK != ( nRet = StreamWriteChar( pStream, '>' ) ) )
            return nRet;
    }

    return OK;
}

//----- BinaryObject ---------------------------------------------------------

typedef struct TBinaryRec  *Binary;

typedef struct TBinaryRec 
{
    ObjHeader   pHeader;
    MMgr         oMMgr;
    Error        oError;
    BYTE        *pValue;
    unsigned int nLen;
} BinaryRec;

unsigned long BinarySetValue  (Binary pObj, BYTE *pValue, unsigned int nLen)
{
    if ( nLen > LIMIT_MAX_STRING_LEN )
		return SetError( pObj->oError, AVS_OFFICEPDFWRITER_ERROR_BINARY_LENGTH_ERR, 0);

	if ( pObj->pValue ) 
	{
		FreeMem( pObj->oMMgr, pObj->pValue );
		pObj->nLen = 0;
    }

	pObj->pValue = (BYTE*)GetMem( pObj->oMMgr, nLen );

	if ( !pObj->pValue )
		return ErrorGetCode( pObj->oError );

    UtilsMemCpy( pObj->pValue, pValue, nLen );
	pObj->nLen = nLen;

    return OK;
}


Binary        BinaryNew       (MMgr   oMMgr, BYTE *pValue, unsigned int nLen)
{
    Binary pObj = (Binary)GetMem( oMMgr, sizeof(BinaryRec) );

    if ( pObj ) 
	{
		UtilsMemSet( &pObj->pHeader, 0, sizeof(ObjHeader) );

		pObj->pHeader.nObjClass = OCLASS_BINARY;

		pObj->oMMgr  = oMMgr;
		pObj->oError = oMMgr->oError;
		pObj->pValue = NULL;
		pObj->nLen   = 0;

        if ( OK != BinarySetValue( pObj, pValue, nLen) ) 
		{
            FreeMem( oMMgr, pObj );
            return NULL;
        }
    }

    return pObj;
}

unsigned long BinaryWrite     (Binary pObj, Stream pStream, Encrypt pEncrypt)
{
    unsigned long nRet = OK;

	if ( 0 == pObj->nLen )
        return StreamWriteStr( pStream, "<>" );

    if ( OK != ( nRet = StreamWriteChar( pStream, '<' ) ) )
        return nRet;

    if ( pEncrypt )
        EncryptReset( pEncrypt );

	if ( OK != ( nRet = StreamWriteBinary( pStream, pObj->pValue, pObj->nLen, pEncrypt ) ) )
        return nRet;

    return StreamWriteChar( pStream, '>' );
}



void          BinaryFree      (Binary pObj)
{
    if ( !pObj )
        return;

	if ( pObj->pValue )
		FreeMem( pObj->oMMgr, pObj->pValue );

	FreeMem( pObj->oMMgr, pObj );
}

unsigned int  BinaryGetLen    (Binary pObj)
{
	return pObj->nLen;
}

BYTE*         BinaryGetValue  (Binary pObj)
{
	return pObj->pValue;
}

//----- ProxyObject ------------------------------------------------------------

typedef struct TProxyRec  *Proxy;

typedef struct TProxyRec 
{
    ObjHeader pHeader;
    void     *pObj;
} ProxyRec;

Proxy ProxyNew  (MMgr oMMgr, void *pObj)
{
    Proxy pNewProxy = (Proxy)GetMem( oMMgr, sizeof(ProxyRec) );

    if ( pNewProxy ) 
	{
		UtilsMemSet( &pNewProxy->pHeader, 0, sizeof(ObjHeader) );
		pNewProxy->pHeader.nObjClass = OCLASS_PROXY;
		pNewProxy->pObj = pObj;
    }

    return pNewProxy;
}


//----- ArrayObject ----------------------------------------------------------

typedef struct TArrayRec  *Array;

typedef struct TArrayRec 
{
    ObjHeader  pHeader;
    MMgr       oMMgr;
    Error      oError;
    List       pList;
} ArrayRec;

Array         ArrayNew       (MMgr  oMMgr)
{
    Array pObj = (Array)GetMem( oMMgr, sizeof(ArrayRec) );

	if ( pObj ) 
	{
        UtilsMemSet( pObj, 0, sizeof(ArrayRec) );
		pObj->pHeader.nObjClass = OCLASS_ARRAY;
        pObj->oMMgr  = oMMgr;
		pObj->oError = oMMgr->oError;
		pObj->pList  = ListNew( oMMgr, DEF_ITEMS_PER_BLOCK );

		if ( !pObj->pList ) 
		{
            FreeMem( oMMgr, pObj );
            pObj = NULL;
        }
    }

    return pObj;
}
unsigned long ArrayWrite     (Array oArray, Stream pStream, Encrypt pEncrypt)
{
    unsigned long nRet = 0;

    nRet = StreamWriteStr( pStream, "[ " );
    if ( OK != nRet )
        return nRet;

	for (unsigned int nIndex = 0; nIndex < oArray->pList->nCount; nIndex++) 
	{
		void *pElement = ListItemAt( oArray->pList, nIndex );

        nRet = ObjWrite( pElement, pStream, pEncrypt);
        if ( OK!= nRet )
            return nRet;

        nRet = StreamWriteChar( pStream, ' ');
        
		if ( OK != nRet )
            return nRet;
    }

    nRet = StreamWriteChar( pStream, ']');

    return nRet;
}


unsigned long ArrayAdd       (Array oArray, void *pObj)
{
    ObjHeader *pHeader = NULL;
    unsigned long nRet = 0;

    if ( !pObj ) 
	{
		if ( OK == ErrorGetCode( oArray->oError ) )
			return SetError( oArray->oError, AVS_OFFICEPDFWRITER_ERROR_INVALID_OBJECT, 0 );
        else
            return AVS_OFFICEPDFWRITER_ERROR_INVALID_OBJECT;
    }

    pHeader = (ObjHeader *)pObj;

	if ( pHeader->nObjId & OTYPE_DIRECT )
		return SetError( oArray->oError, AVS_OFFICEPDFWRITER_ERROR_INVALID_OBJECT, 0 );

	if ( oArray->pList->nCount >= LIMIT_MAX_ARRAY ) 
	{
		ObjFree( oArray->oMMgr, pObj );
		return SetError( oArray->oError, AVS_OFFICEPDFWRITER_ERROR_ARRAY_COUNT_ERR, 0);
    }

	if ( pHeader->nObjId & OTYPE_INDIRECT) 
	{
		Proxy pProxy = ProxyNew( oArray->oMMgr, pObj );

        if ( !pProxy ) 
		{
			ObjFree( oArray->oMMgr, pObj );
			return ErrorGetCode( oArray->oError );
        }

		pProxy->pHeader.nObjId |= OTYPE_DIRECT;
        pObj = pProxy;
    } 
	else
		pHeader->nObjId |= OTYPE_DIRECT;

	nRet = ListAdd( oArray->pList, pObj );
    if ( OK != nRet )
		ObjFree( oArray->oMMgr, pObj );

    return nRet;
}


unsigned long ArrayAddNumber (Array oArray, int nValue)
{
	Number oNumber = NumberNew( oArray->oMMgr, nValue);

	if ( !oNumber )
		return ErrorGetCode( oArray->oError );
    else
        return ArrayAdd( oArray, oNumber );
}


unsigned long ArrayAddReal   (Array oArray, float fValue)
{
    Real oReal = RealNew( oArray->oMMgr, fValue );

    if ( !oReal )
        return ErrorGetCode( oArray->oError );
    else
        return ArrayAdd( oArray, oReal );
}


unsigned long ArrayAddName   (Array oArray, const char *sValue)
{
    Name oName = NameNew( oArray->oMMgr, sValue );

    if ( !oName )
        return ErrorGetCode( oArray->oError );
    else
        return ArrayAdd( oArray, oName );
}

unsigned long ArrayInsert    (Array oArray, void *pTarget, void *pObj)
{
    ObjHeader *pHeader = NULL;
    unsigned long nRet = OK;

	if ( !pObj ) 
	{
		if ( OK == ErrorGetCode( oArray->oError ) )
			return SetError( oArray->oError, AVS_OFFICEPDFWRITER_ERROR_INVALID_OBJECT, 0 );
        else
            return AVS_OFFICEPDFWRITER_ERROR_INVALID_OBJECT;
    }

    pHeader = (ObjHeader *)pObj;

	if ( pHeader->nObjId & OTYPE_DIRECT) 
	{
		return SetError( oArray->oError, AVS_OFFICEPDFWRITER_ERROR_INVALID_OBJECT, 0 );
    }

	if ( oArray->pList->nCount >= LIMIT_MAX_ARRAY ) 
	{
		ObjFree( oArray->oMMgr, pObj );

		return SetError( oArray->oError, AVS_OFFICEPDFWRITER_ERROR_ARRAY_COUNT_ERR, 0 );
    }

	if ( pHeader->nObjId & OTYPE_INDIRECT) 
	{
		Proxy pProxy = ProxyNew( oArray->oMMgr, pObj );

        if ( !pProxy ) 
		{
			ObjFree( oArray->oMMgr, pObj );
			return ErrorGetCode( oArray->oError );
        }

		pProxy->pHeader.nObjId |= OTYPE_DIRECT;
        pObj = pProxy;
    } 
	else
		pHeader->nObjId |= OTYPE_DIRECT;

    //получаем target-object из списка
    //рассмотреть случай, когда указатель на содержимое списка
	//может быть proxy-object.
     
	for ( unsigned int nIndex = 0; nIndex < oArray->pList->nCount; nIndex++) 
	{
		void *pItem = ListItemAt( oArray->pList, nIndex );
        void *pObjItem = NULL;

        pHeader = (ObjHeader *)pObj;
		if ( OCLASS_PROXY == pHeader->nObjClass )
			pObjItem = ((Proxy)pItem)->pObj;
        else
            pObjItem = pItem;

        if ( pObjItem == pTarget ) 
		{
			nRet = ListInsert( oArray->pList, pItem, pObj);

			if ( OK != nRet )
				ObjFree( oArray->oMMgr, pObj);

            return nRet;
        }
    }

	ObjFree( oArray->oMMgr, pObj );

    return AVS_OFFICEPDFWRITER_ERROR_ITEM_NOT_FOUND;
}


void*         ArrayGetItem   (Array oArray, unsigned int nIndex, unsigned int nObjClass)
{
	void *pObj = ListItemAt( oArray->pList, nIndex );
    ObjHeader *pHeader;

    if ( !pObj ) 
	{
		SetError( oArray->oError, AVS_OFFICEPDFWRITER_ERROR_ARRAY_ITEM_NOT_FOUND, 0 );
        return NULL;
    }

    pHeader = (ObjHeader*)pObj;

	if ( pHeader->nObjClass == OCLASS_PROXY) 
	{
		pObj    = ((Proxy)pObj)->pObj;
        pHeader = (ObjHeader *)pObj;
    }

	if ( ( pHeader->nObjClass & OCLASS_ANY ) != nObjClass ) 
	{
		SetError( oArray->oError, AVS_OFFICEPDFWRITER_ERROR_ARRAY_ITEM_UNEXPECTED_TYPE, 0);
        return NULL;
    }

    return pObj;
}

void          ArrayClear     (Array oArray)
{
    if ( !oArray )
        return;

	for (unsigned int nIndex = 0; nIndex < oArray->pList->nCount; nIndex++) 
	{
		void *pObj = ListItemAt( oArray->pList, nIndex );

        if ( pObj )  
		{
			ObjFree( oArray->oMMgr, pObj );
        }
    }

	ListClear( oArray->pList );
}

void          ArrayFree      (Array oArray)
{
    if ( !oArray )
        return;

	ArrayClear( oArray );

    ListFree( oArray->pList );

    oArray->pHeader.nObjClass = 0;

    FreeMem( oArray->oMMgr, oArray );
}

Array         BoxArrayNew    (MMgr  oMMgr, Box oBox)
{
    Array pObj = ArrayNew( oMMgr );
    unsigned long nRet = OK;

	if ( !pObj )
        return NULL;

	nRet += ArrayAdd( pObj, RealNew( oMMgr, oBox.fLeft ) );
	nRet += ArrayAdd( pObj, RealNew( oMMgr, oBox.fBottom ) );
	nRet += ArrayAdd( pObj, RealNew( oMMgr, oBox.fRight ) );
	nRet += ArrayAdd( pObj, RealNew( oMMgr, oBox.fTop ) );

    if ( OK != nRet ) 
	{
        ArrayFree( pObj );
        return NULL;
    }

    return pObj;
}


//----- DictObject -----------------------------------------------------------

typedef struct TXrefRec *Xref;
unsigned long XrefAdd                 (Xref pXref, void *pObj);

typedef struct TDictRec *Dict;

typedef void          (*DictFreeFunc)       (Dict pObj);
typedef unsigned long (*DictBeforeWriteFunc)(Dict pObj);
typedef unsigned long (*DictAfterWriteFunc) (Dict pObj);
typedef unsigned long (*DictOnWriteFunc)    (Dict pObj, Stream pStream);

typedef struct TDictRec 
{
    ObjHeader           pHeader;
    MMgr                oMMgr;
    Error               oError;
    List                pList;
    DictBeforeWriteFunc pBeforeWriteFn;
    DictOnWriteFunc     pWriteFn;
    DictAfterWriteFunc  pAfterWriteFn;
    DictFreeFunc        pFreeFn;
    Stream              pStream;
    unsigned int        nFilter;
	unsigned int        nPredictor;
	CAtlMap<CString, int> *pKeyMap;
    void               *pAttr;

	CString*			sBuffer;
} DictRec;

typedef struct TDictElementRec *DictElement;

typedef struct TDictElementRec 
{
    char  sKey[LIMIT_MAX_NAME_LEN + 1];
    void *pValue;
} DictElementRec;

Dict          DictNew           (MMgr oMMgr)
{
    Dict pObj = (Dict)GetMem( oMMgr, sizeof(DictRec) );

	if ( pObj ) 
	{
        UtilsMemSet( pObj, 0, sizeof(DictRec) );
		pObj->pHeader.nObjClass = OCLASS_DICT;
		pObj->oMMgr   = oMMgr;
		pObj->oError  = oMMgr->oError;
		pObj->pList   = ListNew( oMMgr, DEF_ITEMS_PER_BLOCK );
		pObj->nFilter = STREAM_FILTER_NONE;
		pObj->pKeyMap = new CAtlMap<CString, int>;
		pObj->sBuffer = NULL;

		if ( !pObj->pList ) 
		{
            FreeMem( oMMgr, pObj );
            pObj = NULL;
        }
    }

    return pObj;
}


DictElement   GetElement        (Dict oDict, const char *sKey)
{
	CAtlMap<CString, int>::CPair* pEl = oDict->pKeyMap->Lookup( CString( sKey ) );

	if ( NULL != pEl )
		return (DictElement)ListItemAt( oDict->pList, pEl->m_value );

	/*
	for ( unsigned int nIndex = 0; nIndex < oDict->pList->nCount; nIndex++) 
	{
		DictElement oElement = (DictElement)ListItemAt( oDict->pList, nIndex );
		if ( 0 == UtilsStrCmp( sKey, oElement->sKey ) )
            return oElement;
    }
	*/

    return NULL;
}


unsigned long DictAdd           (Dict oDict, const char *sKey, void *pObj)
{
    ObjHeader *pHeader;
    unsigned long nRet = OK;
    DictElement oElement;

    if ( !pObj ) 
	{
		if ( OK == ErrorGetCode( oDict->oError ) )
			return SetError( oDict->oError, AVS_OFFICEPDFWRITER_ERROR_INVALID_OBJECT, 0 );
        else
            return AVS_OFFICEPDFWRITER_ERROR_INVALID_OBJECT;
    }

    pHeader = (ObjHeader *)pObj;

	if ( pHeader->nObjId & OTYPE_DIRECT )
		return SetError( oDict->oError, AVS_OFFICEPDFWRITER_ERROR_INVALID_OBJECT, 0 );

    if ( !sKey ) 
	{
		ObjFree( oDict->oMMgr, pObj );
		return SetError( oDict->oError, AVS_OFFICEPDFWRITER_ERROR_INVALID_OBJECT, 0 );
    }

	if ( oDict->pList->nCount >= LIMIT_MAX_DICT_ELEMENT ) 
	{
        ObjFree( oDict->oMMgr, pObj );
		return SetError( oDict->oError, AVS_OFFICEPDFWRITER_ERROR_DICT_COUNT_ERR, 0 );
    }

    // Проверяем присутствует ли элемент с таким же названием
    oElement = GetElement( oDict, sKey );

    if ( oElement ) 
	{
		ObjFree( oDict->oMMgr, oElement->pValue );
		oElement->pValue = NULL;
    } 
	else 
	{
		oElement = (DictElement)GetMem( oDict->oMMgr, sizeof(DictElementRec) );

        if ( !oElement ) 
		{
            // невозможно создать объект Element
			if ( !( pHeader->nObjId & OTYPE_INDIRECT ) )
				ObjFree( oDict->oMMgr, pObj );

			return ErrorGetCode( oDict->oError );
        }

		UtilsStrCpy( oElement->sKey, sKey, oElement->sKey + LIMIT_MAX_NAME_LEN + 1);
		oElement->pValue = NULL;

		nRet = ListAdd( oDict->pList, oElement );
		oDict->pKeyMap->SetAt( CString( sKey ), oDict->pList->nCount - 1 );

		if ( OK != nRet ) 
		{
			if ( !( pHeader->nObjId & OTYPE_INDIRECT ) )
                ObjFree( oDict->oMMgr, pObj );

			FreeMem( oDict->oMMgr, oElement );

			return ErrorGetCode( oDict->oError );
        }
    }

	if ( pHeader->nObjId & OTYPE_INDIRECT ) 
	{
		Proxy pProxy = ProxyNew( oDict->oMMgr, pObj );

        if ( !pProxy )
			return ErrorGetCode( oDict->oError );

		oElement->pValue = pProxy;
		pProxy->pHeader.nObjId |= OTYPE_DIRECT;
    } 
	else 
	{
		oElement->pValue = pObj;
		pHeader->nObjId |= OTYPE_DIRECT;
    }

    return nRet;
}


Dict          DictStreamNew     (MMgr oMMgr, Xref oXref, BOOL bMemoryStream = TRUE, const wchar_t *wsTempFile = NULL, unsigned int unCheckSum = 0)
{
    Dict pObj = DictNew( oMMgr );

	if ( !pObj )
        return NULL;

	Number oLength;
    unsigned long nRet = 0;

    // только stream object добавляются в таблицу xref автоматически

    nRet += XrefAdd( oXref, pObj);   
	if ( OK != nRet )
        return NULL;

    oLength = NumberNew( oMMgr, 0 );
    if ( !oLength )
        return NULL;

    nRet = XrefAdd( oXref, oLength);
    if ( OK != nRet )
        return NULL;

    nRet = DictAdd( pObj, "Length", oLength);
    if ( OK != nRet )
        return NULL;

	if ( bMemoryStream )
		pObj->pStream = MemStreamNew( oMMgr, STREAM_BUF_SIZ );
	else
		pObj->pStream = TempFileStreamNew( oMMgr, wsTempFile, unCheckSum );
	
	if ( !pObj->pStream )
        return NULL;

    return pObj;
}


void          DictFree          (Dict oDict)
{
    if ( !oDict )
        return;

	if ( oDict->pFreeFn )
		oDict->pFreeFn( oDict );

	for ( unsigned int nIndex = 0; nIndex < oDict->pList->nCount; nIndex++) 
	{
		DictElement oElement = (DictElement)ListItemAt( oDict->pList, nIndex );

        if ( oElement ) 
		{
			ObjFree( oDict->oMMgr, oElement->pValue );
            FreeMem( oDict->oMMgr, oElement);
        }
    }

	if ( oDict->pStream )
		StreamFree( oDict->pStream );

	ListFree( oDict->pList );

	oDict->pHeader.nObjClass = 0;
	delete oDict->pKeyMap;
	
	RELEASEOBJECT(oDict->sBuffer);

    FreeMem( oDict->oMMgr, oDict );
}


unsigned long DictRemoveElement (Dict oDict, const char *sKey)
{
	CString csKey( sKey );
	CAtlMap<CString, int>::CPair* pEl = oDict->pKeyMap->Lookup( csKey );

	if ( NULL != pEl )
	{
		DictElement oElement = (DictElement)ListItemAt( oDict->pList, pEl->m_value );
		ListRemove( oDict->pList, oElement);

		ObjFree( oDict->oMMgr, oElement->pValue );
		FreeMem( oDict->oMMgr, oElement);
		oDict->pKeyMap->RemoveKey( csKey );

		return OK;
	}

	/*
	for ( unsigned int nIndex = 0; nIndex < oDict->pList->nCount; nIndex++) 
	{
		DictElement oElement = (DictElement)ListItemAt( oDict->pList, nIndex );

		if ( 0 == UtilsStrCmp( sKey, oElement->sKey ) ) 
		{
			ListRemove( oDict->pList, oElement);

			ObjFree( oDict->oMMgr, oElement->pValue );
			FreeMem( oDict->oMMgr, oElement);

            return OK;
        }
    }

	*/

    return AVS_OFFICEPDFWRITER_ERROR_DICT_ITEM_NOT_FOUND;
}
void*         DictGetItem       (Dict oDict, const char *sKey, unsigned short nObjClass )
{
    DictElement oElement = GetElement( oDict, sKey );
    void *pObj = NULL;

	if ( oElement && UtilsStrCmp( sKey, oElement->sKey ) == 0) 
	{
		ObjHeader *pHeader = (ObjHeader *)oElement->pValue;

		if ( pHeader->nObjClass == OCLASS_PROXY ) 
		{
			Proxy pProxy = (Proxy)oElement->pValue;
			pHeader = (ObjHeader *)pProxy->pObj;
			pObj = pProxy->pObj;
        } 
		else
			pObj = oElement->pValue;

		if ( ( pHeader->nObjClass & OCLASS_ANY ) != nObjClass ) 
		{
			SetError( oDict->oError, AVS_OFFICEPDFWRITER_ERROR_DICT_ITEM_UNEXPECTED_TYPE, 0 );
            return NULL;
        }

        return pObj;
    }

    return NULL;
}


unsigned long DictAddName       (Dict oDict, const char *sKey, const char *sValue)
{
    Name oName = NameNew( oDict->oMMgr, sValue);
    if ( !oName )
        return ErrorGetCode( oDict->oError );

    return DictAdd( oDict, sKey, oName);
}


unsigned long DictAddNumber     (Dict oDict, const char *sKey, int         nValue)
{
	Number oNumber = NumberNew( oDict->oMMgr, nValue );

    if ( !oNumber )
		return ErrorGetCode( oDict->oError );

    return DictAdd( oDict, sKey, oNumber );
}


unsigned long DictAddReal       (Dict oDict, const char *sKey, float       fValue)
{
	Real oReal = RealNew( oDict->oMMgr, fValue);

    if ( !oReal )
		return ErrorGetCode( oDict->oError );

    return DictAdd( oDict, sKey, oReal );
}


unsigned long DictAddBoolean    (Dict oDict, const char *sKey, BOOL        bValue)
{
	Boolean oBool = BooleanNew( oDict->oMMgr, bValue );

    if ( !oBool )
		return ErrorGetCode( oDict->oError );

    return DictAdd( oDict, sKey, oBool );
}


unsigned long DictWrite         (Dict oDict, Stream pStream, Encrypt pEncrypt )
{
	unsigned long nRet = 0;

	if (oDict->sBuffer)
	{
		nRet = StreamWriteStr(pStream, CStringA(*oDict->sBuffer));
		RELEASEOBJECT(oDict->sBuffer);
		return nRet;
	}

    nRet = StreamWriteStr( pStream, "<<\012");
    if ( OK != nRet )
        return nRet;

	if ( oDict->pBeforeWriteFn ) 
	{
		if ( OK != ( nRet = oDict->pBeforeWriteFn( oDict ) ) )
            return nRet;
    }

    // Encrypt-dict не надо шифровать 
	if ( oDict->pHeader.nObjClass == (OCLASS_DICT | OSUBCLASS_ENCRYPT) )
        pEncrypt = NULL;

	if ( oDict->pStream ) 
	{
        // записываем эелемент filter 
		if ( STREAM_FILTER_NONE == oDict->nFilter )
            DictRemoveElement( oDict, "Filter");
        else 
		{
            Array oArray = (Array)DictGetItem( oDict, "Filter", OCLASS_ARRAY);

            if ( !oArray) 
			{
				oArray = ArrayNew( oDict->oMMgr );
                if ( !oArray )
					return ErrorGetCode( oDict->oError );

                nRet = DictAdd( oDict, "Filter", oArray );
                if ( OK != nRet)
                    return nRet;
            }

            ArrayClear( oArray );
#ifndef NOZLIB
#ifndef _DEBUG

            if ( oDict->nFilter & STREAM_FILTER_FLATE_DECODE )
                ArrayAddName( oArray, "FlateDecode");
#endif
#endif /* NOZLIB */

			if ( oDict->nFilter & STREAM_FILTER_DCT_DECODE )
                ArrayAddName( oArray, "DCTDecode" );

			if ( oDict->nFilter & STREAM_FILTER_JPX_DECODE )
				ArrayAddName( oArray, "JPXDecode" );

			if ( oDict->nFilter & STREAM_FILTER_JBIG2_DECODE )
				ArrayAddName( oArray, "JBIG2Decode" );

			if ( oDict->nFilter & STREAM_FILTER_LZW_DECODE )
				ArrayAddName( oArray, "LZWDecode" );

			// Записываем элемент DecodeParams
			if ( STREAM_PREDICTOR_NONE == oDict->nPredictor )
				DictRemoveElement( oDict, "DecodeParams" );
			else
			{
				Array oDecodeParams = (Array)DictGetItem( oDict, "DecodeParams", OCLASS_ARRAY );
				if ( !oDecodeParams )
				{
					oDecodeParams = ArrayNew( oDict->oMMgr );
					if ( !oDecodeParams )
						return ErrorGetCode( oDict->oError );

					nRet = DictAdd( oDict, "DecodeParams", oDecodeParams );
					if ( OK != nRet )
						return nRet;					
				}
#ifndef NOZLIB
#ifndef _DEBUG
				if ( oDict->nFilter & STREAM_FILTER_FLATE_DECODE )
				{
					unsigned short nPredictor = 1;
					//if ( STREAM_PREDICTOR_FLATE_TIFF == oDict->nPredictor )

					Null oNullObject = NullNew( oDict->oMMgr );
					ArrayAdd( oDecodeParams, oNullObject );
				}
#endif
#endif /* NOZLIB */
				if ( oDict->nFilter & STREAM_FILTER_DCT_DECODE )
				{
					Null oNullObject = NullNew( oDict->oMMgr );
					ArrayAdd( oDecodeParams, oNullObject );
				}
				if ( oDict->nFilter & STREAM_FILTER_JPX_DECODE )
				{
					Null oNullObject = NullNew( oDict->oMMgr );
					ArrayAdd( oDecodeParams, oNullObject );
				}
				if ( oDict->nFilter & STREAM_FILTER_JBIG2_DECODE )
				{
					Null oNullObject = NullNew( oDict->oMMgr );
					ArrayAdd( oDecodeParams, oNullObject );
				}
				if ( oDict->nFilter & STREAM_FILTER_LZW_DECODE )
				{
					Null oNullObject = NullNew( oDict->oMMgr );
					ArrayAdd( oDecodeParams, oNullObject );
				}
			}
        }
    }

	for ( unsigned int nIndex = 0; nIndex < oDict->pList->nCount; nIndex++) 
	{
		DictElement oElement = (DictElement)ListItemAt( oDict->pList, nIndex);
		ObjHeader *pHeader = (ObjHeader *)(oElement->pValue);

		if ( !oElement->pValue)
			return SetError( oDict->oError, AVS_OFFICEPDFWRITER_ERROR_INVALID_OBJECT, 0 );

		if ( pHeader->nObjId & OTYPE_HIDDEN) 
		{
			// ничего не делаем
        } 
		else 
		{
			nRet = StreamWriteEscapeName( pStream, oElement->sKey );
            if ( OK != nRet )
                return nRet;

            nRet = StreamWriteChar( pStream, ' ');
            if ( OK != nRet )
                return nRet;

			nRet = ObjWrite( oElement->pValue, pStream, pEncrypt);
            if ( OK != nRet )
                return nRet;

            nRet = StreamWriteStr( pStream, "\012" );
            if ( OK != nRet )
                return nRet;
        }
    }

	if ( oDict->pWriteFn ) 
	{
		if ( OK != ( nRet = oDict->pWriteFn( oDict, pStream ) ) )
            return nRet;
    }

    if ( OK != ( nRet = StreamWriteStr( pStream, ">>" ) ) )
        return nRet;

	if ( oDict->pStream ) 
	{
        unsigned int nStrPointer = 0;
        Number oLength;

        // get "oLength" element
        oLength = (Number)DictGetItem( oDict, "Length", OCLASS_NUMBER);
        if ( !oLength )
			return SetError( oDict->oError, AVS_OFFICEPDFWRITER_ERROR_DICT_STREAM_LENGTH_NOT_FOUND, 0 );

        // "length" должен иметь тип indirect-object
        
		if ( !( oLength->pHeader.nObjId & OTYPE_INDIRECT) ) 
		{
			return SetError( oDict->oError, AVS_OFFICEPDFWRITER_ERROR_DICT_ITEM_UNEXPECTED_TYPE, 0);
        }

        if ( OK != ( nRet = StreamWriteStr( pStream, "\012stream\015\012" ) ) )
            return nRet;

		nStrPointer = pStream->nSize;

        if ( pEncrypt )
            EncryptReset( pEncrypt );

		if ( OK != ( nRet = StreamWriteToStream( oDict->pStream, pStream, oDict->nFilter, pEncrypt ) ) )
            return nRet;

		NumberSetValue( oLength, pStream->nSize - nStrPointer );

        nRet = StreamWriteStr( pStream, "\012endstream" );
    }

	if ( oDict->pAfterWriteFn ) 
	{
		if ( OK != ( nRet = oDict->pAfterWriteFn( oDict ) ) )
            return nRet;
    }

    return nRet;
}

const char*   DictGetKeyByObj   (Dict oDict, void *pObj)
{
	for (unsigned int nIndex = 0; nIndex < oDict->pList->nCount; nIndex++) 
	{
        ObjHeader *pHeader;
		DictElement oElement = (DictElement)ListItemAt( oDict->pList, nIndex );

		pHeader = (ObjHeader *)( oElement->pValue );
		if ( pHeader->nObjClass == OCLASS_PROXY) 
		{
			Proxy pProxy = (Proxy)oElement->pValue;

			if ( pObj == pProxy->pObj )
				return oElement->sKey;
        } 
		else 
		{
			if ( pObj == oElement->pValue )
				return oElement->sKey;
        }
    }

    return NULL;
}



//----- XrefObject -----------------------------------------------------------

typedef struct TXrefEntryRec  *XrefEntry;

typedef struct TXrefEntryRec 
{
      char           sEntryType;
      unsigned int   nByteOffset;
      unsigned short nGenNo;
      void*          pObj;
} XrefEntryRec;

typedef struct TXrefRec 
{
      MMgr         oMMgr;
      Error        oError;
      unsigned int nStartOffset;
      List         pEntries;
      unsigned int nAddr;
      Xref         pPrev;
      Dict         oTrailer;
} XrefRec;

typedef Dict  Pages;
typedef Dict  Page;
typedef Dict  Annotation;
typedef Dict  XObject;
typedef Dict  ImageDict;
typedef Dict  Outline;
typedef Dict  EncryptDict;
typedef Dict  Action;
typedef Dict  ExtGState;
typedef Array Destination;

XrefEntry     XrefGetEntry            (Xref pXref, unsigned int nIndex)
{
	return (XrefEntry)ListItemAt( pXref->pEntries, nIndex);
}


void          XrefFree                (Xref pXref)
{
    XrefEntry pEntry = NULL;
    Xref pTempXref = NULL;

    // удаляем элементы xref, у которых значение prev не NULL 
    // удаление происходит рекурсивное
    while ( pXref ) 
	{
        // удаляем все объекты принадлежащие xref

		if ( pXref->pEntries ) 
		{
			for (unsigned int nIndex = 0; nIndex < pXref->pEntries->nCount; nIndex++) 
			{
                pEntry = XrefGetEntry( pXref, nIndex );
				if ( pEntry->pObj ) 
					ObjForceFree( pXref->oMMgr, pEntry->pObj );
				FreeMem( pXref->oMMgr, pEntry );
            }

			ListFree( pXref->pEntries );
        }

		if ( pXref->oTrailer )
			DictFree( pXref->oTrailer );

		pTempXref = pXref->pPrev;
		FreeMem( pXref->oMMgr, pXref);
        pXref = pTempXref;
    }
}


Xref          XrefNew                 (MMgr oMMgr, unsigned int nOffset)
{
    Xref pXref;
    XrefEntry pNewEntry;

    pXref = (Xref)GetMem( oMMgr, sizeof(XrefRec) );
    if ( !pXref )
        return NULL;

    UtilsMemSet( pXref, 0, sizeof(XrefRec) );
	pXref->oMMgr        = oMMgr;
	pXref->oError       = oMMgr->oError;
	pXref->nStartOffset = nOffset;

	pXref->pEntries = ListNew( oMMgr, DEFALUT_XREF_ENTRY_NUM );
	if ( !pXref->pEntries)
	{
		XrefFree( pXref );
		return NULL;
	}

	pXref->nAddr = 0;

	if ( 0 == pXref->nStartOffset ) 
	{
        pNewEntry = (XrefEntry)GetMem( oMMgr, sizeof(XrefEntryRec) );
        if ( !pNewEntry )
		{
			XrefFree( pXref );
			return NULL;
		}

		if ( OK != ListAdd( pXref->pEntries, pNewEntry ) ) 
		{
            FreeMem( oMMgr, pNewEntry );
			XrefFree( pXref );
			return NULL;
        }

        // добавляем первый элемент в таблицу xref
		// он должен иметь вид 0000000000 65535 f
		pNewEntry->sEntryType  = FREE_ENTRY;
		pNewEntry->nByteOffset = 0;
		pNewEntry->nGenNo      = MAX_GENERATION_NUM;
		pNewEntry->pObj        = NULL;
    }

	pXref->oTrailer = DictNew( oMMgr );
	if ( !pXref->oTrailer )
	{
		XrefFree( pXref );
		return NULL;
	}

    return pXref;
}

unsigned long XrefAdd                 (Xref pXref, void *pObj)
{
    XrefEntry pEntry = NULL;
    ObjHeader *pHeader = NULL;

    if ( !pObj ) 
	{
		if ( OK == ErrorGetCode( pXref->oError ) )
			return SetError( pXref->oError, AVS_OFFICEPDFWRITER_ERROR_INVALID_OBJECT, 0);
        else
            return AVS_OFFICEPDFWRITER_ERROR_INVALID_OBJECT;
    }

    pHeader = (ObjHeader *)pObj;

	if ( pHeader->nObjId & OTYPE_DIRECT || pHeader->nObjId & OTYPE_INDIRECT )
		return SetError( pXref->oError, AVS_OFFICEPDFWRITER_ERROR_INVALID_OBJECT, 0 );

	if ( pXref->pEntries->nCount >= LIMIT_MAX_XREF_ELEMENT ) 
	{
		SetError( pXref->oError, AVS_OFFICEPDFWRITER_ERROR_XREF_COUNT_ERR, 0 );
		ObjForceFree( pXref->oMMgr, pObj );
		return ErrorGetCode( pXref->oError );
    }

    // В случае ошибки r объектe нужно применить dispose

	pEntry = (XrefEntry)GetMem( pXref->oMMgr, sizeof(XrefEntryRec) );
    if ( NULL == pEntry )
	{
		ObjForceFree( pXref->oMMgr, pObj );
		return ErrorGetCode( pXref->oError );
	}

	if ( OK != ListAdd( pXref->pEntries, pEntry ) ) 
	{
		FreeMem( pXref->oMMgr, pEntry );
		ObjForceFree( pXref->oMMgr, pObj );
		return ErrorGetCode( pXref->oError );
    }

	pEntry->sEntryType  = IN_USE_ENTRY;
	pEntry->nByteOffset = 0;
	pEntry->nGenNo      = 0;
	pEntry->pObj        = pObj;
	pHeader->nObjId = pXref->nStartOffset + pXref->pEntries->nCount - 1 + OTYPE_INDIRECT;
	pHeader->nGenNo = pEntry->nGenNo;

    return OK;
}

XrefEntry     XrefGetEntryByObjectId  (Xref pXref, unsigned int nObjId)
{
    Xref pTempXref = pXref;

    while ( pTempXref ) 
	{
        if ( pTempXref->pEntries->nCount + pTempXref->nStartOffset > nObjId) 
		{
            SetError( pXref->oError, AVS_OFFICEPDFWRITER_ERROR_INVALID_OBJ_ID, 0 );
            return NULL;
        }

        if ( pTempXref->nStartOffset < nObjId ) 
		{
            for ( unsigned int nIndex = 0; nIndex < pTempXref->pEntries->nCount; nIndex++ ) 
			{
				if ( pTempXref->nStartOffset + nIndex == nObjId ) 
				{
                    XrefEntry pEntry = XrefGetEntry( pTempXref, nIndex );
                    return pEntry;
                }
            }
        }

		pTempXref = pTempXref->pPrev;
    }

    return NULL;
}


static unsigned long WriteTrailer     (Xref pXref, Stream pStream)
{
	unsigned int nMaxObjId = pXref->pEntries->nCount + pXref->nStartOffset;
    unsigned long nRet = 0; 

	if ( OK != ( nRet = DictAddNumber( pXref->oTrailer, "Size", nMaxObjId ) ) )
        return nRet;

	if ( pXref->pPrev )
		if ( OK != ( nRet = DictAddNumber( pXref->oTrailer, "Prev", pXref->pPrev->nAddr ) ) )
            return nRet;

    if ( OK != ( nRet = StreamWriteStr( pStream, "trailer\012") ) )
        return nRet;

	if ( OK != ( nRet = DictWrite( pXref->oTrailer, pStream, NULL) ) )
        return nRet;

    if ( OK != ( nRet = StreamWriteStr( pStream, "\012startxref\012") ) )
        return nRet;

	if ( OK != ( nRet = StreamWriteUInt( pStream, pXref->nAddr ) ) )
        return nRet;

    if ( OK != ( nRet = StreamWriteStr( pStream, "\012%%EOF\012") ) )
        return nRet;

    return OK;
}


unsigned long XrefWriteToStream       (Xref pXref, Stream pStream, Encrypt pEncrypt)
{
    unsigned long nRet = 0;
    char sBuf[SHORT_BUFFER_SIZE];
    char* pBuf;
    char* pEndPtr = sBuf + SHORT_BUFFER_SIZE - 1;
    unsigned int nStartIndex = 0;
    Xref pTempXref = pXref;

    // записываем каждый объект таблицы xref в поток pStream 

    while ( pTempXref ) 
	{
		if ( pTempXref->nStartOffset == 0 )
            nStartIndex = 1;
        else
            nStartIndex = 0;

		for (unsigned int nIndex = nStartIndex; nIndex < pTempXref->pEntries->nCount; nIndex++) 
		{
			XrefEntry pEntry = (XrefEntry)ListItemAt( pTempXref->pEntries, nIndex );
			unsigned int nObjId = pTempXref->nStartOffset + nIndex;
			unsigned short nGenNo = pEntry->nGenNo;

			pEntry->nByteOffset = pStream->nSize;

            pBuf = sBuf;
            pBuf = UtilsIToA( pBuf, nObjId, pEndPtr );
            *pBuf++ = ' ';
            pBuf = UtilsIToA( pBuf, nGenNo, pEndPtr );
            UtilsStrCpy( pBuf, " obj\012", pEndPtr );

            if ( OK != ( nRet = StreamWriteStr( pStream, sBuf ) ) )
               return nRet;

            if ( pEncrypt )
                EncryptInitKey( pEncrypt, nObjId, nGenNo);

			if ( OK != ( nRet = ObjWriteValue( pEntry->pObj, pStream, pEncrypt) ) )
                return nRet;

            if ( OK != ( nRet = StreamWriteStr( pStream, "\012endobj\012") ) )
                return nRet;
       }

		pTempXref = pTempXref->pPrev;
    }

    // начинаем писать cross-reference table

    pTempXref = pXref;

    while ( pTempXref ) 
	{
		pTempXref->nAddr = pStream->nSize;

        pBuf = sBuf;
        pBuf = (char*)UtilsStrCpy( pBuf, "xref\012", pEndPtr );
		pBuf = UtilsIToA( pBuf, pTempXref->nStartOffset, pEndPtr );
        *pBuf++ = ' ';
		pBuf = UtilsIToA( pBuf, pTempXref->pEntries->nCount, pEndPtr );
        UtilsStrCpy( pBuf, "\012", pEndPtr);
        nRet = StreamWriteStr( pStream, sBuf );
        if ( OK != nRet )
            return nRet;

		for ( unsigned int nIndex = 0; nIndex < pTempXref->pEntries->nCount; nIndex++) 
		{
            XrefEntry pEntry = XrefGetEntry( pTempXref, nIndex );

            pBuf = sBuf;
			pBuf = UtilsIToA2( pBuf, pEntry->nByteOffset, BYTE_OFFSET_LEN + 1);
            *pBuf++ = ' ';
			pBuf = UtilsIToA2( pBuf, pEntry->nGenNo, GEN_NO_LEN + 1);
            *pBuf++ = ' ';
			*pBuf++ = pEntry->sEntryType;
            UtilsStrCpy( pBuf, "\015\012", pEndPtr );
            nRet = StreamWriteStr( pStream, sBuf );
            if ( OK != nRet )
                return nRet;
        }
		pTempXref = pTempXref->pPrev;
    }

    // пишем trailer
    nRet = WriteTrailer( pXref, pStream );

    return nRet;
}

// Основные функции для работы с объектами
unsigned long ObjWriteValue (void *pObj, Stream pStream, Encrypt pEncrypt)
{
    ObjHeader *pHeader;
    unsigned long nRet = 0;

    pHeader = (ObjHeader *)pObj;

    switch ( pHeader->nObjClass & OCLASS_ANY ) 
	{
        case OCLASS_NAME:
            nRet = NameWrite( (Name)pObj, pStream);
            break;
        case OCLASS_NUMBER:
            nRet = NumberWrite( (Number)pObj, pStream);
            break;
        case OCLASS_REAL:
            nRet = RealWrite( (Real)pObj, pStream);
            break;
        case OCLASS_STRING:
            nRet = StringWrite( (String)pObj, pStream, pEncrypt);
            break;
        case OCLASS_BINARY:
            nRet = BinaryWrite( (Binary)pObj, pStream, pEncrypt);
            break;
        case OCLASS_ARRAY:
            nRet = ArrayWrite( (Array)pObj, pStream, pEncrypt);
            break;
        case OCLASS_DICT:
            nRet = DictWrite( (Dict)pObj, pStream, pEncrypt);
            break;
        case OCLASS_BOOLEAN:
            nRet = BooleanWrite( (Boolean)pObj, pStream);
            break;
        case OCLASS_NULL:
            nRet = StreamWriteStr( pStream, "null");
			break;
		
		// TODO: передать на прямые вызовы

		case OCLASS_XFORM:
			{
				Proxy proxy = (Proxy)pObj;
				if (proxy)
				{
					PDF::XForm* obj = static_cast<PDF::XForm*>(proxy->pObj);
					if (obj)
					{
						StreamWriteStr(pStream, CStringA(obj->Define()));
					}
				}
			}
			break;

		case OCLASS_MASK_FORM:
			{
				Proxy proxy = (Proxy)pObj;
				if (proxy)
				{
					PDF::PatternAlphaGroup* obj = static_cast<PDF::PatternAlphaGroup*>(proxy->pObj);
					if (obj)
					{
						StreamWriteStr(pStream, CStringA(obj->Define()));
					}
				}
			}
			break;
	
		case OCLASS_MASK_REF:
			{
				Proxy proxy = (Proxy)pObj;
				if (proxy)
				{
					PDF::SoftMask* mask = static_cast<PDF::SoftMask*>(proxy->pObj);
					if (mask)
					{
						StreamWriteStr(pStream, CStringA(mask->Define()));
					}
				}
			}
			break;

		case OCLASS_FUNC_REF:
			{
				Proxy proxy = (Proxy)pObj;
				if (proxy)
				{
					PDF::Function* func = static_cast<PDF::Function*>(proxy->pObj);
					if (func)
					{
						StreamWriteStr(pStream, CStringA(func->Define()));
					}
				}
			}
			break;

		case OCLASS_PATTERN_REF:
			{
				Proxy proxy = (Proxy)pObj;
				if (proxy)
				{
					PDF::Pattern* pattern = static_cast<PDF::Pattern*>(proxy->pObj);
					if (pattern)
					{
						if (PDF::ShadingPatternType == pattern->GetType())
						{
							PDF::ShadingPattern* shading = static_cast<PDF::ShadingPattern*>(proxy->pObj);
							if (shading)
							{
								StreamWriteStr(pStream, CStringA(shading->Define()));
							}
						}

						if (PDF::TilingPatternType == pattern->GetType())
						{
							PDF::TilePattern* shading = static_cast<PDF::TilePattern*>(proxy->pObj);
							if (shading)
							{
								StreamWriteStr(pStream, CStringA(shading->Define()));
							}
						}
					}
				}
			}
			break;

		default:
			nRet = AVS_OFFICEPDFWRITER_ERROR_ERR_UNKNOWN_CLASS;
	}

    return nRet;
}


unsigned long ObjWrite      (void *pObj, Stream pStream, Encrypt pEncrypt)
{
    ObjHeader *pHeader = (ObjHeader *)pObj;

	if ( pHeader->nObjId & OTYPE_HIDDEN) 
	{
         return OK;
    }

	if (OCLASS_PROXY == pHeader->nObjClass) 
	{
        char sBuf[SHORT_BUFFER_SIZE];
        char *pBuf = sBuf;
        char *pEndPtr = sBuf + SHORT_BUFFER_SIZE - 1;
        
		Proxy pProxy = (Proxy)pObj;

        pHeader = (ObjHeader*)pProxy->pObj;

		pBuf = UtilsIToA ( pBuf, pHeader->nObjId & 0x00FFFFFF, pEndPtr);
        *pBuf++ = ' ';
		pBuf = UtilsIToA ( pBuf, pHeader->nGenNo, pEndPtr);
        UtilsStrCpy( pBuf, " R", pEndPtr );

        return StreamWriteStr( pStream, sBuf );
    }

    return ObjWriteValue( pObj, pStream, pEncrypt);
}

void          ObjForceFree  (MMgr oMMgr, void *pObj)
{
    ObjHeader *pHeader;

    if ( !pObj )
        return;

    pHeader = (ObjHeader *)pObj;

    switch ( pHeader->nObjClass & OCLASS_ANY ) 
	{
        case OCLASS_STRING:
            StringFree( (String)pObj );
            break;
        case OCLASS_BINARY:
            BinaryFree( (Binary)pObj );
            break;
        case OCLASS_ARRAY:
            ArrayFree( (Array)pObj );
            break;
        case OCLASS_DICT:
            DictFree ( (Dict)pObj );
            break;
        default:
            FreeMem( oMMgr, pObj );
    }
}

void          ObjFree       (MMgr oMMgr, void *pObj)
{
    ObjHeader *pHeader;

    if ( !pObj )
        return;

    pHeader = (ObjHeader *)pObj;

	if ( !( pHeader->nObjId & OTYPE_INDIRECT ) )
        ObjForceFree( oMMgr, pObj );
}

#endif /* _OBJECTS_H */

