#ifndef _ENCODER_H
#define _ENCODER_H

#pragma once 

#include "Utils.h"
#include "Streams.h"
#include "EncoderConstants.h"

//-- EncoderRecPtr -------------------------------------------------------------------------------------

#define  ENCODER_SIG_BYTES 0x454E4344L

//-- Предопределенные кодировки фонтов -----------------------------------------------------------

#define  ENCODING_FONT_SPECIFIC  "FontSpecific"
#define  ENCODING_STANDARD       "StandardEncoding"
#define  ENCODING_MAC_EXPERT     "MacExpertEncoding"
#define  ENCODING_MAC_ROMAN      "MacRomanEncoding"
#define  ENCODING_WIN_ANSI       "WinAnsiEncoding"
#define  ENCODING_ISO8859_2      "ISO8859-2"
#define  ENCODING_ISO8859_3      "ISO8859-3"
#define  ENCODING_ISO8859_4      "ISO8859-4"
#define  ENCODING_ISO8859_5      "ISO8859-5"
#define  ENCODING_ISO8859_6      "ISO8859-6"
#define  ENCODING_ISO8859_7      "ISO8859-7"
#define  ENCODING_ISO8859_8      "ISO8859-8"
#define  ENCODING_ISO8859_9      "ISO8859-9"
#define  ENCODING_ISO8859_10     "ISO8859-10"
#define  ENCODING_ISO8859_11     "ISO8859-11"
#define  ENCODING_ISO8859_13     "ISO8859-13"
#define  ENCODING_ISO8859_14     "ISO8859-14"
#define  ENCODING_ISO8859_15     "ISO8859-15"
#define  ENCODING_ISO8859_16     "ISO8859-16"
#define  ENCODING_CP1250         "CP1250"
#define  ENCODING_CP1251         "CP1251"
#define  ENCODING_CP1252         "CP1252"
#define  ENCODING_CP1253         "CP1253"
#define  ENCODING_CP1254         "CP1254"
#define  ENCODING_CP1255         "CP1255"
#define  ENCODING_CP1256         "CP1256"
#define  ENCODING_CP1257         "CP1257"
#define  ENCODING_CP1258         "CP1258"
#define  ENCODING_KOI8_R         "KOI8-R"

//-- Определения для кодировки фонтов ------------------------------------------------------------

typedef enum EEncodingType 
{
    StandrardEncoding = 0,
    MacRomanEncoding,
    WinAnsiEncoding,
    FontSpecific,
    EncodingEOF
} EncodingType;

typedef struct TParseTextRec 
{
    const BYTE   *pText;
    unsigned int  nIndex;
    unsigned int  nLen;
    ByteType      eByteType;
} ParseTextRec;

typedef struct EncoderRec* EncoderRecPtr;

typedef ByteType       (*EncoderByteTypeFunc) (EncoderRecPtr pEncoder, ParseTextRec *pState);
typedef unsigned short (*EncoderToUnicodeFunc)(EncoderRecPtr pEncoder, unsigned short nCode);
typedef unsigned long  (*EncoderWriteFunc)    (EncoderRecPtr pEncoder, StreamRecPtr pOut);
typedef unsigned long  (*EncoderInitFunc)     (EncoderRecPtr pEncoder);
typedef void           (*EncoderFreeFunc)     (EncoderRecPtr pEncoder);

struct  EncoderRec 
{
    unsigned int               nSigBytes;
    char                       sName[LIMIT_MAX_NAME_LEN + 1];
    MMgr                       oMMgr;
    Error                      oError;
    EncoderType                eType;

    EncoderByteTypeFunc        pByteTypeFn;
    EncoderToUnicodeFunc       pToUnicodeFn;
    EncoderWriteFunc           pWriteFn;
    EncoderFreeFunc            pFreeFn;
    EncoderInitFunc            pInitFn;

	//char                       aLangCode[3];
    //char                       aCountryCode[3];
    void                       *pAttr;
};

typedef enum EBaseEncodings 
{
    BaseEncodingStandard,
    BaseEncodingWinAnsi,
	BaseEncodingMacExpert,
    BaseEncodingMacRoman,
    BaseEncodingFontSpecific,
    BaseEncodingEOF
} BaseEncodings;

typedef struct TBuiltinEncodingData 
{
    const char           *sEncodingName;
    BaseEncodings         eBaseEncoding;
    const unsigned short *pnOverrideMap;
} BuiltinEncodingData;

static const BuiltinEncodingData  c_aoBuiltInEncodings[] = 
{
    {
        ENCODING_FONT_SPECIFIC,
        BaseEncodingFontSpecific,
        NULL
	},
    {
        ENCODING_STANDARD,
        BaseEncodingStandard,
        NULL
	},
	{
		ENCODING_MAC_EXPERT,
		BaseEncodingMacExpert,
		NULL
	},
    {
        ENCODING_MAC_ROMAN,
        BaseEncodingMacRoman,
        NULL
	},
    {
        ENCODING_WIN_ANSI,
        BaseEncodingWinAnsi,
        NULL
    },
    {
        ENCODING_ISO8859_2,
        BaseEncodingWinAnsi,
        c_anUnicodeMapISO8859_2
    },
    {
        ENCODING_ISO8859_3,
        BaseEncodingWinAnsi,
        c_anUnicodeMapISO8859_3
    },
    {
        ENCODING_ISO8859_4,
        BaseEncodingWinAnsi,
        c_anUnicodeMapISO8859_4
    },
    {
        ENCODING_ISO8859_5,
        BaseEncodingWinAnsi,
        c_anUnicodeMapISO8859_5
    },
    {
        ENCODING_ISO8859_6,
        BaseEncodingWinAnsi,
        c_anUnicodeMapISO8859_6
    },
    {
        ENCODING_ISO8859_7,
        BaseEncodingWinAnsi,
        c_anUnicodeMapISO8859_7
    },
    {
        ENCODING_ISO8859_8,
        BaseEncodingWinAnsi,
        c_anUnicodeMapISO8859_8
    },
    {
        ENCODING_ISO8859_9,
        BaseEncodingWinAnsi,
        c_anUnicodeMapISO8859_9
    },
    {
        ENCODING_ISO8859_10,
        BaseEncodingWinAnsi,
        c_anUnicodeMapISO8859_10
    },
    {
        ENCODING_ISO8859_11,
        BaseEncodingWinAnsi,
        c_anUnicodeMapISO8859_11
    },
    {
        ENCODING_ISO8859_13,
        BaseEncodingWinAnsi,
        c_anUnicodeMapISO8859_13
    },
    {
        ENCODING_ISO8859_14,
        BaseEncodingWinAnsi,
        c_anUnicodeMapISO8859_14
    },
    {
        ENCODING_ISO8859_15,
        BaseEncodingWinAnsi,
        c_anUnicodeMapISO8859_15
    },
    {
        ENCODING_ISO8859_16,
        BaseEncodingWinAnsi,
        c_anUnicodeMapISO8859_16
    },
    {
        ENCODING_CP1250,
        BaseEncodingWinAnsi,
        c_anUnicodeMapCP1250
    },
    {
        ENCODING_CP1251,
        BaseEncodingWinAnsi,
        c_anUnicodeMapCP1251
    },
    {
        ENCODING_CP1252,
        BaseEncodingWinAnsi,
        c_anUnicodeMapCP1252
    },
    {
        ENCODING_CP1253,
        BaseEncodingWinAnsi,
        c_anUnicodeMapCP1253
    },
    {
        ENCODING_CP1254,
        BaseEncodingWinAnsi,
        c_anUnicodeMapCP1254
    },
    {
        ENCODING_CP1255,
        BaseEncodingWinAnsi,
        c_anUnicodeMapCP1255
    },
    {
        ENCODING_CP1256,
        BaseEncodingWinAnsi,
        c_anUnicodeMapCP1256
    },
    {
        ENCODING_CP1257,
        BaseEncodingWinAnsi,
        c_anUnicodeMapCP1257
    },
    {
        ENCODING_CP1258,
        BaseEncodingWinAnsi,
        c_anUnicodeMapCP1258
    },
    {
        ENCODING_KOI8_R,
        BaseEncodingWinAnsi,
        c_anUnicodeMapKOI8_R
    },
    {
        NULL,
        BaseEncodingEOF,
        NULL
    }
};


unsigned long  EncoderValidate     (EncoderRecPtr pEncoder);
void           EncoderSetParseText (EncoderRecPtr pEncoder, ParseTextRec *pState, const BYTE *pText, unsigned int nLen);
ByteType       EncoderByteType     (EncoderRecPtr pEncoder, ParseTextRec *pState);
unsigned short EncoderToUnicode    (EncoderRecPtr pEncoder, unsigned short nCode);
void           EncoderFree         (EncoderRecPtr pEncoder);
unsigned short EncoderToCode       (EncoderRecPtr pEncoder, unsigned short unUnicode);

//-- BasicEncoder --------------------------------------------------------------------------------

#define BASIC_ENCODER_MAX_CHAR_INDEX 512 // Вообще, по смыслу, тут должно быть 256, но некоторые 
                                         // PDF файлы содержат и большое 256 символов, потому что
                                         // символы кодируются в восьмеричной системе из 3-х знаков.
                                         // Поэтому максимальное число элементов = 8^3 = 512

typedef struct TBasicEncoderAttrRec  *BasicEncoderAttr;

typedef struct TBasicEncoderAttrRec 
{
      char           sBaseEncoding[LIMIT_MAX_NAME_LEN + 1];
      BYTE           nFirstChar;
      BYTE           nLastChar;
      unsigned short anUnicodeMap[BASIC_ENCODER_MAX_CHAR_INDEX];
	  char           asGlyphNameMap[BASIC_ENCODER_MAX_CHAR_INDEX][LIMIT_MAX_NAME_LEN + 1];
      BOOL           bHasDifferences;
      BYTE           anDifferences[BASIC_ENCODER_MAX_CHAR_INDEX];
} BasicEncoderAttrRec;

//-- BasicEncoder - Вспомогательные функции ------------------------------------------------------

const BuiltinEncodingData* BasicEncoderFindBuiltinData(const char *sEncodingName );
const char*                UnicodeToGlyphName         (unsigned short nUnicode);
unsigned short             GlyphNameToUnicode         (const char *sGlyphName);
void                       BasicEncoderCopyMap        (EncoderRecPtr pEncoder, const unsigned short *pnMap);
unsigned long              BasicEncoderOverrideMap    (EncoderRecPtr pEncoder, const unsigned short *pnMap);
;
//-- BasicEncoder - Основные функции ------------------------------------------------------------- 

void           BasicEncoderFree     (EncoderRecPtr pEncoder);
unsigned long  BasicEncoderWrite    (EncoderRecPtr pEncoder, StreamRecPtr pOut);
unsigned short BasicEncoderToUnicode(EncoderRecPtr pEncoder, unsigned short nCode);
EncoderRecPtr        BasicEncoderNew      (MMgr oMMgr, const char *sEncodingName);
//-- CMapEncoderStream ---------------------------------------------------------------------------

struct CMapVectorEntry 
{
	BOOL                 bIsVector;
	//union 
	//{
		int              nCID;
		CMapVectorEntry *pVector;
	//};
};

typedef struct TCIDtoCodeMap
{
	char sCode[4];
	int  nLen;
} CIDtoCodeMap;
typedef struct TCMapEncoderStreamAttrRec  *CMapEncoderStreamAttr;

typedef struct TCMapEncoderStreamAttrRec 
{
	char        sName[LIMIT_MAX_NAME_LEN + 1];
	int         nType; // 0 - значит имя предопределенное имя в PDF, 1 - имя - это идентефикатор кодировки, а сама кодировка задана в pStream

	char        sCMapName[LIMIT_MAX_NAME_LEN + 1];
	char        sRegistry[LIMIT_MAX_NAME_LEN + 1];
	char        sOrdering[LIMIT_MAX_NAME_LEN + 1];
	int         nSupplement;
	WritingMode eWritingMode;

	union
	{
		char   sName[LIMIT_MAX_NAME_LEN + 1];
		StreamRecPtr pStream;
	} uUseCMap;

	CIDtoCodeMap anCIDtoCode[65536]; // CID - выступает в роли индекса
	
	CMapVectorEntry *pVector;

	StreamRecPtr      pStream;
} CMapEncoderStreamAttrRec;

void           CMapEncoderStreamFreeVector   (EncoderRecPtr pEncoder, CMapVectorEntry *pVector);
unsigned long  CMapEncoderStreamWrite        (EncoderRecPtr pEncoder, StreamRecPtr pOut);
void           CMapEncoderStreamFree         (EncoderRecPtr pEncoder);
//-- CMapEncoderStream - Основные функции --------------------------------------------------------
EncoderRecPtr        CMapEncoderStreamNew          (MMgr oMMgr, int nType, char *sName, StreamRecPtr pStream);
unsigned long  CMapEncoderStreamSetAttrs     (EncoderRecPtr pEncoder, char *sCMapName, char *sRegistry, char *sOrdering, int nSupplement, WritingMode eMode, char *sUseCMap, StreamRecPtr pUseCMap );
unsigned long  CMapEncoderStreamLoadVector2  (EncoderRecPtr pEncoder, XmlUtils::CXmlNode oNode, CMapVectorEntry **ppVector);
unsigned long  CMapEncoderStreamLoadVector   (EncoderRecPtr pEncoder, CString sCMapPath);
int            CMapEncoderStreamGetCID       (EncoderRecPtr pEncoder, char *sChar, int nLen, int *pnUsed);
void           CMapEncoderStreamGetCodeByCID (EncoderRecPtr pEncoder, int nCID, char **ppBuffer, int *pnLen);
void           CMapEncoderStreamAddCIDtoCode (EncoderRecPtr pEncoder, int nCID, char sBuffer[4], int nLen);
//-- CMapEncoder ---------------------------------------------------------------------------------

typedef BOOL (*CMapEncoderByteTypeFunc) (EncoderRecPtr pEncoder, BYTE nByte);

typedef struct TCIDRangeRec 
{
    unsigned short nFrom;
    unsigned short nTo;
    unsigned short nCID;
} CIDRangeRec;

typedef struct TUnicodeMapRec 
{
    unsigned short nCode;
    unsigned short nUnicode;
} UnicodeMapRec;

typedef struct TCMapEncoderAttrRec  *CMapEncoderAttr;

typedef struct TCMapEncoderAttrRec 
{
      unsigned short            anUnicodeMap[256][256];
      unsigned short            anCIDMap[256][256];
      unsigned short            anJWWLineHead[MAX_JWW_NUM];
      List                      pCMAPRange;
      List                      pNotDefRange;
      List                      pCodeSpaceRange;
      WritingMode               eWritingMode;
      char                      sRegistry[LIMIT_MAX_NAME_LEN + 1];
      char                      sOrdering[LIMIT_MAX_NAME_LEN + 1];
      int                       nSuppliment;
      CMapEncoderByteTypeFunc   pIsLeadByteFn;
      CMapEncoderByteTypeFunc   pIsTrialByteFn;
      int                       nUidOffset;
      unsigned int              anXUid[3];

	  // Используется только для кодировок типа EncoderTypeToUnicode
	  BYTE                      nFirstChar;
	  BYTE                      nLastChar;
} CMapEncoderAttrRec;

//-- CMAPEncoder - Вспомогательные функции ------------------------------------------------------- 

ByteType       CMapEncoderByteType           (EncoderRecPtr pEncoder, ParseTextRec *pState);
unsigned short CMapEncoderToUnicode          (EncoderRecPtr pEncoder, unsigned short nCode);
unsigned short CMapEncoderToCID              (EncoderRecPtr pEncoder, unsigned short nCode);
unsigned long  CMapEncoderWriteHeader        (StreamRecPtr pOut);
unsigned long  CMapEncoderWriteFooter        (StreamRecPtr pOut);
unsigned long  CMapEncoderWriteCodeSpaceRange(EncoderRecPtr pEncoder, StreamRecPtr pOut);
unsigned long  CMapEncoderWriteBFChar        (EncoderRecPtr pEncoder, StreamRecPtr pOut, int nCode, int nUnicode );
unsigned long  CMapEncoderWriteBFChars       (EncoderRecPtr pEncoder, StreamRecPtr pOut, int nStartCode, int nLastCode);
unsigned long  CMapEncoderWriteBFChars       (EncoderRecPtr pEncoder, StreamRecPtr pOut);
unsigned long  CMapEncoderWriteInfo          (EncoderRecPtr pEncoder, StreamRecPtr pOut);
unsigned long  CMapEncoderWrite              (EncoderRecPtr pEncoder, StreamRecPtr pOut);
void           CMapEncoderFree               (EncoderRecPtr pEncoder);
//static unsigned long AddCIDRange    (MMgr oMMgr, CIDRangeRec oRange, List pTarget);

EncoderRecPtr       CMapEncoderNew               (MMgr oMMgr, char *sName, EncoderInitFunc pInitFn);
unsigned long CMapEncoderInitAttr          (EncoderRecPtr pEncoder);
unsigned long CMapEncoderSetParseText      (EncoderRecPtr pEncoder, ParseTextRec *pState, const BYTE *sText, unsigned int nLen);
unsigned long CMapEncoderAddCMap           (EncoderRecPtr pEncoder, const CIDRangeRec *pRange);
unsigned long CMapEncoderAddNotDefRange    (EncoderRecPtr pEncoder, CIDRangeRec oRange);
unsigned long CMapEncoderAddCodeSpaceRange (EncoderRecPtr pEncoder, CIDRangeRec oRange);
void          CMapEncoderSetUnicodeArray   (EncoderRecPtr pEncoder, const UnicodeMapRec *pArray);
unsigned long CMapEncoderAddJWWLineHead    (EncoderRecPtr pEncoder, const unsigned short *pnCode);
BOOL          EncoderCheckJWWLineHead      (EncoderRecPtr pEncoder, const unsigned short nCode);
BOOL          CMapEncoderUpdateUnicodeArray(EncoderRecPtr pEncoder, unsigned short *pArray);
//-- Дополнительные внешние функции -------------------------------------------------------------- 

unsigned short EncoderGetUnicode    (EncoderRecPtr pEncoder, unsigned short nCode);
ByteType       EncoderGetByteType   (EncoderRecPtr pEncoder, const char *sText, unsigned int nIndex);
EncoderType    EncoderGetType       (EncoderRecPtr pEncoder);
WritingMode    EncoderGetWritingMode(EncoderRecPtr pEncoder);
BOOL           EncoderWCharToString (EncoderRecPtr pEncoder, CString bsSrc, CString *psDst);
#endif /* _ENCODER_H */

