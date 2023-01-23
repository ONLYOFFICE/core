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
#ifndef _BUILD_FONT_ENGINE_FONTMANAGER_H_
#define _BUILD_FONT_ENGINE_FONTMANAGER_H_

#include <map>
#include <list>
#include "FontFile.h"

class CFontStream : public NSFonts::IFontStream
{
public:
	BYTE*	m_pData;
	LONG	m_lSize;
    bool    m_bIsAttach;

public:
	CFontStream();
	virtual ~CFontStream();

public:
    virtual int CreateFromFile(const std::wstring& strFileName, BYTE* pDataUse = NULL);
    virtual int CreateFromMemory(BYTE* pData, LONG lSize, bool bClear);
    virtual void GetMemory(BYTE*& pData, LONG& lSize);
};

class CApplicationFontStreams : public NSFonts::IApplicationFontStreams
{
private:
	// этот мап нужно периодически опрашивать и удалять неиспользуемые стримы
	std::map<std::wstring, CFontStream*> m_mapStreams;

public:
	CApplicationFontStreams();
    virtual ~CApplicationFontStreams();

public:
    NSFonts::IFontStream* GetStream(const std::wstring& strFile);
	void CheckStreams(std::map<std::wstring, bool>& mapFiles);
	void Clear();
};

class CFontsCache : public NSFonts::IFontsCache
{
	friend class CFontManager;
	friend class CApplicationFonts;
private:
	std::map<std::string, CFontFile*>	m_mapFiles;
    NSFonts::IApplicationFontStreams*	m_pApplicationFontStreams;

private:
    std::list<std::string>              m_arFiles;
    int m_lCacheSize;

    FT_Library m_pLibrary;
    
public:
    CFontsCache();
    virtual ~CFontsCache();
    virtual void Clear();
    virtual void SetCacheSize(const int& lMaxSize);
    FT_Library GetLibrary();

public:
    virtual void SetStreams(NSFonts::IApplicationFontStreams* pStreams) { m_pApplicationFontStreams = pStreams; }
    virtual NSFonts::IFontFile* LockFont(NSFonts::CLibrary& library, const std::wstring& strFileName, const int& lFaceIndex, const double& dSize);
};

class CApplicationFonts;
class CFontManager : public NSFonts::IFontManager
{
	friend class CApplicationFonts;

public:
	CFontFile*		m_pFont;
	CGlyphString	m_oString;

	std::wstring	m_sName;

    INT	m_bUseDefaultFont;
    double	m_fCharSpacing;
    INT	m_bStringGID;

    int m_lUnits_Per_Em;
    int m_lAscender;
    int m_lDescender;
    int m_lLineHeight;

    int m_nLOAD_MODE;
    int m_nRENDER_MODE;

	bool m_bCorrectFontByName;

	CApplicationFonts*	m_pApplication;
	CFontsCache*		m_pOwnerCache;

public:
	CFontManager();
	~CFontManager();

public:
    virtual void AfterLoad();

    virtual void Initialize();
    virtual void SetOwnerCache(NSFonts::IFontsCache* pCache);

    virtual NSFonts::IFontsCache* GetCache();
    virtual NSFonts::IApplicationFonts* GetApplication();
    virtual NSFonts::IFontFile* GetFile();

    virtual double GetCharSpacing() { return m_fCharSpacing; }
    virtual void SetCharSpacing(const double &dCharSpacing);

    virtual int GetStringGID() { return m_bStringGID; }
    virtual void SetStringGID(const int& bStringGID);

    virtual int GetUnitsPerEm() { return m_lUnits_Per_Em; }
    virtual int GetAscender() { return m_lAscender; }
    virtual int GetDescender() { return m_lDescender; }
    virtual int GetLineHeight() { return m_lLineHeight; }

    virtual std::wstring GetName() { return m_sName; }

    virtual double UpdateSize(const double& dOldSize, const double& dDpi, const double& dNewDpi);
	
    virtual INT LoadString1(const std::wstring& wsBuffer, const float& fX, const float& fY);
    virtual INT LoadString2(const std::wstring& wsBuffer, const float& fX, const float& fY);
    virtual INT LoadString1(const unsigned int* pGids, const unsigned int& nGidsCount, const float& fX, const float& fY);
    virtual INT LoadString2(const unsigned int* pGids, const unsigned int& nGidsCount, const float& fX, const float& fY);
    virtual INT LoadString3(const int& gid, const float& fX, const float& fY);
    virtual INT LoadString3C(const int& gid, const float& fX, const float& fY);
    virtual INT LoadString2C(const int& wsBuffer, const float& fX, const float& fY);

    virtual int GetKerning(UINT unPrevGID, UINT unGID);
    virtual int GetUnderline(float *pfStartX, float *pfStartY, float *pfEndX, float *pfEndY, float *pfSize);

    virtual TFontCacheSizes MeasureChar(const LONG& lUnicode);

    virtual TBBoxAdvance MeasureChar2(const LONG& lUnicode);
    virtual TBBox MeasureString();
    virtual TBBox MeasureString2();
	
    virtual INT GetNextChar2(TGlyph*& pGlyph, float& fX, float& fY);

    virtual INT SetTextMatrix(const double& fA, const double& fB, const double& fC, const double& fD, const double& fE, const double& fF);
    virtual INT SetTextMatrix2(const double& fA, const double& fB, const double& fC, const double& fD, const double& fE, const double& fF);

    virtual INT GetStringPath(NSFonts::ISimpleGraphicsPath* pPath);

    virtual NSFonts::CFontInfo* GetFontInfoByParams(NSFonts::CFontSelectFormat& oFormat, bool bIsDictionaryUse = true);
    virtual std::vector<NSFonts::CFontInfo*> GetAllStylesByFontName(const std::wstring& strName);
    virtual INT LoadFontByName(const std::wstring& sName, const double& dSize, const int& lStyle, const double& dDpiX, const double& dDpiY);
    virtual INT LoadFontFromFile(const std::wstring& sPath, const int& lFaceIndex, const double& dSize, const double& dDpiX, const double& dDpiY);
    virtual INT LoadFontFromFile2(NSFonts::IFontsCache* pCache, const std::wstring& sPath, const int& lFaceIndex, const double& dSize, const double& dDpiX, const double& dDpiY);
    virtual void CloseFont();

    virtual std::wstring GetFontType();
    virtual unsigned int GetNameIndex(const std::wstring& wsName);
	virtual unsigned int GetGIDByUnicode(const unsigned int& unCode);

    virtual void SetSubpixelRendering(const bool& hmul, const bool& vmul);
    
    virtual void GetFace(double& d0, double& d1, double& d2);
    virtual void GetLimitsY(double& dMin, double& dMax);

	virtual void SetUseCorrentFontByName(const bool& use);

    CFontFile* GetFontFileBySymbol(CFontFile* pFile, int code);
};

#endif // _BUILD_FONT_ENGINE_FONTMANAGER_H_
