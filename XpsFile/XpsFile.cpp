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
#include "XpsFile.h"
#include "XpsLib/Document.h"

#include "../DesktopEditor/common/File.h"
#include "../DesktopEditor/common/Directory.h"
#include "../DesktopEditor/graphics/pro/Graphics.h"
#include "../DesktopEditor/raster/BgraFrame.h"
#include "../PdfWriter/PdfRenderer.h"

#include "../OfficeUtils/src/OfficeUtils.h"

using namespace XPS;

class CXpsFile_Private
{
public:
    NSFonts::IApplicationFonts* m_pAppFonts;
    NSFonts::IFontManager*      m_pFontManager;
    std::wstring       m_wsTempFolder;
    XPS::CDocument*    m_pDocument;

public:
    CXpsFile_Private(NSFonts::IApplicationFonts* pAppFonts)
    {
        m_pDocument = NULL;

        m_pAppFonts = pAppFonts;

        // Создаем менеджер шрифтов с собственным кэшем
        m_pFontManager = pAppFonts->GenerateFontManager();
        NSFonts::IFontsCache* pMeasurerCache = NSFonts::NSFontCache::Create();
        pMeasurerCache->SetStreams(pAppFonts->GetStreams());
        m_pFontManager->SetOwnerCache(pMeasurerCache);
        pMeasurerCache->SetCacheSize(16);

        m_wsTempFolder = L"";
    }
    ~CXpsFile_Private()
    {
    }
};

CXpsFile::CXpsFile(NSFonts::IApplicationFonts* pAppFonts)
{
    m_pInternal = new CXpsFile_Private(pAppFonts);
    SetTempDirectory(NSFile::CFileBinary::GetTempPath());
}
CXpsFile::~CXpsFile()
{
    if (L"" != m_pInternal->m_wsTempFolder)
        NSDirectory::DeleteDirectory(m_pInternal->m_wsTempFolder);

	Close();
    RELEASEINTERFACE((m_pInternal->m_pFontManager));
}
std::wstring CXpsFile::GetTempDirectory()
{
    return m_pInternal->m_wsTempFolder;
}
void CXpsFile::SetTempDirectory(const std::wstring& wsPath)
{
    if (L"" != m_pInternal->m_wsTempFolder)
        NSDirectory::DeleteDirectory(m_pInternal->m_wsTempFolder);

	int nCounter = 0;
    m_pInternal->m_wsTempFolder = wsPath + L"/XPS/";
    while (NSDirectory::Exists(m_pInternal->m_wsTempFolder))
	{
        m_pInternal->m_wsTempFolder = wsPath + L"/XPS" + std::to_wstring(nCounter) + L"/";
		nCounter++;
	}
    NSDirectory::CreateDirectory(m_pInternal->m_wsTempFolder);
}
bool CXpsFile::LoadFromFile(const std::wstring& wsSrcFileName, const std::wstring& wsXmlOptions,
                            const std::wstring& owner_password, const std::wstring& user_password)
{
	Close();

	// Распаковываем Zip-архив в темповую папку
	COfficeUtils oUtils(NULL);
    if (S_OK != oUtils.ExtractToDirectory(wsSrcFileName, m_pInternal->m_wsTempFolder, NULL, 0))
		return false;

    m_pInternal->m_pDocument = new XPS::CDocument(m_pInternal->m_pFontManager);
    if (!m_pInternal->m_pDocument)
		return false;

    std::wstring wsPath = m_pInternal->m_wsTempFolder + L"/";
    m_pInternal->m_pDocument->ReadFromPath(wsPath);

	return true;
}
void CXpsFile::Close()
{
    if (m_pInternal->m_pDocument)
	{
        m_pInternal->m_pDocument->Close();
        delete m_pInternal->m_pDocument;
        m_pInternal->m_pDocument = NULL;
	}
}
int CXpsFile::GetPagesCount()
{
    if (!m_pInternal->m_pDocument)
		return 0;

    return m_pInternal->m_pDocument->GetPageCount();
}
void CXpsFile::GetPageInfo(int nPageIndex, double* pdWidth, double* pdHeight, double* pdDpiX, double* pdDpiY)
{
	int nW = 0, nH = 0;

    if (m_pInternal->m_pDocument)
        m_pInternal->m_pDocument->GetPageSize(nPageIndex, nW, nH);

	*pdWidth  = nW * 25.4 / 96;
	*pdHeight = nH * 25.4 / 96;
	*pdDpiX   = 25.4;
	*pdDpiY   = 25.4;
}
void CXpsFile::DrawPageOnRenderer(IRenderer* pRenderer, int nPageIndex, bool* pBreak)
{
    if (!m_pInternal->m_pDocument)
		return;

    m_pInternal->m_pDocument->DrawPage(nPageIndex, pRenderer, pBreak);
}
void CXpsFile::ConvertToRaster(int nPageIndex, const std::wstring& wsDstPath, int nImageType, const int nRasterW, const int nRasterH)
{
    NSFonts::IFontManager *pFontManager = m_pInternal->m_pAppFonts->GenerateFontManager();
    NSFonts::IFontsCache* pFontCache = NSFonts::NSFontCache::Create();
    pFontCache->SetStreams(m_pInternal->m_pAppFonts->GetStreams());
	pFontManager->SetOwnerCache(pFontCache);

    NSGraphics::IGraphicsRenderer* pRenderer = NSGraphics::Create();
    pRenderer->SetFontManager(pFontManager);

	double dPageDpiX, dPageDpiY;
	double dWidth, dHeight;
	GetPageInfo(nPageIndex, &dWidth, &dHeight, &dPageDpiX, &dPageDpiY);

    int nWidth  = (nRasterW > 0) ? nRasterW : ((int)dWidth * 96 / dPageDpiX);
    int nHeight = (nRasterH > 0) ? nRasterH : ((int)dHeight * 96 / dPageDpiX);

	BYTE* pBgraData = new BYTE[nWidth * nHeight * 4];
	if (!pBgraData)
		return;

	memset(pBgraData, 0xff, nWidth * nHeight * 4);
	CBgraFrame oFrame;
	oFrame.put_Data(pBgraData);
	oFrame.put_Width(nWidth);
	oFrame.put_Height(nHeight);
	oFrame.put_Stride(-4 * nWidth);

    pRenderer->CreateFromBgraFrame(&oFrame);
    pRenderer->SetSwapRGB(false);
    pRenderer->put_Width(dWidth);
    pRenderer->put_Height(dHeight);

	bool bBreak = false;
    DrawPageOnRenderer(pRenderer, nPageIndex, &bBreak);

	oFrame.SaveFile(wsDstPath, nImageType);
	RELEASEINTERFACE(pFontManager);
    RELEASEOBJECT(pRenderer);
}
void CXpsFile::ConvertToPdf(const std::wstring& wsPath)
{
    CPdfRenderer oPdf(m_pInternal->m_pAppFonts);
	bool bBreak = false;

	int nPagesCount = GetPagesCount();
	for (int nPageIndex = 0; nPageIndex < nPagesCount; nPageIndex++)
	{
		oPdf.NewPage();
		oPdf.BeginCommand(c_nPageType);

		double dPageDpiX, dPageDpiY;
		double dWidth, dHeight;
		GetPageInfo(nPageIndex, &dWidth, &dHeight, &dPageDpiX, &dPageDpiY);

		dWidth  *= 25.4 / dPageDpiX;
		dHeight *= 25.4 / dPageDpiY;

		oPdf.put_Width(dWidth);
		oPdf.put_Height(dHeight);

		DrawPageOnRenderer(&oPdf, nPageIndex, &bBreak);

		oPdf.EndCommand(c_nPageType);

#ifdef _DEBUG
		printf("page %d / %d\n", nPageIndex + 1, nPagesCount);
#endif
	}

	oPdf.SaveToFile(wsPath);
}
