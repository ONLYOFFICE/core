#include "ApplicationFonts.h"
#include "../common/File.h"
#include <stdio.h>

///////////////////////////////////////////////////////////////////////////////////////////////////////
CFontStream::CFontStream()
{
	m_pData	= NULL;
	m_lSize = 0;
}
CFontStream::~CFontStream()
{
	RELEASEARRAYOBJECTS(m_pData);
}

int CFontStream::AddRef()
{
	++m_lRef;
	return m_lRef;
}
int CFontStream::Release()
{
	int ret = --m_lRef;
	if (0 == m_lRef)
		delete this;
	return ret;
}

BOOL CFontStream::CreateFromFile(const std::wstring& strFileName)
{
	NSFile::CFileBinary oFile;
	if (!oFile.OpenFile(strFileName))
		return FALSE;

	m_lSize = oFile.GetFileSize();
	m_pData = new BYTE[m_lSize];

	DWORD dwRead = 0;
	DWORD dwNeedRead = (DWORD)m_lSize;
	oFile.ReadFile(m_pData, dwNeedRead, dwRead);

	if (dwNeedRead != dwRead)
	{
		RELEASEARRAYOBJECTS(m_pData);
		m_lSize = 0;
		return FALSE;
	}
	
	oFile.CloseFile();
	return true;
}

CApplicationFontStreams::CApplicationFontStreams()
{
}
CApplicationFontStreams::~CApplicationFontStreams()
{
}

CFontStream* CApplicationFontStreams::GetStream(const std::wstring &strFile)
{
	CFontStream* pStream = m_mapStreams[strFile];

	if (NULL != pStream)
		return pStream;

	pStream = new CFontStream();
	pStream->CreateFromFile(strFile);

	m_mapStreams[strFile] = pStream;
	return pStream;
}
void CApplicationFontStreams::CheckStreams(std::map<std::wstring,bool> &mapFiles)
{
	// TODO:
}

///////////////////////////////////////////////////////////////////////////////////////////////////////
CFontFile* CFontManager::LoadFontFile(FT_Library library, CFontStream* pStream, LONG lFaceIndex)
{
	FT_Open_Args oOpenArgs;
	oOpenArgs.flags			= FT_OPEN_MEMORY;
	oOpenArgs.memory_base	= pStream->m_pData;
	oOpenArgs.memory_size	= pStream->m_lSize;

	FT_Face pFace;
	if ( FT_Open_Face( library, &oOpenArgs, lFaceIndex, &pFace ) )
		return NULL;

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
		FT_Done_Face(pFace);
		delete pFont;
		return NULL;
	}

	pFont->ResetTextMatrix();
	pFont->ResetFontMatrix();

	return pFont;
}

CFontFile* CFontsCache::LockFont(FT_Library library, const std::wstring& strFileName, const LONG& lFaceIndex, const double& dSize)
{
	if (NULL == m_pApplicationFontStreams)
		return NULL;

	std::string sLock = NSFile::CUtf8Converter::GetUtf8StringFromUnicode2(strFileName.c_str(), strFileName.length(), false);
	
	char buffer[50];
	sprintf(buffer, "%d_%.2lf", lFaceIndex, dSize);
	std::string s(buffer);

	sLock += s;

	CFontFile* pFile = m_mapFiles[sLock];

	if (NULL != pFile)
		return pFile;

	CFontStream* pStream = m_pApplicationFontStreams->GetStream(strFileName);
	pFile = CFontManager::LoadFontFile(library, pStream, lFaceIndex);
	if (NULL == pFile)
		return NULL;

	pFile->m_pStream = pStream;
	pFile->m_pStream->AddRef();
	m_mapFiles[sLock] = pFile;	
	return pFile;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////
CFontManager::CFontManager()
{
	m_pLibrary = NULL;
	FT_Init_FreeType(&m_pLibrary);

	m_pFont = NULL;
	m_pApplication = NULL;
	m_pOwnerCache = NULL;

	m_bStringGID = FALSE;
	m_nLOAD_MODE = 40968;
	m_lRef = 1;

    m_bUseDefaultFont = FALSE;
    m_fCharSpacing = 0;
    
    m_lUnits_Per_Em = 0;
    m_lAscender = 0;
    m_lDescender = 0;
    m_lLineHeight = 0;
}

CFontManager::~CFontManager()
{
	if (m_pLibrary)
	{
		FT_Done_FreeType(m_pLibrary);
	}
	RELEASEOBJECT(m_pOwnerCache);
}
void CFontManager::SetOwnerCache(CFontsCache* pCache)
{
	m_pOwnerCache = pCache;
}

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

BOOL CFontManager::LoadString(const std::wstring &wsBuffer, const float &fX, const float &fY)
{
	if (NULL == m_pFont)
		return FALSE;

	m_oString.SetString(wsBuffer, fX, fY);

	// TODO: 1!!!
	m_pFont->GetString2(m_oString);
	return TRUE;
}

BOOL CFontManager::LoadString2(const std::wstring &wsBuffer, const float &fX, const float &fY)
{
	if (NULL == m_pFont)
		return FALSE;

	m_oString.SetString(wsBuffer, fX, fY);
	m_pFont->GetString2(m_oString);
	return TRUE;
}

BOOL CFontManager::LoadString3(const LONG& gid, const float &fX, const float &fY)
{
	if (NULL == m_pFont)
		return FALSE;

	SetStringGID(TRUE);
	m_oString.SetStringGID(gid, fX, fY);
	m_pFont->GetString2(m_oString);
	SetStringGID(FALSE);

	return TRUE;
}

BOOL CFontManager::LoadString3C(const LONG& gid, const float &fX, const float &fY)
{
	if (NULL == m_pFont)
		return FALSE;

	SetStringGID(TRUE);
	m_oString.SetStringGID(gid, fX, fY);
	m_pFont->GetString2C(m_oString);
	SetStringGID(FALSE);

	return TRUE;
}

BOOL CFontManager::LoadString2C(const LONG& code, const float &fX, const float &fY)
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

int CFontManager::GetKerning(UINT unPrevGID, UINT unGID)
{
    if (NULL == m_pFont)
		return 0;

	return m_pFont->GetKerning(unPrevGID, unGID);
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

		if (oBox.fMaxX < fB)
			oBox.fMaxX = fB;

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

BOOL CFontManager::GetNextChar2(TGlyph*& pGlyph, float& fX, float& fY)
{
	if (!m_oString.GetNext(pGlyph))
		return FALSE;

	if (glyphstateNormal == pGlyph->eState || (glyphstateDeafault == pGlyph->eState && NULL != m_pFont->m_pDefaultFont))
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

BOOL CFontManager::SetTextMatrix(const double &fA, const double &fB, const double &fC, const double &fD, const double &fE, const double &fF)
{
    if (NULL == m_pFont)
        return FALSE;

    if (m_pFont->SetTextMatrix(fA, fB, fC, fD, 0, 0))
		m_oString.SetCTM((float)fA, (float)fB, (float)fC, (float)fD, 0, 0);
	m_oString.SetTrans((float)fE, (float)fF);

    return TRUE;
}

BOOL CFontManager::SetTextMatrix2(const double &fA, const double &fB, const double &fC, const double &fD, const double &fE, const double &fF)
{
    if (NULL == m_pFont)
        return FALSE;

    m_pFont->SetTextMatrix(fA, fB, fC, fD, 0, 0);
    m_oString.SetCTM((float)fA, (float)fB, (float)fC, (float)fD, 0, 0);
    m_oString.SetTrans((float)fE, (float)fF);

    return TRUE;
}

void CFontManager::SetStringGID(const BOOL &bStringGID)
{
	m_bStringGID = bStringGID;

	if (NULL == m_pFont)
		return;

	m_pFont->SetStringGID(m_bStringGID);
}

BOOL CFontManager::GetStringPath(ISimpleGraphicsPath* pInterface)
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
		if (glyphstateNormal == pCurGlyph->eState || (glyphstateDeafault == pCurGlyph->eState && NULL != m_pFont->m_pDefaultFont))
		{
			if (glyphstateNormal == pCurGlyph->eState)
			{
				pPath = m_pFont->GetGlyphPath(pCurGlyph->lUnicode);
			}
			else
			{
				pPath = m_pFont->m_pDefaultFont->GetGlyphPath(pCurGlyph->lUnicode);
			}
		}

		if (!pPath)
			continue;

		pPath->Reverse();
		pPath->ToMM(m_pFont->m_unHorDpi, m_pFont->m_unVerDpi);

		pPath->Offset((pCurGlyph->fX * 25.4 * m_pFont->m_unHorDpi) + m_oString.m_fX,
						(pCurGlyph->fY * 25.4 * m_pFont->m_unVerDpi) + m_oString.m_fY);

		pOverallPath->Append(pPath);
		delete pPath;
	}

	pOverallPath->ToInterface(pInterface);
	delete pOverallPath;

	return TRUE;
}

int CFontManager::AddRef()
{
	++m_lRef;
	return m_lRef;
}
int CFontManager::Release()
{
	int ret = --m_lRef;
	if (0 == m_lRef)
		delete this;
	return ret;
}

BOOL CFontManager::LoadFontByName(const std::wstring& sName, const double& dSize, const LONG& lStyle, const double& dDpiX, const double& dDpiY)
{
	if (NULL == m_pApplication)
		return FALSE;

	CFontSelectFormat oFormat;
	oFormat.wsName = new std::wstring(sName);

	oFormat.bBold = new BOOL(FALSE);
	oFormat.bItalic = new BOOL(FALSE);

	if (lStyle & 0x01)
		*oFormat.bBold = TRUE;
	if (lStyle & 0x02)
		*oFormat.bItalic = TRUE;

	CFontInfo* pInfo = m_pApplication->GetList()->GetByParams(oFormat);
	if (NULL == pInfo)
		return FALSE;

	return LoadFontFromFile(pInfo->m_wsFontPath, pInfo->m_lIndex, dSize, dDpiX, dDpiY);
}

BOOL CFontManager::LoadFontFromFile(const std::wstring& sPath, const int& lFaceIndex, const double& dSize, const double& dDpiX, const double& dDpiY)
{
	if (NULL == m_pApplication)
		return FALSE;

	CFontsCache* pCache		= (m_pOwnerCache != NULL) ? m_pOwnerCache : m_pApplication->GetCache();
	
	m_pFont = pCache->LockFont(m_pLibrary, sPath, lFaceIndex, dSize);
	m_pFont->m_pFontManager = this;
	m_pFont->SetSizeAndDpi(dSize, (UINT)dDpiX, (UINT)dDpiY);

	return TRUE;
}

BOOL CFontManager::LoadFontFromFile2(CFontsCache* pCache, const std::wstring& sPath, const int& lFaceIndex, const double& dSize, const double& dDpiX, const double& dDpiY)
{
	if (NULL == pCache)
		return FALSE;

	m_pFont = pCache->LockFont(m_pLibrary, sPath, lFaceIndex, dSize);
	m_pFont->m_pFontManager = this;
	m_pFont->SetSizeAndDpi(dSize, (UINT)dDpiX, (UINT)dDpiY);

	return TRUE;
}