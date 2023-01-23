/*
 * (c) Copyright Ascensio System SIA 2010-2019
 *
 * This program is a free software product. You can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public License (AGPL)
 * version 3 as published by the Free Software Foundation. In accordance with
 * Section 7(a) of the GNU AGPL its Section 15 shall be amended to the effect
 * that Ascensio System SIA expressly excludes the warranty of non-infringement
 * of any third-party rights.
 *
 * This program is distributed WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR  PURPOSE. For
 * details, see the GNU AGPL at: http://www.gnu.org/licenses/agpl-3.0.html
 *
 * You can contact Ascensio System SIA at 20A-12 Ernesta Birznieka-Upisha
 * street, Riga, Latvia, EU, LV-1050.
 *
 * The  interactive user interfaces in modified source and object code versions
 * of the Program must display Appropriate Legal Notices, as required under
 * Section 5 of the GNU AGPL version 3.
 *
 * Pursuant to Section 7(b) of the License you must retain the original Product
 * logo when distributing the program. Pursuant to Section 7(e) we decline to
 * grant you any rights under trademark law for use of our trademarks.
 *
 * All the Product's GUI elements, including illustrations and icon sets, as
 * well as technical writing content are licensed under the terms of the
 * Creative Commons Attribution-ShareAlike 4.0 International. See the License
 * terms at http://creativecommons.org/licenses/by-sa/4.0/legalcode
 *
 */
#ifndef _BUILD_FONT_ENGINE_FONTFILE_H_
#define _BUILD_FONT_ENGINE_FONTFILE_H_

// freetype
#include <ft2build.h>
#include FT_OUTLINE_H
#include FT_SIZES_H
#include FT_GLYPH_H
#include FT_TRUETYPE_IDS_H
#include FT_TRUETYPE_TABLES_H
#include FT_XFREE86_H
#include FT_ADVANCES_H

#include "FontPath.h"
#include "GlyphString.h"
#include "../common/File.h"
#include <map>

static std::wstring GetCorrectSfntName(const char* name)
{
    if (NULL == name)
        return L"";
    const char* name_cur = name;
    int name_len = 0;
    while (*name_cur++)
        ++name_len;

    name_cur = name;
    bool isUtf8 = false;
    if (6 < name_len)
    {
        if ('<' == name[0] && 'u' == name[1] && 't' == name[2] &&
            'f' == name[3] && '8' == name[4] && '>' == name[5])
        {
            name_cur = name + 6;
            name_len -= 6;
            isUtf8 = true;
        }
    }

    if (isUtf8)
    {
        return NSFile::CUtf8Converter::GetUnicodeStringFromUTF8((BYTE*)name_cur, (LONG)name_len);
    }
    return NSFile::CUtf8Converter::GetUnicodeFromCharPtr(name_cur, (LONG)name_len);
}

//-------------------------------------------------------------------------------------------------------------------------------
// TODO: RasterHeep
//-------------------------------------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------------------------------------
// TODO: TFontCacheSizes
//-------------------------------------------------------------------------------------------------------------------------------
class TFontCacheSizes
{
public:
	int			Unicode; // Значение символа в юникоде
	EGlyphState eState;     // Есть ли символ в шрифте/стандартном шрифте
	int			nCMapIndex; // Номер таблицы 'cmap', в которой был найден данный символ

	int			GID;

	float		fAdvanceX; 
	float		fAdvanceY;

	TBBox		oBBox;
	TMetrics	oMetrics;

	bool		bBitmap;
	TGlyphBitmap oBitmap;

public:
	TFontCacheSizes()
	{
		bBitmap = false;
		fAdvanceX = 0;
	}
	~TFontCacheSizes()
	{
	}
};

#define LOAD_MODE FT_LOAD_NO_HINTING | FT_LOAD_NO_AUTOHINT | FT_LOAD_NO_BITMAP | FT_LOAD_LINEAR_DESIGN

#define FONT_ITALIC_ANGLE  0.3090169943749 // Синус 18 градусов (подбиралось под Word 2007)

#define MAX_UNICODE_VALUE 0x10FFFF
#define MAX_UNICODE_COUNT  0x110000
#define MAX_UNICODE_COUNT2 0x220000

#define FONT_CASHE_SIZES_SIZE 255
#define FONT_CACHE_SIZES_INDEXES_SIZE   MAX_UNICODE_COUNT
#define FONT_CACHE_SIZES_INDEXES_SIZE_2 MAX_UNICODE_COUNT2 // MAX_UNICODE_COUNT * sizeof(unsigned short)

class CVectorWorker
{
public:
    FT_Outline_Funcs*  func_interface;
    void*              user;
};

class CFontFile;
class CCacheGlyphs
{
public:
	std::map<int, TFontCacheSizes> m_mapGids;
	std::map<int, TFontCacheSizes> m_mapUnicodes;

	// потом гиды поменять на массив из индексов.
	// особенно если не так много глифов. быстрота доступа важнее.

	CFontFile* m_pFile;

public:
	CCacheGlyphs()
	{
		m_pFile = NULL;
	}

public:
	TFontCacheSizes* Get(const int& code);
	void Add(const TFontCacheSizes& item);
	void Clear(bool bIsFree = false);
};

class CFontStream;
class CFontManager;
class CFontFile : public NSFonts::IFontFile
{
public:
	double m_arrdFontMatrix[6];
    double m_arrdTextMatrix[6];

    bool m_bAntiAliasing;
    bool m_bUseKerning;

    double m_dSize;			// Размер шрифта
    double m_unHorDpi;		// Горизонтальное разрешение
    double m_unVerDpi;		// Вертикальное разрешение

    INT m_bNeedDoItalic;
    INT m_bNeedDoBold;

    double m_dCharSpacing;

    int m_nMinX;		//
    int m_nMinY;        // Glyph BBox
    int m_nMaxX;        //
    int m_nMaxY;        //
    
	CFontStream*	m_pStream;
	std::wstring	m_sFileName;
    int				m_lFaceIndex;
	
	FT_Face m_pFace;
	
	double m_dUnitsKoef;
	int m_nDefaultChar;
	int m_nSymbolic;
	double m_dTextScale;

    INT m_bStringGID;
	
	FT_Matrix m_oFontMatrix;
	FT_Matrix m_oTextMatrix;
	
	int m_nNum_charmaps;

    int m_lAscender;
    int m_lDescender;
    int m_lLineHeight;
    int m_lUnits_Per_Em;

    INT		m_bUseDefaultFont;
	CFontFile*	m_pDefaultFont;

    INT		m_bIsNeedUpdateMatrix12;

	CFontManager* m_pFontManager;
    INT m_bHintsSupport;

    std::wstring m_sName;

	CCacheGlyphs m_oCache;

public:

	CFontFile();
	~CFontFile();

	void SetDefaultFont(CFontFile* pDefFont);
	void LoadDefaultCharAndSymbolicCmapIndex();
	
	void ResetFontMatrix();	
	void ResetTextMatrix();

    void CheckTextMatrix();
	
	void UpdateMatrix0();
    void UpdateMatrix1();
    void UpdateMatrix2();
	
	void SetSizeAndDpi(double dSize, double unHorDpi, double unVerDpi);
	
	void ClearCache();
	void ClearCacheNoAttack(bool bIsFree = false);
    void Destroy();

	bool SetTextMatrix(const double& fA, const double& fB, const double& fC, const double fD, double fE, double fF);
    void SetFontMatrix(const double& fA, const double& fB, const double& fC, const double fD, double fE, double fF);
    
    TFontCacheSizes CacheGlyph(const int& code, const bool& isRaster, CVectorWorker* pWorker = NULL, const bool& isFromPicker = false);

	INT GetString(CGlyphString& oString);
	INT GetString2(CGlyphString& oString);
    INT GetString2C(CGlyphString& oString);
	TFontCacheSizes GetChar(LONG lUnicode);
	
	int SetCMapForCharCode(long lUnicode, int *pnCMapIndex);
	int SetCMapForCharCode2(long lUnicode);

    double GetCharWidth(int gid);

    int GetGIDByUnicode(int code);

    int GetKerning(FT_UInt unPrevGID, FT_UInt unGID);
    void SetStringGID(const INT& bGID);
    INT GetStringGID();
    void SetUseDefaultFont(const INT& bUse);
    INT GetUseDefaultFont();
    void SetCharSpacing(const double& dCharSpacing);
    double GetCharSpacing();
    std::string GetStyleName();
    void GetPanose(BYTE* pData);

    bool IsFixedWidth();

    int IsUnicodeRangeAvailable(unsigned long ulBit, unsigned int un4ByteIndex);
    
	void UpdateStyles(const INT& bBold, const INT& bItalic);
    
	void SetItalic(const INT& value);
    void SetNeedBold(const INT& value);
    
	int GetAscender();
    int GetDescender();
    int GetHeight();
    int Units_Per_Em();

    void CheckHintsSupport();

    std::wstring GetFontFormat();
	unsigned int GetNameIndex(const std::wstring& wsName) const;

	// path
    NSFonts::IFontPath* GetGlyphPath(int nCode);

    virtual bool IsItalic();
    virtual bool IsBold();

    virtual bool IsSymbolic(bool bIsOS2Check = false);

    static NSFonts::EFontFormat GetFontFormatType(FT_Face pFace);
    virtual int GetEmbeddingLicenceType();
    virtual void FillFontSelectFormat(NSFonts::CFontSelectFormat& oFormat);
};


//-------------------------------------------------------------------------------------------------------------------------------
struct TFreeTypeFontPath 
{
	CFontPath*	pPath;
	INT		bNeedClose;
};

static int GlyphPathMoveTo(const FT_Vector *pPoint, void *pPath);
static int GlyphPathLineTo(const FT_Vector *pPoint, void *pPath);
static int GlyphPathConicTo(const FT_Vector *pControlPoint, const FT_Vector *pEndPoint, void *pPath);
static int GlyphPathCubicTo(const FT_Vector *pFirstControlPoint, const FT_Vector *pSecondControlPoint, const FT_Vector *pEndPoint, void *pPath);

#endif // _BUILD_FONT_ENGINE_FONTFILE_H_
