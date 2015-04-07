#ifndef _OBJECTS_H
#define _OBJECTS_H

#pragma once 

#include "Encoder.h"
#include "ToUnicode.h"
#include "Utils.h"
#include "Pattern.h"
#include "Encrypt.h"
#include "Streams.h"

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

//unsigned long ObjWriteValue (void *pObj, StreamRecPtr pStream, EncryptRecPtr pEncrypt);
//unsigned long ObjWrite      (void *pObj, StreamRecPtr pStream, EncryptRecPtr pEncrypt);
//void          ObjForceFree  (MMgr oMMgr, void *pObj);
//void          ObjFree       (MMgr oMMgr, void *pObj);
//----- NullObject -------------------------------------------------------------

typedef struct TNullRec  *Null;

typedef struct TNullRec 
{
    ObjHeader pHeader;
} NullRec;

Null NullNew  (MMgr oMMgr);
//----- BooleanObject --------------------------------------------------------
typedef struct TBooleanRec {
    ObjHeader pHeader;
    BOOL      bValue;
} BooleanRec;

typedef BooleanRec* BooleanRecPtr;


BooleanRecPtr       BooleanNew   (MMgr oMMgr, BOOL bValue);
unsigned long BooleanWrite (BooleanRecPtr pObj, StreamRecPtr pStream);
//----- NumberObject (int value) ---------------------------------------------

typedef struct TNumberRec  *Number;

typedef struct TNumberRec 
{
    ObjHeader pHeader;
    int       nValue;
} NumberRec;

Number        NumberNew     (MMgr oMMgr, int nValue);
unsigned long NumberWrite   (Number pObj, StreamRecPtr pStream);
void          NumberSetValue(Number pObj, int nValue);
//----- Real ----------------------------------------------------------------

typedef struct TRealRec  *Real;

typedef struct TRealRec 
{
    ObjHeader pHeader;
    Error     oError;
    float     fValue;
} RealRec;

unsigned long RealSetValue (Real pObj, float fValue);
Real          RealNew      (MMgr oMMgr, float fValue);
unsigned long RealWrite    (Real pObj, StreamRecPtr pStream);
//----- NameObjects -----------------------------------------------------------

typedef struct TNameRec *Name;

typedef struct TNameRec 
{
    ObjHeader pHeader;
    Error     oError;
    char      sValue[LIMIT_MAX_NAME_LEN + 1];
} NameRec;

unsigned long NameSetValue(Name pObj, const char *sValue);
Name          NameNew     (MMgr oMMgr, const char *sValue);
unsigned long NameWrite   (Name pObj, StreamRecPtr pStream);
const char*   NameGetValue(Name pObj);
//----- StringObject ---------------------------------------------------------

typedef struct TStringRec  *String;

typedef struct TStringRec 
{
    ObjHeader		pHeader;
    MMgr			oMMgr;
    Error			oError;
    EncoderRecPtr   pEncoder;
    BYTE			*pValue;
    unsigned int	nLen;
} StringRec;

static const BYTE UNICODE_HEADER[] = { 0xFE, 0xFF };

unsigned long StringSetValue(String  pObj, const char *sValue);
String        StringNew     (MMgr oMMgr, const char *sValue, EncoderRecPtr pEncoder);
void          StringFree    (String  pObj);
unsigned long StringWrite   (String  pObj, StreamRecPtr pStream, EncryptRecPtr pEncrypt);
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

unsigned long BinarySetValue  (Binary pObj, BYTE *pValue, unsigned int nLen);
Binary        BinaryNew       (MMgr   oMMgr, BYTE *pValue, unsigned int nLen);
unsigned long BinaryWrite     (Binary pObj, StreamRecPtr pStream, EncryptRecPtr pEncrypt);
void          BinaryFree      (Binary pObj);
unsigned int  BinaryGetLen    (Binary pObj);
BYTE*         BinaryGetValue  (Binary pObj);
//----- ProxyObject ------------------------------------------------------------

typedef struct TProxyRec  *Proxy;

typedef struct TProxyRec 
{
    ObjHeader pHeader;
    void     *pObj;
} ProxyRec;

Proxy ProxyNew  (MMgr oMMgr, void *pObj);
//----- ArrayObject ----------------------------------------------------------

typedef struct TArrayRec  *Array;

typedef struct TArrayRec 
{
    ObjHeader  pHeader;
    MMgr       oMMgr;
    Error      oError;
    List       pList;
} ArrayRec;

Array         ArrayNew       (MMgr  oMMgr);
unsigned long ArrayWrite     (Array oArray, StreamRecPtr pStream, EncryptRecPtr pEncrypt);
unsigned long ArrayAdd       (Array oArray, void *pObj);
unsigned long ArrayAddNumber (Array oArray, int nValue);
unsigned long ArrayAddReal   (Array oArray, float fValue);
unsigned long ArrayAddName   (Array oArray, const char *sValue);
unsigned long ArrayInsert    (Array oArray, void *pTarget, void *pObj);
void*         ArrayGetItem   (Array oArray, unsigned int nIndex, unsigned int nObjClass);
void          ArrayClear     (Array oArray);
void          ArrayFree      (Array oArray);
Array         BoxArrayNew    (MMgr  oMMgr, Box oBox);
//----- DictObject -----------------------------------------------------------

typedef struct TXrefRec *Xref;
unsigned long XrefAdd                 (Xref pXref, void *pObj);

typedef struct TDictRec *Dict;

typedef void          (*DictFreeFunc)       (Dict pObj);
typedef unsigned long (*DictBeforeWriteFunc)(Dict pObj);
typedef unsigned long (*DictAfterWriteFunc) (Dict pObj);
typedef unsigned long (*DictOnWriteFunc)    (Dict pObj, StreamRecPtr pStream);

typedef struct TDictRec 
{
    ObjHeader				pHeader;
    MMgr					oMMgr;
    Error					oError;
    List					pList;
    DictBeforeWriteFunc		pBeforeWriteFn;
    DictOnWriteFunc			pWriteFn;
    DictAfterWriteFunc		pAfterWriteFn;
    DictFreeFunc			pFreeFn;
    StreamRecPtr					pStream;
    unsigned int			nFilter;
	unsigned int			nPredictor;
	std::map<CString, int>	*pKeyMap;
    void					*pAttr;

	CString*				sBuffer;
} DictRec;

typedef struct TDictElementRec *DictElement;

typedef struct TDictElementRec 
{
    char  sKey[LIMIT_MAX_NAME_LEN + 1];
    void *pValue;
} DictElementRec;

Dict          DictNew           (MMgr oMMgr);
DictElement   GetElement        (Dict oDict, const char *sKey);
unsigned long DictAdd           (Dict oDict, const char *sKey, void *pObj);
Dict          DictStreamNew     (MMgr oMMgr, Xref oXref, BOOL bMemoryStream = TRUE, const wchar_t *wsTempFile = NULL, unsigned int unCheckSum = 0);
void          DictFree          (Dict oDict);
unsigned long DictRemoveElement (Dict oDict, const char *sKey);
void*         DictGetItem       (Dict oDict, const char *sKey, unsigned short nObjClass );
unsigned long DictAddName       (Dict oDict, const char *sKey, const char *sValue);
unsigned long DictAddNumber     (Dict oDict, const char *sKey, int         nValue);
unsigned long DictAddReal       (Dict oDict, const char *sKey, float       fValue);
unsigned long DictAddBoolean    (Dict oDict, const char *sKey, BOOL        bValue);
unsigned long DictWrite         (Dict oDict, StreamRecPtr pStream, EncryptRecPtr pEncrypt );
const char*   DictGetKeyByObj   (Dict oDict, void *pObj);
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

XrefEntry     XrefGetEntry            (Xref pXref, unsigned int nIndex);
void          XrefFree                (Xref pXref);
Xref          XrefNew                 (MMgr oMMgr, unsigned int nOffset);
unsigned long XrefAdd                 (Xref pXref, void *pObj);
XrefEntry     XrefGetEntryByObjectId  (Xref pXref, unsigned int nObjId);
static unsigned long WriteTrailer     (Xref pXref, StreamRecPtr pStream);
unsigned long XrefWriteToStream       (Xref pXref, StreamRecPtr pStream, EncryptRecPtr pEncrypt);
// Основные функции для работы с объектами
unsigned long ObjWriteValue (void *pObj, StreamRecPtr pStream, EncryptRecPtr pEncrypt);
unsigned long ObjWrite      (void *pObj, StreamRecPtr pStream, EncryptRecPtr pEncrypt);
void          ObjForceFree  (MMgr oMMgr, void *pObj);
void          ObjFree       (MMgr oMMgr, void *pObj);
#endif /* _OBJECTS_H */

