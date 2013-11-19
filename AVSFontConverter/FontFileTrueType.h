#ifndef _FONT_FILE_TRUETYPE_H
#define _FONT_FILE_TRUETYPE_H

#include "FontFileBase.h"

class StringExt;
class CHash;

//------------------------------------------------------------------------

#define ttcfTag 0x74746366

//------------------------------------------------------------------------

struct TrueTypeTable 
{
	unsigned int unTag;
	unsigned int unChecksum;
	int          nOffset;
	int          nOrigOffset;
	int          nLen;
};

struct TrueTypeCmap 
{
	int nPlatform;
	int nEncoding;
	int nOffset;
	int nLen;
	int nFormat;
};

struct TrueTypeLoca 
{
	int nIndex;
	int nOrigOffset;
	int nNewOffset;
	int nLen;
};

#define  cffTag 0x43464620
#define maxpTag 0x6d617870
#define cmapTag 0x636d6170
#define glyfTag 0x676c7966
#define headTag 0x68656164
#define hheaTag 0x68686561
#define hmtxTag 0x686d7478
#define locaTag 0x6c6f6361
#define nameTag 0x6e616d65
#define  os2Tag 0x4f532f32
#define postTag 0x706f7374

#define  cvtTag 0x63767420
#define fpgmTag 0x6670676d
#define prepTag 0x70726570

static int CompareTrueTypeLocaOffset(const void *pL1, const void *pL2) 
{
	TrueTypeLoca *pLoca1 = (TrueTypeLoca *)pL1;
	TrueTypeLoca *pLoca2 = (TrueTypeLoca *)pL2;

	if ( pLoca1->nOrigOffset == pLoca2->nOrigOffset )
		return pLoca1->nIndex - pLoca2->nIndex;

	return pLoca1->nOrigOffset - pLoca2->nOrigOffset;
}

static int CompareTrueTypeLocaIndex(const void *pL1, const void *pL2) 
{
	TrueTypeLoca *pLoca1 = (TrueTypeLoca *)pL1;
	TrueTypeLoca *pLoca2 = (TrueTypeLoca *)pL2;

	return pLoca1->nIndex - pLoca2->nIndex;
}

static int CompareTrueTypeTableTag(const void *pTab1, const void *pTab2) 
{
	TrueTypeTable *pTable1 = (TrueTypeTable *)pTab1;
	TrueTypeTable *pTable2 = (TrueTypeTable *)pTab2;

	return (int)pTable1->unTag - (int)pTable2->unTag;
}

//------------------------------------------------------------------------

struct T42Table 
{
	char *sTag;		 // 4-байтовое название
	BOOL  bRequired; // Требуется ли по спецификации TrueType?
};

// TrueType tables to be embedded in Type 42 fonts.
// NB: the table names must be in alphabetical order here.
#define nT42Tables 11
static T42Table t42Tables[nT42Tables] = 
{
	{ "cvt ", TRUE  },
	{ "fpgm", TRUE  },
	{ "glyf", TRUE  },
	{ "head", TRUE  },
	{ "hhea", TRUE  },
	{ "hmtx", TRUE  },
	{ "loca", TRUE  },
	{ "maxp", TRUE  },
	{ "prep", TRUE  },
	{ "vhea", FALSE },
	{ "vmtx", FALSE }
};
#define t42HeadTable  3
#define t42LocaTable  6
#define t42GlyfTable  2
#define t42VheaTable  9
#define t42VmtxTable 10

//------------------------------------------------------------------------
// CFontFileTrueType
//------------------------------------------------------------------------

class CFontFileTrueType: public CFontFileBase 
{
public:

	// Создаем объект TTF из буфера.
	static CFontFileTrueType *LoadFromBuffer(char *sBuffer, int lenA);

	// Создаем объект TTF из файла.
	static CFontFileTrueType *LoadFromFile(wchar_t *wsFileName);

	virtual ~CFontFileTrueType();

	// TRUE, если данный OpenType фонт содержите данные формата CFF.
	// FALSE,если это TrueType фонт ( или OpenType фонт с данными в формате TrueType).
	BOOL IsOpenTypeCFF() 
	{ 
		return m_bOpenTypeCFF; 
	}

	int  GetCmapsCount();

	int  GetCmapPlatform(int nIndex);

	int  GetCmapEncoding(int nIndex);

	int  FindCmap(int nPlatform, int nEncoding);

	// Возвращает GID, соответствующий символу <nChar> в <nIndex>ной CMap.
	unsigned short MapCodeToGID(int nCMapIndex, int nChar);

	// Возвращает GID, соответствующий <sName> в таблице post. Возвращает 0,
	// если символа с таким именем не нашли, или таблицы post нет.
	int MapNameToGID(char *sName);

	// Возвращает карту CIDs в GIDs, и возваращет количество элементов
	// CIDs в *<pnCIDs>.  Только для CID фонтов( OpenType CFF )
	unsigned short *GetCIDToGIDMap(int *pnCIDs);

	// Лицензионные ограничения на включение фонта( в соответствие со
	// спецификацией True Type):

	// * 4: таблицы OS/2 не найдена или некорректна
	// * 3: разрешено устанавливать
	// * 2: разрешено редактировать
	// * 1: разрешено просматривать и печатать
	// * 0: ограничено лицензией
	int GetEmbeddingRestrictions();

	// Convert to a Type 42 font, suitable for embedding in a PostScript
	// file.  <psName> will be used as the PostScript font name (so we
	// don't need to depend on the 'name' table in the font).  The
	// <encoding> array specifies the mapping from char codes to names.
	// If <encoding> is NULL, the encoding is unknown or undefined.  The
	// <codeToGID> array specifies the mapping from char codes to GIDs.
	// (Not useful for OpenType CFF fonts.)
	void ToType42(char *sPSName, char **ppEncoding, unsigned short *pCodeToGID, FontFileOutputFunc pOutputFunc, void *pOutputStream );

	// Convert to a Type 1 font, suitable for embedding in a PostScript
	// file.  This is only useful with 8-bit fonts.  If <newEncoding> is
	// not NULL, it will be used in place of the encoding in the Type 1C
	// font.  If <ascii> is true the eexec section will be hex-encoded,
	// otherwise it will be left as binary data.  If <psName> is
	// non-NULL, it will be used as the PostScript font name.  (Only
	// useful for OpenType CFF fonts.)
	void ToType1(char *sPSName, char **ppNewEncoding, BOOL bASKII, FontFileOutputFunc pOutputFunc, void *pOutputStream );

	// Convert to a Type 2 CIDFont, suitable for embedding in a
	// PostScript file.  <psName> will be used as the PostScript font
	// name (so we don't need to depend on the 'name' table in the
	// font).  The <cidMap> array maps CIDs to GIDs; it has <nCIDs>
	// entries.  (Not useful for OpenType CFF fonts.)
	void ToCIDType2(char *sPSName, unsigned short *pCIDMap, int nCIDCount, BOOL bNeedVerticalMetrics, FontFileOutputFunc pOutputFunc, void *pOutputStream);

	// Convert to a Type 0 CIDFont, suitable for embedding in a
	// PostScript file.  <psName> will be used as the PostScript font
	// name.  (Only useful for OpenType CFF fonts.)
	void ToCIDType0(char *sPSName, FontFileOutputFunc pOutputFunc, void *pOutputStream);

	// Convert to a Type 0 (but non-CID) composite font, suitable for
	// embedding in a PostScript file.  <psName> will be used as the
	// PostScript font name (so we don't need to depend on the 'name'
	// table in the font).  The <cidMap> array maps CIDs to GIDs; it has
	// <nCIDs> entries.  (Not useful for OpenType CFF fonts.)
	void ToType0(char *sPSName, unsigned short *pCIDMap, int nCIDCount, BOOL bNeedVerticalMetrics, FontFileOutputFunc pOutputFunc, void *pOutputStream);

	// Convert to a Type 0 (but non-CID) composite font, suitable for
	// embedding in a PostScript file.  <psName> will be used as the
	// PostScript font name.  (Only useful for OpenType CFF fonts.)
	void ToType0(char *sPSName, FontFileOutputFunc pOutputFunc, void *pOutputStream);

	// Записываем TrueTypeFont File, заполняя недостающие таблицы и корректируя 
	// различные ошибки. Если задан парметр <sName>, в шрифте переписываем таблицу
	// 'name'. Если задан парамтре <pCodeToGID>, тогда в шрифте переписываем
	// таблицу 'cmap'.
	void WriteTTF(FontFileOutputFunc pOutputFunc, void *pOutputStream, char *sName = NULL, unsigned short *pCodeToGID = NULL, unsigned char *pUseGlyfs = NULL, long lGlyfsCount = 0) ;

private:

	CFontFileTrueType(char *sFileName, int nLen, BOOL bFreeFileData);

	void ConvertEncoding   (char **ppEncoding, FontFileOutputFunc pOutputFunc, void *pOutputStream);
	void ConvertCharStrings(char **ppEncoding, unsigned short *pnCodeToGID, FontFileOutputFunc pOutputFunc, void *pOutputStream);
	void ConvertSfnts      (FontFileOutputFunc pOutputFunc, void *pOutputStream, StringExt *seName, BOOL bNeedVerticalMetrics);
	void DumpString(unsigned char *sString, int nLength, FontFileOutputFunc pOutputFunc, void *pOutputStream);
	unsigned int ComputeTableChecksum(unsigned char *sData, int nLength);
	void Parse();
	void ReadPostTable();
	int  SeekTable(char *sTag);

private:

	TrueTypeTable   *m_pTables;
	int              m_nTablesCount;
	TrueTypeCmap    *m_pCMaps;
	int              m_nCMapsCount;
	int              m_nGlyphs;
	int              m_nLocaFormat;
	int              m_arrBBox[4];
	CHash           *m_pNameToGID;
	BOOL             m_bOpenTypeCFF;

	BOOL             m_bSuccess;
};

#endif /* _FONT_FILE_TRUETYPE_H */
