/*
 * Copyright (C) Ascensio System SIA, 2009-2026
 *
 * This program is a free software product. You can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public License (AGPL)
 * version 3 as published by the Free Software Foundation, together with the
 * additional terms provided in the LICENSE file.
 *
 * This program is distributed WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. For
 * details, see the GNU AGPL at: https://www.gnu.org/licenses/agpl-3.0.html
 *
 * You can contact Ascensio System SIA by email at info@onlyoffice.com
 * or by postal mail at 20A-6 Ernesta Birznieka-Upisha Street, Riga,
 * LV-1050, Latvia, European Union.
 *
 * The interactive user interfaces in modified versions of the Program
 * are required to display Appropriate Legal Notices in accordance with
 * Section 5 of the GNU AGPL version 3.
 *
 * No trademark rights are granted under this License.
 *
 * All non-code elements of the Product, including illustrations,
 * icon sets, and technical writing content, are licensed under the
 * Creative Commons Attribution-ShareAlike 4.0 International License:
 * https://creativecommons.org/licenses/by-sa/4.0/legalcode
 *
 * This license applies only to such non-code elements and does not
 * modify or replace the licensing terms applicable to the Program's
 * source code, which remains licensed under the GNU Affero General
 * Public License v3.
 *
 * SPDX-License-Identifier: AGPL-3.0-only
 */
#include "XpsFile.h"
#include "XpsLib/Document.h"

#include "../DesktopEditor/common/File.h"
#include "../DesktopEditor/common/Directory.h"
#include "../DesktopEditor/graphics/pro/Graphics.h"
#include "../DesktopEditor/raster/BgraFrame.h"

#ifndef DISABLE_PDF_CONVERTATION
#include "../PdfFile/PdfFile.h"
#endif

#include "../OfficeUtils/src/OfficeUtils.h"
#include "../OfficeUtils/src/ZipFolder.h"

using namespace XPS;

class CXpsFile_Private
{
public:
    NSFonts::IApplicationFonts* m_pAppFonts;
    NSFonts::IFontManager*      m_pFontManager;
    IFolder*           m_wsTempFolder;
    XPS::CDocument*    m_pDocument;

public:
    CXpsFile_Private(NSFonts::IApplicationFonts* pAppFonts)
    {
        m_pDocument = NULL;

        m_pAppFonts = pAppFonts;

        // Create a font manager with its own cache
        m_pFontManager = pAppFonts->GenerateFontManager();
        NSFonts::IFontsCache* pMeasurerCache = NSFonts::NSFontCache::Create();
        pMeasurerCache->SetStreams(pAppFonts->GetStreams());
        m_pFontManager->SetOwnerCache(pMeasurerCache);
        pMeasurerCache->SetCacheSize(16);

        m_wsTempFolder = NULL;
    }
    ~CXpsFile_Private()
    {
    }
};

CXpsFile::CXpsFile(NSFonts::IApplicationFonts* pAppFonts)
{
    m_pInternal = new CXpsFile_Private(pAppFonts);
}
CXpsFile::~CXpsFile()
{
	Close();
    RELEASEOBJECT(m_pInternal->m_wsTempFolder);
    RELEASEINTERFACE((m_pInternal->m_pFontManager));
}
std::wstring CXpsFile::GetTempDirectory()
{
    return m_pInternal->m_wsTempFolder->getFullFilePath(L"");
}
void CXpsFile::SetTempDirectory(const std::wstring& wsPath)
{
    RELEASEOBJECT(m_pInternal->m_wsTempFolder);

	int nCounter = 0;
    std::wstring wsTempFolder = wsPath + L"/XPS/";
    while (NSDirectory::Exists(wsTempFolder))
	{
        wsTempFolder = wsPath + L"/XPS" + std::to_wstring(nCounter) + L"/";
		nCounter++;
	}
    NSDirectory::CreateDirectory(wsTempFolder);
    m_pInternal->m_wsTempFolder = new CFolderSystem(wsTempFolder);
}
bool CXpsFile::LoadFromFile(const std::wstring& wsSrcFileName, const std::wstring& wsXmlOptions,
							const wchar_t* owner_password, const wchar_t* user_password)
{
	Close();

	// Extract the Zip archive to a temporary folder
	COfficeUtils oUtils(NULL);
    if (S_OK != oUtils.ExtractToDirectory(wsSrcFileName, m_pInternal->m_wsTempFolder->getFullFilePath(L""), NULL, 0))
		return false;

    m_pInternal->m_pDocument = new XPS::CDocument(m_pInternal->m_pFontManager);
    if (!m_pInternal->m_pDocument)
		return false;

    m_pInternal->m_pDocument->Read(m_pInternal->m_wsTempFolder);

	return true;
}
bool CXpsFile::LoadFromMemory(BYTE* data, DWORD length, const std::wstring& options,
							  const wchar_t* owner_password, const wchar_t* user_password)
{
    Close();

    m_pInternal->m_wsTempFolder = new CZipFolderMemory(data, length);

    m_pInternal->m_pDocument = new XPS::CDocument(m_pInternal->m_pFontManager);
    if (!m_pInternal->m_pDocument)
        return false;

    m_pInternal->m_pDocument->Read(m_pInternal->m_wsTempFolder);
    return true;
}
void CXpsFile::Close()
{
    if (m_pInternal->m_pDocument)
	{
        m_pInternal->m_pDocument->Close();
        delete m_pInternal->m_pDocument;
        m_pInternal->m_pDocument = NULL;
        m_pInternal->m_wsTempFolder = NULL;
	}
}
NSFonts::IApplicationFonts* CXpsFile::GetFonts()
{
    return m_pInternal->m_pAppFonts;
}

OfficeDrawingFileType CXpsFile::GetType()
{
    return odftXPS;
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
void CXpsFile::DrawPageOnRenderer(IRenderer* pRenderer, int nPageIndex, bool* pBreak, COfficeDrawingPageParams* pParams)
{
    if (!m_pInternal->m_pDocument)
		return;

    m_pInternal->m_pDocument->DrawPage(nPageIndex, pRenderer, pBreak);
}
std::wstring CXpsFile::GetInfo()
{
    return m_pInternal->m_pDocument->GetInfo();
}

#ifndef DISABLE_PDF_CONVERTATION
void CXpsFile::ConvertToPdf(const std::wstring& wsPath)
{
	CPdfFile oPdf(m_pInternal->m_pAppFonts);
	oPdf.CreatePdf();
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
#endif

BYTE* CXpsFile::GetStructure()
{
	return m_pInternal->m_pDocument->GetStructure();
}
BYTE* CXpsFile::GetLinks (int nPageIndex)
{
	return m_pInternal->m_pDocument->GetPageLinks(nPageIndex);
}
