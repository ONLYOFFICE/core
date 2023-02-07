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
 #include "ApplicationFonts.h"
#include "../common/File.h"
#include <stdio.h>
#include "ftsnames.h"
#include FT_LCD_FILTER_H
#include "internal/tttypes.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////
CFontStream::CFontStream() : NSFonts::IFontStream()
{
	m_pData	= NULL;
	m_lSize = 0;
    m_bIsAttach = false;
}
CFontStream::~CFontStream()
{
    if (!m_bIsAttach)
        RELEASEARRAYOBJECTS(m_pData);
}

int CFontStream::CreateFromFile(const std::wstring& strFileName, BYTE* pDataUse)
{
	NSFile::CFileBinary oFile;
	if (!oFile.OpenFile(strFileName))
		return FALSE;

	m_lSize = oFile.GetFileSize();
    
    if (NULL == pDataUse)
	m_pData = new BYTE[m_lSize];
    else
    {
        m_bIsAttach = true;
        m_pData = pDataUse;
    }

	DWORD dwRead = 0;
	DWORD dwNeedRead = (DWORD)m_lSize;
	oFile.ReadFile(m_pData, dwNeedRead, dwRead);

	if (dwNeedRead != dwRead)
	{
        if (!m_bIsAttach)
		RELEASEARRAYOBJECTS(m_pData);
        
		m_lSize = 0;
		return FALSE;
	}
	
	oFile.CloseFile();
	return true;
}

int CFontStream::CreateFromMemory(BYTE* pData, LONG lSize, bool bClear)
{
    if (bClear)
    {
        m_pData = new BYTE[lSize];
        memcpy(m_pData, pData, lSize);
        m_bIsAttach = false;
    }
    else
    {
        m_pData = pData;
        m_bIsAttach = true;
    }
    m_lSize = lSize;

    if (!m_pData)
    {
        m_lSize = 0;
        return FALSE;
    }
    return TRUE;
}
void CFontStream::GetMemory(BYTE*& pData, LONG& lSize)
{
    pData = m_pData;
    lSize = m_lSize;
}

CApplicationFontStreams::CApplicationFontStreams() : NSFonts::IApplicationFontStreams()
{
}
CApplicationFontStreams::~CApplicationFontStreams()
{
    Clear();
}

NSFonts::IFontStream* CApplicationFontStreams::GetStream(const std::wstring &strFile)
{
    CFontStream* pStream = m_mapStreams[strFile];

	if (NULL != pStream)
		return pStream;

    if (NSFonts::NSApplicationFontStream::GetGlobalMemoryStorage())
    {
        pStream = (CFontStream*)NSFonts::NSApplicationFontStream::GetGlobalMemoryStorage()->Get(strFile);
        if (pStream)
        {
            // чтобы удалить и из мапа и из стораджа
            pStream->AddRef();
        }
    }

#ifndef FONT_ENGINE_DISABLE_FILESYSTEM
    if (!pStream)
	{
		pStream = new CFontStream();
		pStream->CreateFromFile(strFile);
	}
#endif

	m_mapStreams[strFile] = pStream;
	return pStream;
}
void CApplicationFontStreams::CheckStreams(std::map<std::wstring,bool> &mapFiles)
{
    std::map<std::wstring, CFontStream*>::iterator iter = m_mapStreams.begin();
    while (iter != m_mapStreams.end())
    {
        CFontStream* pFile = iter->second;

        if (mapFiles.find(iter->first) != mapFiles.end())
        {
            RELEASEINTERFACE(pFile);
            iter = m_mapStreams.erase(iter);
        }
        else
            iter++;
    }
}

void CApplicationFontStreams::Clear()
{
    if (NSFonts::NSApplicationFontStream::GetGlobalMemoryStorage())
        NSFonts::NSApplicationFontStream::GetGlobalMemoryStorage()->Clear();

    for (std::map<std::wstring, CFontStream*>::iterator iter = m_mapStreams.begin(); iter != m_mapStreams.end(); ++iter)
    {
        CFontStream* pFile = iter->second;
        RELEASEINTERFACE(pFile);
    }
    m_mapStreams.clear();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////
NSFonts::IFontFile* NSFonts::IFontManager::LoadFontFile(NSFonts::CLibrary& library, NSFonts::IFontStream* pStreamI, int lFaceIndex)
{
    CFontStream* pStream = (CFontStream*)pStreamI;
	FT_Open_Args oOpenArgs;
	oOpenArgs.flags			= FT_OPEN_MEMORY | FT_OPEN_PARAMS;
	oOpenArgs.memory_base	= pStream->m_pData;
	oOpenArgs.memory_size	= pStream->m_lSize;

	FT_Parameter *pParams = (FT_Parameter *)::malloc( sizeof(FT_Parameter) * 4 );
	pParams[0].tag  = FT_MAKE_TAG( 'i', 'g', 'p', 'f' );
	pParams[0].data = NULL;
	pParams[1].tag  = FT_MAKE_TAG( 'i', 'g', 'p', 's' );
	pParams[1].data = NULL; 
	pParams[2].tag  = FT_PARAM_TAG_IGNORE_PREFERRED_FAMILY;
	pParams[2].data = NULL; 
	pParams[3].tag  = FT_PARAM_TAG_IGNORE_PREFERRED_SUBFAMILY;
	pParams[3].data = NULL; 

	oOpenArgs.params = pParams;
	oOpenArgs.num_params = 4;

	FT_Face pFace;
    if ( FT_Open_Face( library.m_internal->m_library, &oOpenArgs, lFaceIndex, &pFace ) )
		return NULL;

	::free(pParams);

	CFontFile* pFont = new CFontFile();
	pFont->m_lFaceIndex = lFaceIndex;

	pFont->m_lUnits_Per_Em	= pFace->units_per_EM;
	pFont->m_lAscender		= pFace->ascender;
	pFont->m_lDescender		= pFace->descender;
	pFont->m_lLineHeight	= pFace->height;

	pFont->m_nNum_charmaps	= pFace->num_charmaps;

	pFont->m_pFace = pFace;
	pFont->LoadDefaultCharAndSymbolicCmapIndex();

	if (FT_Set_Char_Size(pFace, 0, (FT_F26Dot6)(pFont->m_dSize * 64), 0, 0))
	{		
		delete pFont;
		return NULL;
	}

	pFont->ResetTextMatrix();
	pFont->ResetFontMatrix();

	return pFont;
}

NSFonts::IFontFile* CFontsCache::LockFont(NSFonts::CLibrary& library, const std::wstring& strFileName, const int& lFaceIndex, const double& dSize)
{
	if (NULL == m_pApplicationFontStreams)
		return NULL;

	std::string sLock = U_TO_UTF8(strFileName);
	sLock += (std::to_string(lFaceIndex) + "_" + std::to_string((int)(100 * dSize + 0.5)));

	CFontFile* pFile = m_mapFiles[sLock];

	if (NULL != pFile)
    {
        pFile->AddRef();
        return pFile;
    }

    CFontStream* pStream = (CFontStream*)m_pApplicationFontStreams->GetStream(strFileName);
    if (NULL == pStream)
        return NULL;
    pFile = (CFontFile*)CFontManager::LoadFontFile(library, pStream, lFaceIndex);
	if (NULL == pFile)
		return NULL;

    if (-1 != m_lCacheSize)
    {
        m_arFiles.push_back(sLock);
        if ((int)m_arFiles.size() > m_lCacheSize)
        {
            std::string sPop = *m_arFiles.begin();
            std::map<std::string, CFontFile*>::iterator _find = m_mapFiles.find(sPop);

            if (m_mapFiles.end() != _find)
            {
                CFontFile* pFontRemove = _find->second;
                RELEASEINTERFACE(pFontRemove);
                m_arFiles.pop_front();
                m_mapFiles.erase(_find);
            }
            else
            {
                // такого быть не должно
                m_mapFiles.erase(_find);
            }
        }
    }

	pFile->m_pStream = pStream;
	pFile->m_pStream->AddRef();
    m_mapFiles[sLock] = pFile;

    pFile->AddRef();
	return pFile;
}

CFontsCache::CFontsCache() : NSFonts::IFontsCache()
{
    m_pApplicationFontStreams = NULL;
    m_lCacheSize = -1;

    m_pLibrary = NULL;
    FT_Init_FreeType(&m_pLibrary);
    FT_Library_SetLcdFilter(m_pLibrary, FT_LCD_FILTER_DEFAULT);
}
CFontsCache::~CFontsCache()
{
    Clear();

    if (m_pLibrary)
        FT_Done_FreeType(m_pLibrary);
}
void CFontsCache::Clear()
{
    for (std::map<std::string, CFontFile*>::iterator iter = m_mapFiles.begin(); iter != m_mapFiles.end(); ++iter)
    {
        CFontFile* pFile = iter->second;
        RELEASEINTERFACE(pFile);
    }
    m_mapFiles.clear();

    if (-1 != m_lCacheSize)
        m_arFiles.clear();
}
void CFontsCache::SetCacheSize(const int& lMaxSize)
{
    if (lMaxSize <= 0)
        m_lCacheSize = -1;
    else
        m_lCacheSize = lMaxSize;
}
FT_Library CFontsCache::GetLibrary()
{
    return m_pLibrary;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////
CFontManager::CFontManager() : NSFonts::IFontManager()
{
    m_pFont = NULL;
	m_pApplication = NULL;
	m_pOwnerCache = NULL;

	m_bStringGID = FALSE;
	m_nLOAD_MODE = 40968;

	m_nRENDER_MODE = FT_RENDER_MODE_NORMAL;

    m_bUseDefaultFont = FALSE;
    m_fCharSpacing = 0;
    
    m_lUnits_Per_Em = 0;
    m_lAscender = 0;
    m_lDescender = 0;
    m_lLineHeight = 0;

	m_bCorrectFontByName = true;
}

CFontManager::~CFontManager()
{
    RELEASEINTERFACE(m_pFont);
    RELEASEOBJECT(m_pOwnerCache);
}
void CFontManager::SetOwnerCache(NSFonts::IFontsCache* pCache)
{
    m_pOwnerCache = (CFontsCache*)pCache;
}

NSFonts::IFontsCache* CFontManager::GetCache() { return m_pOwnerCache; }
NSFonts::IApplicationFonts* CFontManager::GetApplication() { return m_pApplication; }
NSFonts::IFontFile* CFontManager::GetFile() { return m_pFont; }

void CFontManager::AfterLoad()
{
    if (NULL == m_pFont)
    {
        m_lUnits_Per_Em = 0;
        m_lAscender = 0;
        m_lDescender = 0;
        m_lLineHeight = 0;
    }
    else
    {
        m_lUnits_Per_Em = m_pFont->m_lUnits_Per_Em;
        m_lAscender		= m_pFont->m_lAscender;
        m_lDescender	= m_pFont->m_lDescender;
        m_lLineHeight	= m_pFont->m_lLineHeight;

        m_pFont->CheckHintsSupport();
    }
}

void CFontManager::Initialize()
{
}

double CFontManager::UpdateSize(const double &dOldSize, const double &dDpi, const double &dNewDpi)
{
	if (0 == dNewDpi && 0 == dDpi)
		return dOldSize;
	else if (0 == dNewDpi)
		return dOldSize * dDpi / 72.0;
	else if (0 == dDpi)
		return dOldSize * 72.0 / dNewDpi;

	return dOldSize * dDpi / dNewDpi;
}

INT CFontManager::LoadString1(const std::wstring &wsBuffer, const float &fX, const float &fY)
{
	if (NULL == m_pFont)
		return FALSE;

	m_oString.SetString(wsBuffer, fX, fY);
	m_pFont->GetString(m_oString);
	return TRUE;
}

INT CFontManager::LoadString2(const std::wstring &wsBuffer, const float &fX, const float &fY)
{
	if (NULL == m_pFont)
		return FALSE;

	m_oString.SetString(wsBuffer, fX, fY);
	m_pFont->GetString2(m_oString);
	return TRUE;
}

INT CFontManager::LoadString1(const unsigned int* pGids, const unsigned int& nGidsCount, const float &fX, const float &fY)
{
    if (NULL == m_pFont)
        return FALSE;

    m_oString.SetString(pGids, nGidsCount, fX, fY);
    m_pFont->GetString(m_oString);
    return TRUE;
}

INT CFontManager::LoadString2(const unsigned int* pGids, const unsigned int& nGidsCount, const float &fX, const float &fY)
{
    if (NULL == m_pFont)
        return FALSE;

    m_oString.SetString(pGids, nGidsCount, fX, fY);
    m_pFont->GetString2(m_oString);
    return TRUE;
}

INT CFontManager::LoadString3(const int& gid, const float &fX, const float &fY)
{
	if (NULL == m_pFont)
		return FALSE;

	SetStringGID(TRUE);
	m_oString.SetStringGID(gid, fX, fY);
	m_pFont->GetString2(m_oString);
	SetStringGID(FALSE);

	return TRUE;
}

INT CFontManager::LoadString3C(const int& gid, const float &fX, const float &fY)
{
	if (NULL == m_pFont)
		return FALSE;

	SetStringGID(TRUE);
	m_oString.SetStringGID(gid, fX, fY);
	m_pFont->GetString2C(m_oString);
	SetStringGID(FALSE);

	return TRUE;
}

INT CFontManager::LoadString2C(const int& code, const float &fX, const float &fY)
{
	if (NULL == m_pFont)
		return FALSE;

	m_oString.SetStringGID(code, fX, fY);
	m_pFont->GetString2C(m_oString);

	return TRUE;
}

TFontCacheSizes CFontManager::MeasureChar(const LONG &lUnicode)
{
	if (NULL == m_pFont)
	{
		TFontCacheSizes oRet;
		return oRet;
	}
	return m_pFont->GetChar(lUnicode);
}

TBBoxAdvance CFontManager::MeasureChar2(const LONG& lUnicode)
{
    TBBoxAdvance ret;
    if (NULL == m_pFont)
        return ret;

    TFontCacheSizes sizes = m_pFont->GetChar(lUnicode);
    ret.box = sizes.oBBox;
    ret.fAdvanceX = sizes.fAdvanceX;
    ret.fAdvanceY = sizes.fAdvanceY;
    return ret;
}

int CFontManager::GetKerning(UINT unPrevGID, UINT unGID)
{
    if (NULL == m_pFont)
		return 0;

	return m_pFont->GetKerning(unPrevGID, unGID);
}

int CFontManager::GetUnderline(float *pfStartX, float *pfStartY, float *pfEndX, float *pfEndY, float *pfSize)
{
	if ( m_oString.GetLength() <= 0 || !m_pFont )
	{
		*pfStartX = 0;
		*pfStartY = 0;
		*pfEndX = 0;
		*pfEndY = 0;
		*pfSize = 0;

		return FALSE;
	}

	float fStartX = m_oString.m_fX;
	float fStartY = m_oString.m_fY;
	float fEndX   = m_oString.m_fEndX;
	float fEndY   = m_oString.m_fEndY;

	short shUnderlinePos  = m_pFont->m_pFace->underline_position;
	short shUnderlineSize = m_pFont->m_pFace->underline_thickness;	

	float fKoef = (float)(m_pFont->m_dSize / 1000 * m_pFont->m_unVerDpi / 72.0f);
	float fUnderlinePos  = shUnderlinePos  * fKoef;
	float fUnderLineSize = shUnderlineSize * fKoef;

	fStartY -= fUnderlinePos;
	fEndY   -= fUnderlinePos;

	m_oString.Transform( &fStartX, &fStartY );
	m_oString.Transform( &fEndX, &fEndY );

	*pfStartX = fStartX;
	*pfStartY = fStartY;

	*pfEndX = fEndX;
	*pfEndY = fEndY;

	*pfSize = fUnderLineSize;

	return TRUE;
}
TBBox CFontManager::MeasureString()
{
	TBBox oBox;
	oBox.fMinX = 0xFFFF;
	oBox.fMinY = 0xFFFF;
	oBox.fMaxX = -0xFFFF;
	oBox.fMaxY = -0xFFFF;

	int nLen = m_oString.GetLength();
	if (nLen <= 0)
		return oBox;

	for (int nIndex = 0; nIndex < nLen; ++nIndex)
	{
		float fT, fB, fL, fR;
		m_oString.GetBBox(&fL, &fT, &fR, &fB, nIndex);

		if (oBox.fMaxY < fB)
			oBox.fMaxY = fB;

		if (oBox.fMinY > fT)
			oBox.fMinY = fT;

		if (oBox.fMinX > fL)
			oBox.fMinX = fL;

		if (oBox.fMaxX < fR)
			oBox.fMaxX = fR;
	}

	return oBox;
}

TBBox CFontManager::MeasureString2()
{
	TBBox oBox;
	oBox.fMinX = 0;
	oBox.fMinY = 0;
	oBox.fMaxX = 0;
	oBox.fMaxY = 0;

	int nLen = m_oString.GetLength();
	if (nLen <= 0)
		return oBox;

	m_oString.GetBBox2(&oBox.fMinX, &oBox.fMinY, &oBox.fMaxX, &oBox.fMaxY);
	return oBox;
}

INT CFontManager::GetNextChar2(TGlyph*& pGlyph, float& fX, float& fY)
{
	if (!m_oString.GetNext(pGlyph))
		return FALSE;

    if (glyphstateNormal == pGlyph->eState || (glyphstateDefault == pGlyph->eState && NULL != m_pFont->m_pDefaultFont))
	{
		fX = m_oString.m_fX + pGlyph->fX + pGlyph->oBitmap.nX;
		fY = m_oString.m_fY + pGlyph->fY - pGlyph->oBitmap.nY;
	}
	else
	{
		fX = m_oString.m_fX + pGlyph->fX;
		fY = m_oString.m_fY + pGlyph->fY;
	}
	return TRUE;
}

INT CFontManager::SetTextMatrix(const double &fA, const double &fB, const double &fC, const double &fD, const double &fE, const double &fF)
{
    if (NULL == m_pFont)
        return FALSE;

    if (fabs(fB) > 0.1)
    {
        int y = 0;
        ++y;
    }

    /*
    if (m_pFont->SetTextMatrix(fA, fB, fC, fD, 0, 0))
		m_oString.SetCTM((float)fA, (float)fB, (float)fC, (float)fD, 0, 0);
     */
    m_pFont->SetTextMatrix(fA, fB, fC, fD, 0, 0);
    m_oString.SetCTM((float)fA, (float)fB, (float)fC, (float)fD, 0, 0);
    
	m_oString.SetTrans((float)fE, (float)fF);

    return TRUE;
}

INT CFontManager::SetTextMatrix2(const double &fA, const double &fB, const double &fC, const double &fD, const double &fE, const double &fF)
{
    if (NULL == m_pFont)
        return FALSE;

    m_pFont->SetTextMatrix(fA, fB, fC, fD, 0, 0);
    m_oString.SetCTM((float)fA, (float)fB, (float)fC, (float)fD, 0, 0);
    m_oString.SetTrans((float)fE, (float)fF);

    return TRUE;
}

void CFontManager::SetStringGID(const INT &bStringGID)
{
	m_bStringGID = bStringGID;

	if (NULL == m_pFont)
		return;

	m_pFont->SetStringGID(m_bStringGID);
}

void CFontManager::SetCharSpacing(const double &dCharSpacing)
{
	m_fCharSpacing = dCharSpacing;

	if (NULL == m_pFont)
		return;

	m_pFont->SetCharSpacing(m_fCharSpacing);
}

INT CFontManager::GetStringPath(NSFonts::ISimpleGraphicsPath* pInterface)
{
	if (NULL == pInterface)
		return FALSE;

	int nCount = m_oString.GetLength();
	if ( !m_pFont || nCount <= 0 )
		return FALSE;

	CFontPath* pOverallPath = new CFontPath();
	if (!pOverallPath)
		return FALSE;

	for (int nIndex = 0; nIndex < nCount; ++nIndex)
	{
		TGlyph* pCurGlyph = m_oString.GetAt(nIndex);
		CFontPath* pPath = NULL;
		if (glyphstateNormal == pCurGlyph->eState || (glyphstateDefault == pCurGlyph->eState && NULL != m_pFont->m_pDefaultFont))
		{
			if (glyphstateNormal == pCurGlyph->eState)
			{
				pPath = (CFontPath*)m_pFont->GetGlyphPath(pCurGlyph->lUnicode);
			}
			else
			{
				pPath = (CFontPath*)m_pFont->m_pDefaultFont->GetGlyphPath(pCurGlyph->lUnicode);
			}
		}

		if (!pPath)
			continue;

		pPath->Reverse();
		pPath->ToMM(m_pFont->m_unHorDpi, m_pFont->m_unVerDpi);

		pPath->Offset(m_oString.m_fX + pCurGlyph->fX * 25.4 / m_pFont->m_unHorDpi,
						m_oString.m_fY + pCurGlyph->fY * 25.4 / m_pFont->m_unVerDpi);

		pOverallPath->Append(pPath);
		delete pPath;
	}

	pOverallPath->ToInterface(pInterface);
	delete pOverallPath;

	return TRUE;
}

NSFonts::CFontInfo* CFontManager::GetFontInfoByParams(NSFonts::CFontSelectFormat& oFormat, bool bIsDictionaryUse)
{
    NSFonts::CFontInfo* pRes = NULL;
	if (NULL != m_pApplication)
	{
		pRes = m_pApplication->GetList()->GetByParams(oFormat, bIsDictionaryUse);
	}
	return pRes;
}
std::vector<NSFonts::CFontInfo*> CFontManager::GetAllStylesByFontName(const std::wstring& strName)
{
    std::vector<NSFonts::CFontInfo*> aRes;
	if (NULL != m_pApplication)
        aRes = ((CFontList*)(m_pApplication->GetList()))->GetAllByName(strName);
	return aRes;
}
INT CFontManager::LoadFontByName(const std::wstring& sName, const double& dSize, const int& lStyle, const double& dDpiX, const double& dDpiY)
{
	if (NULL == m_pApplication)
		return FALSE;

    NSFonts::CFontSelectFormat oFormat;
	oFormat.wsName = new std::wstring(sName);

    oFormat.bBold = new INT(FALSE);
    oFormat.bItalic = new INT(FALSE);

	if (lStyle & 0x01)
		*oFormat.bBold = TRUE;
	if (lStyle & 0x02)
		*oFormat.bItalic = TRUE;

	NSFonts::CFontInfo* pInfo = m_pApplication->GetList()->GetByParams(oFormat, m_bCorrectFontByName);
    if (NULL == pInfo)
        return FALSE;

    INT bLoad = LoadFontFromFile(pInfo->m_wsFontPath, pInfo->m_lIndex, dSize, dDpiX, dDpiY);

    if (bLoad == TRUE)
    {
		bool bIsNeedBold = false;
        if (NULL != oFormat.bBold && (*oFormat.bBold) == TRUE && pInfo->m_bBold == FALSE)
            bIsNeedBold = true;
        bool bIsNeedItalic = false;
        if (NULL != oFormat.bItalic && (*oFormat.bItalic) == TRUE && pInfo->m_bItalic == FALSE)
            bIsNeedItalic = true;

        m_pFont->SetNeedBold(bIsNeedBold);
        m_pFont->SetItalic(bIsNeedItalic);
    }
    return bLoad;
}

INT CFontManager::LoadFontFromFile(const std::wstring& sPath, const int& lFaceIndex, const double& dSize, const double& dDpiX, const double& dDpiY)
{
	if (NULL == m_pApplication)
		return FALSE;

    CFontsCache* pCache		= (CFontsCache*)((m_pOwnerCache != NULL) ? m_pOwnerCache : m_pApplication->GetCache());
    RELEASEINTERFACE(m_pFont);
	
    NSFonts::CLibrary library;
    library.m_internal->m_library = pCache->GetLibrary();
    m_pFont = (CFontFile*)pCache->LockFont(library, sPath, lFaceIndex, dSize);
    if (NULL == m_pFont)
        return FALSE;

	m_pFont->m_pFontManager = this;
	m_pFont->SetSizeAndDpi(dSize, dDpiX, dDpiY);

    m_sName = L"";
    if (m_pFont->m_pFace)
    {
        m_pFont->m_sName = GetCorrectSfntName(m_pFont->m_pFace->family_name);
        m_sName = m_pFont->m_sName;
    }

	return TRUE;
}

INT CFontManager::LoadFontFromFile2(NSFonts::IFontsCache* pCache, const std::wstring& sPath, const int& lFaceIndex, const double& dSize, const double& dDpiX, const double& dDpiY)
{
	if (NULL == pCache)
		return FALSE;

    RELEASEINTERFACE(m_pFont);

    NSFonts::CLibrary library;
    library.m_internal->m_library = ((CFontsCache*)pCache)->GetLibrary();
    m_pFont = (CFontFile*)pCache->LockFont(library, sPath, lFaceIndex, dSize);
	if (NULL == m_pFont)
		return FALSE;

	m_pFont->m_pFontManager = this;
	m_pFont->SetSizeAndDpi(dSize, dDpiX, dDpiY);

    m_sName = L"";
    if (m_pFont->m_pFace)
    {
        m_pFont->m_sName = GetCorrectSfntName(m_pFont->m_pFace->family_name);
        m_sName = m_pFont->m_sName;
    }

	return TRUE;
}

void CFontManager::CloseFont()
{
    if (m_pFont)
    {
        m_pFont->ClearCache();
        RELEASEINTERFACE(m_pFont);
    }
}

std::wstring CFontManager::GetFontType()
{
	if (!m_pFont)
		return L"";

	return m_pFont->GetFontFormat();
}
unsigned int CFontManager::GetNameIndex(const std::wstring& wsName)
{
	if (!m_pFont)
		return 0;

	return m_pFont->GetNameIndex(wsName);
}
unsigned int CFontManager::GetGIDByUnicode(const unsigned int& unCode)
{
	if (!m_pFont)
		return 0;

	return m_pFont->GetGIDByUnicode(unCode);
}
void CFontManager::SetSubpixelRendering(const bool& hmul, const bool& vmul)
{
    if (hmul)
        m_nRENDER_MODE = FT_RENDER_MODE_LCD;
    else if (vmul)
        m_nRENDER_MODE = FT_RENDER_MODE_LCD_V;
    else
        m_nRENDER_MODE = FT_RENDER_MODE_NORMAL;
}

void CFontManager::GetFace(double& d0, double& d1, double& d2)
{
    d0 = 2048;
    d1 = 0;
    d2 = 0;
    
    if (m_pFont)
    {
        TT_OS2* os2 = NULL;
        TT_Header* header = NULL;
        if (m_pFont->m_pFace)
        {
            if ((header = (TT_Header*)FT_Get_Sfnt_Table(m_pFont->m_pFace, ft_sfnt_head)) != NULL)
            {
                d1 = header->yMax;
                d2 = header->yMin;
                d0 = header->Units_Per_EM;
            }
            
            if ((os2 = (TT_OS2*)FT_Get_Sfnt_Table(m_pFont->m_pFace, ft_sfnt_os2)) != NULL && os2->version != 0xFFFFU)
            {
                d1 = os2->usWinAscent;
                d2 = -os2->usWinDescent;
            }
        }
    }
}
void CFontManager::GetLimitsY(double& dMin, double& dMax)
{
    dMin = m_pFont->m_pFace->descender;
    dMax = m_pFont->m_pFace->ascender;
    if (FT_IS_SFNT(m_pFont->m_pFace))
    {
        TT_Face ttface = (TT_Face)m_pFont->m_pFace;
        dMin = ttface->header.yMin;
        dMax = ttface->header.yMax;
    }
}

void CFontManager::SetUseCorrentFontByName(const bool& use)
{
	m_bCorrectFontByName = use;
}

CFontFile* CFontManager::GetFontFileBySymbol(CFontFile* pFile, int code)
{
    std::wstring sName = m_pApplication->GetFontBySymbol(code);
    if (sName.empty())
        return NULL;

    // чтобы не зарелизить при ограниченном кэше
    CFontFile* pFontOld = m_pFont;
    m_pFont = NULL;
    std::wstring sOldName = m_sName;

    int nStyle = 0;
    if (pFile->m_bNeedDoBold || pFile->IsBold())
        nStyle |= 1;
    if (pFile->m_bNeedDoItalic || pFile->IsItalic())
        nStyle |= 2;

    LoadFontByName(sName, pFile->m_dSize, nStyle, pFile->m_unHorDpi, pFile->m_unVerDpi);

    if (!m_pFont)
    {
        m_pFont = pFontOld;
        return NULL;
    }

    CFontFile* pFontNew = m_pFont;
    m_pFont = pFontOld;
    m_sName = sOldName;

    memcpy(pFontNew->m_arrdTextMatrix, pFile->m_arrdTextMatrix, 6 * sizeof(double));
    pFontNew->UpdateMatrix2();

    return pFontNew;
}
