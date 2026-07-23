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
#include "DjVu.h"
#include "DjVuFileImplementation.h"

CDjVuFile::CDjVuFile(NSFonts::IApplicationFonts* pFonts)
{
    m_pImplementation = new CDjVuFileImplementation(pFonts);
}
CDjVuFile::~CDjVuFile()
{
	if (m_pImplementation)
		delete m_pImplementation;
}
bool CDjVuFile::LoadFromFile(const std::wstring& file, const std::wstring& options,
							 const wchar_t* owner_password, const wchar_t* user_password)
{
    if (m_pImplementation)
        return m_pImplementation->LoadFromFile(file, options);

    return false;
}
bool CDjVuFile::LoadFromMemory(BYTE* data, DWORD length, const std::wstring& options,
							   const wchar_t* owner_password, const wchar_t* user_password)
{
    if (m_pImplementation)
        return m_pImplementation->LoadFromMemory(data, length, options);
    return false;
}

void CDjVuFile::Close()
{
	if (m_pImplementation)
		m_pImplementation->Close();
}

OfficeDrawingFileType CDjVuFile::GetType()
{
    return odftDJVU;
}
NSFonts::IApplicationFonts* CDjVuFile::GetFonts()
{
    return m_pImplementation->GetFonts();
}

std::wstring CDjVuFile::GetTempDirectory()
{
    return m_pImplementation ? m_pImplementation->GetTempDirectory() : L"";
}
void CDjVuFile::SetTempDirectory(const std::wstring& wsDirectory)
{
	if (m_pImplementation)
		m_pImplementation->SetTempDirectory(wsDirectory);
}

int CDjVuFile::GetPagesCount()
{
	if (m_pImplementation)
		return m_pImplementation->GetPagesCount();
	return 0;
}
void CDjVuFile::GetPageInfo(int nPageIndex, double* pdWidth, double* pdHeight, double* pdDpiX, double* pdDpiY)
{
	if (m_pImplementation)
		m_pImplementation->GetPageInfo(nPageIndex, pdWidth, pdHeight, pdDpiX, pdDpiY);
}
void CDjVuFile::DrawPageOnRenderer(IRenderer* pRenderer, int nPageIndex, bool* pBreak, COfficeDrawingPageParams* pParams)
{
	if (m_pImplementation)
		m_pImplementation->DrawPageOnRenderer(pRenderer, nPageIndex, pBreak);
}
std::wstring CDjVuFile::GetInfo()
{
    if (m_pImplementation)
        return m_pImplementation->GetInfo();
    return L"";
}
void CDjVuFile::ConvertToPdf(const std::wstring& wsDstPath)
{
	if (m_pImplementation)
        m_pImplementation->ConvertToPdf(wsDstPath);
}
BYTE* CDjVuFile::GetStructure()
{
    if (m_pImplementation)
        return m_pImplementation->GetStructure();
    return NULL;
}
BYTE* CDjVuFile::GetLinks (int nPageIndex)
{
    if (m_pImplementation)
        return m_pImplementation->GetPageLinks(nPageIndex);
    return NULL;
}

unsigned char* CDjVuFile::ConvertToPixels(
	int nPageIndex,
	int nRasterW, int nRasterH, bool bIsFlip,
	NSFonts::IFontManager* pFonts,
	int nBackgroundColor, bool bIsDarkMode,
	int nBackgroundOpacity)
{
	if (m_pImplementation)
		return m_pImplementation->ConvertToPixels(nPageIndex, nRasterW, nRasterH, !bIsFlip);
	return NULL;
}
