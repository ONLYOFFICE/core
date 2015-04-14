#ifndef _FONTDEF_H
#define _FONTDEF_H

#include "Objects.h"

#define FONTDEF_SIG_BYTES 0x464F4E54L

//------ Список флагов для определения свойств --------------------------------------

#define FONT_FIXED_WIDTH    1
#define FONT_SERIF          2
#define FONT_SYMBOLIC       4
#define FONT_SCRIPT         8
//   Reserved               16 
#define FONT_STD_CHARSET    32
#define FONT_ITALIC         64
//   Reserved               128
//   Reserved               256
//   Reserved               512
//   Reserved               1024
//   Reserved               2048
//   Reserved               4096
//   Reserved               8192
//   Reserved               16384
//   Reserved               32768 
#define FONT_ALL_CAP        65536
#define FONT_SMALL_CAP      131072
#define FONT_FOURCE_BOLD    262144

#define CID_W_TYPE_FROM_TO     0
#define CID_W_TYPE_FROM_ARRAY  1

//------ FontDef --------------------------------------------------------------------

typedef struct TCharData 
{
    short          nCharCode;
    unsigned short nUnicode;
    short          nWidth;
} CharData;

typedef enum  EFontDefType 
{
    FontDefType1,          // Type1
    FontDefTrueType,       // TryeType
    FontDefCID,            // CID
	FontBase,              // Шрифт без FontFile
    FontDefUninitialized,
    FontDefEOF
} FontDefType;

typedef struct TCIDWidth 
{
    unsigned short unCid_First; // Ширины могут быть заданы в двух различных форматах
	unsigned short unCid_Last;  // C [W1 W2 ... Wn] - для такого формата создаем n элементов c с1 = с2
    short          shWidth;     // C1 ... C2 W
} CIDWidth;

typedef struct TCIDWidth2
{
	unsigned short unCid_First; // Вертикальные метрики тоже задаются в двух форматах:
	unsigned short unCid_Last;  // C [W1 V1x V1y W2 V2x V2y ... Wn Vnx Vny]
	short          shW;         // С1 ... С2 W V1x V1y
	short          shVx;
	short          shVy;
} CIDWidth2;

//------ FontDef --------------------------------------------------------------------

typedef struct TFontDefRec  *FontDef;

typedef void          (*FontDefFreeFunc)  (FontDef pFontDef);
typedef void          (*FontDefCleanFunc) (FontDef pFontDef);
typedef unsigned long (*FontDefInitFunc)  (FontDef pFontDef);

typedef struct TFontDefRec 
{
    unsigned int        nSigBytes;
    char                sBaseFont[LIMIT_MAX_NAME_LEN + 1];
    MMgr                oMMgr;
    Error               oError;
    FontDefType         eType;
    FontDefCleanFunc    pCleanFn;
    FontDefFreeFunc     pFreeFn;
    FontDefInitFunc     pInitFn;

    short               nAscent;
    short               nDescent;
    unsigned int        nFlags;
    Box                 oFontBox;
    short               nItalicAngle;
    unsigned short      nStemV;
    short               nAvgWidth;
    short               nMaxWidth;
    short               nMissingWidth;
    unsigned short      nStemH;
    unsigned short      nXHeight;
    unsigned short      nCapHeight;

	// Начальное значение поля pDescriptor = NULL. Когда создается первый объект Font,
	// основанный на объекте FontDef, тогда же и создается объект FontDescriptor, и в
	// поле pDescriptor мы ставим ссылку на этот объект.
    Dict                pDescriptor;
    StreamRecPtr              pData;

	unsigned int        nID; // идентефикатор, для случая различных FontDef с одинаковым BaseFont

    bool                bValid;
    void               *pAttr;
} FontDefRec;

//------ Общие функции --------------------------------------------------------------

void FontDefFree    (FontDef pFontDef);
void FontDefCleanup (FontDef pFontDef);
bool FontDefValidate(FontDef pFontDef);
//------ Type1FontDef  --------------------------------------------------------------

typedef struct TType1FontDefAttrRec   *Type1FontDefAttr;

typedef struct TType1FontDefAttrRec 
{
    BYTE          nFirstChar;                               // Required 
    BYTE          nLastChar;                                // Required 
    CharData     *aoWidths;                                 // Required 
    unsigned int  nWidthsCount;

    short         nLeading;
    char         *sCharSet;
    char          sEncodingScheme[LIMIT_MAX_NAME_LEN + 1];
    unsigned int  nLength1;
    unsigned int  nLength2;
    unsigned int  nLength3;
    bool          bIsBase14Font;
    bool          bIsFixedPitch;
	short         nType;          // Type1 = 0, Type1C = 1, Type1COT = 2

    StreamRecPtr        pFontData;
} Type1FontDefAttrRec;

//------ TrueTypeFontDef ------------------------------------------------------------

#define TTF_FONT_TAG_LEN  6
// Cтруктуры для чтения FontFile OpenType и TrueType(см. http://www.microsoft.com/typography/otspec/)
typedef struct TTrueTypeFontTable 
{
        char          anTag[4];   // 4 - byte identifier.
        unsigned int  nCheckSum;  // CheckSum for this table.
        unsigned int  nOffset;    // Offset from beginning of TrueType font file.
        unsigned int  nLength;    // Length of this table.
} TTFTable;

typedef struct TTrueTypeFontOffsetTable 
{
        unsigned int     nSFNTVersion;     // 0x00010000 for version 1.0.
        unsigned short   nNumTables;       // Number of tables.
        unsigned short   nSearchRange;     // (Maximum power of 2 <= numTables) x 16.
        unsigned short   nEntrySelector;   // Log2(maximum power of 2 <= numTables).
        unsigned short   nRangeShift;      // NumTables x 16-searchRange.
        TTFTable        *pTable;           // Pointer to the Table.
} TTFOffsetTable;

typedef struct TTrueTypeFontCMapRange 
{
        unsigned short   nFormat;            // Format number is set to 2.
        unsigned short   nLength;            // This is the length in bytes of the subtable.
        unsigned short   nLanguage;          //
        unsigned short   nSegCountX2;        // 2 x segCount.
        unsigned short   nSearchRange;       // 2 x (2**floor(log2(segCount)))
        unsigned short   nEntrySelector;     // log2(searchRange/2)
        unsigned short   nRangeShift;        // 2 x segCount - searchRange
        unsigned short  *pnEndCount;         // End characterCode for each segment, last=0xFFFF.
        unsigned short   nReservedPad;       // Set to 0.
        unsigned short  *pnStartCount;       // Start character code for each segment.
        short           *pnIDDelta;          // Delta for all character codes in segment.
        unsigned short  *pnIDRangeOffset;    // Offsets into glyphIdArray or 0
        unsigned short  *pnGlyphIdArray;     // Glyph index array (arbitrary length)
        unsigned int     nGlyphIdArrayCount; // Glyph index array count
} TTFCMapRange;

typedef struct TTrueTypeFontGlyphOffsets 
{
        unsigned int  nBaseOffset;
        unsigned int *pnOffsets;
        BYTE         *pFlags;   // 0: не используется, 1: используется
} TTFGlyphOffsets;

typedef struct TTrueTypeFontLongHorMetric 
{
        unsigned short nAdvanceWidth;     // Paired advance width and left side bearing values for each glyph. 
		                                  // The value numOfHMetrics comes from the 'hhea' table. If the font 
		                                  // is monospaced, only one entry need be in the array, but that entry 
		                                  // is required. The last entry applies to all subsequent glyphs.
        short          nLeftSideBearing;  // Here the advanceWidth is assumed to be the same as the advanceWidth 
		                                  // for the last entry above. The number of entries in this array is derived 
		                                  // from numGlyphs (from 'maxp' table) minus numberOfHMetrics. This generally 
		                                  // is used with a run of monospaced glyphs (e.g., Kanji fonts or Courier fonts). 
		                                  // Only one run is allowed and it must be at the end. This allows a monospaced 
		                                  // font to vary the left side bearing values for each glyph.
} TTFLongHorMetric;

typedef struct TTrueTypeFontHeader 
{
    BYTE           anVersionNumber[4];      // 0x00010000 for version 1.0.
    unsigned int   nFontRevision;           // Set by font manufacturer.
    unsigned int   nCheckSumAdjustment;     // To compute: set it to 0, sum the entire font as ULONG, then store 0xB1B0AFBA - sum.
    unsigned int   nMagicNumber;            // Set to 0x5F0F3CF5.
    unsigned short nFlags;                  // Bit 0: Baseline for font at y=0;
                                            // Bit 1: Left sidebearing point at x=0;
                                            // Bit 2: Instructions may depend on point size; 
                                            // Bit 3: Force ppem to integer values for all internal scaler math; may use fractional ppem sizes if this bit is clear; 
                                            // Bit 4: Instructions may alter advance width (the advance widths might not scale linearly); 
                                            // Bits 5-10: These should be set according to Apple's specification . However, they are not implemented in OpenType. 
                                            // Bit 11: Font data is 'lossless,' as a result of having been compressed and decompressed with the Agfa MicroType Express engine.
                                            // Bit 12: Font converted (produce compatible metrics)
                                            // Bit 13: Font optimized for ClearType™. Note, fonts that rely on embedded bitmaps (EBDT) for rendering should not be considered optimized for ClearType, and therefore should keep this bit cleared.
                                            // Bit 14: Reserved, set to 0
                                            // Bit 15: Reserved, set to 0
    unsigned short nUnitsPerEm;             // Valid range is from 16 to 16384. This value should be a power of 2 for fonts that have TrueType outlines.
    BYTE           anCreated[8];            // Number of seconds since 12:00 midnight, January 1, 1904. 64-bit integer
    BYTE           anModified[8];           // Number of seconds since 12:00 midnight, January 1, 1904. 64-bit integer
    short          nXmin;                   // For all glyph bounding boxes.
    short          nYmin;                   // For all glyph bounding boxes.
    short          nXmax;                   // For all glyph bounding boxes.
    short          nYmax;                   // For all glyph bounding boxes.
    unsigned short nMacStyle;               // Bit 0: Bold (if set to 1); 
                                            // Bit 1: Italic (if set to 1) 
                                            // Bit 2: Underline (if set to 1) 
                                            // Bit 3: Outline (if set to 1) 
                                            // Bit 4: Shadow (if set to 1) 
                                            // Bit 5: Condensed (if set to 1) 
                                            // Bit 6: Extended (if set to 1) 
                                            // Bits 7-15: Reserved (set to 0).
    unsigned short nLowestRecPPEM;          // Smallest readable size in pixels.
    short          nFontDirectionHint;      // Deprecated (Set to 2). 
                                            // 0: Fully mixed directional glyphs; 
                                            // 1: Only strongly left to right; 
                                            // 2: Like 1 but also contains neutrals; 
                                            // -1: Only strongly right to left; 
                                            // -2: Like -1 but also contains neutrals.
    short          nIndexToLocFormat;       // 0 for short offsets, 1 for long.
    short          nGlyphDataFormat;        // 0 for current format.
} TTFFontHeader;

typedef struct TTrueTypeFontNameRecord 
{
    unsigned short nPlatformID;           // Platform ID.
    unsigned short nEncodingID;           // Platform-specific encoding ID.
    unsigned short nLanguageID;           // Language ID.
    unsigned short nNameID;               // Name ID.
    unsigned short nLength;               // String length (in bytes).
    unsigned short nOffset;               // String offset from start of storage area (in bytes).
} TTFNameRecord;

typedef struct TTrueTypeFontNamingTable 
{
    unsigned short     nFormat;           // Format selector.
    unsigned short     nCount;            // Number of name records.
    unsigned short     nStringOffset;     // Offset to start of string storage (from start of table).
    TTFNameRecord     *pNameRecords;      // Pointer to NameRecord.
} TTFNamingTable;

typedef struct TTrueTypeFontPair
{
	unsigned short ushCharCode;
	unsigned short ushGlyphID;

	float          fAdvance;
	struct TBBox
	{
		float fLeft;
		float fBottom;
		float fRight;
		float fTop;
	} oBBox;
} TTFPair;

typedef struct TTrueTypeFontCmap
{
	TTrueTypeFontPair *pPairs;
	unsigned int       unCount;
} TTFCmap;

typedef struct TTrueTypeFontDefAttrRec *TTFontDefAttrRecPtr;

typedef struct TTrueTypeFontDefAttrRec 
{
    char                sBaseFont[LIMIT_MAX_NAME_LEN + 1];
	char                sToUnicodeName[LIMIT_MAX_NAME_LEN + 1];
    BYTE                nFirstChar;
    BYTE                nLastChar;
    char               *pnCharSet;
    char                sTagName[TTF_FONT_TAG_LEN + 1];
    char                sTagName2[(TTF_FONT_TAG_LEN + 1) * 2];
    TTFFontHeader       oHeader;
    TTFGlyphOffsets     oGlyphTable;
    unsigned short      nNumGlyphs;
    TTFNamingTable      oNameTable;
    TTFLongHorMetric   *pHMetric;
    unsigned short      nNumHMetric;
    TTFOffsetTable      oOffsetTable;
    TTFCMapRange        oCMap;
	TTFCmap             oCmap;
    unsigned short      nFsType;
    BYTE                anPanose[12];
    unsigned int        nCodePageRange1;
    unsigned int        nCodePageRange2;

    unsigned int        nLength1;

    bool                bEmbedding;
    bool                bIsCIDFont;

    bool                bCFF;                  // FALSE - работаем как с чистым TTF, TRUE - работаем как с OTF с таблицей 'CFF '

    bool                bEncoding;             // Три данных параметра необходимы для точного определения
    bool                bSymbolic;             // таблицы Cmap.
    bool                bUsesMacRomanEncoding; //

    wchar_t             wsFilePath[MAX_PATH];
    unsigned int        unFontIndex;

    StreamRecPtr        pStream;
} TTFontDefAttrRec;

//------ CIDFontDef -----------------------------------------------------------------

typedef enum ECIDFontFileType
{
	cidType0    = 0,
	cidType0C   = 1,
	cidType0COT = 2,
	cidType2    = 3,
	cidType2OT  = 4,
	cidEOF
} CIDFontFileType;

typedef struct TCIDFontDefAttrRec   *CIDFontDefAttr;

typedef struct TCIDFontDefAttrRec 
{
    List     pW;            // ширины для горизонтальных шрифтов
    short    nDW;           // стандартное значение ширины
    short    anDW2[2];      // для вертикальных шрифтов 
	List     pW2;           //

	char     sRegistry[LIMIT_MAX_NAME_LEN + 1];
	char     sOrdering[LIMIT_MAX_NAME_LEN + 1];
	int      nSupplement;

	int      nCIDtoGIDtype; // -1 вообще не задан, 0 - задан именем, 1 - задан потоком
	char     sCIDtoGIDMap[LIMIT_MAX_NAME_LEN + 1];
	StreamRecPtr   pCIDtoGIDMap;

	CIDFontFileType eType;
	StreamRecPtr          pFontData;
} CIDFontDefAttrRec;

#endif /*_FONTDEF_H */

