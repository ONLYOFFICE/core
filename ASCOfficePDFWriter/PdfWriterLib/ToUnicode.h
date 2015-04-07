#ifndef _TO_UNICODE_H
#define _TO_UNICODE_H

#pragma once
//-- ToUnicode -----------------------------------------------------------------------------------

#include "Encoder.h"

#define  TO_UNICODE_SIG_BYTES 0xB4EFB5EEL // ToUn

typedef enum EToUnicodeType
{
    ToUnicodeStream = 0,
    ToUnicodeBuilt  = 1,
    ToUnicodeEOF
} ToUnicodeType;

typedef struct TToUnicodeRec *PToUnicode;

typedef unsigned short (*ToUnicodeToUnicodeFunc)(PToUnicode pToUnicode, unsigned short nCode);
typedef unsigned long  (*ToUnicodeWriteFunc)    (PToUnicode pToUnicode, StreamRecPtr pOut);
typedef unsigned long  (*ToUnicodeInitFunc)     (PToUnicode pToUnicode);
typedef void           (*ToUnicodeFreeFunc)     (PToUnicode pToUnicode);

typedef struct  TToUnicodeRec 
{
    unsigned int               nSigBytes;
    char                       sName[LIMIT_MAX_NAME_LEN + 1];
    MMgr                       oMMgr;
    Error                      oError;
    ToUnicodeType              eType;

    ToUnicodeToUnicodeFunc     pToUnicodeFn;
    ToUnicodeWriteFunc         pWriteFn;
    ToUnicodeFreeFunc          pFreeFn;
    ToUnicodeInitFunc          pInitFn;

    void                       *pAttr;
}  ToUnicodeRec;

unsigned long  ToUnicodeValidate     (PToUnicode pToUnicode);
unsigned short ToUnicodeToUnicode    (PToUnicode pToUnicode, unsigned short nCode);
void           ToUnicodeFree         (PToUnicode pToUnicode);
unsigned short ToUnicodeToCode       (PToUnicode pToUnicode, unsigned short unUnicode);
//-- ToUnicodeStream -----------------------------------------------------------------------------

typedef struct TToUnicodeStreamAttrRec  *ToUnicodeStreamAttr;

typedef struct TToUnicodeStreamAttrRec 
{
	StreamRecPtr pStream;

} ToUnicodeStreamAttrRec;

//-- ToUnicodeStream - Вспомогательные функции ---------------------------------------------------

//-- ToUnicodeStream - Основные функции ----------------------------------------------------------
void           ToUnicodeStreamFree     (PToUnicode pToUnicode);
unsigned long  ToUnicodeStreamWrite    (PToUnicode pToUnicode, StreamRecPtr pOut);
PToUnicode     ToUnicodeStreamNew      (MMgr oMMgr, BYTE *pBuffer, unsigned int unSize, const char *sName);
//-- ToUnicodeBuilt ------------------------------------------------------------------------------

typedef struct TToUnicodeBuiltAttrRec  *ToUnicodeBuiltAttr;

typedef struct TToUnicodeBuiltAttrRec 
{
      unsigned short            anUnicodeMap[256][256];
	  BYTE                      nFirstChar;
	  BYTE                      nLastChar;
	  unsigned short            nBitCount; // 8 или 16

} ToUnicodeBuiltAttrRec;

//-- ToUnicodeBuilt - Вспомогательные функции ----------------------------------------------------

unsigned short ToUnicodeBuiltToUnicode          (PToUnicode pToUnicode, unsigned short nCode);
unsigned long  ToUnicodeBuiltWriteHeader        (StreamRecPtr pOut);
unsigned long  ToUnicodeBuiltWriteFooter        (StreamRecPtr pOut);
unsigned long  ToUnicodeBuiltWriteCodeSpaceRange(PToUnicode pToUnicode, StreamRecPtr pOut);
unsigned long  ToUnicodeBuiltWriteBFChar        (PToUnicode pToUnicode, StreamRecPtr pOut, int nCode, int nUnicode );
unsigned long  ToUnicodeBuiltWriteBFChars       (PToUnicode pToUnicode, StreamRecPtr pOut, int nStartCode, int nLastCode);
unsigned long  ToUnicodeBuiltWriteBFChars       (PToUnicode pToUnicode, StreamRecPtr pOut);
unsigned long  ToUnicodeBuiltWriteInfo          (PToUnicode pToUnicode, StreamRecPtr pOut);
unsigned long  ToUnicodeBuiltWrite              (PToUnicode pToUnicode, StreamRecPtr pOut);
void           ToUnicodeBuiltFree               (PToUnicode pToUnicode);
//-- ToUnicodeBuilt - Основные функции -----------------------------------------------------------

PToUnicode    ToUnicodeBuiltNew               (MMgr oMMgr, char *sName, unsigned int unBitCount = 8);
void          ToUnicodeBuiltSetUnicodeArray   (PToUnicode pToUnicode, const UnicodeMapRec *pArray);
BOOL          ToUnicodeUpdateUnicodeArray     (PToUnicode pToUnicode, unsigned short *pArray, unsigned int unSize = 0);
BOOL          ToUnicodeWCharToString  (PToUnicode pToUnicide, std::wstring sSrc, std::wstring & psDst);
#endif /* _TO_UNICODE_H */

